#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication2.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QtWidgetsApplication2Class; };
QT_END_NAMESPACE

class QtWidgetsApplication2 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication2(QWidget *parent = nullptr);
    ~QtWidgetsApplication2();

    void openImageDialog();
    void clearImage();
    void applyGaussianNoise();
    void applySaltAndPepperNoise();
    void apply_GaussianFilter();
    void apply_AverageFilter();
    void apply_MedianFilter();
    void plot_Histogram();
    //cv::Mat calculateHistogram();
    void applyUniformNoise();


private:
    Ui::QtWidgetsApplication2Class *ui;
};
