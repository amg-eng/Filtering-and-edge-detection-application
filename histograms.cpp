#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "histograms.h"

void onTrackbar(int, void*) {
    // Empty callback for trackbar
}

cv::Mat calculateHistogram(cv::Mat& input, int channel) {
    int histSize = 256;  // Number of bins
    float range[] = { 0, 256 };
    const float* histRange = { range };

    cv::Mat hist = cv::Mat::zeros(1, histSize, CV_32F);

    for (int y = 0; y < input.rows; ++y) {
        for (int x = 0; x < input.cols; ++x) {
            hist.at<float>(input.at<cv::Vec3b>(y, x)[channel]) += 1.0;
        }
    }

    return hist;
}

cv::Mat calculateCDF(const cv::Mat& hist) {
    cv::Mat cdf = cv::Mat::zeros(1, hist.cols, CV_32F);
    cdf.at<float>(0) = hist.at<float>(0);

    for (int i = 1; i < hist.cols; ++i) {
        cdf.at<float>(i) = cdf.at<float>(i - 1) + hist.at<float>(i);
    }

    cdf /= cdf.at<float>(hist.cols - 1);

    return cdf;
}

cv::Mat plotHistogram( const cv::Mat& hist, const cv::Scalar& color) {
    int histWidth = 512;
    int histHeight = 400;
    int binWidth = hist.cols > 0 ? histWidth / hist.cols : 1;

    cv::Mat histImage(histHeight, histWidth, CV_8UC3, cv::Scalar(255, 255, 255));

    normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

    for (int i = 1; i < hist.cols; i++) {
        cv::line(histImage, cv::Point(binWidth * (i - 1), histHeight),
            cv::Point(binWidth * i, histHeight - cvRound(hist.at<float>(i) * histHeight)), color, 2, 8, 0);
    }

    return histImage;
}

void plotCDF(const std::string& title, const cv::Mat& cdf, const cv::Scalar& color) {
    int cdfWidth = 512;
    int cdfHeight = 400;

    cv::Mat cdfImage(cdfHeight, cdfWidth, CV_8UC3, cv::Scalar(255, 255, 255));

    for (int i = 1; i < cdf.cols; i++) {
        cv::line(cdfImage, cv::Point(cdfWidth * (i - 1) / cdf.cols, cdfHeight - cvRound(cdf.at<float>(i - 1) * cdfHeight)),
            cv::Point(cdfWidth * i / cdf.cols, cdfHeight - cvRound(cdf.at<float>(i) * cdfHeight)), color, 2, 8, 0);
    }

    cv::imshow(title, cdfImage);
}

cv::Mat calculateGrayscaleCDF(const cv::Mat& grayscaleImage) {
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };

    cv::Mat hist = cv::Mat::zeros(1, histSize, CV_32F);

    for (int y = 0; y < grayscaleImage.rows; ++y) {
        for (int x = 0; x < grayscaleImage.cols; ++x) {
            hist.at<float>(grayscaleImage.at<uchar>(y, x)) += 1.0;
        }
    }

    cv::Mat cdf = cv::Mat::zeros(1, hist.cols, CV_32F);
    cdf.at<float>(0) = hist.at<float>(0);

    for (int i = 1; i < hist.cols; ++i) {
        cdf.at<float>(i) = cdf.at<float>(i - 1) + hist.at<float>(i);
    }

    cdf /= cdf.at<float>(hist.cols - 1);

    return cdf;
}

void plotGrayscaleCDF(const std::string& title, const cv::Mat& cdf, const cv::Scalar& lineColor, const cv::Scalar& fillColor) {
    int cdfWidth = 512;
    int cdfHeight = 400;

    cv::Mat cdfImage(cdfHeight, cdfWidth, CV_8UC3, cv::Scalar(255, 255, 255));

    for (int i = 1; i < cdf.cols; i++) {
        cv::line(cdfImage, cv::Point(cdfWidth * (i - 1) / cdf.cols, cdfHeight - cvRound(cdf.at<float>(i - 1) * cdfHeight)),
            cv::Point(cdfWidth * i / cdf.cols, cdfHeight - cvRound(cdf.at<float>(i) * cdfHeight)), lineColor, 2, 8, 0);

        /////////////////////////////////// color the area under the CDF curve////////////////////////
        // Fill the area under the curve
        cv::rectangle(cdfImage, cv::Point(cdfWidth * (i - 1) / cdf.cols, cdfHeight),
            cv::Point(cdfWidth * i / cdf.cols, cdfHeight - cvRound(cdf.at<float>(i) * cdfHeight)), fillColor, CV_16F);
    }

    cv::imshow(title, cdfImage);
}

void displayNormalizedEqualizedImages(const cv::Mat& image) {
    cv::Mat normalizedImage, equalizedImage;

    cv::normalize(image, normalizedImage, 0, 255, cv::NORM_MINMAX, CV_8U);
    cv::equalizeHist(normalizedImage, equalizedImage);

    cv::imshow("Normalized Image", normalizedImage);
    cv::imshow("Equalized Image", equalizedImage);
}

void loadAndProcessImage(const std::string& filePath) {
    cv::Mat image = cv::imread(filePath);
    if (image.empty()) {
        std::cerr << "Error: Unable to load image." << std::endl;
        return;
    }

    cv::Mat grayscaleImage;
    cv::cvtColor(image, grayscaleImage, cv::COLOR_BGR2GRAY);

    cv::imshow("Image Processing App", image);
    cv::imshow("Grayscale Image", grayscaleImage);

    int histWidth = std::max(image.cols, 512);  // Adjust minimum width
    int histHeight = std::max(image.rows, 400);  // Adjust minimum height

    cv::Mat rHist = calculateHistogram(image, 0);
    cv::Mat gHist = calculateHistogram(image, 1);
    cv::Mat bHist = calculateHistogram(image, 2);

    cv::Mat cdfR = calculateCDF(rHist);
    cv::Mat cdfG = calculateCDF(gHist);
    cv::Mat cdfB = calculateCDF(bHist);

    plotHistogram( rHist, cv::Scalar(0, 0, 255));
    plotHistogram( gHist, cv::Scalar(0, 255, 0));
    plotHistogram( bHist, cv::Scalar(255, 0, 0));

    plotCDF("CDF Curves R", cdfR, cv::Scalar(0, 0, 255));
    plotCDF("CDF Curves G", cdfG, cv::Scalar(0, 255, 0));
    plotCDF("CDF Curves B", cdfB, cv::Scalar(255, 0, 0));

    cv::Mat grayscaleCDF = calculateGrayscaleCDF(grayscaleImage); 


    // color the area under the CDF curve
    cv::Scalar curveColor(255, 0, 0);        // Black color for the curve
    cv::Scalar fillColor(200, 0, 0);       // Green color for the area under the curve
    plotGrayscaleCDF("Grayscale CDF", grayscaleCDF, curveColor, fillColor);


    displayNormalizedEqualizedImages(grayscaleImage);

    cv::waitKey(0);
}

    //cv::namedWindow("Image Processing App", cv::WINDOW_NORMAL);
    //cv::resizeWindow("Image Processing App", 800, 600);

    //cv::createTrackbar("Alpha", "Image Processing App", nullptr, 100, onTrackbar);

    //std::string filePath;
    //std::cout << "Enter the path to the image: ";
    //std::cin >> filePath;

    //loadAndProcessImage(filePath);
