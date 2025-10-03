#pragma once
#include <opencv2/opencv.hpp>
#include <string>

struct OSDConfig {
    cv::Size resolution;
    std::string text;
    cv::Scalar color;
    double speed;
};

class VideoOSD {
public:
    VideoOSD(const OSDConfig& cfg, bool reverse=false);
    void run(const std::string& camera_device);

private:
    OSDConfig config_;
    bool reverse_;
    double angle_;

    void drawOSD(cv::Mat& frame);
};
