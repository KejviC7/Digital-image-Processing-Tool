#include "utility.h"
#define MATH_PI 3.14
#define MAXRGB 255
#define MINRGB 0
#define DEFAULT 999999999

#include <iostream>
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
void utility::add(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int value1, int x2, int y2, int Sx2, int Sy2, int value2, int x3, int y3, int Sx3, int Sy3, int value3)
{

	
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value1));
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value2)); 
			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value3)); 
			} else {
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
			}
		}
	
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int threshhold1, int x2, int y2, int Sx2, int Sy2, int threshhold2, int x3, int y3, int Sx3, int Sy3, int threshhold3)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				if (src.getPixel(i,j) < threshhold1)
					tgt.setPixel(i,j,MINRGB);
				else
					tgt.setPixel(i,j,MAXRGB);
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				if (src.getPixel(i,j) < threshhold2)
					tgt.setPixel(i,j,MINRGB);
				else
					tgt.setPixel(i,j,MAXRGB);
			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				if (src.getPixel(i,j) < threshhold3)
					tgt.setPixel(i,j,MINRGB);
				else
					tgt.setPixel(i,j,MAXRGB); 
			} else {
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
			}
		}
}

/*-----------------------------------------------------------------------**/
void utility::uniformsmooth(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int windowsize1, int x2, int y2, int Sx2, int Sy2, int windowsize2, int x3, int y3, int Sx3, int Sy3, int windowsize3)
{

	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	//cout << "\ni value: " << src.getNumberOfRows();
	//cout << "\nj value: " << src.getNumberOfColumns();
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
					
				double sum = 0.0;
				int kernel_size = windowsize1 / 2;
				int edge_pixel_counter = 0;
				for (int k=-kernel_size; k <=kernel_size; k++) {
					for (int t=-kernel_size; t <=kernel_size; t++) {
						
						// We need to handle the edges
						if ((i+k) < 0 || (j+t) < 0 || (i+k) > src.getNumberOfRows() || (j+t) > src.getNumberOfColumns()) {
							// Do not take that pixel into consideration because the kernel is out of the image
							// Increment edge pixel counter.
							edge_pixel_counter++;
							continue;
						} else { 
							sum += src.getPixel((i+k),(j+t));
						}
						//cout << "This is the sum: " << sum;
						
					}
				}
				//cout << "This is the sum: " << sum;
				double avg = sum / ((pow(windowsize1,2) - edge_pixel_counter));
				tgt.setPixel(i,j,avg);
		      
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				double sum = 0.0;
				int edge_pixel_counter = 0;
				int kernel_size = windowsize2 / 2;
				for (int k=-kernel_size; k <=kernel_size; k++) {
					for (int t=-kernel_size; t <=kernel_size; t++) {
						
						// We need to handle the edges
						if ((i+k) < 0 || (j+t) < 0 || (i+k) > src.getNumberOfRows() || (j+t) > src.getNumberOfColumns()) {
							// Do not take that pixel into consideration because the kernel is out of the image
							// Increment edge pixel counter.
							edge_pixel_counter++;
							continue;
						} else { 
							sum += src.getPixel((i+k),(j+t));
						}
						
					}
				}
				//cout << "This is the sum: " << sum;
				double avg = sum / ((pow(windowsize2,2) - edge_pixel_counter));
				tgt.setPixel(i,j,avg);

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				double sum = 0.0;
				int kernel_size = windowsize3 / 2;
				int edge_pixel_counter = 0;
				for (int k=-kernel_size; k <=kernel_size; k++) {
					for (int t=-kernel_size; t <=kernel_size; t++) {
						
						// We need to handle the edges
						if ((i+k) < 0 || (j+t) < 0 || (i+k) > src.getNumberOfRows() || (j+t) > src.getNumberOfColumns()) {
							// Do not take that pixel into consideration because the kernel is out of the image
							// Increment edge pixel counter.
							edge_pixel_counter++;
							continue;
						} else { 
							sum += src.getPixel((i+k),(j+t));
						}
						
					}
				}
				//cout << "This is the sum: " << sum;
				double avg = sum / ((pow(windowsize3,2) - edge_pixel_counter));
				tgt.setPixel(i,j,avg);

			} else {
			
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
			}

		
		}
}

/*-----------------------------------------------------------------------**/
void utility::adptvsmooth(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int windowsize1, int threshhold1, int x2, int y2, int Sx2, int Sy2, int windowsize2, int threshhold2, int x3, int y3, int Sx3, int Sy3, int windowsize3, int threshhold3)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
					
				
				double sum = 0.0;
			    int edge_pixel_counter = 0;
				double kernel[windowsize1][windowsize1];
				int kernel_size = windowsize1 / 2;

				for (int k=-kernel_size; k <=kernel_size; k++) {
					for (int t=-kernel_size; t <=kernel_size; t++) {
						// We need to handle the edges
						if ((i+k) < 0 || (j+t) < 0 || (i+k) > src.getNumberOfRows() || (j+t) > src.getNumberOfColumns()) {
							// Do not take that pixel into consideration because the kernel is out of the image
							// Increment edge pixel counter.
							edge_pixel_counter++;
							continue;
						} else { 
							sum += src.getPixel((i+k),(j+t));
						}
						
					}
				}
				//cout << "This is the sum: " << sum;
				double avg = sum / ((pow(windowsize1,2) - edge_pixel_counter));
				double change = src.getPixel(i,j) - avg;
				
				if (abs(change) > threshhold1) {
					// Change is greater than the threshhold. Keep original
					tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 

				} else {

					tgt.setPixel(i,j,avg);
				}
		        
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				
				double sum = 0.0;
				int edge_pixel_counter = 0;
				int kernel_size = windowsize2 / 2;
				for (int k=-kernel_size; k <=kernel_size; k++) {
					for (int t=-kernel_size; t <=kernel_size; t++) {
						// We need to handle the edges
						if ((i+k) < 0 || (j+t) < 0 || (i+k) > src.getNumberOfRows() || (j+t) > src.getNumberOfColumns()) {
							// Do not take that pixel into consideration because the kernel is out of the image
							// Increment edge pixel counter.
							edge_pixel_counter++;
							continue;
						} else { 
							sum += src.getPixel((i+k),(j+t));
						}
						
					}
				}
				//cout << "This is the sum: " << sum;
				double avg = sum / ((pow(windowsize2,2) - edge_pixel_counter));
				
				double change = src.getPixel(i,j) - avg;
				if (abs(change) > threshhold2) {
					// Change is greater than the threshhold. Keep original
					tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
				} else {

					tgt.setPixel(i,j,avg);
				}
				

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
		
				double sum = 0.0;
				int edge_pixel_counter = 0;
				int kernel_size = windowsize3 / 2;
				for (int k=-kernel_size; k <=kernel_size; k++) {
					for (int t=-kernel_size; t <=kernel_size; t++) {
						// We need to handle the edges
						if ((i+k) < 0 || (j+t) < 0 || (i+k) > src.getNumberOfRows() || (j+t) > src.getNumberOfColumns()) {
							// Do not take that pixel into consideration because the kernel is out of the image
							// Increment edge pixel counter.
							edge_pixel_counter++;
							continue;
						} else { 
							sum += src.getPixel((i+k),(j+t));
						}
						
					}
				}
				//cout << "This is the sum: " << sum;
				double avg = sum / ((pow(windowsize3,2) - edge_pixel_counter));
				
				double change = src.getPixel(i,j) - avg;
				if (abs(change) > threshhold3) {
					// Change is greater than the threshhold. Keep original
					tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
				} else {

					tgt.setPixel(i,j,avg);
				}
                
			} else {
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
			}
		}
}

/*-----------------------------------------------------------------------**/
void utility::mulcolorbright(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, float DR, int x2, int y2, int Sx2, int Sy2, float DG, int x3, int y3, int Sx3, int Sy3, float DB)
{

	 
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			//cout << "R: " << src.getPixel(i,j);
			// Channel 0 - Red; Channnel 1 - Green; Channel 2 - Blue
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {

				tgt.setPixel(i,j,0,checkValue(src.getPixel(i,j,0)*DR));
				tgt.setPixel(i,j,1,checkValue(src.getPixel(i,j,1)));
				tgt.setPixel(i,j,2,checkValue(src.getPixel(i,j,2)));

			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {

				tgt.setPixel(i,j,0,checkValue(src.getPixel(i,j,0)));
				tgt.setPixel(i,j,1,checkValue(src.getPixel(i,j,1)*DG));
				tgt.setPixel(i,j,2,checkValue(src.getPixel(i,j,2)));

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {

				tgt.setPixel(i,j,0,checkValue(src.getPixel(i,j,0)));
				tgt.setPixel(i,j,1,checkValue(src.getPixel(i,j,1)));
				tgt.setPixel(i,j,2,checkValue(src.getPixel(i,j,2)*DB));

			} else {
				tgt.setPixel(i,j,RED,checkValue(src.getPixel(i,j,RED)));
				tgt.setPixel(i,j,GREEN,checkValue(src.getPixel(i,j,GREEN)));
				tgt.setPixel(i,j,BLUE,checkValue(src.getPixel(i,j,BLUE)));
				//cout << "R: " << src.getPixel(i,j,RED);

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
			/* Map the pixel of new image back to original image */
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				/* Directly copy the value */
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				/* Average the values of four pixels */
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}
