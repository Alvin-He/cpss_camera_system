#pragma once
#include <opencv2/opencv.hpp>

struct Frame
{
    cv::Mat data; 
    int64_t timestamp; 
};