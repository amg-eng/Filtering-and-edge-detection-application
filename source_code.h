#ifndef SOURCE_CODE_H
#define SOURCE_CODE_H

#include <opencv2/core.hpp> 

using namespace std;
using namespace cv;

// Constants
/*
const int GAUSSIAN_KERNEL_SIZE = 5;
const int AVERAGE_KERNEL_SIZE = 3;
const int MEDIAN_KERNEL_SIZE = 3;
const double GAUSSIAN_SIGMA = 1.0;
const double GAUSSIAN_NOISE_STDDEV = 25;
const double UNIFORM_NOISE_LOW = 0;
const double UNIFORM_NOISE_HIGH = 50;
const float SALT_PROBABILITY = 0.01;
const float PEPPER_PROBABILITY = 0.01;
*/

// Function Prototypes

Mat addGaussianNoise(const Mat& image, double mean = 0, double stddev = 25);
Mat addUniformNoise(const Mat& image, double low = 0, double high = 50);
Mat addSaltAndPepperNoise(const Mat& image, float salt_prob = 0.01, float pepper_prob = 0.01);
Mat applyGaussianFilter(const Mat& image, int kernel_size, double sigma);
Mat applyAverageFilter(const Mat& image, int kernel_size);
Mat applyMedianFilter(const Mat& image, int kernel_size);




#endif // SOURCE_CODE_H