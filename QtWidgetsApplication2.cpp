#include "QtWidgetsApplication2.h"
#include <opencv2/imgproc/imgproc.hpp> // Include for Canny

#include "./ui_QtWidgetsApplication2.h"
#include <iostream>
using namespace cv;

//// Your function to add Gaussian noise
//Mat addGaussianNoise(const Mat& image, double mean = 0, double stddev = 25
Mat img_detection;
Mat image;
QString fileName;


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

    connect(ui->open_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::openImageDialog);
    connect(ui->clear_btn, &QPushButton::clicked, this, &QtWidgetsApplication2::clearImage);




    // Connect QPushButton clicks to slots
    connect(ui->sobel, &QPushButton::clicked, this, &QtWidgetsApplication2::onSobelClicked);
    connect(ui->prewitt, &QPushButton::clicked, this, &QtWidgetsApplication2::onPrewittClicked);
    connect(ui->robert, &QPushButton::clicked, this, &QtWidgetsApplication2::onRobertsClicked);
    connect(ui->canny, &QPushButton::clicked, this, &QtWidgetsApplication2::applyCannyFilter);
    // connect(ui->filter_size, &QListWidget::currentRowChanged, this, &QtWidgetsApplication2::onFilterSizeChanged);



}

QtWidgetsApplication2::~QtWidgetsApplication2()
{
    delete ui;
}

// void QtWidgetsApplication2::openImageDialog()
// {
//     // Open a file dialog to select an image file
//     fileName = QFileDialog::getOpenFileName(this,
//                                             tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

//     if (!fileName.isEmpty()) {
//         // Read the selected image using OpenCV
//         image = imread(fileName.toStdString());

//         if (!image.empty()) {
//             // Convert the OpenCV Mat image to QImage with the same color format
//             QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888); // Assuming OpenCV loads in BGR format
//             if (qImage.isNull()) {
//                 std::cerr << "Error: Unable to convert image to QImage." << std::endl;
//                 return;
//             }

//             // Display the image in the input_image_label QLabel
//             QPixmap pixmap = QPixmap::fromImage(qImage);
//             ui->input_image_label->setPixmap(pixmap.scaled(ui->input_image_label->size(), Qt::KeepAspectRatio));
//             ui->input_image_label->setAlignment(Qt::AlignCenter);

//         }
//         else {
//             std::cerr << "Error: Unable to load image: " << fileName.toStdString() << std::endl;
//         }
//     }
// }
void QtWidgetsApplication2::openImageDialog()
{
    // Open a file dialog to select an image file
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (!fileName.isEmpty()) {
        // Read the selected image using OpenCV
        image = imread(fileName.toStdString());
        img_detection = image;

        if (!image.empty()) {
            // Call the function to display the image in the label
            displayImageInLabel(image, ui->input_image_label);
            displayImageInLabel(image, ui->inputLabel_3);
            displayImageInLabel(image, ui->inputLabel_4);
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
void QtWidgetsApplication2::displayOutputImage(const cv::Mat& image, QLabel* label)
{
    if (!image.empty()) {
        cv::Mat displayImage;

        // Convert grayscale image to RGB for displaying
        if (image.channels() == 1) {
            cv::cvtColor(image, displayImage, cv::COLOR_GRAY2RGB);
        } else {
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
    } else {
        std::cerr << "Error: Image is empty." << std::endl;
    }
}

// void QtWidgetsApplication2::openImageDialog()
// {
//     openImage(); // Call the function to open and store the image

//     // Display the image in the input_image_label QLabel
//     displayImageOnLabel(ui->input_image_label);
// }

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

/*----------------------------------------------------------------Edge Detection----------------------------------------------------------------------------*/


void QtWidgetsApplication2::onSobelClicked()
{
    cv::Mat output;
    applySobel(img_detection, output);
    displayOutputImage(output, ui->outputLabel_3);
}

void QtWidgetsApplication2::onPrewittClicked()
{
    cv::Mat output;
    applyPrewitt(img_detection, output);
    displayOutputImage(output, ui->outputLabel_3);
}

void QtWidgetsApplication2::onRobertsClicked()
{
    cv::Mat output;
    applyRoberts(img_detection, output);
    displayOutputImage(output, ui->outputLabel_3);
}

void QtWidgetsApplication2::applyCannyFilter()
{
    if (!img_detection.empty()) {
        // Define Canny thresholds (adjust as needed)
        double lowThreshold = 50.0;
        double highThreshold = 150.0;

        // Convert image to grayscale if it's not already
        cv::Mat grayImage;
        if (img_detection.channels() > 1) {
            cv::cvtColor(img_detection, grayImage, cv::COLOR_BGR2GRAY);
        } else {
            grayImage = img_detection.clone();
        }

        // Apply Canny edge detection
        cv::Mat edges;
        cv::Canny(grayImage, edges, lowThreshold, highThreshold);

        // Display the result
        displayOutputImage(edges, ui->outputLabel_3);
    } else {
        std::cerr << "Error: No image loaded." << std::endl;
    }
}
