#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <opencv2/opencv.hpp>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <sstream>
#include <math.h>
#define DEFAULT 999999999
class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &src, image &tgt, int value);
		static void binarize(image &src, image &tgt, int threshold);
		static void scale(image &src, image &tgt, float ratio);
		static void cv_gray(cv::Mat &src, cv::Mat &tgt);
		static void cv_avgblur(cv::Mat &src, cv::Mat &tgt, int WindowSize);
		
		static void crop_and_rearrange(cv::Mat& roi); 
		static void fourierTransform(cv::Mat &src, cv::Mat &tgt, string name);
		static void inverseFourierTransform(cv::Mat &src, cv::Mat &tgt);
		static void low_pass_fourier(cv::Mat &src, cv::Mat &tgt, int threshold);
		static void high_pass_fourier(cv::Mat &src, cv::Mat &tgt, int threshold);
		static void low_pass_color_fourier(cv::Mat &src, cv::Mat &tgt, int threshold, int channel);
		static void high_pass_color_fourier(cv::Mat &src, cv::Mat &tgt, int threshold, int channel);

		static void unsharp_mask_compute(cv::Mat &src, cv::Mat &tgt, int multiplier);
		static void unsharp_mask_low_compute(cv::Mat &src, cv::Mat &tgt, int multiplier);
		

		static void cv_DisplayAmplitude(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void cv_Inverse_Fourier(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void low_pass(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int threshold1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int threshold2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int threshold3=DEFAULT);
		static void high_pass(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int threshold1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int threshold2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int threshold3=DEFAULT);
		static void unsharp_mask(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int multiplier1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int multiplier2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int multiplier3=DEFAULT);
		
		static void unsharp_mask_low(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int multiplier1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int multiplier2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int multiplier3=DEFAULT);
		static void low_pass_color(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int threshold1, int channel1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int threshold2=DEFAULT, int channel2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int threshold3=DEFAULT, int channel3=DEFAULT);
		static void high_pass_color(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int threshold1, int channel1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int threshold2=DEFAULT, int channel2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int threshold3=DEFAULT, int channel3=DEFAULT);


};

#endif

