#ifndef HISTOGRAMS_H
#define HISTOGRAMS_H


	cv::Mat calculateHistogram(cv::Mat& input, int channel) ;
	cv::Mat calculateCDF(const cv::Mat& hist);

	cv::Mat plotHistogram( const cv::Mat & hist, const cv::Scalar & color);
	void plotCDF(const std::string& title, const cv::Mat& cdf, const cv::Scalar& color); // 	cv::Mat calculateCDF(const cv::Mat& hist);



	cv::Mat calculateGrayscaleCDF(const cv::Mat& grayscaleImage);
	void plotGrayscaleCDF(const std::string& title, const cv::Mat& cdf, const cv::Scalar& lineColor, const cv::Scalar& fillColor);
	void displayNormalizedEqualizedImages(const cv::Mat& image);
	void loadAndProcessImage(const std::string& filePath);




#endif // HISTOGRAMS_H