#include <iostream>
#include "fmtAll.hpp"
#include "opencv2/opencv.hpp"
#include "datastructures.cpp"
#include "opencv2/imgcodecs.hpp"
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

#include "encoder.cpp"
#include "decoder.cpp"


std::queue<cv::Mat> frameQueue;
std::mutex queueMutex;
std::condition_variable available;
bool stopThread = false; // I am not sure how we want to implement stopping the thread




void encodeThread() {
    long long i = 0; // I have no idea what index the frames should start at


    std::chrono::nanoseconds timeSpentEncoding {0}; 
    uint64_t numPixelsEncoded = 0;
    uint64_t totalDataWrittenBytes = 0;
    uint64_t totalRawBytes = 0;

    auto startTime = std::chrono::system_clock::now();

    while(!stopThread) {
        std::unique_lock<std::mutex> lock(queueMutex);
        available.wait(lock);
        cv::Mat encodeFrame = frameQueue.front();
        frameQueue.pop();
        lock.unlock();
        std::cout << "Encoding Frame";


        auto start = std::chrono::high_resolution_clock::now();


        Encoder encoder {}; // I have no idea how to transfer your encoding from Main into this
        encoder.Encode(encodeFrame, i);

        uint64_t numBytes = encoder.Encode(encodeFrame, i);
        totalDataWrittenBytes += numBytes;
        totalRawBytes += encodeFrame.total() * encodeFrame.elemSize();

        auto end = std::chrono::high_resolution_clock::now();
        numPixelsEncoded += encodeFrame.total();
        timeSpentEncoding += end - start;

        i++;

        uint64_t megaPixelsEncoded = numPixelsEncoded / 1000000;
        auto timeSpentEncodingSecs = std::chrono::duration_cast<std::chrono::duration<double>>(timeSpentEncoding).count();
        auto totalTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now() - startTime).count(); 
        fmt::println("Frame {} \t Total Time: {:.2f}s \t Time Spent Encoding: {:.2f}s \t Current Speed: {:.3f}Mp/s", i, totalTime, timeSpentEncodingSecs, static_cast<double>(megaPixelsEncoded)/timeSpentEncodingSecs);
        fmt::println("Raw: {}mb \t Compressed: {}mB \t Compression Ratio: {:.2f}%", totalRawBytes/1000000, totalDataWrittenBytes/1000000, (((double)totalRawBytes)/totalDataWrittenBytes)*100);



        
        if (cv::waitKey(30) != -1) {
                break;
        }
    }
    std::terminate();
}

int main(int argc, char** argv) {
    fmt::printf("Hello world from CPSS Camera");

    

    // auto cap = cv::VideoCapture(0);
    // cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

    auto cap = cv::VideoCapture("test.mp4");

    Encoder encoder {};
    Decoder decoder {};

    fmt::printf("{}", cap.isOpened()); 

    // std::chrono::nanoseconds timeSpentEncoding {0}; 
    // uint64_t numPixelsEncoded = 0;
    // uint64_t totalDataWrittenBytes = 0;
    // uint64_t totalRawBytes = 0;

    // int i = 0;
    // auto startTime = std::chrono::system_clock::now();

    std::thread t(encodeThread);

    while (cap.isOpened())
    {
        // i += 1;
        cv::Mat frame;
        cap.read(frame);
        if (frame.total() <= 10) return 0;
        int64_t timestamp = 0;
        // Frame tframe {
        //     .data = frame.clone(),
        //     .timestamp = timestamp 
        // };

        // send to encode thread
        std::unique_lock<std::mutex> lock(queueMutex);
        frameQueue.push(frame);  // null is the raw frame we send to be 
        lock.unlock();
        available.notify_all();


 
        // work here
        // cv::imshow("test", frame);
        
        if (cv::waitKey(30) != -1) {
            break;
        }
        
        // auto start = std::chrono::high_resolution_clock::now();
        
        // uint64_t numBytes = encoder.Encode(frame, i);
        // totalDataWrittenBytes += numBytes;
        // totalRawBytes += frame.total() * frame.elemSize();

        // auto end = std::chrono::high_resolution_clock::now();
        // numPixelsEncoded += frame.total();
        // timeSpentEncoding += end - start;

        // uint64_t megaPixelsEncoded = numPixelsEncoded / 1000000;
        // auto timeSpentEncodingSecs = std::chrono::duration_cast<std::chrono::duration<double>>(timeSpentEncoding).count();
        // auto totalTime = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now() - startTime).count(); 
        // fmt::println("Frame {} \t Total Time: {:.2f}s \t Time Spent Encoding: {:.2f}s \t Current Speed: {:.3f}Mp/s", i, totalTime, timeSpentEncodingSecs, static_cast<double>(megaPixelsEncoded)/timeSpentEncodingSecs);
        // fmt::println("Raw: {}mb \t Compressed: {}mB \t Compression Ratio: {:.2f}%", totalRawBytes/1000000, totalDataWrittenBytes/1000000, (((double)totalRawBytes)/totalDataWrittenBytes)*100);

        // cv::Mat resDecoded = decoder.Decode(encoder.GetEncodedFrame());

        // cv::imshow("result", resDecoded); 
                
        // if (cv::waitKey(1) != -1) {
        //     break;
        // }

    }

    return 0;
}