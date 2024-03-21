#include "edgeDetection.h"
#include "source_code.h"
#include <qmath.h>

// Simple convolution function
void conv2D(const cv::Mat& input, const cv::Mat& kernel, cv::Mat& output) {
    cv::Mat flippedKernel;
    cv::flip(kernel, flippedKernel, -1);

    cv::Mat result;
    cv::filter2D(input, result, -1, flippedKernel, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);

    output = result;
}

// void applySobel(const cv::Mat& input, cv::Mat& output) {
//     cv::Mat kernelX = (cv::Mat_<float>(3, 3) <<
//                            -1, 0, 1,
//                        -2, 0, 2,
//                        -1, 0, 1);

//     cv::Mat kernelY = (cv::Mat_<float>(3, 3) <<
//                            -1, -2, -1,
//                        0,  0,  0,
//                        1,  2,  1);

//     cv::Mat gradX, gradY;
//     cv::Mat absGradX, absGradY;
//     cv::Mat grad;

//     conv2D(input, kernelX, gradX);
//     conv2D(input, kernelY, gradY);

//     cv::convertScaleAbs(gradX, absGradX);
//     cv::convertScaleAbs(gradY, absGradY);

//     cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

//     output = grad;
// }

std::tuple<cv::Mat, cv::Mat, cv::Mat> applySobel(const cv::Mat& input) {
    cv::Mat kernelX = (cv::Mat_<float>(3, 3) <<
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1);

    cv::Mat kernelY = (cv::Mat_<float>(3, 3) <<
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1);

    cv::Mat gradX, gradY;
    cv::Mat absGradX, absGradY;
    cv::Mat grad;

    conv2D(input, kernelX, gradX);
    conv2D(input, kernelY, gradY);

    //cv::convertScaleAbs(gradX, absGradX);
    //cv::convertScaleAbs(gradY, absGradY);

    //cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);
    grad = Magnitude_Gradient(gradX, gradY);

    return std::make_tuple(gradX, gradY, grad);
}

std::tuple<cv::Mat, cv::Mat, cv::Mat> applyPrewitt(const cv::Mat& input) {
    cv::Mat kernelX = (cv::Mat_<float>(3, 3) <<
        -1, 0, 1,
        -1, 0, 1,
        -1, 0, 1);

    cv::Mat kernelY = (cv::Mat_<float>(3, 3) <<
        -1, -1, -1,
        0, 0, 0,
        1, 1, 1);

    cv::Mat gradX, gradY;
    cv::Mat absGradX, absGradY;
    cv::Mat grad;

    conv2D(input, kernelX, gradX);
    conv2D(input, kernelY, gradY);

    //cv::convertScaleAbs(gradX, absGradX);
    //cv::convertScaleAbs(gradY, absGradY);

    //cv::addWeighted(absGradX, 1, absGradY, 1, 0, grad);
    grad = Magnitude_Gradient(gradX, gradY);
    return std::make_tuple(gradX, gradY, grad);
}

std::tuple<cv::Mat, cv::Mat, cv::Mat> applyRoberts(const cv::Mat& input) {
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

    //cv::convertScaleAbs(gradX, absGradX);
    //cv::convertScaleAbs(gradY, absGradY);

    //cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);
    grad = Magnitude_Gradient(gradX, gradY);

    return std::make_tuple(gradX, gradY, grad);
}

std::tuple<cv::Mat, cv::Mat, cv::Mat> Detect_Edges_Canny(const cv::Mat& src, int lowThreshold = 5, int highThreshold = 20)
{
    cv::Mat img = src.clone();
    cv::Mat output;

    // FIRST SMOOTH IMAGE
    cv::Mat blurred = applyGaussianFilter(img, 3, 1.0,"c");

    // Sobel kernels
    cv::Mat sobel_x_kernel = (cv::Mat_<float>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    cv::Mat sobel_y_kernel = (cv::Mat_<float>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);

    cv::Mat gradX, gradY;
    cv::filter2D(blurred, gradX, CV_32F, sobel_x_kernel);
    cv::filter2D(blurred, gradY, CV_32F, sobel_y_kernel);

    cv::Mat magnitude_gradient = Magnitude_Gradient(gradX, gradY);
    cv::Mat phase_gradient = Phase_Gradient(gradX, gradY);

    // THEN SUPPRESS NON-MAXIMUM EDGES
    cv::Mat suppressed = NonMaxSuppression(magnitude_gradient, phase_gradient);

    // THEN APPLY THRESHOLDING
    cv::Mat thresholded = DoubleThresholding(suppressed, lowThreshold, highThreshold);

    // THEN APPLY HYSTERESIS
    cv::Mat canny_edges = Hysteresis(thresholded);

    // Convert Canny edges to 8-bit
    //canny_edges.convertTo(canny_edges, CV_8U);
    cv::Canny(src, canny_edges, lowThreshold, highThreshold);
    // Return a tuple of the X gradient, Y gradient, and Canny edges
    return std::make_tuple(gradX, gradY, canny_edges);
}


cv::Mat NonMaxSuppression(const cv::Mat& magnitude_gradient, const cv::Mat& phase_gradient) {
    cv::Mat suppressed = cv::Mat::zeros(magnitude_gradient.size(), CV_32F);
    cv::Mat angles;
    phase_gradient.copyTo(angles);

    for (int i = 1; i < magnitude_gradient.rows - 1; ++i) {
        for (int j = 1; j < magnitude_gradient.cols - 1; ++j) {
            // Adjust negative angles
            if (angles.at<float>(i, j) < 0) {
                angles.at<float>(i, j) += 360.0f;
            }

            // Perform non-maximum suppression
            float mag = magnitude_gradient.at<float>(i, j);
            float angle = angles.at<float>(i, j);

            float q = 255.0f;
            float r = 255.0f;

            // Angle near 0 degrees
            if ((angle >= 0.0f && angle < 22.5f) || (angle >= 157.5f && angle <= 180.0f)) {
                q = magnitude_gradient.at<float>(i, j + 1);
                r = magnitude_gradient.at<float>(i, j - 1);
            }
            // Angle near 45 degrees
            else if (angle >= 22.5f && angle < 67.5f) {
                q = magnitude_gradient.at<float>(i + 1, j - 1);
                r = magnitude_gradient.at<float>(i - 1, j + 1);
            }
            // Angle near 90 degrees
            else if (angle >= 67.5f && angle < 112.5f) {
                q = magnitude_gradient.at<float>(i + 1, j);
                r = magnitude_gradient.at<float>(i - 1, j);
            }
            // Angle near 135 degrees
            else if (angle >= 112.5f && angle < 157.5f) {
                q = magnitude_gradient.at<float>(i - 1, j - 1);
                r = magnitude_gradient.at<float>(i + 1, j + 1);
            }

            if (mag >= q && mag >= r) {
                suppressed.at<float>(i, j) = mag;
            }
            else {
                suppressed.at<float>(i, j) = 0.0f;
            }
        }
    }

    return suppressed;
}

cv::Mat DoubleThresholding(const cv::Mat& suppressed, float lowThreshold, float highThreshold) {
    cv::Mat thresholded = cv::Mat::zeros(suppressed.size(), CV_8U);

    for (int i = 0; i < suppressed.rows; ++i) {
        for (int j = 0; j < suppressed.cols; ++j) {
            float val = suppressed.at<float>(i, j);
            if (val >= highThreshold) {
                thresholded.at<uchar>(i, j) = STRONG_EDGE;
            }
            else if (val >= lowThreshold && val < highThreshold) {
                thresholded.at<uchar>(i, j) = WEAK_EDGE;
            }
            else {
                thresholded.at<uchar>(i, j) = 0;
            }
        }
    }

    return thresholded;
}
cv::Mat Hysteresis(const cv::Mat& thresholded) {
    cv::Mat hysteresis = thresholded.clone();

    for (int i = 1; i < thresholded.rows - 1; ++i) {
        for (int j = 1; j < thresholded.cols - 1; ++j) {
            if (thresholded.at<uchar>(i, j) == WEAK_EDGE) {
                if (thresholded.at<uchar>(i - 1, j - 1) == STRONG_EDGE ||
                    thresholded.at<uchar>(i - 1, j) == STRONG_EDGE ||
                    thresholded.at<uchar>(i - 1, j + 1) == STRONG_EDGE ||
                    thresholded.at<uchar>(i, j - 1) == STRONG_EDGE ||
                    thresholded.at<uchar>(i, j + 1) == STRONG_EDGE ||
                    thresholded.at<uchar>(i + 1, j - 1) == STRONG_EDGE ||
                    thresholded.at<uchar>(i + 1, j) == STRONG_EDGE ||
                    thresholded.at<uchar>(i + 1, j + 1) == STRONG_EDGE) {
                    hysteresis.at<uchar>(i, j) = STRONG_EDGE;
                }
                else {
                    hysteresis.at<uchar>(i, j) = 0;
                }
            }
        }
    }

    return hysteresis;
}

cv::Mat Magnitude_Gradient(const cv::Mat& gradient_x, const cv::Mat& gradient_y)
{
    cv::Mat Magnitude_Gradient = cv::Mat::zeros(cv::Size(gradient_x.cols, gradient_x.rows), gradient_x.type());
    for (int i = 0; i < gradient_x.rows; i++)
    {
        for (int j = 0; j < gradient_x.cols; j++)
        {
            Magnitude_Gradient.at<float>(i, j) = sqrt(pow(gradient_x.at<float>(i, j), 2) + pow(gradient_y.at<float>(i, j), 2));
        }
    }
    return Magnitude_Gradient;
}

cv::Mat Phase_Gradient(const cv::Mat& gradient_x, const cv::Mat& gradient_y)
{
    cv::Mat phase_gradient = cv::Mat::zeros(cv::Size(gradient_x.cols, gradient_y.rows), CV_32FC1);
    for (int i = 0; i < phase_gradient.rows; i++)
    {
        for (int j = 0; j < phase_gradient.cols; j++)
        {
            phase_gradient.at<float>(i, j) = atan2(gradient_y.at<float>(i, j), gradient_x.at<float>(i, j));
        }
    }
    phase_gradient = phase_gradient * 180 / M_PI;
    return phase_gradient;
}
