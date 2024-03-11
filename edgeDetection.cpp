#include "edgeDetection.h"

// Simple convolution function
void conv2D(const cv::Mat& input, const cv::Mat& kernel, cv::Mat& output) {
    cv::Mat flippedKernel;
    cv::flip(kernel, flippedKernel, -1);

    cv::Mat result;
    cv::filter2D(input, result, -1, flippedKernel, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);

    output = result;
}

void applySobel(const cv::Mat& input, cv::Mat& output) {
    cv::Mat kernelX = (cv::Mat_<float>(3, 3) <<
                           -1, 0, 1,
                       -2, 0, 2,
                       -1, 0, 1);

    cv::Mat kernelY = (cv::Mat_<float>(3, 3) <<
                           -1, -2, -1,
                       0,  0,  0,
                       1,  2,  1);

    cv::Mat gradX, gradY;
    cv::Mat absGradX, absGradY;
    cv::Mat grad;

    conv2D(input, kernelX, gradX);
    conv2D(input, kernelY, gradY);

    cv::convertScaleAbs(gradX, absGradX);
    cv::convertScaleAbs(gradY, absGradY);

    cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

    output = grad;
}

void applyPrewitt(const cv::Mat& input, cv::Mat& output) {
    cv::Mat kernelX = (cv::Mat_<float>(3, 3) <<
                           -1, 0, 1,
                       -1, 0, 1,
                       -1, 0, 1);

    cv::Mat kernelY = (cv::Mat_<float>(3, 3) <<
                           -1, -1, -1,
                       0,  0,  0,
                       1,  1,  1);

    cv::Mat gradX, gradY;
    cv::Mat absGradX, absGradY;
    cv::Mat grad;

    conv2D(input, kernelX, gradX);
    conv2D(input, kernelY, gradY);

    cv::convertScaleAbs(gradX, absGradX);
    cv::convertScaleAbs(gradY, absGradY);

    cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

    output = grad;
}

void applyRoberts(const cv::Mat& input, cv::Mat& output) {
    cv::Mat kernelX = (cv::Mat_<float>(2, 2) <<
                           1, 0,
                       0, -1);

    cv::Mat kernelY = (cv::Mat_<float>(2, 2) <<
                           0, 1,
                       -1, 0);

    cv::Mat gradX, gradY;
    cv::Mat absGradX, absGradY;
    cv::Mat grad;

    conv2D(input, kernelX, gradX);
    conv2D(input, kernelY, gradY);

    cv::convertScaleAbs(gradX, absGradX);
    cv::convertScaleAbs(gradY, absGradY);

    cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

    output = grad;
}

// Function to apply Canny edge detection
cv::Mat applyCanny(const cv::Mat& input, double lowThreshold, double highThreshold) {
    cv::Mat blurred;
    cv::GaussianBlur(input, blurred, cv::Size(5, 5), 1.4);

    cv::Mat edges;
    cv::Canny(blurred, edges, lowThreshold, highThreshold);

    return edges;
}
