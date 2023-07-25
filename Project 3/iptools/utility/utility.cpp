#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

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

/*-----------------------------------------------------------------------**/
void utility::sobel_grayscale(Mat &roi, Mat &tgt_roi, int KernelSize)
{
	Mat grad_x, grad_y, grad, abs_grad_x, abs_grad_y, roi_gray;

	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);

	// Firstly, we use the sobel function from OpenCV to compute the gradients in the x and y direction individually
	Sobel(roi_gray, grad_x, -1, 1, 0, KernelSize);
	Sobel(roi_gray, grad_y, -1, 0, 1, KernelSize);
	
	// Secondly, we convert the grad_x, grad_y to 8U using the convertScaleAbs() from OpenCV
	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);

	// Thirdly, we add the two gradients together
	addWeighted(abs_grad_x, 1, abs_grad_y, 1, 0, grad);

	tgt_roi = grad.clone();

}

/*-----------------------------------------------------------------------**/
void utility::histoeq_grayscale(Mat &roi, Mat &tgt_roi)
{
	Mat roi_gray, roi_out;

	// Convert the image roi to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);

    // Apply Histogram Equalization to roi
	equalizeHist(roi_gray, roi_out);
	
	tgt_roi = roi_out.clone();

}

/*-----------------------------------------------------------------------**/
void utility::canny_grayscale(Mat &roi, Mat &tgt_roi, int KernelSize)
{
	Mat grad_x, grad_y, grad, abs_grad_x, abs_grad_y, roi_gray;
	Mat roi_gray_blur;
	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);
	// Apply blur
	blur(roi_gray, roi_gray_blur, Size(3,3));
    
	
    // Lower threshold = 100, Higher threshold = 200
	// We apply Canny function
	int lower_threshold = 100;
	Canny(roi_gray_blur, grad, lower_threshold, lower_threshold*3, KernelSize);

	tgt_roi = grad.clone();

}
/*-----------------------------------------------------------------------**/
void utility::otsu_grayscale(Mat &roi, Mat &tgt_roi)
{
	Mat roi_gray, roi_out;
	Mat roi_gray_blur, roi_gray_equalized;
	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);

	// Apply Otsu Algorithm
	threshold(roi_gray, roi_out, 0, 255, THRESH_OTSU);

	tgt_roi = roi_out.clone();
}

/*-----------------------------------------------------------------------**/
void utility::otsu_histo_grayscale(Mat &roi, Mat &tgt_roi)
{
	Mat roi_gray, roi_out;
	Mat roi_gray_blur, roi_gray_equalized;
	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);
    
	// Apply Otsu Algorithm to simply get the threshold level to use further down. The actual Otsu will be applied later after we have applied Histogram Equalization on the background pixels as specified by that level.
	long double thres = threshold(roi_gray, roi_out, 0, 255, THRESH_OTSU);
	
	// Create a copy
	Mat roi_gray_copy = roi_gray.clone();
    
	  
    // Create counter variable to collect the total background pixels which will determine the column length of the data structure to be equalized
	int counter = 0;

	for(int i = 0; i < roi_gray_copy.rows; i++)
	{
		for(int j = 0; j < roi_gray_copy.cols; j++)
		{
			
			int pixel_value = (int)roi_gray_copy.at<unsigned char>(j,i);

			if (pixel_value <= thres) {
				// Based on Otsu this would be a background pixels so we will need it
				counter++;
			} else {
				// Foreground pixel. Do nothing.
				continue;
			}
			
		}
	}

	//cout << "\n THE COUNTER IS: " << counter;
	
	// Create new Mat with size 1xcounter and instatiate it with zeroes. 
	Mat background_pixels = Mat::zeros(1, counter, CV_8UC1);
	
	//cout << "\nbackground_pixel rows: " << background_pixels.rows;
	//cout << "\nbackground_pixel cols: " << background_pixels.cols;

	
	// Now let's populate the background_pixels mat
    
	int iterative_counter = 0;
	for(int i = 0; i < roi_gray_copy.rows; i++)
	{
		for(int j = 0; j < roi_gray_copy.cols; j++)
		{
			
			int pixel_value = (int)roi_gray_copy.at<unsigned char>(j,i);

			if (pixel_value <= thres) {
				// Based on Otsu this would be a background pixels so we will need it and we store it in background_pixels
				background_pixels.at<unsigned char>(0, iterative_counter) = (uchar)pixel_value;
				iterative_counter++;
				
				/* FOR TESTING PURPOSES
				if (iterative_counter < 10) {
					cout << " \nPixel value: " << pixel_value;
					cout << " \n Pixel value stored in background: " << (int)background_pixels.at<unsigned char>(0, iterative_counter-1);
				}
				*/
			} else {
				// Foreground pixel
				continue;
			}
			
		}
	}
	
	// Create a copy and Apply OpenCV Histogram Equalization
	Mat background_pixels_equalized = background_pixels.clone();

	equalizeHist(background_pixels, background_pixels_equalized);

	// Now that we have performed histogram equalization on the background pixels we do our favorite part. We update the original background pixels with the new background pixels in the SAME ORDER.

	int update_counter = 0;
	int new_background_pixel;
	for(int i = 0; i < roi_gray.rows; i++)
	{
		for(int j = 0; j < roi_gray.cols; j++)
		{
			
			int pixel_value = (int)roi_gray.at<unsigned char>(j,i);

			if (pixel_value <= thres) {
				// Based on Otsu this would be a background pixels so we will update those with their corresponding new pixel values.
				new_background_pixel = (int)background_pixels_equalized.at<unsigned char>(0, update_counter);
				roi_gray.at<unsigned char>(j,i) = (uchar)new_background_pixel;
				update_counter++;
				//grad.at<unsigned char>(j,i) = (uchar)255;
			} else {
				// Foreground pixel
				continue;
			}
			
		}
	}

	// Now that we updated the original image we can perform Otsu Thresholding on it

	Mat otsu_histo_output = roi_gray.clone();
	threshold(roi_gray, otsu_histo_output, 0, 255, THRESH_OTSU);


	tgt_roi = otsu_histo_output.clone();



	//cout << "\nbackground_pixel rows: " << background_pixels.rows;
	//cout << "\nbackground_pixel cols: " << background_pixels.cols;

	//cout << "\nbackground_pixel_hist rows: " << background_pixels_equalized.rows;
	//cout << "\nbackground_pixel_hist cols: " << background_pixels_equalized.cols;
	//tgt_roi = roi_gray.clone();
	/* FOR TESTING PURPOSES
	cout << " \n BEFORE HISTOGRAM EQUALIZATION!";
	for(int i = 0; i < background_pixels.rows; i++)
	{
		for(int j = 0; j < background_pixels.cols; j++)
		{


			
			//int pixel_value = (int)background_pixels.at<unsigned char>(j,i);
			
			if (j < 5) {
				cout << " \n Row: " << i;
				cout << " \n Col: " << j;
				cout << " \n Background Pixel: " << (int)background_pixels.at<unsigned char>(0,j);
			}
			
			
		}
	}
    
    cout << " \n AFTER HISTOGRAM EQUALIZATION!";
	for(int i = 0; i < background_pixels_equalized.rows; i++)
	{
		for(int j = 0; j < background_pixels_equalized.cols; j++)
		{


			
			//int pixel_value = (int)background_pixels.at<unsigned char>(j,i);
			
			if (j < 5) {
				cout << " \n Row: " << i;
				cout << " \n Col: " << j;
				cout << " \n Background Pixel: " << (int)background_pixels_equalized.at<unsigned char>(0,j);
			}
			
			
		}
	}

	*/


}
/*-----------------------------------------------------------------------**/
void utility::canny_hist_grayscale(Mat &roi, Mat &tgt_roi, int KernelSize)
{	
	Mat grad_x, grad_y, grad, abs_grad_x, abs_grad_y, roi_gray;
	Mat roi_gray_blur, roi_gray_equalized;
	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);
	
	// Apply histogram Equalization
	equalizeHist(roi_gray, roi_gray_equalized);

	// Apply blur
	blur(roi_gray_equalized, roi_gray_blur, Size(3,3));
    
	
    // Lower threshold = 100, Higher threshold = 200
	// We apply Canny function
	int lower_threshold = 100;
	Canny(roi_gray_blur, grad, lower_threshold, lower_threshold*3, KernelSize);

	tgt_roi = grad.clone();
	
}
/*-----------------------------------------------------------------------**/
void utility::sobel_grayscale_threshold(Mat &roi, Mat &tgt_roi, int KernelSize, int threshold)
{
	Mat grad_x, grad_y, grad, abs_grad_x, abs_grad_y, roi_gray;

	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);

	// Firstly, we use the sobel function from OpenCV to compute the gradients in the x and y direction individually
	Sobel(roi_gray, grad_x, -1, 1, 0, KernelSize);
	Sobel(roi_gray, grad_y, -1, 0, 1, KernelSize);
	
	// Secondly, we convert the grad_x, grad_y to 8U using the convertScaleAbs() from OpenCV
	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);

	// Thirdly, we add the two gradients together
	addWeighted(abs_grad_x, 1, abs_grad_y, 1, 0, grad);

	// Update grad matrix values based on threshold
	for(int i = 0; i < grad.rows; i++)
	{
		for(int j = 0; j < grad.cols; j++)
		{
			int current_pixel_value = (int)grad.at<unsigned char>(j,i);
			if (current_pixel_value > threshold) {
				// If value is greater than threshold set to MAX
				grad.at<unsigned char>(j,i) = (uchar)255;
			} else {
				// If value is less than threshold set to MIN
				grad.at<unsigned char>(j,1) = (uchar)0;
			}
		}
	}
	tgt_roi = grad.clone();

}

/*-----------------------------------------------------------------------**/
void utility::sobel_grayscale_threshold_direction(Mat &roi, Mat &tgt_roi, int KernelSize, int threshold)
{
	Mat grad_x, grad_y, grad, abs_grad_x, abs_grad_y, roi_gray;

	// Convert the image to grayscale
	cvtColor(roi, roi_gray, COLOR_BGR2GRAY);

	// Firstly, we use the sobel function from OpenCV to compute the gradients in the x and y direction individually
	//Sobel(roi_gray, grad_x, -1, 1, 0, KernelSize);
	//Sobel(roi_gray, grad_y, -1, 0, 1, KernelSize);

	Sobel(roi_gray, grad_x, CV_16S, 1, 0, KernelSize);
	Sobel(roi_gray, grad_y, CV_16S, 0, 1, KernelSize);
	
	// Secondly, we convert the grad_x, grad_y to 8U using the convertScaleAbs() from OpenCV
	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);

	// Thirdly, we add the two gradients together
	addWeighted(abs_grad_x, 1, abs_grad_y, 1, 0, grad);

	//cout << "\ngrad x type: " << grad_x.type();
	//cout << "\ngrad y type: " << grad_y.type();
	//cout << "\ngrad type: " << grad.type();
	//cout << "\n grad y rows: " << grad_y.rows;
	//cout << " \n grad y cols: " << grad_y.cols;
	//float y_comp, x_comp, div;
	//float y_comp_1, x_comp_1;

	short y_comp_short, x_comp_short;
	//float angle;
	//float y_comp_int, x_comp_int;
	unsigned char max = 255;
	unsigned char min = 0;
	// Update grad matrix values based on direction
	for(int i = 0; i < grad.rows; i++)
	{
		for(int j = 0; j < grad.cols; j++)
		{
			
			
			y_comp_short = grad_y.at<short>(j,i);
			x_comp_short = grad_x.at<short>(j,i);
		
			double angle_rad = atan2((double)y_comp_short, (double)x_comp_short);
			int direction_degrees = (int)(180.0 + angle_rad / M_PI * 180.0);

			int pixel_value = (int)grad.at<unsigned char>(j,i);
			if (direction_degrees > 35 && direction_degrees < 55 && pixel_value > 80) {
				// If angle value is within range then set to MAX
				grad.at<unsigned char>(j,i) = (uchar)255;
			} else {
				// If angle value is not within range then set to MIN
				grad.at<unsigned char>(j,i) = (uchar)0;
			}
			
		}
	}
	
	tgt_roi = grad.clone();

}
/*-----------------------------------------------------------------------**/
void utility::sobel_color(Mat &roi, Mat &tgt_roi, int KernelSize)
{
	Mat grad_x, grad_y, grad, abs_grad_x, abs_grad_y, roi_hsv;

	// Convert the image to HSV
	cvtColor(roi, roi_hsv, COLOR_BGR2HSV);

	// We only need to apply Sobel to the V component
	Mat roi_v;
	std::vector<cv::Mat> channels;
	split(roi_hsv, channels);

	Mat v_component = channels[2];

	// Make copy of v component
	Mat v_sobel = v_component.clone();

	// Firstly, we use the sobel function from OpenCV to compute the gradients in the x and y direction individually
	Sobel(v_sobel, grad_x, -1, 1, 0, KernelSize);
	Sobel(v_sobel, grad_y, -1, 0, 1, KernelSize);
	
	// Secondly, we convert the grad_x, grad_y to 8U using the convertScaleAbs() from OpenCV
	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);

	// Thirdly, we add the two gradients together
	addWeighted(abs_grad_x, 1, abs_grad_y, 1, 0, grad);

	// Update the V component of the original ROI with the new updated V values after Sobel
	//v_sobel.copyTo(v_component);

	// Update without pointer
	for(int i = 0; i < roi_hsv.rows; i++)
	{
		for(int j = 0; j < roi_hsv.cols; j++)
		{
			// Update the old V value with the new V value after sobel
			roi_hsv.at<Vec3b>(j,i)[2] = grad.at<unsigned char>(j,i);
		}
	}
	// Return the original roi which should have been modified
	tgt_roi = roi_hsv.clone();

}





/*-----------------------------------------------------------------------**/
void utility::cv_sobel3(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
{
	
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

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
		
		sobel_grayscale(src_roi1, roi1_output, 3);
		sobel_grayscale(src_roi2, roi2_output, 3);
		sobel_grayscale(src_roi3, roi3_output, 3);

	} else if (roi == 2) {

		sobel_grayscale(src_roi1, roi1_output, 3);
		sobel_grayscale(src_roi2, roi2_output, 3);
	
	} else {

		sobel_grayscale(src_roi1, roi1_output, 3);

	}
	

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
	cout << " \nFunction sobel3 was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::cv_sobel5(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
{
	
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
    
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

	// Apply Sobel with Kernel Size 5x5 on ROIs
	if (roi == 3) {
		
		sobel_grayscale(src_roi1, roi1_output, 5);
		sobel_grayscale(src_roi2, roi2_output, 5);
		sobel_grayscale(src_roi3, roi3_output, 5);

	} else if (roi == 2) {

		sobel_grayscale(src_roi1, roi1_output, 5);
		sobel_grayscale(src_roi2, roi2_output, 5);
	
	} else {

		sobel_grayscale(src_roi1, roi1_output, 5);

	}
	

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

	//cout << "\nsobel5 was used!";
	cout << " \nFunction sobel5 was utilized. ";
    // Output	
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::cv_sobel3_color(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
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
		
		sobel_color(src_roi1, roi1_output, 3);
		sobel_color(src_roi2, roi2_output, 3);
		sobel_color(src_roi3, roi3_output, 3);

	} else if (roi == 2) {

		sobel_color(src_roi1, roi1_output, 3);
		sobel_color(src_roi2, roi2_output, 3);
	
	} else {

		sobel_color(src_roi1, roi1_output, 3);

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
	
	cout << " \nFunction coloredge3 was utilized. ";
	tgt = output_color.clone();

	
	
}

/*-----------------------------------------------------------------------**/
void utility::cv_sobel5_color(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
{
	//cout << "\n sobel5_color is called";
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
		
		sobel_color(src_roi1, roi1_output, 5);
		sobel_color(src_roi2, roi2_output, 5);
		sobel_color(src_roi3, roi3_output, 5);

	} else if (roi == 2) {

		sobel_color(src_roi1, roi1_output, 5);
		sobel_color(src_roi2, roi2_output, 5);
	
	} else {

		sobel_color(src_roi1, roi1_output, 5);

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
	tgt = output_color.clone();

	cout << " \nFunction coloredge5 was utilized. ";
	
}

/*-----------------------------------------------------------------------**/
void utility::cv_binaryedge(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
{
	
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

	// Internal Threshold Parameters
	int threshold1 = 150;
	int threshold2 = 100;
	int threshold3 = 220;

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
		
		sobel_grayscale_threshold(src_roi1, roi1_output, 3, threshold1);
		sobel_grayscale_threshold(src_roi2, roi2_output, 3, threshold2);
		sobel_grayscale_threshold(src_roi3, roi3_output, 3, threshold3);

	} else if (roi == 2) {

		sobel_grayscale_threshold(src_roi1, roi1_output, 3, threshold1);
		sobel_grayscale_threshold(src_roi2, roi2_output, 3, threshold2);
	
	} else {

		sobel_grayscale_threshold(src_roi1, roi1_output, 3, threshold1);

	}
	
	
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
    
    //cout << "\nbinaryedge was used!";
	cout << " \nFunction binaryedge was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::cv_binaryedge_direction(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
{
	
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
    
	// Internal Threshold Parameters
	int threshold1 = 150;
	int threshold2 = 100;
	int threshold3 = 220;
	
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
		
		sobel_grayscale_threshold_direction(src_roi1, roi1_output, 3, threshold1);
		sobel_grayscale_threshold_direction(src_roi2, roi2_output, 3, threshold2);
		sobel_grayscale_threshold_direction(src_roi3, roi3_output, 3, threshold3);

	} else if (roi == 2) {

		sobel_grayscale_threshold_direction(src_roi1, roi1_output, 3, threshold1);
		sobel_grayscale_threshold_direction(src_roi2, roi2_output, 3, threshold2);
	
	} else {

		sobel_grayscale_threshold_direction(src_roi1, roi1_output, 3, threshold1);

	}
	
	
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
    
    //cout << "\n Binary Edge direction thresholding was used!";
	cout << " \nFunction binaryedge_direction was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::cv_canny(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
{
	
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

	// Declare offsets 
	//int roi1_i_offset, roi1_j_offset, roi2_i_offset, roi2_j_offset, roi3_i_offset, roi3_j_offset;

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
		
		canny_grayscale(src_roi1, roi1_output, 3);
		canny_grayscale(src_roi2, roi2_output, 3);
		canny_grayscale(src_roi3, roi3_output, 3);

	} else if (roi == 2) {

		canny_grayscale(src_roi1, roi1_output, 3);
		canny_grayscale(src_roi2, roi2_output, 3);
	
	} else {

		canny_grayscale(src_roi1, roi1_output, 3);

	}
	
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

    
    cout << " \nFunction cannycv was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::cv_histoeq(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
{
	
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

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
		
		histoeq_grayscale(src_roi1, roi1_output);
		histoeq_grayscale(src_roi2, roi2_output);
		histoeq_grayscale(src_roi3, roi3_output);

	} else if (roi == 2) {

		histoeq_grayscale(src_roi1, roi1_output);
		histoeq_grayscale(src_roi2, roi2_output);
	
	} else {

		histoeq_grayscale(src_roi1, roi1_output);

	}
	

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
    
    cout << " \nFunction histoeq was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::cv_canny_hist(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
{
	
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

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
		
		canny_hist_grayscale(src_roi1, roi1_output, 3);
		canny_hist_grayscale(src_roi2, roi2_output, 3);
		canny_hist_grayscale(src_roi3, roi3_output, 3);

	} else if (roi == 2) {

		canny_hist_grayscale(src_roi1, roi1_output, 3);
		canny_hist_grayscale(src_roi2, roi2_output, 3);
	
	} else {

		canny_hist_grayscale(src_roi1, roi1_output, 3);

	}
	

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
    
    cout << " \nFunction cannyhistoeq was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::cv_otsucv(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
{
	
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

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
		
		otsu_grayscale(src_roi1, roi1_output);
		otsu_grayscale(src_roi2, roi2_output);
		otsu_grayscale(src_roi3, roi3_output);

	} else if (roi == 2) {

		otsu_grayscale(src_roi1, roi1_output);
		otsu_grayscale(src_roi2, roi2_output);
	
	} else {

		otsu_grayscale(src_roi1, roi1_output);

	}
	

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
    
    cout << " \nFunction otsucv was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

/*-----------------------------------------------------------------------**/
void utility::cv_otsuhisto(Mat &src, Mat &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int x2, int y2, int Sx2, int Sy2, int x3, int y3, int Sx3, int Sy3)
{
	
	// Convert original image to grayscale
	Mat src_gray;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

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
		
		otsu_histo_grayscale(src_roi1, roi1_output);
		otsu_histo_grayscale(src_roi2, roi2_output);
		otsu_histo_grayscale(src_roi3, roi3_output);

	} else if (roi == 2) {

		otsu_histo_grayscale(src_roi1, roi1_output);
		otsu_histo_grayscale(src_roi2, roi2_output);
	
	} else {

		otsu_histo_grayscale(src_roi1, roi1_output);

	}
	

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
    
    cout << " \nFunction otsuhisto was utilized. ";
	// Output
	tgt = src_gray.clone();
	
}

void utility::cv_QRcode(Mat &src, Mat &tgt)
{
	// Convert original image to grayscale
	Mat src_gray, points, rectifiedImage;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

	// Instatiate object
	QRCodeDetector qr_code = QRCodeDetector();
	std::string data = qr_code.detectAndDecode(src_gray, points, rectifiedImage);
	
	cout << "\nFunction QRcode was utilized. ";

	cout <<"\n Data after decoding: " << data <<endl;
	
	

	tgt = rectifiedImage.clone();


}

void utility::cv_QRcodequl(Mat &src, Mat &tgt)
{
	// Convert original image to grayscale
	Mat src_gray, box, rectified, src_gray_equalized;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

	// Apply Histogram Equalization before
	equalizeHist(src_gray, src_gray_equalized);
	// Instatiate object
	QRCodeDetector qr_code = QRCodeDetector();

	std::string data = qr_code.detectAndDecode(src_gray_equalized, box, rectified);
	
	cout << " \nFunction QRcodequl was utilized. ";

	cout <<"\n Data after decoding: " << data <<endl;
	
	
	tgt = rectified.clone();


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