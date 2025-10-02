#include <iostream>
#include "fmtAll.hpp"
#include "opencv2/opencv.hpp"

int main(int argc, char** argv) {
    fmt::printf("Hello world from CPSS Camera");
    
    auto cap = cv::VideoCapture(0);

    fmt::printf("{}", cap.isOpened()); 

    while (cap.isOpened())
    {
        cv::Mat frame;
        cap.read(frame);
        cv::imshow("test", frame);
        
        if (cv::waitKey(30) != -1) {
            break;
        }
    }

    return 0;
}