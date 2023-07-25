#include "utility.h"

#define MAXRGB 255
#define MINRGB 0
#define MAXLEN 256
using namespace cv;

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int value)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value)); 
		}
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int threshold)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

// OpenCV code for Fourier and Inverse Fourier Transformations taken from OpenCV documentation. https://docs.opencv.org/4.x/d8/d01/tutorial_discrete_fourier_transform.html

void utility::inverseFourierTransform(Mat &roi, Mat &tgt_roi)
{
	
	Mat roi_gray, roi_out;
	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);
	//cout << "display mag reached";
	Mat optimal_image;
	int r = getOptimalDFTSize(roi_gray.rows);
	int c = getOptimalDFTSize(roi_gray.cols);
	copyMakeBorder(roi_gray, optimal_image, 0, r - roi_gray.rows, 0, c - roi_gray.cols, BORDER_CONSTANT, Scalar::all(0));

	// We have both complex and real values
	Mat planes[] = {Mat_<float>(optimal_image), Mat::zeros(optimal_image.size(), CV_32F)};
	Mat complex_val;
	merge(planes, 2, complex_val);

	// Discrete Fourier Transform
	dft(complex_val, complex_val, cv::DFT_COMPLEX_OUTPUT);
	//crop_and_rearrange(complex_val);
	
	
	// Inverse Discrete Fourier Transform
	Mat inverse_transform;
	cv::dft(complex_val, inverse_transform, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
	//inverse_transform.convertTo(inverse_transform, CV_8U);
	normalize(inverse_transform, inverse_transform, 0, 255, NORM_MINMAX);

	char outfile[MAXLEN];
	string file_name = "mic_fourier_to_orig.jpg";
	strcpy(outfile, file_name.c_str());
	cv::imwrite(outfile, inverse_transform);

	tgt_roi = inverse_transform.clone();

}

void utility::crop_and_rearrange(cv::Mat& roi)
{
	// Crop to get rid of the extra levels from the expanded image
	roi = roi(cv::Rect(0, 0, roi.cols & -2, roi.rows & -2));
    
	int center_x = roi.cols / 2;
    int center_y = roi.rows / 2;

	// For each Quadrant create an ROI. 
	
	// Corresponds to Top-Left
	Mat quadrant1(roi, Rect(0, 0, center_x, center_y));  
	// Corresponds to Bottom-Left
	Mat quadrant2(roi, Rect(0, center_y, center_x, center_y));  
	// Corresponds to Top-Right
	Mat quadrant3(roi, Rect(center_x, 0, center_x, center_y));  
	// Corresponds to Bottom-Right
	Mat quadrant4(roi, Rect(center_x, center_y, center_x, center_y));  

	// Start swapping quadrants
     
	// Swap Top-Left with Bottom-Right
	Mat temp;
	quadrant1.copyTo(temp);
	quadrant4.copyTo(quadrant1);
	temp.copyTo(quadrant4);

	// Swap Top-Right with Bottom-Left
	quadrant3.copyTo(temp);
	quadrant2.copyTo(quadrant3);
	temp.copyTo(quadrant2);

}

void utility::low_pass_fourier(Mat &roi, Mat &tgt_roi, int threshold_D)
{
	Mat roi_gray, roi_out;
	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);
	//cout << "display mag reached";
	
	// Float representation. Convert to 32-bit float.
	//Mat float_roi_gray;
	//roi_gray.convertTo(float_roi_gray, CV_32F);

	// In order to improve the performance of Discrete Fourier Transform we need to expand the image to an optimal size
	Mat optimal_image;
	int r = getOptimalDFTSize(roi_gray.rows);
	int c = getOptimalDFTSize(roi_gray.cols);
	copyMakeBorder(roi_gray, optimal_image, 0, r - roi_gray.rows, 0, c - roi_gray.cols, BORDER_CONSTANT, Scalar::all(0));

	// We have both complex and real values
	Mat planes[] = {Mat_<float>(optimal_image), Mat::zeros(optimal_image.size(), CV_32F)};
	Mat complex_val;
	merge(planes, 2, complex_val);

	// In-place Discrete Fourier Transform
	dft(complex_val, complex_val, cv::DFT_COMPLEX_OUTPUT);

	
	/* ******************* LOW PASS FILTERING LOGIC  ************** */
	//cout << "\n Reached Low Pass filter logic";
	
	// Firstly, we construct a mask 
	Mat mask_low_pass;
	//mask_low_pass = complex_val.clone();
	mask_low_pass = cv::Mat(complex_val.rows,complex_val.cols,CV_32F);
    // Initialize Distance variable
	float distance;
	float dist_r, dist_c;
	
	// The mask has the same dimensions as the image in frequency domain, but we will assign 0, 1 to pixels in the mask to determine which values to keep in the complex_val
	for(int i = 0; i < complex_val.rows; i++) 
	{
		for(int j = 0; j < complex_val.cols; j++) 
		{ 
			// Circular filter
			dist_r = pow((i - (complex_val.rows / 2)),2);
			dist_c = pow((j - (complex_val.cols / 2)),2);
			distance = sqrt(dist_r + dist_c);
            
			
			if (distance <= threshold_D) {
				mask_low_pass.at<float>(i, j) = 1;
			} else {
				mask_low_pass.at<float>(i, j) = 0;
				
			}
						
		   
		}
	}
	
	Mat planes_H[] = {mask_low_pass, mask_low_pass};
	Mat dft_filter = complex_val.clone();
	merge(planes_H, 2, dft_filter);

	crop_and_rearrange(complex_val);
    //cv::mulSpectrums(complex_val,dft_filter,complex_val,0);

	//complex_val = complex_val.mul(dft_filter);
	complex_val = complex_val.mul(dft_filter);
	
	crop_and_rearrange(complex_val);
	//merge(planes_out, 2, filtered);
    
    
	Mat filtered = complex_val.clone(); 
	
	// ***************** INVERSE COMPONENT ****************** //
	Mat filtered_inverse_transform;
	cv::dft(filtered, filtered_inverse_transform, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
	normalize(filtered_inverse_transform, filtered_inverse_transform, 0, 255, NORM_MINMAX);

	char outfile[MAXLEN];
	string file_name = "baboon_low_pass_filter_2.jpg";
	strcpy(outfile, file_name.c_str());
	cv::imwrite(outfile, filtered_inverse_transform);
	
	filtered_inverse_transform.convertTo(filtered_inverse_transform, CV_8U);
    //cout << "\n filtered inverse transform type: " << filtered_inverse_transform.type();
	tgt_roi = filtered_inverse_transform.clone();
	
    
}

void utility::high_pass_fourier(Mat &roi, Mat &tgt_roi, int threshold_D)
{
	Mat roi_gray, roi_out;
	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);
	//cout << "display mag reached";
	
	// Float representation. Convert to 32-bit float.
	//Mat float_roi_gray;
	//roi_gray.convertTo(float_roi_gray, CV_32F);

	// In order to improve the performance of Discrete Fourier Transform we need to expand the image to an optimal size
	Mat optimal_image;
	int r = getOptimalDFTSize(roi_gray.rows);
	int c = getOptimalDFTSize(roi_gray.cols);
	copyMakeBorder(roi_gray, optimal_image, 0, r - roi_gray.rows, 0, c - roi_gray.cols, BORDER_CONSTANT, Scalar::all(0));

	// We have both complex and real values
	Mat planes[] = {Mat_<float>(optimal_image), Mat::zeros(optimal_image.size(), CV_32F)};
	Mat complex_val;
	merge(planes, 2, complex_val);

	// In-place Discrete Fourier Transform
	dft(complex_val, complex_val, cv::DFT_COMPLEX_OUTPUT);

	
	/* ******************* HIGH PASS FILTERING LOGIC  ************** */
	//cout << "\n Reached High Pass filter logic";
	
	// Firstly, we construct a mask 
	Mat mask_high_pass;
	//mask_low_pass = complex_val.clone();
	mask_high_pass = cv::Mat(complex_val.rows,complex_val.cols,CV_32F);
    // Initialize Distance variable
	float distance;
	float dist_r, dist_c;
	
	// The mask has the same dimensions as the image in frequency domain, but we will assign 0, 1 to pixels in the mask to determine which values to keep in the complex_val
	for(int i = 0; i < complex_val.rows; i++) 
	{
		for(int j = 0; j < complex_val.cols; j++) 
		{ 
			// Circular filter
			dist_r = pow((i - (complex_val.rows / 2)),2);
			dist_c = pow((j - (complex_val.cols / 2)),2);
			distance = sqrt(dist_r + dist_c);
            
			
			if (distance >= threshold_D) {
				mask_high_pass.at<float>(i, j) = 1;
			} else {
				mask_high_pass.at<float>(i, j) = 0;
				
			}
		   
		}
	}
	
	Mat planes_H[] = {mask_high_pass, mask_high_pass};
	Mat dft_filter = complex_val.clone();
	merge(planes_H, 2, dft_filter);

	crop_and_rearrange(complex_val);
    //cv::mulSpectrums(complex_val,dft_filter,complex_val,0);

	//complex_val = complex_val.mul(dft_filter);
	complex_val = complex_val.mul(dft_filter);
	
	crop_and_rearrange(complex_val);
	//merge(planes_out, 2, filtered);
    
    
	Mat filtered = complex_val.clone(); 
	
	// ***************** INVERSE COMPONENT ****************** //
	Mat filtered_inverse_transform;
	cv::dft(filtered, filtered_inverse_transform, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
	normalize(filtered_inverse_transform, filtered_inverse_transform, 0, 255, NORM_MINMAX);

	
	char outfile[MAXLEN];
	string file_name = "baboon_high_pass_filter_2.jpg";
	strcpy(outfile, file_name.c_str());
	cv::imwrite(outfile, filtered_inverse_transform);
	
	filtered_inverse_transform.convertTo(filtered_inverse_transform, CV_8U);
    //cout << "\n Filtered inverse transform type: " << filtered_inverse_transform.type();
	tgt_roi = filtered_inverse_transform.clone();
	
    
}

void utility::high_pass_color_fourier(Mat &roi, Mat &tgt_roi, int threshold_D, int channel)
{
	
	Mat roi_hsv, roi_out;
	// Convert the image to HSV
	cvtColor(roi, roi_hsv, COLOR_BGR2HSV);

	// Isolate the channel of interest: 0->H, 1->S; 2->V
	std::vector<cv::Mat> channels;
	split(roi_hsv, channels);

	Mat channel_selection = channels[channel];

	// Make copy of channel of interest
	Mat color_channel = channel_selection.clone();

	// In order to improve the performance of Discrete Fourier Transform we need to expand the image to an optimal size
	Mat optimal_image;
	int r = getOptimalDFTSize(color_channel.rows);
	int c = getOptimalDFTSize(color_channel.cols);
	copyMakeBorder(color_channel, optimal_image, 0, r - color_channel.rows, 0, c - color_channel.cols, BORDER_CONSTANT, Scalar::all(0));

	// We have both complex and real values
	Mat planes[] = {Mat_<float>(optimal_image), Mat::zeros(optimal_image.size(), CV_32F)};
	Mat complex_val;
	merge(planes, 2, complex_val);

	// In-place Discrete Fourier Transform
	dft(complex_val, complex_val, cv::DFT_COMPLEX_OUTPUT);
	
	/* ******************* HIGH PASS FILTERING LOGIC  ************** */
	//cout << "\n Reached High Pass filter logic";
	
	// Firstly, we construct a mask 
	Mat mask_high_pass;
	//mask_low_pass = complex_val.clone();
	mask_high_pass = cv::Mat(complex_val.rows,complex_val.cols,CV_32F);
    // Initialize Distance variable
	float distance;
	float dist_r, dist_c;
	
	// The mask has the same dimensions as the image in frequency domain, but we will assign 0, 1 to pixels in the mask to determine which values to keep in the complex_val
	for(int i = 0; i < complex_val.rows; i++) 
	{
		for(int j = 0; j < complex_val.cols; j++) 
		{ 
			// Circular filter
			dist_r = pow((i - (complex_val.rows / 2)),2);
			dist_c = pow((j - (complex_val.cols / 2)),2);
			distance = sqrt(dist_r + dist_c);
            
			
			if (distance >= threshold_D) {
				mask_high_pass.at<float>(i, j) = 1;
			} else {
				mask_high_pass.at<float>(i, j) = 0;
				
			}
		   
		}
	}
	
	Mat planes_H[] = {mask_high_pass, mask_high_pass};
	Mat dft_filter = complex_val.clone();
	merge(planes_H, 2, dft_filter);

	crop_and_rearrange(complex_val);
    //cv::mulSpectrums(complex_val,dft_filter,complex_val,0);

	//complex_val = complex_val.mul(dft_filter);
	complex_val = complex_val.mul(dft_filter);
	
	crop_and_rearrange(complex_val);
	//merge(planes_out, 2, filtered);
    
    
	Mat filtered = complex_val.clone(); 
	
	// ***************** INVERSE COMPONENT ****************** //
	Mat filtered_inverse_transform;
	cv::dft(filtered, filtered_inverse_transform, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
	
	// We need to normalize but keep in mind that channels have different ranges. 
	if (channel == 0) {
		// H channel
		//normalize(filtered_inverse_transform, filtered_inverse_transform, 0, 360, NORM_MINMAX);
	} else if (channel == 1) {
		// S channel
		//normalize(filtered_inverse_transform, filtered_inverse_transform, 0, 1, NORM_MINMAX);
	} else if (channel == 2) {
		// V channel
		normalize(filtered_inverse_transform, filtered_inverse_transform, 0, 255, NORM_MINMAX);
	}
	
	//char outfile[MAXLEN];
	//string file_name = "flower_high_pass_filter_H.jpg";
	//strcpy(outfile, file_name.c_str());
	//cv::imwrite(outfile, filtered_inverse_transform);
	
	filtered_inverse_transform.convertTo(filtered_inverse_transform, CV_8U);
    //cout << "\n Filtered inverse transform type: " << filtered_inverse_transform.type();
	// Update without pointer
	for(int i = 0; i < roi_hsv.rows; i++)
	{
		for(int j = 0; j < roi_hsv.cols; j++)
		{
			// Update the old channel value with the new channel value after high pass filter
			roi_hsv.at<Vec3b>(j,i)[channel] = filtered_inverse_transform.at<unsigned char>(j,i);
		}
	}

	// Return the original roi which should have been modified
	tgt_roi = roi_hsv.clone();
	    
}

void utility::low_pass_color_fourier(Mat &roi, Mat &tgt_roi, int threshold_D, int channel)
{
	
	Mat roi_hsv, roi_out;
	// Convert the image to HSV
	cvtColor(roi, roi_hsv, COLOR_BGR2HSV);

	// Isolate the channel of interest: 0->H, 1->S; 2->V
	std::vector<cv::Mat> channels;
	split(roi_hsv, channels);

	Mat channel_selection = channels[channel];

	// Make copy of channel of interest
	Mat color_channel = channel_selection.clone();

	// In order to improve the performance of Discrete Fourier Transform we need to expand the image to an optimal size
	Mat optimal_image;
	int r = getOptimalDFTSize(color_channel.rows);
	int c = getOptimalDFTSize(color_channel.cols);
	copyMakeBorder(color_channel, optimal_image, 0, r - color_channel.rows, 0, c - color_channel.cols, BORDER_CONSTANT, Scalar::all(0));

	// We have both complex and real values
	Mat planes[] = {Mat_<float>(optimal_image), Mat::zeros(optimal_image.size(), CV_32F)};
	Mat complex_val;
	merge(planes, 2, complex_val);

	// In-place Discrete Fourier Transform
	dft(complex_val, complex_val, cv::DFT_COMPLEX_OUTPUT);
	
	/* ******************* LOW PASS FILTERING LOGIC  ************** */
	
	
	// Firstly, we construct a mask 
	Mat mask_low_pass;
	//mask_low_pass = complex_val.clone();
	mask_low_pass = cv::Mat(complex_val.rows,complex_val.cols,CV_32F);
    // Initialize Distance variable
	float distance;
	float dist_r, dist_c;
	
	// The mask has the same dimensions as the image in frequency domain, but we will assign 0, 1 to pixels in the mask to determine which values to keep in the complex_val
	for(int i = 0; i < complex_val.rows; i++) 
	{
		for(int j = 0; j < complex_val.cols; j++) 
		{ 
			// Circular filter
			dist_r = pow((i - (complex_val.rows / 2)),2);
			dist_c = pow((j - (complex_val.cols / 2)),2);
			distance = sqrt(dist_r + dist_c);
            
			
			if (distance <= threshold_D) {
				mask_low_pass.at<float>(i, j) = 1;
			} else {
				mask_low_pass.at<float>(i, j) = 0;
				
			}
		   
		}
	}
	
	Mat planes_H[] = {mask_low_pass, mask_low_pass};
	Mat dft_filter = complex_val.clone();
	merge(planes_H, 2, dft_filter);

	crop_and_rearrange(complex_val);
    //cv::mulSpectrums(complex_val,dft_filter,complex_val,0);

	//complex_val = complex_val.mul(dft_filter);
	complex_val = complex_val.mul(dft_filter);
	
	crop_and_rearrange(complex_val);
	//merge(planes_out, 2, filtered);
    
    
	Mat filtered = complex_val.clone(); 
	
	// ***************** INVERSE COMPONENT ****************** //
	Mat filtered_inverse_transform;
	cv::dft(filtered, filtered_inverse_transform, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
	// We need to normalize but keep in mind that channels have different ranges. 
	if (channel == 0) {
		// H channel
		//normalize(filtered_inverse_transform, filtered_inverse_transform, 0, 360, NORM_MINMAX);
	} else if (channel == 1) {
		// S channel
		//normalize(filtered_inverse_transform, filtered_inverse_transform, 0, 1, NORM_MINMAX);
	} else if (channel == 2) {
		// V channel
		normalize(filtered_inverse_transform, filtered_inverse_transform, 0, 255, NORM_MINMAX);
	}

	//char outfile[MAXLEN];
	//string file_name = "flower_low_pass_filter_H.jpg";
	//strcpy(outfile, file_name.c_str());
	//cv::imwrite(outfile, filtered_inverse_transform);
	
	filtered_inverse_transform.convertTo(filtered_inverse_transform, CV_8U);
    //cout << "\n Filtered inverse transform type: " << filtered_inverse_transform.type();
	// Update without pointer
	for(int i = 0; i < roi_hsv.rows; i++)
	{
		for(int j = 0; j < roi_hsv.cols; j++)
		{
			// Update the old channel value with the new channel value after high pass filter
			roi_hsv.at<Vec3b>(j,i)[channel] = filtered_inverse_transform.at<unsigned char>(j,i);
		}
	}

	// Return the original roi which should have been modified
	tgt_roi = roi_hsv.clone();
	    
}

void utility::unsharp_mask_compute(Mat &roi, Mat &tgt_roi, int multiplier)
{
	//tgt_roi = roi.clone();
	// UNMASK SHARPING HIGH-FREQUENCY
	
	int threshold_D = 40;

	Mat roi_gray, roi_out;
	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);
	//cout << "display mag reached";
	
	// Float representation. Convert to 32-bit float.
	//Mat float_roi_gray;
	//roi_gray.convertTo(float_roi_gray, CV_32F);

	// In order to improve the performance of Discrete Fourier Transform we need to expand the image to an optimal size
	Mat optimal_image;
	int r = getOptimalDFTSize(roi_gray.rows);
	int c = getOptimalDFTSize(roi_gray.cols);
	copyMakeBorder(roi_gray, optimal_image, 0, r - roi_gray.rows, 0, c - roi_gray.cols, BORDER_CONSTANT, Scalar::all(0));

	// We have both complex and real values
	Mat planes[] = {Mat_<float>(optimal_image), Mat::zeros(optimal_image.size(), CV_32F)};
	Mat complex_val;
	merge(planes, 2, complex_val);

	// In-place Discrete Fourier Transform
	dft(complex_val, complex_val, cv::DFT_COMPLEX_OUTPUT);

	
	/* ******************* HIGH PASS FILTERING LOGIC  ************** */
	//cout << "\n Reached High Pass filter logic";
	
	// Firstly, we construct a mask 
	Mat mask_high_pass;
	//mask_low_pass = complex_val.clone();
	mask_high_pass = cv::Mat(complex_val.rows,complex_val.cols,CV_32F);
    // Initialize Distance variable
	float distance;
	float dist_r, dist_c;
	
	// The mask has the same dimensions as the image in frequency domain, but we will assign 0, 1 to pixels in the mask to determine which values to keep in the complex_val
	// Since we are applying unsharp masking we will use the formula sharpened = original + (original - blurred)*multiplier but in fourier space it is updated to simply passing multiplier instead of 1 to high frequency pixels.
	// The low frequency pixels are kept the same so mask is 1
	for(int i = 0; i < complex_val.rows; i++) 
	{
		for(int j = 0; j < complex_val.cols; j++) 
		{ 
			// Circular filter
			dist_r = pow((i - (complex_val.rows / 2)),2);
			dist_c = pow((j - (complex_val.cols / 2)),2);
			distance = sqrt(dist_r + dist_c);
            
			
			if (distance >= threshold_D) {
				mask_high_pass.at<float>(i, j) = multiplier;
			} else {
				mask_high_pass.at<float>(i, j) = 1;
				
			}
		   
		}
	}
	
	Mat planes_H[] = {mask_high_pass, mask_high_pass};
	Mat dft_filter = complex_val.clone();
	merge(planes_H, 2, dft_filter);

	crop_and_rearrange(complex_val);
    //cv::mulSpectrums(complex_val,dft_filter,complex_val,0);

	//complex_val = complex_val.mul(dft_filter);
	complex_val = complex_val.mul(dft_filter);
	
	crop_and_rearrange(complex_val);
	//merge(planes_out, 2, filtered);
    
    
	Mat filtered = complex_val.clone(); 
	
	// ***************** INVERSE COMPONENT ****************** //
	Mat filtered_inverse_transform;
	cv::dft(filtered, filtered_inverse_transform, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
	normalize(filtered_inverse_transform, filtered_inverse_transform, 0, 255, NORM_MINMAX);

	
	//char outfile[MAXLEN];
	//string file_name = "baboon_high_pass_filter_2.jpg";
	//strcpy(outfile, file_name.c_str());
	//cv::imwrite(outfile, filtered_inverse_transform);
	
	filtered_inverse_transform.convertTo(filtered_inverse_transform, CV_8U);
    //cout << "\n Filtered inverse transform type: " << filtered_inverse_transform.type();
	tgt_roi = filtered_inverse_transform.clone();


}

void utility::unsharp_mask_low_compute(Mat &roi, Mat &tgt_roi, int multiplier)
{
	//tgt_roi = roi.clone();
	// UNMASK SHARPING LOW-FREQUENCY
	
	int threshold_D = 40;
	Mat roi_gray, roi_out;
	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);
	//cout << "display mag reached";
	
	// Float representation. Convert to 32-bit float.
	//Mat float_roi_gray;
	//roi_gray.convertTo(float_roi_gray, CV_32F);

	// In order to improve the performance of Discrete Fourier Transform we need to expand the image to an optimal size
	Mat optimal_image;
	int r = getOptimalDFTSize(roi_gray.rows);
	int c = getOptimalDFTSize(roi_gray.cols);
	copyMakeBorder(roi_gray, optimal_image, 0, r - roi_gray.rows, 0, c - roi_gray.cols, BORDER_CONSTANT, Scalar::all(0));

	// We have both complex and real values
	Mat planes[] = {Mat_<float>(optimal_image), Mat::zeros(optimal_image.size(), CV_32F)};
	Mat complex_val;
	merge(planes, 2, complex_val);

	// In-place Discrete Fourier Transform
	dft(complex_val, complex_val, cv::DFT_COMPLEX_OUTPUT);

	
	/* ******************* LOW PASS FILTERING LOGIC  ************** */
	//cout << "\n Reached High Pass filter logic";
	
	// Firstly, we construct a mask 
	Mat mask_low_pass;
	//mask_low_pass = complex_val.clone();
	mask_low_pass = cv::Mat(complex_val.rows,complex_val.cols,CV_32F);
    // Initialize Distance variable
	float distance;
	float dist_r, dist_c;
	
	// The mask has the same dimensions as the image in frequency domain, but we will assign 0, 1 to pixels in the mask to determine which values to keep in the complex_val
	// Since we are applying unsharp masking we will use the formula sharpened = original + (original - blurred)*multiplier but in fourier space it is updated to simply passing multiplier instead of 1 to low frequency pixels.
	// The high frequency pixels are kept the same so mask is 1
	for(int i = 0; i < complex_val.rows; i++) 
	{
		for(int j = 0; j < complex_val.cols; j++) 
		{ 
			// Circular filter
			dist_r = pow((i - (complex_val.rows / 2)),2);
			dist_c = pow((j - (complex_val.cols / 2)),2);
			distance = sqrt(dist_r + dist_c);
            
			
			if (distance <= threshold_D) {
				mask_low_pass.at<float>(i, j) = multiplier;
			} else {
				mask_low_pass.at<float>(i, j) = 1;
				
			}
		   
		}
	}
	
	Mat planes_H[] = {mask_low_pass, mask_low_pass};
	Mat dft_filter = complex_val.clone();
	merge(planes_H, 2, dft_filter);

	crop_and_rearrange(complex_val);
    //cv::mulSpectrums(complex_val,dft_filter,complex_val,0);

	//complex_val = complex_val.mul(dft_filter);
	complex_val = complex_val.mul(dft_filter);
	
	crop_and_rearrange(complex_val);
	//merge(planes_out, 2, filtered);
    
    
	Mat filtered = complex_val.clone(); 
	
	// ***************** INVERSE COMPONENT ****************** //
	Mat filtered_inverse_transform;
	cv::dft(filtered, filtered_inverse_transform, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
	normalize(filtered_inverse_transform, filtered_inverse_transform, 0, 255, NORM_MINMAX);

	
	//char outfile[MAXLEN];
	//string file_name = "baboon_high_pass_filter_2.jpg";
	//strcpy(outfile, file_name.c_str());
	//cv::imwrite(outfile, filtered_inverse_transform);
	
	filtered_inverse_transform.convertTo(filtered_inverse_transform, CV_8U);
    //cout << "\n Filtered inverse transform type: " << filtered_inverse_transform.type();
	tgt_roi = filtered_inverse_transform.clone();
	
	
}

void utility::fourierTransform(Mat &roi, Mat &tgt_roi, string roi_name)
{
	Mat roi_gray, roi_out;
	Mat roi_gray_blur, roi_gray_equalized;
	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);
	//cout << "display mag reached";
	
	// Float representation
	Mat float_roi_gray;
	roi_gray.convertTo(float_roi_gray, CV_32F);

	// In order to improve the performance of Discrete Fourier Transform we need to expand the image to an optimal size
	Mat optimal_image;
	int r = getOptimalDFTSize(roi_gray.rows);
	int c = getOptimalDFTSize(roi_gray.cols);
	copyMakeBorder(roi_gray, optimal_image, 0, r - roi_gray.rows, 0, c - roi_gray.cols, BORDER_CONSTANT, Scalar::all(0));

	// We have both complex and real values
	Mat planes[] = {Mat_<float>(optimal_image), Mat::zeros(optimal_image.size(), CV_32F)};
	Mat complex_val;
	merge(planes, 2, complex_val);

	// In-place Discrete Fourier Transform
	dft(complex_val, complex_val);

	//Mat magnitude_val = complex_val.clone();
	
	// We need to get the magnitude so we can transform the real and complex values. Real -> planes[0]; Imaginary -> planes[1]
	split(complex_val, planes); 
	magnitude(planes[0], planes[1], planes[0]);
	Mat magnitude_val = planes[0];

	
	// The dynamic range of the Fourier coefficients cannot be displayed because it is too large, therefore we need to convert to logarithmic scale.
	magnitude_val = magnitude_val + Scalar::all(1);
	log(magnitude_val, magnitude_val);

	// Initially, we had to expand the image because we need to improve the performance of DFT. However, now we need to remove those additional values that were added.
	// Furthermore, we will also rearrange the quadrants so that the origin (0,0) will correspond to the image center. 

	magnitude_val = magnitude_val(Rect(0, 0, magnitude_val.cols & -2, magnitude_val.rows & -2));

	crop_and_rearrange(magnitude_val);
	
	//Mat inverse_transform;
	//cv::dft(magnitude_val, inverse_transform, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
	// Now we normalize the magnitudes because currently they are out of the image display range
	//normalize(inverse_transform, inverse_transform, 0, 1, NORM_MINMAX);
	normalize(magnitude_val, magnitude_val, 0, 255, NORM_MINMAX);
    
	/* INVERSE COMPONENT */
	//Mat inverse_transform;
	//cv::dft(complex_val, inverse_transform, cv::DFT_INVERSE|cv::DFT_REAL_OUTPUT);
	//normalize(inverse_transform, inverse_transform, 0, 255, NORM_MINMAX);
	//inverse_transform.convertTo(inverse_transform, CV_8U);



	char outfile[MAXLEN];
	string file_name = roi_name;
	strcpy(outfile, file_name.c_str());
	cv::imwrite(outfile, magnitude_val);

	tgt_roi = magnitude_val.clone();
}

/*-----------------------------------------------------------------------**/
void utility::cv_DisplayAmplitude(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
{
	//cout << " " << roi;
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
    //src_gray = src.clone();
	// Declare offsets 
	int roi1_i_offset, roi1_j_offset, roi2_i_offset, roi2_j_offset, roi3_i_offset, roi3_j_offset;


	// Image ROI display
	
	// Declare Mat type ROIs
	Mat src_roi1, src_roi2, src_roi3;
	Mat src_test = src.clone();
	Mat src_roi1_test = src_test(Rect(x1, y1, Sx1, Sy1));
	if (roi == 3) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));
		src_roi3 = src(Rect(x3, y3, Sx3, Sy3));

	} else if (roi == 2) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));

	} else {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));

	}
    
	// Declare Mat type ROIs outputs
	Mat roi1_output, roi2_output, roi3_output;

	// Apply Sobel with Kernel Size 3x3 on ROIs
	if (roi == 3) {
		
		fourierTransform(src_roi1, roi1_output, "roi1_fourier_spectrum.jpg");
		fourierTransform(src_roi2, roi2_output, "roi2_fourier_spectrum.jpg");
		fourierTransform(src_roi3, roi3_output, "roi3_fourier_spectrum.jpg");

	} else if (roi == 2) {

		fourierTransform(src_roi1, roi1_output, "roi1_fourier_spectrum.jpg");
		fourierTransform(src_roi2, roi2_output, "roi2_fourier_spectrum.jpg");
	
	} else {

		fourierTransform(src_roi1, roi1_output, "roi1_fourier_spectrum.jpg");

	}
	
    //cout << "\nreached after display call";
	// Update the original image with the output ROIs using pointers
    
	if (roi == 3) {
		
		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_gray(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

		Mat src_roi3_1 = src_gray(Rect(x3, y3, Sx3, Sy3));
		roi3_output.copyTo(src_roi3_1);

	} else if (roi == 2) {

		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_gray(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

	
	} else {

		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	}
    
    //cout << "\nsobel3 was used!";
	cout << " \nFunction Display Amplitude was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::cv_Inverse_Fourier(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
{
	//cout << " " << roi;
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
    //src_gray = src.clone();
	// Declare offsets 
	int roi1_i_offset, roi1_j_offset, roi2_i_offset, roi2_j_offset, roi3_i_offset, roi3_j_offset;

	// Declare Mat type ROIs
	Mat src_roi1, src_roi2, src_roi3;
	Mat src_test = src.clone();
	Mat src_roi1_test = src_test(Rect(x1, y1, Sx1, Sy1));
	if (roi == 3) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));
		src_roi3 = src(Rect(x3, y3, Sx3, Sy3));

	} else if (roi == 2) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));

	} else {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));

	}
    
	// Declare Mat type ROIs outputs
	Mat roi1_output, roi2_output, roi3_output;

	// Apply Sobel with Kernel Size 3x3 on ROIs
	if (roi == 3) {
		
		inverseFourierTransform(src_roi1, roi1_output);
		inverseFourierTransform(src_roi2, roi2_output);
		inverseFourierTransform(src_roi3, roi3_output);

	} else if (roi == 2) {

		inverseFourierTransform(src_roi1, roi1_output);
		inverseFourierTransform(src_roi2, roi2_output);
	
	} else {

		inverseFourierTransform(src_roi1, roi1_output);

	}
	
    //cout << "\nreached after display call";
	// Update the original image with the output ROIs using pointers
    
	if (roi == 3) {
		
		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_gray(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

		Mat src_roi3_1 = src_gray(Rect(x3, y3, Sx3, Sy3));
		roi3_output.copyTo(src_roi3_1);

	} else if (roi == 2) {

		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_gray(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

	
	} else {

		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	}
    
    //cout << "\nsobel3 was used!";
	cout << " \nFunction Inverse Fourier was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::low_pass(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int threshold1, int x2, int y2, int Sx2, int Sy2, int threshold2, int x3, int y3, int Sx3, int Sy3, int threshold3)
{
	
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
    //src_gray = src.clone();
	// Declare offsets 
	int roi1_i_offset, roi1_j_offset, roi2_i_offset, roi2_j_offset, roi3_i_offset, roi3_j_offset;

	// Declare Mat type ROIs
	Mat src_roi1, src_roi2, src_roi3;
	Mat src_test = src.clone();
	Mat src_roi1_test = src_test(Rect(x1, y1, Sx1, Sy1));
	if (roi == 3) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));
		src_roi3 = src(Rect(x3, y3, Sx3, Sy3));

	} else if (roi == 2) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));

	} else {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));

	}
    
	// Declare Mat type ROIs outputs
	Mat roi1_output, roi2_output, roi3_output;

	// Apply Sobel with Kernel Size 3x3 on ROIs
	if (roi == 3) {
		
		low_pass_fourier(src_roi1, roi1_output, threshold1);
		low_pass_fourier(src_roi2, roi2_output, threshold2);
		low_pass_fourier(src_roi3, roi3_output, threshold3);

	} else if (roi == 2) {

		low_pass_fourier(src_roi1, roi1_output, threshold1);
		low_pass_fourier(src_roi2, roi2_output, threshold2);
	
	} else {
		//cout << "\n Calling low_pass_fourier";
		low_pass_fourier(src_roi1, roi1_output, threshold1);

	}
	
    //cout << "\nreached after display call";
	// Update the original image with the output ROIs using pointers
    
	if (roi == 3) {
		
		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_gray(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

		Mat src_roi3_1 = src_gray(Rect(x3, y3, Sx3, Sy3));
		roi3_output.copyTo(src_roi3_1);

	} else if (roi == 2) {

		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_gray(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

	
	} else {

		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	}
    
    //cout << "\nsobel3 was used!";
	cout << " \nFunction Low Pass Filter was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::high_pass(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int threshold1, int x2, int y2, int Sx2, int Sy2, int threshold2, int x3, int y3, int Sx3, int Sy3, int threshold3)
{
	//cout << " " << roi;
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
    //src_gray = src.clone();
	// Declare offsets 
	int roi1_i_offset, roi1_j_offset, roi2_i_offset, roi2_j_offset, roi3_i_offset, roi3_j_offset;

	// Declare Mat type ROIs
	Mat src_roi1, src_roi2, src_roi3;
	Mat src_test = src.clone();
	Mat src_roi1_test = src_test(Rect(x1, y1, Sx1, Sy1));
	if (roi == 3) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));
		src_roi3 = src(Rect(x3, y3, Sx3, Sy3));

	} else if (roi == 2) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));

	} else {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));

	}
    
	// Declare Mat type ROIs outputs
	Mat roi1_output, roi2_output, roi3_output;

	// Apply Sobel with Kernel Size 3x3 on ROIs
	if (roi == 3) {
		
		high_pass_fourier(src_roi1, roi1_output, threshold1);
		high_pass_fourier(src_roi2, roi2_output, threshold2);
		high_pass_fourier(src_roi3, roi3_output, threshold3);

	} else if (roi == 2) {

		high_pass_fourier(src_roi1, roi1_output, threshold1);
		high_pass_fourier(src_roi2, roi2_output, threshold2);
	
	} else {

		high_pass_fourier(src_roi1, roi1_output, threshold1);

	}
	
    //cout << "\nreached after display call";
	// Update the original image with the output ROIs using pointers
    
	if (roi == 3) {
		
		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_gray(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

		Mat src_roi3_1 = src_gray(Rect(x3, y3, Sx3, Sy3));
		roi3_output.copyTo(src_roi3_1);

	} else if (roi == 2) {

		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_gray(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

	
	} else {

		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	}
    
    //cout << "\nsobel3 was used!";
	cout << " \nFunction High Pass Filter was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::unsharp_mask(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int multiplier1, int x2, int y2, int Sx2, int Sy2, int multiplier2, int x3, int y3, int Sx3, int Sy3, int multiplier3)
{
	//cout << " " << roi;
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
    //src_gray = src.clone();
	// Declare offsets 
	int roi1_i_offset, roi1_j_offset, roi2_i_offset, roi2_j_offset, roi3_i_offset, roi3_j_offset;

	// Declare Mat type ROIs
	Mat src_roi1, src_roi2, src_roi3;
	Mat src_test = src.clone();
	Mat src_roi1_test = src_test(Rect(x1, y1, Sx1, Sy1));
	if (roi == 3) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));
		src_roi3 = src(Rect(x3, y3, Sx3, Sy3));

	} else if (roi == 2) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));

	} else {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));

	}
    
	// Declare Mat type ROIs outputs
	Mat roi1_output, roi2_output, roi3_output;

	// Apply Sobel with Kernel Size 3x3 on ROIs
	if (roi == 3) {
		
		unsharp_mask_compute(src_roi1, roi1_output, multiplier1);
		unsharp_mask_compute(src_roi2, roi2_output, multiplier2);
		unsharp_mask_compute(src_roi3, roi3_output, multiplier3);

	} else if (roi == 2) {

		unsharp_mask_compute(src_roi1, roi1_output, multiplier1);
		unsharp_mask_compute(src_roi2, roi2_output, multiplier2);
	
	} else {

		unsharp_mask_compute(src_roi1, roi1_output, multiplier1);

	}
	
    //cout << "\nreached after display call";
	// Update the original image with the output ROIs using pointers
    
	if (roi == 3) {
		
		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_gray(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

		Mat src_roi3_1 = src_gray(Rect(x3, y3, Sx3, Sy3));
		roi3_output.copyTo(src_roi3_1);

	} else if (roi == 2) {

		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_gray(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

	
	} else {

		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	}
    
    //cout << "\nsobel3 was used!";
	cout << " \nFunction Unsharp Mask was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::unsharp_mask_low(cv::Mat &src, cv::Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int multiplier1, int x2, int y2, int Sx2, int Sy2, int multiplier2, int x3, int y3, int Sx3, int Sy3, int multiplier3)
{
	//cout << " " << roi;
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
    //src_gray = src.clone();
	// Declare offsets 
	int roi1_i_offset, roi1_j_offset, roi2_i_offset, roi2_j_offset, roi3_i_offset, roi3_j_offset;

	// Declare Mat type ROIs
	Mat src_roi1, src_roi2, src_roi3;
	Mat src_test = src.clone();
	Mat src_roi1_test = src_test(Rect(x1, y1, Sx1, Sy1));
	if (roi == 3) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));
		src_roi3 = src(Rect(x3, y3, Sx3, Sy3));

	} else if (roi == 2) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));

	} else {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));

	}
    
	// Declare Mat type ROIs outputs
	Mat roi1_output, roi2_output, roi3_output;

	// Apply Sobel with Kernel Size 3x3 on ROIs
	if (roi == 3) {
		
		unsharp_mask_low_compute(src_roi1, roi1_output, multiplier1);
		unsharp_mask_low_compute(src_roi2, roi2_output, multiplier2);
		unsharp_mask_low_compute(src_roi3, roi3_output, multiplier3);

	} else if (roi == 2) {

		unsharp_mask_low_compute(src_roi1, roi1_output, multiplier1);
		unsharp_mask_low_compute(src_roi2, roi2_output, multiplier2);
	
	} else {

		unsharp_mask_low_compute(src_roi1, roi1_output, multiplier1);

	}
	
    //cout << "\nreached after display call";
	// Update the original image with the output ROIs using pointers
    
	if (roi == 3) {
		
		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_gray(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

		Mat src_roi3_1 = src_gray(Rect(x3, y3, Sx3, Sy3));
		roi3_output.copyTo(src_roi3_1);

	} else if (roi == 2) {

		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_gray(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

	
	} else {

		Mat src_roi1_1 = src_gray(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	}
    
    //cout << "\nsobel3 was used!";
	cout << " \nFunction Unsharp Mask Low was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::low_pass_color(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int threshold1, int channel1, int x2, int y2, int Sx2, int Sy2, int threshold2, int channel2, int x3, int y3, int Sx3, int Sy3, int threshold3, int channel3)
{
	//cout << "\n sobel3_color is called";
	// Convert original image to grayscale
	Mat src_hsv;
	cvtColor(src, src_hsv, COLOR_BGR2HSV);
    
	// Declare offsets 
	//int roi1_i_offset, roi1_j_offset, roi2_i_offset, roi2_j_offset, roi3_i_offset, roi3_j_offset;

	// Declare Mat type ROIs
	Mat src_roi1, src_roi2, src_roi3;
	//Mat src_test = src.clone();
	//Mat src_roi1_test = src_test(Rect(x1, y1, Sx1, Sy1));
	if (roi == 3) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));
		src_roi3 = src(Rect(x3, y3, Sx3, Sy3));

	} else if (roi == 2) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));

	} else {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));

	}
    
	// Declare Mat type ROIs outputs
	Mat roi1_output, roi2_output, roi3_output;

	// Apply Sobel with Kernel Size 3x3 on ROIs
	if (roi == 3) {
		
		low_pass_color_fourier(src_roi1, roi1_output, threshold1, channel1);
		low_pass_color_fourier(src_roi2, roi2_output, threshold2, channel2);
		low_pass_color_fourier(src_roi3, roi3_output, threshold3, channel3);

	} else if (roi == 2) {

		low_pass_color_fourier(src_roi1, roi1_output, threshold1, channel1);
		low_pass_color_fourier(src_roi2, roi2_output, threshold2, channel2);
	
	} else {

		low_pass_color_fourier(src_roi1, roi1_output, threshold1, channel1);

	}
	

	// Update the original image with the output ROIs using pointers
    
	if (roi == 3) {
		
		Mat src_roi1_1 = src_hsv(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_hsv(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

		Mat src_roi3_1 = src_hsv(Rect(x3, y3, Sx3, Sy3));
		roi3_output.copyTo(src_roi3_1);

	} else if (roi == 2) {

		Mat src_roi1_1 = src_hsv(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_hsv(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

	
	} else {

		Mat src_roi1_1 = src_hsv(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	}


	//cout << "\nSrc_hsv type: " << src_hsv.type();
	// Convert back to BGR
	Mat output_color;
	//cvtColor(src, src_hsv, COLOR_BGR2HSV);
	cvtColor(src_hsv, output_color, COLOR_HSV2BGR);
    // Output	
	//cout << "Output type: " << output_color.type();
	//cout << "\n Sobel 3 color was called! ";
	
	cout << " \nFunction Low Pass Color was utilized.";
	tgt = output_color.clone();

	
	
}

/*-----------------------------------------------------------------------**/
void utility::high_pass_color(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int threshold1, int channel1, int x2, int y2, int Sx2, int Sy2, int threshold2, int channel2, int x3, int y3, int Sx3, int Sy3, int threshold3, int channel3)
{
	//cout << "\n sobel3_color is called";
	// Convert original image to grayscale
	Mat src_hsv;
	cvtColor(src, src_hsv, COLOR_BGR2HSV);
    
	// Declare offsets 
	//int roi1_i_offset, roi1_j_offset, roi2_i_offset, roi2_j_offset, roi3_i_offset, roi3_j_offset;

	// Declare Mat type ROIs
	Mat src_roi1, src_roi2, src_roi3;
	//Mat src_test = src.clone();
	//Mat src_roi1_test = src_test(Rect(x1, y1, Sx1, Sy1));
	if (roi == 3) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));
		src_roi3 = src(Rect(x3, y3, Sx3, Sy3));

	} else if (roi == 2) {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));
		src_roi2 = src(Rect(x2, y2, Sx2, Sy2));

	} else {

		src_roi1 = src(Rect(x1, y1, Sx1, Sy1));

	}
    
	// Declare Mat type ROIs outputs
	Mat roi1_output, roi2_output, roi3_output;

	// Apply Sobel with Kernel Size 3x3 on ROIs
	if (roi == 3) {
		
		high_pass_color_fourier(src_roi1, roi1_output, threshold1, channel1);
		high_pass_color_fourier(src_roi2, roi2_output, threshold2, channel2);
		high_pass_color_fourier(src_roi3, roi3_output, threshold3, channel3);

	} else if (roi == 2) {

		high_pass_color_fourier(src_roi1, roi1_output, threshold1, channel1);
		high_pass_color_fourier(src_roi2, roi2_output, threshold2, channel2);
	
	} else {

		high_pass_color_fourier(src_roi1, roi1_output, threshold1, channel1);

	}
	

	// Update the original image with the output ROIs using pointers
    
	if (roi == 3) {
		
		Mat src_roi1_1 = src_hsv(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_hsv(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

		Mat src_roi3_1 = src_hsv(Rect(x3, y3, Sx3, Sy3));
		roi3_output.copyTo(src_roi3_1);

	} else if (roi == 2) {

		Mat src_roi1_1 = src_hsv(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	
		Mat src_roi2_1 = src_hsv(Rect(x2, y2, Sx2, Sy2));
		roi2_output.copyTo(src_roi2_1);

	
	} else {

		Mat src_roi1_1 = src_hsv(Rect(x1, y1, Sx1, Sy1));
		roi1_output.copyTo(src_roi1_1);
	}


	//cout << "\nSrc_hsv type: " << src_hsv.type();
	// Convert back to BGR
	Mat output_color;
	//cvtColor(src, src_hsv, COLOR_BGR2HSV);
	cvtColor(src_hsv, output_color, COLOR_HSV2BGR);
    // Output	
	//cout << "Output type: " << output_color.type();
	//cout << "\n Sobel 3 color was called! ";
	
	cout << " \nFunction High Pass Color was utilized. ";
	tgt = output_color.clone();

	
	
}

/*-----------------------------------------------------------------------**/
void utility::cv_gray(Mat &src, Mat &tgt)
{
	cvtColor(src, tgt, COLOR_BGR2GRAY);
}

/*-----------------------------------------------------------------------**/
void utility::cv_avgblur(Mat &src, Mat &tgt, int WindowSize)
{
	blur(src,tgt,Size(WindowSize,WindowSize));
}