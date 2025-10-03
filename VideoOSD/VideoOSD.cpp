#include "VideoOSD.h"
#include <cmath>
#include <stdexcept>

VideoOSD::VideoOSD(const OSDConfig& cfg, bool reverse)
    : config_(cfg), reverse_(reverse), angle_(0.0) {}

void VideoOSD::drawOSD(cv::Mat& frame) {
    double scale = frame.cols / 640.0;
    int baseline = 0;
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;

    // Text
    cv::putText(frame, config_.text, cv::Point(10, frame.rows - 10),
                fontFace, 1.0 * scale, config_.color, 2);

    // Compass 
    cv::Point center(frame.cols - 50 * scale, frame.rows - 50 * scale);
    int radius = static_cast<int>(40 * scale);
    double rad = angle_ * CV_PI / 180.0;
    cv::Point tip(center.x + radius * cos(rad), center.y - radius * sin(rad));

    cv::circle(frame, center, radius, cv::Scalar(255,255,255), 2);
    cv::arrowedLine(frame, center, tip, cv::Scalar(255,255,255), 2, cv::LINE_AA, 0, 0.2);

    // Update angle
    angle_ += reverse_ ? -config_.speed : config_.speed;
    if(angle_ >= 360.0) angle_ -= 360.0;
    if(angle_ < 0.0) angle_ += 360.0;
}


void VideoOSD::run(const std::string& camera_device) {
    cv::VideoCapture cap(camera_device);
    if(!cap.isOpened()){
        throw std::runtime_error("Cannot open camera: " + camera_device);
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, config_.resolution.width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, config_.resolution.height);

    cv::Mat frame;
    while(true){
        cap >> frame;
        if(frame.empty()) break;

        cv::Mat stretched;
        cv::resize(frame, stretched, config_.resolution, 0, 0, cv::INTER_LINEAR);

        drawOSD(stretched);

        cv::imshow("Video OSD", stretched);
        if(cv::waitKey(30) >= 0) break;
    }
}
