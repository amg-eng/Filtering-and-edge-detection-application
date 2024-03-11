#ifndef EDGEDETECTION_H
#define EDGEDETECTION_H
#include <opencv2/opencv.hpp>

// Function declarations
void applySobel(const cv::Mat& input, cv::Mat& output);
void applyPrewitt(const cv::Mat& input, cv::Mat& output);
void applyRoberts(const cv::Mat& input, cv::Mat& output);
// void applyCanny(const cv::Mat& input, cv::Mat& output);
cv::Mat applyCanny(const cv::Mat& input, double lowThreshold, double highThreshold);
#endif // EDGEDETECTION_H
