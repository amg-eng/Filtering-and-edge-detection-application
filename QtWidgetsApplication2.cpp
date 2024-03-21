#include "QtWidgetsApplication2.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "source_code.h"
#include "histograms.h"
#include "edgeDetection.h"
#include "thresholding.h"
#include "Hybrid.h"
#include <QFileDialog>
#include "histogramFun.h"

using namespace cv;

int kernelSize = 3;
Mat img_detection;
Mat image;
Mat gray_image;
Mat gray_image_noise;
QString fileName;

Mat image1_hybrid, image2_hybrid;
Mat gray_image1_hybrid, gray_image2_hybrid;
String type_filter_img1 = "low";
String type_filter_img2 = "low";
int d1 = 0;
int d2 = 0;
int d3 = 0;

bool swap_flag = 1;


QtWidgetsApplication2::QtWidgetsApplication2(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::QtWidgetsApplication2Class())
{
    ui->setupUi(this);

    // Connect the open_btn button to open an image dialog
    connect(ui->apply_uniform_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::applyUniformNoise);
    connect(ui->apply_salt_pepper_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::applySaltAndPepperNoise);
    connect(ui->apply_gaussian, &QPushButton::clicked, this, &QtWidgetsApplication2::applyGaussianNoise);
    connect(ui->applyGaussianFilter_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::apply_GaussianFilter);
    connect(ui->applyAverageFilter_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::apply_AverageFilter);
    connect(ui->applyMedianFilter_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::apply_MedianFilter);

    /* -------------------------        Threshold         ---------------*/
    connect(ui->pushButton_10, &QPushButton::clicked, this, &QtWidgetsApplication2::global_Thresholding);
    connect(ui->Local_Mean_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::local_AdaptiveMeanThreshold);
    connect(ui->Gussian_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::Local_Gaussian);
    connect(ui->Padding_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::Padding_Threshold);

    /*-------------------------        Open & Clear       ---------------*/
    connect(ui->open_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::openImageDialog);
    connect(ui->clear_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::clearImage);
    connect(ui->sobel, &QPushButton::clicked, this, &QtWidgetsApplication2::onSobelClicked);
    connect(ui->prewitt, &QPushButton::clicked, this, &QtWidgetsApplication2::onPrewittClicked);
    connect(ui->robert, &QPushButton::clicked, this, &QtWidgetsApplication2::onRobertsClicked);
    connect(ui->canny, &QPushButton::clicked, this, &QtWidgetsApplication2::applyCannyFilter);

    /*-------------------------        Radio_button_for_filters       ---------------*/
    connect(ui->radioButton_3x3, &QRadioButton::clicked, this, &QtWidgetsApplication2::onRadioButtonClicked);
    connect(ui->radioButton_5x5, &QRadioButton::clicked, this, &QtWidgetsApplication2::onRadioButtonClicked);
    connect(ui->radioButton_11x11, &QRadioButton::clicked, this, &QtWidgetsApplication2::onRadioButtonClicked);

    /*-------------------------        Hybrid            ---------------*/
    connect(ui->open_image_1, &QPushButton::clicked, this, &QtWidgetsApplication2::open_ImageDialog);
    connect(ui->open_image_2, &QPushButton::clicked, this, &QtWidgetsApplication2::open_ImageDialog2);
    connect(ui->Slider_1, &QSlider::valueChanged, this, &QtWidgetsApplication2::onSlider1_ValueChanged);
    connect(ui->Low_Pass_radio1, &QRadioButton::clicked, this, &QtWidgetsApplication2::onHybrid_RadioButton_1Clicked);
    connect(ui->High_Pass_radio1, &QRadioButton::clicked, this, &QtWidgetsApplication2::onHybrid_RadioButton_1Clicked);
    connect(ui->Slider_2, &QSlider::valueChanged, this, &QtWidgetsApplication2::onSlider2_ValueChanged);
    connect(ui->Low_Pass_radio2, &QRadioButton::clicked, this, &QtWidgetsApplication2::onHybrid_RadioButton_2Clicked);
    connect(ui->High_Pass_radio2, &QRadioButton::clicked, this, &QtWidgetsApplication2::onHybrid_RadioButton_2Clicked);
    connect(ui->hybrid_slider_mix, &QSlider::valueChanged, this, &QtWidgetsApplication2::onSlider3_ValueChanged);
    connect(ui->Swap_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::apply_hybrid_swap);

}

QtWidgetsApplication2::~QtWidgetsApplication2()
{
    delete ui;
}

void QtWidgetsApplication2::openImageDialog()
{
    // Open a file dialog to select an image file
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (!fileName.isEmpty()) {
        // Read the selected image using OpenCV
        image = imread(fileName.toStdString());
        img_detection = image;
        gray_image = convertToGrayScale(img_detection);
        // gray image for noises
        gray_image_noise = convertToGrayScale(img_detection);

        if (!image.empty()) {

            // Call the function to display the image in the label
            // swapped by fault
            Normalize_Histogram();
            equalize_Histogram();
            plot_Histogram();
            Mat equalize = equalizeHistogram(gray_image);
            Mat Normalize = normalizeImage(gray_image);
            displayOutputImage(Normalize, ui->inputLabel_9);
            displayOutputImage(equalize, ui->inputLabel_10);



            displayImageInLabel(image, ui->input_image_label);
            displayImageInLabel(image, ui->input_edge);
            displayImageInLabel(image, ui->inputLabel_4);
            displayOutputImage(gray_image, ui->Gray_Scaled_Image);
            displayOutputImage(gray_image_noise, ui->input_gray);
            displayOutputImage(gray_image, ui->label_72);
            displayImageInLabel(image, ui->inputLabel);

        }
        else {
            std::cerr << "Error: Unable to load image: " << fileName.toStdString() << std::endl;
        }
    }
}

void QtWidgetsApplication2::displayImageInLabel(const cv::Mat& image, QLabel* label)
{
    if (!image.empty()) {
        // Convert the OpenCV Mat image to QImage with the same color format
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); // Assuming OpenCV loads in BGR format
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }

        // Display the image in the specified QLabel
        QPixmap pixmap = QPixmap::fromImage(qImage);
        label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio));
        label->setAlignment(Qt::AlignCenter);
    }
}

void QtWidgetsApplication2::displayImageInLabel_32(const cv::Mat& image, QLabel* label)
{
    if (!image.empty()) {
        // Convert the OpenCV Mat image to QImage with the same color format
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB32); // Assuming OpenCV loads in BGR format
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }

        // Display the image in the specified QLabel
        QPixmap pixmap = QPixmap::fromImage(qImage);
        label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio));
        label->setAlignment(Qt::AlignCenter);
    }
}

void QtWidgetsApplication2::displayOutputImage(const cv::Mat& image, QLabel* label)
{
    if (!image.empty()) {
        cv::Mat displayImage;

        // Convert grayscale image to RGB for displaying
        if (image.channels() == 1) {
            cv::cvtColor(image, displayImage, cv::COLOR_GRAY2RGB);
        }
        else {
            displayImage = image.clone();
        }

        // Convert the OpenCV Mat image to QImage
        QImage qImage(displayImage.data, displayImage.cols, displayImage.rows, displayImage.step, QImage::Format_RGB888);
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }

        // Display the image in the specified QLabel
        QPixmap pixmap = QPixmap::fromImage(qImage);
        label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio));
        label->setAlignment(Qt::AlignCenter);
    }
    else {
        std::cerr << "Error: Image is empty." << std::endl;
    }
}

void QtWidgetsApplication2::displayOutputImageTest(const cv::Mat& gradX, const cv::Mat& gradY, const cv::Mat& gradient, QLabel* labelX, QLabel* labelY, QLabel* labelGradient)
{
    // Create images for display
    cv::Mat displayX, displayY, displayGradient;
    gradX.convertTo(displayX, CV_8U);
    gradY.convertTo(displayY, CV_8U);
    gradient.convertTo(displayGradient, CV_8U);

    // Resize the images to match the label size
    cv::resize(displayX, displayX, cv::Size(labelX->width(), labelX->height()));
    cv::resize(displayY, displayY, cv::Size(labelY->width(), labelY->height()));
    cv::resize(displayGradient, displayGradient, cv::Size(labelGradient->width(), labelGradient->height()));

    // Convert the images to 8-bit grayscale
    if (displayX.channels() > 1) {
        cv::cvtColor(displayX, displayX, cv::COLOR_BGR2GRAY);
    }
    if (displayY.channels() > 1) {
        cv::cvtColor(displayY, displayY, cv::COLOR_BGR2GRAY);
    }
    if (displayGradient.channels() > 1) {
        cv::cvtColor(displayGradient, displayGradient, cv::COLOR_BGR2GRAY);
    }

    // Convert the OpenCV Mat images to QImage
    QImage qImageX(displayX.data, displayX.cols, displayX.rows, displayX.step, QImage::Format_Grayscale8);
    QImage qImageY(displayY.data, displayY.cols, displayY.rows, displayY.step, QImage::Format_Grayscale8);
    QImage qImageGradient(displayGradient.data, displayGradient.cols, displayGradient.rows, displayGradient.step, QImage::Format_Grayscale8);

    // Set QPixmap for each label
    QPixmap pixmapX = QPixmap::fromImage(qImageX);
    QPixmap pixmapY = QPixmap::fromImage(qImageY);
    QPixmap pixmapGradient = QPixmap::fromImage(qImageGradient);

    // Set Pixmaps to labels
    labelX->setPixmap(pixmapX);
    labelY->setPixmap(pixmapY);
    labelGradient->setPixmap(pixmapGradient);

    // Set alignment for labels
    labelX->setAlignment(Qt::AlignCenter);
    labelY->setAlignment(Qt::AlignCenter);
    labelGradient->setAlignment(Qt::AlignCenter);
}

void QtWidgetsApplication2::displayOutput_ft_Image(const cv::Mat& image, QLabel* label)
{
    if (!image.empty()) {
        cv::Mat displayImage;

        // Convert grayscale image to RGB for displaying
        if (image.channels() == 1) {
            cv::cvtColor(image, displayImage, cv::COLOR_GRAY2RGB);
        }
        else {
            displayImage = image.clone();
        }

        // Convert the OpenCV Mat image to QImage
        QImage qImage(displayImage.data, displayImage.cols, displayImage.rows, displayImage.step, QImage::Format_RGB888);
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }

        // Display the image in the specified QLabel
        QPixmap pixmap = QPixmap::fromImage(qImage);
        label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio));
        label->setAlignment(Qt::AlignCenter);
    }
    else {
        std::cerr << "Error: Image is empty." << std::endl;
    }
}

void QtWidgetsApplication2::displayOutput_gray_Image(const cv::Mat& image, QLabel* label)
{
    if (!image.empty()) {
        cv::Mat displayImage;

        // Convert grayscale image to RGB for displaying
        if (image.channels() == 1) {
            cv::cvtColor(image, displayImage, cv::COLOR_GRAY2RGB);
        }
        else {
            displayImage = image.clone();
        }

        // Convert the OpenCV Mat image to QImage
        QImage qImage(displayImage.data, displayImage.cols, displayImage.rows, displayImage.step, QImage::Format_BGR888);
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }

        // Display the image in the specified QLabel
        QPixmap pixmap = QPixmap::fromImage(qImage);
        label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio));
        label->setAlignment(Qt::AlignCenter);
    }
    else {
        std::cerr << "Error: Image is empty." << std::endl;
    }
}

void QtWidgetsApplication2::clearImage() {
    // Reset the image variable to its original state (clear any noise or filters)
    image = imread(fileName.toStdString());
    gray_image_noise = convertToGrayScale(image);

    // Check if the image was loaded successfully
    if (!image.empty()) {

        displayImageInLabel(image, ui->input_image_label);
        displayOutputImage(gray_image_noise, ui->input_gray);

        ui->output_filtered_image_label->clear();
        ui->output_gray->clear();
    }
}

void QtWidgetsApplication2::applyGaussianNoise()
{
    // Check if the image has been loaded
    if (!image.empty()) {
        // Apply Gaussian noise to the image
        Mat noisyImage = addGaussianNoise(image, 0, 25,"c"); // Mean = 0, Standard Deviation = 25
        Mat noisyImage_gray = addGaussianNoise(gray_image_noise, 0, 25,"g"); // Mean = 0, Standard Deviation = 25
        image = noisyImage;
        gray_image_noise = noisyImage_gray;

        displayImageInLabel(image, ui->output_filtered_image_label);
        displayOutputImage(gray_image_noise, ui->output_gray);
    }

}

void QtWidgetsApplication2::applyUniformNoise()
{
    if (!image.empty()) {
        Mat noisyImage = addUniformNoise(image, 0, 50,"c");
        Mat noisyImage_gray = addUniformNoise(gray_image_noise, 0, 50,"g");
        image = noisyImage;
        gray_image_noise = noisyImage_gray;

        displayImageInLabel(image, ui->output_filtered_image_label);
        displayOutputImage(gray_image_noise, ui->output_gray);
    }
}

void QtWidgetsApplication2::applySaltAndPepperNoise()
{
    if (!image.empty()) {
        Mat noisyImage = addSaltAndPepperNoise(image, 0.01, 0.01,"c");
        Mat noisyImage_gray = addSaltAndPepperNoise(gray_image_noise, 0.01, 0.01,"g");
        image = noisyImage;
        gray_image_noise = noisyImage_gray;

        displayImageInLabel(image, ui->output_filtered_image_label);
        displayOutputImage(gray_image_noise, ui->output_gray);
    }
}

void QtWidgetsApplication2::onRadioButtonClicked()
{
    if (ui->radioButton_3x3->isChecked()) {
        kernelSize = 3;
    }
    else if (ui->radioButton_5x5->isChecked()) {
        kernelSize = 5;
    }
    else if (ui->radioButton_11x11->isChecked()) {
        kernelSize = 11;
    }

}

void QtWidgetsApplication2::apply_GaussianFilter()
{
    // Check if the image has been loaded
    if (!image.empty()) {
        Mat noisyImage = applyGaussianFilter(image, kernelSize, 1.0,"c");
        image = noisyImage;

        Mat noisyImage_gray = applyGaussianFilter(gray_image_noise, kernelSize, 1.0,"g");
        gray_image_noise = noisyImage_gray;

        displayImageInLabel(image, ui->output_filtered_image_label);
        displayOutputImage(gray_image_noise, ui->output_gray);

    }
    else {
        std::cerr << "Error: No image loaded." << std::endl;
    }
}

void QtWidgetsApplication2::apply_AverageFilter()
{
    // Check if the image has been loaded
    if (!image.empty()) {

        Mat noisyImage = applyAverageFilter(image, 3,"c");
        image = noisyImage;

        Mat noisyImage_gray = applyAverageFilter(gray_image_noise, 3,"g");
        gray_image_noise = noisyImage_gray;

        displayImageInLabel(image, ui->output_filtered_image_label);
        displayOutputImage(gray_image_noise, ui->output_gray);
    }
}

void QtWidgetsApplication2::apply_MedianFilter()
{
    // Check if the image has been loaded
    if (!image.empty()) {

        Mat noisyImage = applyMedianFilter(image, 3,"c");
        image = noisyImage;

        Mat noisyImage_gray = applyMedianFilter(gray_image_noise, 3,"g");
        gray_image_noise = noisyImage_gray;

        displayImageInLabel(image, ui->output_filtered_image_label);
        displayOutputImage(gray_image_noise, ui->output_gray);
    }
}

/*---------------------------------------------------------------- Edge Detection ----------------------------------------------------------------------------*/


void QtWidgetsApplication2::onSobelClicked()
{
    std::tuple<cv::Mat, cv::Mat, cv::Mat> result = applySobel(img_detection);
    displayOutputImageTest(std::get<0>(result), std::get<1>(result), std::get<2>(result), ui->output_edge_x, ui->output_edge_y, ui->output_edge);
}

void QtWidgetsApplication2::onPrewittClicked()
{
    std::tuple<cv::Mat, cv::Mat, cv::Mat> result = applyPrewitt(img_detection);
    displayOutputImageTest(std::get<0>(result), std::get<1>(result), std::get<2>(result), ui->output_edge_x, ui->output_edge_y, ui->output_edge);
}

void QtWidgetsApplication2::onRobertsClicked()
{
    std::tuple<cv::Mat, cv::Mat, cv::Mat> result = applyRoberts(img_detection);
    displayOutputImageTest(std::get<0>(result), std::get<1>(result), std::get<2>(result), ui->output_edge_x, ui->output_edge_y, ui->output_edge);
}


void QtWidgetsApplication2::applyCannyFilter()
{
    int lowThreshold = ui->lowThreshold->text().toInt();
    int highThreshold = ui->highThreshold->text().toInt();

    cv::Mat gradX, gradY, cannyEdges;
    std::tie(gradX, gradY, cannyEdges) = Detect_Edges_Canny(img_detection, lowThreshold, highThreshold);

    displayOutputImageTest(gradX, gradY, cannyEdges, ui->output_edge_x, ui->output_edge_y, ui->output_edge);
}


/*---------------------------------------------------------------- Histogram ----------------------------------------------------------------------------*/

void QtWidgetsApplication2::plot_Histogram()
{
    // red
    Mat histR, histG, histB;
    Mat histImageR, histImageG, histImageB;
    int histSize = 256;
    int histWidth = 600, histHeight = 400;
    double maxIntensity = 0;
    calculateHistograms(image, histR, histG, histB);
    drawHistograms(histR, histG, histB, histImageR, histImageG, histImageB, histSize, maxIntensity);
    addLabels(histImageR, histImageG, histImageB, histR.rows, maxIntensity, histHeight, histWidth);
    saveHistogramImage("red_df.jpg", histImageR);
    saveHistogramImage("green_df.jpg", histImageG);
    saveHistogramImage("blue_df.jpg", histImageB);
    Mat image_hist_df_red = imread("red_df.jpg");
    Mat image_hist_df_green = imread("green_df.jpg");
    Mat image_hist_df_blue = imread("blue_df.jpg");

    displayImageInLabel(image_hist_df_red, ui->label_29);
    displayImageInLabel(image_hist_df_green, ui->label_33);
    displayImageInLabel(image_hist_df_blue, ui->label_31);

    /*************   CDF   ***********/
    Mat red_image_histogram = calculateHistogram(img_detection, 2);
    Mat hist_red = plotHistogram(red_image_histogram, Scalar(0, 0, 255), 2000, 1700);
 
    Mat CDF_RED = calculateCDF(red_image_histogram);
    Mat CDFR_PLOT = plotCDF(CDF_RED, Scalar(0, 0, 255));
    Mat axes_hist_red_cdf = addAxesToHistogram(CDFR_PLOT);
    saveHistogramImage("R_hist_cdf.jpg", axes_hist_red_cdf);
    Mat image_hist_cdf_red = imread("R_hist_cdf.jpg");
    displayImageInLabel(image_hist_cdf_red, ui->label_35);






    //// green
    Mat green_image_histogram = calculateHistogram(img_detection, 1);
    Mat hist_green = plotHistogram(green_image_histogram, Scalar(0, 255, 0), 2000, 1700);
    Mat CDF_GREEN = calculateCDF(green_image_histogram);
    Mat CDFG_PLOT = plotCDF(CDF_GREEN, Scalar(0, 255, 0));
    Mat axes_hist_green_cdf = addAxesToHistogram(CDFG_PLOT);
    saveHistogramImage("G_hist_cdf.jpg", axes_hist_green_cdf);
    Mat image_hist_green_cdf = imread("G_hist_cdf.jpg");
    displayImageInLabel(image_hist_green_cdf, ui->label_39);



    //// blue
    Mat blue_image_histogram = calculateHistogram(img_detection, 0);
    Mat hist_blue = plotHistogram(blue_image_histogram, Scalar(255, 0, 0), 2000, 1700);
    Mat CDF_BLUE = calculateCDF(blue_image_histogram);
    Mat CDFB_PLOT = plotCDF(CDF_BLUE, Scalar(255, 0, 0));
    Mat axes_hist_blue_cdf = addAxesToHistogram(CDFB_PLOT);
    saveHistogramImage("B_hist_cdf.jpg", axes_hist_blue_cdf);
    Mat image_hist_blue_cdf = imread("B_hist_cdf.jpg");
    displayImageInLabel(image_hist_blue_cdf, ui->label_37);


}


/*---------------------------------------------------------------- Threshold ----------------------------------------------------------------------------*/

void QtWidgetsApplication2::global_Thresholding()
{
    // Check if the image has been loaded
    if (!image.empty()) {
        // Apply Gaussian noise to the image
        Mat global_Threshold_gray = globalThreshold(gray_image, 128, 255, 0);
        displayOutputImage(global_Threshold_gray, ui->grayscale_output);

    }

}

void QtWidgetsApplication2::local_AdaptiveMeanThreshold()
{
    // Check if the image has been loaded
    if (!image.empty()) {
        // Apply Gaussian noise to the image
        Mat Mean_Threshold_gray = localAdaptiveMeanThreshold(gray_image, 5, 2, 255, 0);
        displayOutputImage(Mean_Threshold_gray, ui->grayscale_output);

    }

}

void QtWidgetsApplication2::display_NormalizedImages()
{
    // Check if the image has been loaded
    if (!image.empty()) {
        // Apply Gaussian noise to the image
        Mat Mean_Threshold_gray = displayNormalizedEqualizedImages(gray_image);
        displayOutputImage(Mean_Threshold_gray, ui->grayscale_output);

    }

}

void QtWidgetsApplication2::Local_Gaussian()
{
    // Check if the image has been loaded
    if (!image.empty()) {
        Mat image_t;

        Mat blur_img = gaussianBlur(img_detection);
        Mat res = gaussianThreshold(blur_img, 127);
        displayOutput_gray_Image(res, ui->grayscale_output);


    }

}

void QtWidgetsApplication2::Padding_Threshold()
{
    // Check if the image has been loaded
    if (!image.empty()) {
        // Apply Gaussian noise to the image
        Mat Padding_Threshold_gray = adaptivePaddingFunction(gray_image, 11);
        displayOutputImage(Padding_Threshold_gray, ui->grayscale_output);

    }

}

/*--------------------------------------------------        Hybrid     -------------------------------------*/

void QtWidgetsApplication2::onHybrid_RadioButton_1Clicked()
{
    if (ui->Low_Pass_radio1->isChecked()) {
        type_filter_img1 = "low";
    }
    else if (ui->High_Pass_radio1->isChecked()) {
        type_filter_img1 = "high";

    }
    Mat ft_img = image_fourier_filter(gray_image1_hybrid, type_filter_img1, d1);
    Mat res = normalize_image(ft_img);

    displayOutput_ft_Image(res, ui->output_img1);
}

void QtWidgetsApplication2::onSlider1_ValueChanged(int value)
{

    d1 = value;
    Mat ft_img = image_fourier_filter(gray_image1_hybrid, type_filter_img1, d1);
    Mat res = normalize_image(ft_img);

    displayOutput_ft_Image(res, ui->output_img1);
}

void QtWidgetsApplication2::open_ImageDialog()
{
    // Open a file dialog to select an image file
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (!fileName.isEmpty()) {
        // Read the selected image using OpenCV
        image1_hybrid = imread(fileName.toStdString());
        gray_image1_hybrid = convertToGrayScale(image1_hybrid);

        displayOutputImage(gray_image1_hybrid, ui->hybrid_img1);
    }
    // Assuming the slider is named Slider_1

    ui->Slider_1->setMinimum(0); // Set the minimum value
    ui->Slider_1->setMaximum(gray_image1_hybrid.rows * .07); // Set the maximum value


    Mat ft_img = image_fourier_filter(gray_image1_hybrid, type_filter_img1, d1);
    Mat res = normalize_image(ft_img);

    /*displayOutput_ft_Image(res, ui->output_img1);*/
    displayOutput_ft_Image(res, ui->output_img1);
}

void QtWidgetsApplication2::onHybrid_RadioButton_2Clicked()
{
    if (ui->Low_Pass_radio2->isChecked()) {
        type_filter_img2 = "low";
    }
    else if (ui->High_Pass_radio2->isChecked()) {
        type_filter_img2 = "high";

    }
    Mat ft_img = image_fourier_filter(gray_image2_hybrid, type_filter_img2, d2);
    Mat res = normalize_image(ft_img);

    displayOutput_ft_Image(res, ui->output_img2);
}

void QtWidgetsApplication2::onSlider2_ValueChanged(int value)
{

    d2 = value;
    Mat ft_img = image_fourier_filter(gray_image2_hybrid, type_filter_img2, d2);
    Mat res = normalize_image(ft_img);

    displayOutput_ft_Image(res, ui->output_img2);
}

void QtWidgetsApplication2::open_ImageDialog2()
{
    // Open a file dialog to select an image file
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (!fileName.isEmpty()) {
        // Read the selected image using OpenCV
        image2_hybrid = imread(fileName.toStdString());
        gray_image2_hybrid = convertToGrayScale(image2_hybrid);

        displayOutputImage(gray_image2_hybrid, ui->hybrid_img_2);
    }

    ui->Slider_2->setMinimum(0); // Set the minimum value
    ui->Slider_2->setMaximum(gray_image2_hybrid.rows * 0.07); // Set the maximum value


    Mat ft_img = image_fourier_filter(gray_image2_hybrid, type_filter_img2, d2);
    Mat res = normalize_image(ft_img);

    /*displayOutput_ft_Image(res, ui->output_img1);*/
    displayOutput_ft_Image(res, ui->output_img2);

}

void QtWidgetsApplication2::onSlider3_ValueChanged(int value)
{

    if (!(gray_image1_hybrid.empty() && gray_image2_hybrid.empty())) {
        if (swap_flag == 1) {
            d3 = value;
            Mat hybrid_mix = hybrid_image(gray_image2_hybrid, gray_image1_hybrid, d3);
            Mat res = normalize_image(hybrid_mix);

            displayOutput_ft_Image(res, ui->output_label_hybrid);
        }
        else {
            d3 = value;
            Mat hybrid_mix = hybrid_image(gray_image1_hybrid, gray_image2_hybrid, d3);
            Mat res = normalize_image(hybrid_mix);

            displayOutput_ft_Image(res, ui->output_label_hybrid);

        }
    }
}

void QtWidgetsApplication2::apply_hybrid_swap()
{
    swap_flag = !swap_flag;

    if (!(gray_image1_hybrid.empty() && gray_image2_hybrid.empty())) {
        if (swap_flag == 1) {
            Mat hybrid_mix = hybrid_image(gray_image2_hybrid, gray_image1_hybrid, d3);
            Mat res = normalize_image(hybrid_mix);

            displayOutput_ft_Image(res, ui->output_label_hybrid);
        }
        else {
            Mat hybrid_mix = hybrid_image(gray_image1_hybrid, gray_image2_hybrid, d3);
            Mat res = normalize_image(hybrid_mix);

            displayOutput_ft_Image(res, ui->output_label_hybrid);

        }
    }


}

void QtWidgetsApplication2::equalize_Histogram()
{
    // Check if the image has been loaded
    if (!image.empty()) {

        Mat equalize = equalizeHistogram(img_detection);

        displayImageInLabel(equalize, ui->Gray_Scaled_Image_6);

    }
}

void QtWidgetsApplication2::Normalize_Histogram()
{
    // Check if the image has been loaded
    if (!image.empty()) {
        Mat equalize = equalizeHistogram(gray_image);
        displayImageInLabel(equalize, ui->inputLabel_10);

        Mat calc_hist = calculateHistogram(gray_image, 0);
        Mat norm_img = normalizeHistogram_n(calc_hist);
        Mat hist = plotHistogram(norm_img, Scalar(0), 720, 400);
        Mat axes_hist = addAxesToHistogram(hist);

        saveHistogramImage("Image_1.jpg", axes_hist);

        Mat image_hist = imread("Image_1.jpg");
        displayImageInLabel(image_hist, ui->Gray_Scaled_Image_5);

    }
}





