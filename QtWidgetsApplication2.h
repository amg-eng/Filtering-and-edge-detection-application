#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication2.h"
#include "edgeDetection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QtWidgetsApplication2Class; };
QT_END_NAMESPACE

class QtWidgetsApplication2 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication2(QWidget* parent = nullptr);
    ~QtWidgetsApplication2();

    void clearImage();
    void onSlider3_ValueChanged(int value);
    void apply_hybrid();
    void apply_hybrid_swap();
    void equalize_Histogram();
    void Normalize_Histogram();
    void applyGaussianNoise();
    void applySaltAndPepperNoise();
    void onRadioButtonClicked();
    void onRadioButton3x3Clicked();
    void apply_GaussianFilter();
    void apply_AverageFilter();
    void apply_MedianFilter();
    void applyUniformNoise();
    // void displayImageOnLabel();
    void openImageDialog();
    void displayImageInLabel(const cv::Mat& image, QLabel* label);
    void displayImageInLabel_32(const cv::Mat& image, QLabel* label);
    void displayOutputImage(const cv::Mat& image, QLabel* label);

    void displayOutputImageTest(const cv::Mat& gradX, const cv::Mat& gradY, const cv::Mat& gradient, QLabel* labelX, QLabel* labelY, QLabel* labelGradient);
    void displayOutput_ft_Image(const cv::Mat& image, QLabel* label);

    void displayOutput_gray_Image(const cv::Mat& image, QLabel* label);

    void onSobelClicked();
    void onPrewittClicked();
    void onRobertsClicked();

    void applyCannyFilter();

    void plot_Histogram();
    void global_Thresholding();
    void local_AdaptiveMeanThreshold();
    void display_NormalizedImages();
    void Local_Gaussian();
    void Padding_Threshold();
    void Mean_Threshold();
    void onHybrid_RadioButton_1Clicked();
    void onHybrid_RadioButton_2Clicked();
    void onSlider2_ValueChanged(int value);
    void onSlider1_ValueChanged(int value);
    void open_ImageDialog();
    void open_ImageDialog2();
    //void updateImage(cv::Mat& inputMat, QLabel* image, bool rgb_flag);
//void adaptive_PaddingFunction();
//void plot_Histogram_cdf();
//void onSliderValueChanged(int value);


//void display_i_mg_in__label(cv::Mat img, QLabel* label);

private:
    Ui::QtWidgetsApplication2Class* ui;

};
