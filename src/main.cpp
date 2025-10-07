#include <iostream>
#include "fmtAll.hpp"
#include "opencv2/opencv.hpp"
#include "datastructures.cpp"
#include "opencv2/imgcodecs.hpp"

#include "encoder.cpp"

int main(int argc, char** argv) {
    fmt::printf("Hello world from CPSS Camera");
    
    auto cap = cv::VideoCapture(0);
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

    Encoder encoder {};

    fmt::printf("{}", cap.isOpened()); 
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
        fmt::println("{}", i);
        i += 1;
        if (i >= 10) {
            encoder.Encode(frame, i);
        }
    }

    return 0;
}