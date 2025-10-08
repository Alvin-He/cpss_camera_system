#include "fmtAll.hpp"
#include "datastructures.cpp"

#include "jxl/decode_cxx.h"
#include "jxl/resizable_parallel_runner_cxx.h"
#include <memory_resource>
#include <fstream>


class JxlDecoderStatusChecker {
public:
    JxlDecoderStatus operator=(JxlDecoderStatus value) {
        if (value == JXL_DEC_ERROR) {
            throw std::runtime_error("Got unexpected JXL_DEC_ERROR!"); 
        }
        return value;
    }
};

class Decoder {
public:
    Decoder() {
        
    }  

    cv::Mat Decode(std::span<uint8_t> data) {
        reset();
        cv::Mat result {}; 

        JxlDecoderStatusChecker check;
        check = JxlDecoderSubscribeEvents(this->decoder.get(), JXL_DEC_BASIC_INFO | JXL_DEC_COLOR_ENCODING);

        JxlBasicInfo info;
        JxlPixelFormat format;

        check = JxlDecoderSetInput(this->decoder.get(), data.data(), data.size());
        JxlDecoderCloseInput(this->decoder.get()); 
        
        for(;;) {
            switch (check = JxlDecoderProcessInput(decoder.get()))
            {
            case JXL_DEC_BASIC_INFO: {
                check = JxlDecoderGetBasicInfo(this->decoder.get(), &info);
                
                auto totalChannels = info.num_color_channels + info.num_extra_channels; 
                int depth;
                JxlDataType dtype;
                switch (info.exponent_bits_per_sample)
                {
                case 0:
                    switch (info.bits_per_sample)
                    {
                    case 8:
                        depth = CV_8U;
                        dtype = JXL_TYPE_UINT8;
                        break;
                    case 16:
                        depth = CV_16U;
                        dtype = JXL_TYPE_UINT16;
                    default:
                        throw std::runtime_error("Unable to handle pixel data type");
                        break;
                    }
                    break;
                case 5:
                    depth = CV_16F;
                    dtype = JXL_TYPE_FLOAT16;
                    break;
                case 8:
                    depth = CV_32F;
                    dtype = JXL_TYPE_FLOAT;
                    break;
                default:
                    throw std::runtime_error("Unable to handle pixel data type");
                    break;
                }

                result.create(info.ysize, info.xsize, CV_MAKETYPE(depth, totalChannels));
                format = {totalChannels, dtype, JXL_NATIVE_ENDIAN, 0};

                break;
            }
            // case JXL_DEC_COLOR_ENCODING:
            //     JxlColorEncoding encoding; 
            //     check = JxlDecoderGetColorAsEncodedProfile(this->decoder.get(), JxlColorProfileTarget::JXL_COLOR_PROFILE_TARGET_DATA, &encoding);
            //     break;
            case JXL_DEC_NEED_IMAGE_OUT_BUFFER: {
                check = JxlDecoderSetImageOutBuffer(this->decoder.get(), &format, result.data, result.total() * result.elemSize()); 
            } case JXL_DEC_SUCCESS: {
                cv::Mat converted;
                cv::cvtColor(result, converted, cv::COLOR_RGB2BGR);
                return converted;
            } default:
                break;
            };
        }

    }

    void reset() {
        JxlDecoderReset(this->decoder.get()); 
    }
private:
    JxlDecoderPtr decoder = JxlDecoderMake(NULL);

};