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

		static void sobel_grayscale(cv::Mat &roi, cv::Mat &tgt_roi, int KernelSize);
		static void canny_grayscale(cv::Mat &roi, cv::Mat &tgt_roi, int KernelSize);
		static void canny_hist_grayscale(cv::Mat &roi, cv::Mat &tgt_roi, int KernelSize);
		static void otsu_grayscale(cv::Mat &roi, cv::Mat &tgt_roi);
		static void otsu_histo_grayscale(cv::Mat &roi, cv::Mat &tgt_roi);
		static void histoeq_grayscale(cv::Mat &roi, cv::Mat &tgt_roi);
		static void sobel_grayscale_threshold(cv::Mat &roi, cv::Mat &tgt_roi, int KernelSize, int threshold);
		static void sobel_grayscale_threshold_direction(cv::Mat &roi, cv::Mat &tgt_roi, int KernelSize, int threshold);
		static void sobel_color(cv::Mat &roi, cv::Mat &tgt_roi, int KernelSize);
		
		static void cv_sobel3(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void cv_sobel5(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void cv_sobel3_color(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void cv_sobel5_color(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void cv_binaryedge(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void cv_binaryedge_direction(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void cv_canny(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void cv_histoeq(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void cv_canny_hist(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void cv_otsucv(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void cv_otsuhisto(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT);
		static void cv_QRcode(cv::Mat &src, cv::Mat &tgt);
		static void cv_QRcodequl(cv::Mat &src, cv::Mat &tgt);
};

#endif

