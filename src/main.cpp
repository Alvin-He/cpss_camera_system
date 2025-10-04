#include <iostream>
#include "fmtAll.hpp"
#include "opencv2/opencv.hpp"
#include "datastructures.cpp"

int main(int argc, char** argv) {
    fmt::printf("Hello world from CPSS Camera");
    
    auto cap = cv::VideoCapture(0);

    fmt::printf("{}", cap.isOpened()); 

    while (cap.isOpened())
    {
        cv::Mat frame;
        cap.read(frame);

        int64_t timestamp = 0;
        Frame frame {
            .data = frame.clone(),
            .timestamp = timestamp 
        };

        // send to encode thread

        // work here
        cv::imshow("test", frame);
        
        if (cv::waitKey(30) != -1) {
            break;
        }
    }

    return 0;
}