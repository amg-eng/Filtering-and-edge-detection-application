#ifndef QTWIDGETSAPPLICATION2_H
#define QTWIDGETSAPPLICATION2_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "qlabel.h"
#include <QFileDialog>
#include <QMainWindow>
#include "source_code.h"
#include "edgeDetection.h"
#pragma once

QT_BEGIN_NAMESPACE
namespace Ui { class QtWidgetsApplication2Class; };
QT_END_NAMESPACE

class QtWidgetsApplication2 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication2(QWidget *parent = nullptr);
    ~QtWidgetsApplication2();

    void clearImage();
    void applyGaussianNoise();
    void applySaltAndPepperNoise();
    void apply_GaussianFilter();
    void apply_AverageFilter();
    void apply_MedianFilter();
    void applyUniformNoise();
    void displayImageOnLabel();
    void openImageDialog();
    void displayImageInLabel(const cv::Mat &image, QLabel *label);
    void displayOutputImage(const cv::Mat& image, QLabel* label);

    void onSobelClicked();
    void onPrewittClicked();
    void onRobertsClicked();
    void onCannyClicked();
    cv::Mat applyFilter(const cv::Mat& input, const cv::Mat& kernel);
    void applyEdgeDetection(const cv::Mat& input, cv::Mat& output, const std::string& method);
    void applyCannyFilter();

private:
    Ui::QtWidgetsApplication2Class *ui;
};

#endif // QTWIDGETSAPPLICATION2_H
