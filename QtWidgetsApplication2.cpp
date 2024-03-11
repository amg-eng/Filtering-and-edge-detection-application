#include "QtWidgetsApplication2.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <random>
#include <iostream>
#include "source_code.h"
#include "histograms.h"
#include <QFileDialog>

using namespace cv;

//// Your function to add Gaussian noise
//Mat addGaussianNoise(const Mat& image, double mean = 0, double stddev = 25);

Mat image;
QString fileName;

Mat image_hist;



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

    connect(ui->pushButton, &QPushButton::clicked, this, &QtWidgetsApplication2::plot_Histogram);
    


    connect(ui->open_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::openImageDialog);
    connect(ui->clear_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::clearImage);

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
        image_hist = imread(fileName.toStdString());
        if (!image.empty()) {
            // Convert the OpenCV Mat image to QImage with the same color format
            QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); // Assuming OpenCV loads in BGR format
            if (qImage.isNull()) {
                std::cerr << "Error: Unable to convert image to QImage." << std::endl;
                return;
            }



            // Display the image in the input_image_label QLabel
            QPixmap pixmap = QPixmap::fromImage(qImage);
            ui->input_image_label->setPixmap(pixmap.scaled(ui->input_image_label->size(), Qt::KeepAspectRatio));
            ui->input_image_label->setAlignment(Qt::AlignCenter);

        }
        else {
            std::cerr << "Error: Unable to load image: " << fileName.toStdString() << std::endl;
        }

        if (!image_hist.empty()) {
            // Convert the OpenCV Mat image to QImage with the same color format
            QImage qImage1(image_hist.data, image_hist.cols, image_hist.rows, image_hist.step, QImage::Format_BGR888); // Assuming OpenCV loads in BGR format
            if (qImage1.isNull()) {
                std::cerr << "Error: Unable to convert image to QImage." << std::endl;
                return;
            }



            // Display the image in the input_image_label QLabel
            QPixmap pixmap1 = QPixmap::fromImage(qImage1);
            ui->inputLabel_4->setPixmap(pixmap1.scaled(ui->inputLabel_4->size(), Qt::KeepAspectRatio));
            ui->inputLabel_4->setAlignment(Qt::AlignCenter);

        }
        else {
            std::cerr << "Error: Unable to load image: " << fileName.toStdString() << std::endl;
        }
    }
}

void QtWidgetsApplication2::clearImage() {
    // Reset the image variable to its original state (clear any noise or filters)
    image = imread(fileName.toStdString());

    // Check if the image was loaded successfully
    if (!image.empty()) {
        // Convert the OpenCV Mat image to QImage with the same color format
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); // Assuming OpenCV loads in BGR format
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }

        // Display the image in the input_image_label QLabel
        QPixmap pixmap = QPixmap::fromImage(qImage);
        ui->input_image_label->setPixmap(pixmap.scaled(ui->input_image_label->size(), Qt::KeepAspectRatio));
        ui->input_image_label->setAlignment(Qt::AlignCenter);

        // Clear the filtered image label
        ui->output_filtered_image_label->clear();
    }
    else {
        std::cerr << "Error: Unable to reload the original image." << std::endl;
    }
}

void QtWidgetsApplication2::applyGaussianNoise()
{
    // Check if the image has been loaded
    if (!image.empty()) {
        // Apply Gaussian noise to the image
        Mat noisyImage = addGaussianNoise(image, 0, 25); // Mean = 0, Standard Deviation = 25
        image = noisyImage;

        // Convert the OpenCV Mat image to QImage with the same color format
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); // Assuming OpenCV loads in BGR format
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }


            QPixmap pixmap = QPixmap::fromImage(qImage);
            ui->input_image_label->setPixmap(pixmap.scaled(ui->input_image_label->size(), Qt::KeepAspectRatio));
            ui->input_image_label->setAlignment(Qt::AlignCenter);
    }
    else {
        std::cerr << "Error: No image loaded." << std::endl;
    }
}

void QtWidgetsApplication2::applyUniformNoise()
{
    // Check if the image has been loaded
    if (!image.empty()) {
        // Apply uniform noise to the image
        Mat noisyImage = addUniformNoise(image, 0, 50); // Low = 0, High = 50
        image = noisyImage;

        // Convert the OpenCV Mat image to QImage with the same color format
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); // Assuming OpenCV loads in BGR format
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }

            QPixmap pixmap = QPixmap::fromImage(qImage);
            ui->input_image_label->setPixmap(pixmap.scaled(ui->input_image_label->size(), Qt::KeepAspectRatio));
            ui->input_image_label->setAlignment(Qt::AlignCenter);
    }
    else {
        std::cerr << "Error: No image loaded." << std::endl;
    }
}

void QtWidgetsApplication2::applySaltAndPepperNoise()
{
    // Check if the image has been loaded
    if (!image.empty()) {
        // Apply salt and pepper noise to the image
        Mat noisyImage = addSaltAndPepperNoise(image, 0.01, 0.01); // Salt density = 0.01, Pepper density = 0.01
        image = noisyImage;

        // Convert the OpenCV Mat image to QImage with the same color format
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); // Assuming OpenCV loads in BGR format
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }

        // Display the image in the input_image_label QLabel
        QPixmap pixmap = QPixmap::fromImage(qImage);
        ui->input_image_label->setPixmap(pixmap.scaled(ui->input_image_label->size(), Qt::KeepAspectRatio));
        ui->input_image_label->setAlignment(Qt::AlignCenter);

    }
    else {
        std::cerr << "Error: No image loaded." << std::endl;
    }
}

void QtWidgetsApplication2::apply_GaussianFilter()
{
    // Check if the image has been loaded
    if (!image.empty()) {

        Mat noisyImage = applyGaussianFilter(image, 5, 1.0); 
        image = noisyImage;

        // Convert the OpenCV Mat image to QImage with the same color format
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); // Assuming OpenCV loads in BGR format
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }
        // Display the image in the input_image_label QLabel
        QPixmap pixmap = QPixmap::fromImage(qImage);
        ui->output_filtered_image_label->setPixmap(pixmap.scaled(ui->input_image_label->size(), Qt::KeepAspectRatio));
        ui->output_filtered_image_label->setAlignment(Qt::AlignCenter);

    }
    else {
        std::cerr << "Error: No image loaded." << std::endl;
    }
}

void QtWidgetsApplication2::apply_AverageFilter()
{
    // Check if the image has been loaded
    if (!image.empty()) {

        Mat noisyImage = applyAverageFilter(image, 3);
        image = noisyImage;

        // Convert the OpenCV Mat image to QImage with the same color format
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); // Assuming OpenCV loads in BGR format
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }

        // Display the image in the input_image_label QLabel
        QPixmap pixmap = QPixmap::fromImage(qImage);
        ui->output_filtered_image_label->setPixmap(pixmap.scaled(ui->input_image_label->size(), Qt::KeepAspectRatio));
        ui->output_filtered_image_label->setAlignment(Qt::AlignCenter);

    }
    else {
        std::cerr << "Error: No image loaded." << std::endl;
    }
}

void QtWidgetsApplication2::apply_MedianFilter()
{
    // Check if the image has been loaded
    if (!image.empty()) {

        Mat noisyImage = applyMedianFilter(image, 3);
        image = noisyImage;

        // Convert the OpenCV Mat image to QImage with the same color format
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); // Assuming OpenCV loads in BGR format
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }

        // Display the image in the input_image_label QLabel
        QPixmap pixmap = QPixmap::fromImage(qImage);
        ui->output_filtered_image_label->setPixmap(pixmap.scaled(ui->input_image_label->size(), Qt::KeepAspectRatio));
        ui->output_filtered_image_label->setAlignment(Qt::AlignCenter);

    }
    else {
        std::cerr << "Error: No image loaded." << std::endl;
    }
}





void QtWidgetsApplication2::plot_Histogram()
{
    // Check if the image has been loaded
    if (!image_hist.empty()) {
        // Calculate histogram
        Mat hist = calculateHistogram(image_hist, 0); // Assuming channel 0 for grayscale image

        // Plot histogram with blue color
        Mat histogramImage = plotHistogram(hist, Scalar(0, 0, 255)); // Scalar(Blue, Green, Red)

        // Convert the OpenCV Mat image to QImage with the same color format
        QImage qImage(histogramImage.data, histogramImage.cols, histogramImage.rows, histogramImage.step, QImage::Format_BGR888); // Assuming OpenCV loads in BGR format
        if (qImage.isNull()) {
            std::cerr << "Error: Unable to convert image to QImage." << std::endl;
            return;
        }

        // Display the histogram in the inputLabel_4 QLabel
        QPixmap pixmap1 = QPixmap::fromImage(qImage);
        ui->label_29->setPixmap(pixmap1.scaled(ui->label_29->size(), Qt::KeepAspectRatio));
        ui->label_29->setAlignment(Qt::AlignCenter);



    }
    else {
        std::cerr << "Error: No image loaded." << std::endl;
    }
}

