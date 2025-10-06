// reference implenmentation: https://github.com/opencv/opencv/blob/4.x/modules/imgcodecs/src/grfmt_jpegxl.cpp

#include "fmtAll.hpp"
#include "datastructures.cpp"

#include "jxl/encode_cxx.h"
#include <memory_resource>
#include <fstream>

class OutputProcessor {
    public: 
        OutputProcessor() {}

        operator JxlEncoderOutputProcessor()  {
            return JxlEncoderOutputProcessor {
                .opaque = this,
                .get_buffer = OutputProcessor::get_buffer, 
                .release_buffer = OutputProcessor::release_buffer,
                .seek = OutputProcessor::seek,
                .set_finalized_position = OutputProcessor::set_finalized_position,
            };
        };

        static void* get_buffer(void* opaque, size_t* size) {
            OutputProcessor* self = reinterpret_cast<OutputProcessor*>(opaque);
            // basic string is better than std::vector cuz of this one feature lmao
            // why tf did wg21 not give std::vector resize_and_overwrite
            self->buffer.resize_and_overwrite(self->position + *size, [](uint8_t*, size_t s){ return s; });
            fmt::println("extend {}", self->position + *size);
            return self->buffer.data() + self->position; 
        };

        static void release_buffer(void* opaque, size_t written_bytes) {
            OutputProcessor* self = reinterpret_cast<OutputProcessor*>(opaque);
            fmt::println("wrote {}", written_bytes);
            self->position += written_bytes;
        };

        static void seek(void* opaque, uint64_t position) {
            OutputProcessor* self = reinterpret_cast<OutputProcessor*>(opaque);
            self->position = static_cast<size_t>(position);
            fmt::println("seek");
        };

        static void set_finalized_position(void* opaque, uint64_t finalized_position) {
            OutputProcessor* self = reinterpret_cast<OutputProcessor*>(opaque);
            self->finalPosition = static_cast<size_t>(finalized_position);
            fmt::println("finished {}", finalized_position);
        };

        void reset() {
            this->position = 0;
            this->finalPosition = 0;
        }

        uint8_t* data() {
            return this->buffer.data(); 
        }

        void writeFile() {
            std::ofstream fileStream; 
            fileStream.open("test.jxl");
            fileStream.write(reinterpret_cast<const char*>(this->buffer.data()), this->finalPosition); 
            fileStream.flush();
            fileStream.close();
            fmt::println("outputted");
        }

    private:
        size_t position = 0;
        uint64_t finalPosition = 0;
        std::basic_string<uint8_t> buffer {}; 

};

class Encoder {
public:
    Encoder():
        ptr_jxl(JxlEncoderMake(NULL)) 
    {
        this->settings = JxlEncoderFrameSettingsCreate(ptr_jxl.get(), NULL);
        
        OutputProcessor proc; 

        JxlBasicInfo info {}; 
        JxlEncoderInitBasicInfo(&info);

    }

    void Encode(cv::Mat frame) {

        JxlDataType dtype; 
        uint32_t exponent_bits;
        switch (frame.depth())
        {
            case CV_8U:
                dtype = JXL_TYPE_UINT8;
                exponent_bits = 0;
                break;
            case CV_16U:
                dtype = JXL_TYPE_UINT16;
                exponent_bits = 0;
                break;
            case CV_16F:
                dtype = JXL_TYPE_FLOAT16;
                exponent_bits = 5;
                break;
            case CV_32F:
                dtype = JXL_TYPE_FLOAT;
                exponent_bits = 8;
                break;
            default:
                throw std::runtime_error(fmt::format("Encoder can not handle frame with data type: {}", frame.depth()));
                break;
        };

        JxlPixelFormat pixelFormat {
            .num_channels = frame.channels(),
            .data_type = dtype,
            .endianness = JXL_NATIVE_ENDIAN, 
            .align = 0
        }; 

        JxlBasicInfo basicInfo;
        JxlEncoderInitBasicInfo(&basicInfo); 
        basicInfo.xsize = frame.cols;
        basicInfo.ysize = frame.rows;

        if( frame.channels() == 4 )
        {
            basicInfo.num_color_channels = 3;
            basicInfo.num_extra_channels = 1;

            basicInfo.bits_per_sample =
            basicInfo.alpha_bits      = 8 * static_cast<int>(frame.elemSize1());

            basicInfo.exponent_bits_per_sample = exponent_bits;
            basicInfo.alpha_exponent_bits      = exponent_bits;
        }else{
            basicInfo.num_color_channels = frame.channels();
            basicInfo.bits_per_sample = 8 * static_cast<int>(frame.elemSize1());
            basicInfo.exponent_bits_per_sample = exponent_bits;
        }

        cv::Mat image; 
        // convert BGR image to RGB images and ensure image is continous 
        switch (frame.channels())
        {
        case 3:
            cv::cvtColor(frame, image, cv::COLOR_BGR2RGB);
            break;
        case 4:
            cv::cvtColor(frame, image, cv::COLOR_BGRA2RGBA);
            break;
        case 1: 
            if (frame.isContinuous()) image = frame;
            else image = frame.clone(); 
        default:
            throw std::runtime_error(fmt::format("Encoder cannot handle frame with {} channels", frame.channels())); 
            break;
        }

        JxlColorEncoding encoding;
        JxlEncoderStatus status;
        JxlColorEncodingSetToSRGB(&encoding, pixelFormat.num_channels < 3 ? JXL_TRUE : JXL_FALSE);
        
        status = JxlEncoderSetBasicInfo(this->ptr_jxl.get(), &basicInfo);
        
        status = JxlEncoderSetFrameLossless(settings, JXL_FALSE);
        status = JxlEncoderSetFrameDistance(settings, 1);
        basicInfo.uses_original_profile = JXL_FALSE; // lossless requires true

        status = JxlEncoderFrameSettingsSetOption(settings, JXL_ENC_FRAME_SETTING_EFFORT, 1);
        status = JxlEncoderFrameSettingsSetOption(settings, JXL_ENC_FRAME_SETTING_DECODING_SPEED, 0);

        status = JxlEncoderSetColorEncoding(this->ptr_jxl.get(), &encoding); 
        status = JxlEncoderSetOutputProcessor(this->ptr_jxl.get(), this->outputProcessor);

        status = JxlEncoderAddImageFrame(settings, &pixelFormat, static_cast<const void*>(image.ptr<uint8_t>()), image.total() * image.elemSize());
        JxlEncoderCloseInput(ptr_jxl.get()); 

        status = JxlEncoderFlushInput(this->ptr_jxl.get());
        this->outputProcessor.writeFile();

        fmt::println("encode finished, {}", status == JxlEncoderStatus::JXL_ENC_SUCCESS);
        }
private:
    JxlEncoderPtr ptr_jxl; 
    JxlEncoderFrameSettings* settings;
    OutputProcessor outputProcessor;
}; 
