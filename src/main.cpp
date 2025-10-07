#include <iostream>
#include "fmtAll.hpp"
#include "opencv2/opencv.hpp"
#include "datastructures.cpp"
#include "opencv2/imgcodecs.hpp"
#include <chrono>

#include "encoder.cpp"

int main(int argc, char** argv) {
    fmt::printf("Hello world from CPSS Camera");
    
    auto cap = cv::VideoCapture(0);
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    Encoder encoder {};

    fmt::printf("{}", cap.isOpened()); 

    std::chrono::nanoseconds timeSpentEncoding {0}; 
    uint64_t numPixelsEncoded = 0;
    uint64_t totalDataWrittenBytes = 0;
    uint64_t totalRawBytes = 0;

    int i = 0;
    while (cap.isOpened())
    {
        cv::Mat frame;
        cap.read(frame);

        int64_t timestamp = 0;
        // Frame tframe {
        //     .data = frame.clone(),
        //     .timestamp = timestamp 
        // };

        // send to encode thread

        // work here
        cv::imshow("test", frame);
        
        if (cv::waitKey(30) != -1) {
            break;
        }
        i += 1;
        if (i >= 10) {
            auto start = std::chrono::high_resolution_clock::now();
            
            totalDataWrittenBytes += encoder.Encode(frame, i);
            totalRawBytes += frame.total() * frame.elemSize();

            auto end = std::chrono::high_resolution_clock::now();
            numPixelsEncoded += frame.total();
            timeSpentEncoding += end - start;

            uint64_t megaPixelsEncoded = numPixelsEncoded / 1000000;
            auto seconds = std::chrono::duration_cast<std::chrono::duration<double>>(timeSpentEncoding).count();
            fmt::println("Frame {} \t Time Spent Encoding: {:.2f}s \t Current Speed: {:.3f}Mp/s", i, seconds, static_cast<double>(megaPixelsEncoded)/seconds);
            fmt::println("Raw: {}mb \t Compressed: {}mB \t Compression Ratio: {:.2f}%", totalRawBytes/1000000, totalDataWrittenBytes/1000000, (((double)totalRawBytes)/totalDataWrittenBytes)*100);
        }

        if (i >= 2000) {
            break;
        }
    }

    return 0;
}