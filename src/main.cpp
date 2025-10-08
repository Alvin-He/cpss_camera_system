#include <iostream>
#include "fmtAll.hpp"
#include "opencv2/opencv.hpp"
#include "datastructures.cpp"
#include "opencv2/imgcodecs.hpp"
#include <chrono>

#include "encoder.cpp"
#include "decoder.cpp"
int main(int argc, char** argv) {
    fmt::printf("Hello world from CPSS Camera");
    
    // auto cap = cv::VideoCapture(0);
    // cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

    auto cap = cv::VideoCapture("test.mp4");

    Encoder encoder {};
    Decoder decoder {};

    fmt::printf("{}", cap.isOpened()); 

    std::chrono::nanoseconds timeSpentEncoding {0}; 
    uint64_t numPixelsEncoded = 0;
    uint64_t totalDataWrittenBytes = 0;
    uint64_t totalRawBytes = 0;

    int i = 0;
    auto startTime = std::chrono::system_clock::now();
    while (cap.isOpened())
    {
        i += 1;
        cv::Mat frame;
        cap.read(frame);
        if (frame.total() <= 10) return 0;
        int64_t timestamp = 0;
        // Frame tframe {
        //     .data = frame.clone(),
        //     .timestamp = timestamp 
        // };

        // send to encode thread

        // work here
        // cv::imshow("test", frame);
        
        if (cv::waitKey(30) != -1) {
            break;
        }
        
        auto start = std::chrono::high_resolution_clock::now();
        
        uint64_t numBytes = encoder.Encode(frame, i);
        totalDataWrittenBytes += numBytes;
        totalRawBytes += frame.total() * frame.elemSize();

        auto end = std::chrono::high_resolution_clock::now();
        numPixelsEncoded += frame.total();
        timeSpentEncoding += end - start;

        uint64_t megaPixelsEncoded = numPixelsEncoded / 1000000;
        auto timeSpentEncodingSecs = std::chrono::duration_cast<std::chrono::duration<double>>(timeSpentEncoding).count();
        auto totalTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now() - startTime).count(); 
        fmt::println("Frame {} \t Total Time: {:.2f}s \t Time Spent Encoding: {:.2f}s \t Current Speed: {:.3f}Mp/s", i, totalTime, timeSpentEncodingSecs, static_cast<double>(megaPixelsEncoded)/timeSpentEncodingSecs);
        fmt::println("Raw: {}mb \t Compressed: {}mB \t Compression Ratio: {:.2f}%", totalRawBytes/1000000, totalDataWrittenBytes/1000000, (((double)totalRawBytes)/totalDataWrittenBytes)*100);

        // cv::Mat resDecoded = decoder.Decode(encoder.GetEncodedFrame());

        // cv::imshow("result", resDecoded); 
                
        // if (cv::waitKey(1) != -1) {
        //     break;
        // }

    }

    return 0;
}