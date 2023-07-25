#include "utility.h"
#define MATH_PI 3.14159265358979323846
#define MAXRGB 255
#define MINRGB 0
#define DEFAULT 999999999
#define MAXLEN 256
#include <iostream>
#include <cmath>
#include <cstring>
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

int utility::checkChannel(char channel)
{
	if (channel == 'R') {
		return 0;
	} else if (channel == 'G') {
		return 1;
	} else if (channel == 'B') {
		return 2;
	} else if (channel == 'H') {
		return 3;
	} else if (channel == 'S') {
		return 4;
	} else if (channel == 'I') {
		return 6;
	}
	return 5;
}


void utility::rgbtohsi(image &src, image &conversion_image)
{
	// Step 1: Normalize RGB Values
	float r, g, b;
	float h;
	float intensity;
	float s;
	float min;
	float R,G,B;
	float numerator, denominator;
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			
			R = src.getPixel(i,j,0);
			G = src.getPixel(i,j,1);
			B = src.getPixel(i,j,2);
			// Step 1: Normalize RGB Values
			r = R / (R + G + B);
			g = G / (R + G + B);
			b = B / (R + G + B);
            
			//cout << "\nr: " << r;
			// Step 2: Logic for converting to H value
			if (b <= g) {
				
				numerator = 0.5 * ((r - g) + (r - b));
				denominator = sqrt(pow((r - g),2) + ((r-b) * (g-b)));
				h = acos(numerator / denominator);
				// Convert h to range [0,360]
				h = h * 180 / MATH_PI;
				//cout << "\nh value is: " << h;
				// Set the new h value as the corresponding "RED channel" of the HSI image (conversion image)
				conversion_image.setPixel(i,j,0,h);

			} else if (b > g) {
				numerator = 0.5 * ((r - g) + (r - b));
				denominator = sqrt(pow((r - g),2) + ((r-b) * (g-b)));
				h = (2 * MATH_PI) - acos(numerator / denominator);
				// Convert h to range [0,360]
				h = h * 180 / MATH_PI;
                //cout << "\nh value is: " << h;
				// Set the new h value as the corresponding "RED channel" of the HSI image (conversion image)
				conversion_image.setPixel(i,j,0,h);
			}
            
			// Step 3: Logic for converting to S value
			min = r;
			if (g < min) {
				min = g;
			}

			if (b < min) {
				min = b;
			}

			s = 1 - (3 * min);
			// Convert s to range [0,100]
			s = s * 100;
	        //cout << "\n S value: " << s;
			// Set the new s value as the corresponding "GREEN channel" of the HSI image (conversion image)
			conversion_image.setPixel(i,j,1,s);
			
            
			// Step 4: Logic for converting to I value
			
			R = src.getPixel(i,j,0);
			G = src.getPixel(i,j,1);
			B = src.getPixel(i,j,2);

			intensity = (R + G + B) / (3 * 255);

			// Convert i to range [0, 255]
			intensity = intensity * 255;

			// Set the new i value as the corresponding "BLUE channel" of the HSI image (conversion image)
			conversion_image.setPixel(i,j,2,round(intensity));
            
		}
}

void utility::hsitorgb(image &conversion_image, image &tgt)
{
	// Step 1: Normalize RGB Values
	float r, g, b;
	float h;
	float intensity;
	float s;
	float min;
	float R,G,B;
	float res;
	float numerator, denominator;
	float H, S, Intensity;
	for (int i=0; i<conversion_image.getNumberOfRows(); i++)
		for (int j=0; j<conversion_image.getNumberOfColumns(); j++)
		{
			
			// Convert H,S,I to their normal ranges
			H = conversion_image.getPixel(i,j,0);
			S = conversion_image.getPixel(i,j,1);
			Intensity = conversion_image.getPixel(i,j,2);
			
			h = H * MATH_PI / 180;
			s = S / 100;
			intensity = Intensity / 255;
            

			//cout << "\nh value: " << h;
			//cout << "\ns value: " << s;
			//cout << "\nintensity value:" << intensity;
			//cout <<endl;
			
			if (h >= 0 && h < (2*MATH_PI/3)) {
				
				b = intensity * (1 - s);
				
				numerator = s * cos(h);
				denominator = cos((MATH_PI/3) - h);
				res = numerator / denominator;

				r = intensity * (1 + res);

				g = 3*intensity - (b + r);

				//cout << "\nr value: " << r;
				//cout << "\ng value: " << g;
				//cout << "\nb value:" << b;
				//cout <<endl;

			} else if (h >= (2 * MATH_PI / 3) && h < (4*MATH_PI/3)) {

				h = h - (2 * MATH_PI / 3);

				r = intensity * (1 - s);
				
				numerator = s * cos(h);
				denominator = cos((MATH_PI/3) - h);
				res = numerator / denominator;

				g = intensity * (1 + res);

				b = 3*intensity - (r + g);
				
				//cout << "\nr value: " << r;
				//cout << "\ng value: " << g;
				//cout << "\nb value:" << b;
				//cout <<endl;

			} else if (h >= (4 * MATH_PI / 3) && h < (2 * MATH_PI)) {
				
				h = h - (4 * MATH_PI / 3);

				g = intensity * (1 - s);
				
				numerator = s * cos(h);
				denominator = cos((MATH_PI/3) - h);
				res = numerator / denominator;

				b = intensity * (1 + res);

				r = 3*intensity - (g + b);

				//cout << "\nr value: " << r;
				//cout << "\ng value: " << g;
				//cout << "\nb value:" << b;
				//cout <<endl;

			}

			// As we can see for each h value we have different conditions for computing r,g,b
			// Before we populate tgt image with the r,g,b values we need to convert r,g,b from normalized

			R = r * 255;
			G = g * 255;
			B = b * 255;

			//cout << "\nR value: " << R;
			//cout << "\nG value: " << G;
			//cout << "\nB value:" << B;
			//cout <<endl;

			// Now we pass these to tgt

			tgt.setPixel(i,j,0,checkValue(round(R)));
			tgt.setPixel(i,j,1,checkValue(round(G)));
			tgt.setPixel(i,j,2,checkValue(round(B)));

            
		}
}

void utility::initializeMinMax(int histogram[256][2], int &C, int &D)
{
	
	for (int i=0; i<256;i++) 
	{

		if (histogram[i][1] != 0) {
			C = histogram[i][0];
			D = histogram[i][0];
			break;;
		} else {
			continue;
		}
	}

}
void utility::findMinMax(int histogram[256][2], int &C, int &D)
{
	
	for (int i=0; i<256;i++) 
	{

		if (histogram[i][1] != 0) {
			if (histogram[i][0] > D) {
				// Set new max intensity with frequency > 0
				D = histogram[i][0];
			}

			if (histogram[i][0] < C) {
				// Set new min intensity with frequency > 0
				C = histogram[i][0];
			}
		} else {
			continue;
		}
	}

}

void utility::visualizeHistogram(int histogram[256][2]) 
{
	// Find level that has max frequency
	int level = 0;
	for (int i=0; i<256;i++) 
	{
		if (histogram[i][1] > level) 
		{
			level = histogram[i][1];
		}
	}

	// Visualize Histogram Vertically
	for (;level>0; level--) {
		for (int i =0; i < 256; i++)
		{
			if (histogram[i][1] > level) {
				cout << "*";
			} else {
				cout << "-";
			}
		
		}
		cout << endl;
	}
}

void utility::visualizeHistogramHorizontally(int histogram[256][2]) 
{
	
	for (int i =0; i<256; i++)
	{
		cout << i;
		for (int k=0; k<histogram[i][1];k++) 
		{
			cout << '*';
		}
        cout << endl;
	}
	
}

void utility::Histostretch(int histogram[256][2], string name)
{
	image histogram_output;
	char hist_out[MAXLEN];
	string name_hist = name;
	
    
	// First we find the level that has the highest frequency, and store that level as we will use it as out row size.
	// Columns size will be 256
	int level = 0;
	for (int i=0; i<256;i++) 
	{
		if (histogram[i][1] > level) 
		{
			level = histogram[i][1];
			
		}
	}

	// Resize the Histogram Image
	histogram_output.resize(level, 256);
	

	for (;level>0; level--) 
		for (int j =0; j < 256; j++)
		{
			if (histogram[j][1] > level) {
				histogram_output.setPixel(level,j, MAXRGB);
				//continue;
			} else {
				//hist.setPixel(level,j, MINRGB);
				//continue;
			}
		
		}
	
	//histogram_output.resize(256, 256);
	
	
	strcpy(hist_out, name_hist.c_str());
	// Output Histogram as pgm file
	histogram_output.save(hist_out);
	
}




/*-----------------------------------------------------------------------**/
void utility::althistostretch(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int A1, int B1, int x2, int y2, int Sx2, int Sy2, int A2, int B2, int x3, int y3, int Sx3, int Sy3, int A3, int B3)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	int histogram_roi1[256][2];
	int histogram_roi2[256][2];
	int histogram_roi3[256][2];
	
	int histogram_roi1_post[256][2];
	int histogram_roi2_post[256][2];
	int histogram_roi3_post[256][2];

	int ROI1_C1, ROI1_D1;
	int ROI2_C2, ROI2_D2;
	int ROI3_C3, ROI3_D3;
	// Firstly, we need to initialize these arrays

	for (int i =0; i<256; i++)
		for (int j=0; j<2; j++)
		{
			if (j == 0) {
				// This is the intensity level (range 0 - 255)
				histogram_roi1[i][j] = i;
				histogram_roi2[i][j] = i;
				histogram_roi3[i][j] = i;
				histogram_roi1_post[i][j] = i;
				histogram_roi2_post[i][j] = i;
				histogram_roi3_post[i][j] = i;
			} else if (j == 1) {
				// Corresponding frequency per intensity level. Set to 0 initially
				histogram_roi1[i][j] = 0;
				histogram_roi2[i][j] = 0;
				histogram_roi3[i][j] = 0;
				histogram_roi1_post[i][j] = 0;
				histogram_roi2_post[i][j] = 0;
				histogram_roi3_post[i][j] = 0;

			}
		}
	
	// Secondly, we need to populate these arrays. 

	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				int current_pixel_value = src.getPixel(i,j);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<256; row++)
				{	
					
					if (current_pixel_value == histogram_roi1[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1[row][1]++;
					} 
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				int current_pixel_value = src.getPixel(i,j);
				
				for (int row=0; row<256; row++)
				{
					if (current_pixel_value == histogram_roi2[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2[row][1]++;
					} 
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				int current_pixel_value = src.getPixel(i,j);
				
				for (int row=0; row<256; row++)
				{
					if (current_pixel_value == histogram_roi3[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3[row][1]++;
					} 
				}
			} else {
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
			}
		}

	// Step 3: Search in the computed histogram for the C (min intensity) and D (max intensity) values.
	
	// Call helper function to initialize C, D values from histogram. We will use these later to pass to MinMax functions to compute C-min and D-max in histogram
	initializeMinMax(histogram_roi1, ROI1_C1, ROI1_D1);
	initializeMinMax(histogram_roi2, ROI2_C2, ROI2_D2);
	initializeMinMax(histogram_roi3, ROI3_C3, ROI3_D3);
    
	// Call helper function to find C-min and D-max from histogram
	findMinMax(histogram_roi1, ROI1_C1, ROI1_D1);
	findMinMax(histogram_roi2, ROI2_C2, ROI2_D2);
	findMinMax(histogram_roi3, ROI3_C3, ROI3_D3);

	// Step 4: Now that we have the histogram and the C, D values for each ROI we can start stretching the histogram by modifying the pixel values using the formula new_p = (P-C) * ((B-A)/(D-C)) + A.
 
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// 5th and 95th percentile condition
				if (src.getPixel(i,j) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,checkValue(A1));
				} else if (src.getPixel(i,j) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,checkValue(B1));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j) - ROI1_C1;
					new_value *= (B1 - A1); 
					new_value /= (ROI1_D1 - ROI1_C1);
					new_value += A1;
					tgt.setPixel(i,j,checkValue(new_value));
				}
				
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = tgt.getPixel(i,j);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<256; row++)
				{	
					
					if (current_pixel_value == histogram_roi1_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_post[row][1]++;
					} 
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// 5th and 95th percentile condition
				if (src.getPixel(i,j) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,checkValue(A2));
				} else if (src.getPixel(i,j) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,checkValue(B2));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j) - ROI2_C2;
					new_value *= (B2 - A2); 
					new_value /= (ROI2_D2 - ROI2_C2);
					new_value += A2;
					tgt.setPixel(i,j,checkValue(new_value));
				}

				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = tgt.getPixel(i,j);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<256; row++)
				{	
					
					if (current_pixel_value == histogram_roi2_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_post[row][1]++;
					} 
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// 5th and 95th percentile condition
				if (src.getPixel(i,j) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,checkValue(A3));
				} else if (src.getPixel(i,j) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,checkValue(B3));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j) - ROI3_C3;
					new_value *= (B3 - A3); 
					new_value /= (ROI3_D3 - ROI3_C3);
					new_value += A3;
					tgt.setPixel(i,j,checkValue(new_value));
				}	

				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = tgt.getPixel(i,j);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<256; row++)
				{	
					
					if (current_pixel_value == histogram_roi3_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_post[row][1]++;
					} 
				}

			} else {
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
			}
		}

     

	// Step 5. Compute and Plot the New Histograms. 

    Histostretch(histogram_roi1, "altstretch_roi1_pre_stretching.pgm");
	Histostretch(histogram_roi1_post, "altstretch_roi1_post_stretching.pgm");
	Histostretch(histogram_roi2, "altstretch_roi2_pre_stretching.pgm");
	Histostretch(histogram_roi2_post, "altstretch_roi2_post_stretching.pgm");
	Histostretch(histogram_roi3, "altstretch_roi3_pre_stretching.pgm");
	Histostretch(histogram_roi3_post, "altstretch_roi3_post_stretching.pgm");

    
}

/*-----------------------------------------------------------------------**/
void utility::histothres(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int threshold1, int A1, int B1, int x2, int y2, int Sx2, int Sy2, int threshold2, int A2, int B2, int x3, int y3, int Sx3, int Sy3, int threshold3, int A3, int B3)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	int histogram_roi1[256][2];
	int histogram_roi2[256][2];
	int histogram_roi3[256][2];
	
	int histogram_roi1_post[256][2];
	int histogram_roi2_post[256][2];
	int histogram_roi3_post[256][2];

	int ROI1_C1, ROI1_D1;
	int ROI2_C2, ROI2_D2;
	int ROI3_C3, ROI3_D3;
	// Firstly, we need to initialize these arrays

	for (int i =0; i<256; i++)
		for (int j=0; j<2; j++)
		{
			if (j == 0) {
				// This is the intensity level (range 0 - 255)
				histogram_roi1[i][j] = i;
				histogram_roi2[i][j] = i;
				histogram_roi3[i][j] = i;
				histogram_roi1_post[i][j] = i;
				histogram_roi2_post[i][j] = i;
				histogram_roi3_post[i][j] = i;
			} else if (j == 1) {
				// Corresponding frequency per intensity level. Set to 0 initially
				histogram_roi1[i][j] = 0;
				histogram_roi2[i][j] = 0;
				histogram_roi3[i][j] = 0;
				histogram_roi1_post[i][j] = 0;
				histogram_roi2_post[i][j] = 0;
				histogram_roi3_post[i][j] = 0;

			}
		}
	
	// Secondly, we need to populate these arrays. Remember that now we will be using thresholding so not all pixel values will be used!

	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				int current_pixel_value = src.getPixel(i,j);
				
				//IMPORTANT! Now we are using thresholding so we basically need to populate the histogram with pixel values < threshold
				// We split the image into two parts dark and bright via an extra if-else condition.
				if (current_pixel_value < threshold1) {

					// These will be our dark pixels on which we will perform histogram stretching on. So we will be using these pixel values
					for (int row=0; row<256; row++)
					{
						
						if (current_pixel_value == histogram_roi1[row][0]) {
							// Increment the number of occurrences for that particular grey level
							histogram_roi1[row][1]++;
						} 
					}
				} else {
					// We are not going to use bright pixels!
					continue;
				}
				
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				int current_pixel_value = src.getPixel(i,j);
				//IMPORTANT! Now we are using thresholding so we basically need to populate the histogram with pixel values < threshold
				// We split the image into two parts dark and bright via an extra if-else condition.
				if (current_pixel_value < threshold2) {

					// These will be our dark pixels on which we will perform histogram stretching on. So we will be using these pixel values
					for (int row=0; row<256; row++)
					{
						
						if (current_pixel_value == histogram_roi2[row][0]) {
							// Increment the number of occurrences for that particular grey level
							histogram_roi2[row][1]++;
						} 
					}
				} else {
					// We are not going to use bright pixels!
					continue;
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				int current_pixel_value = src.getPixel(i,j);
				//IMPORTANT! Now we are using thresholding so we basically need to populate the histogram with pixel values < threshold
				// We split the image into two parts dark and bright via an extra if-else condition.
				if (current_pixel_value < threshold3) {

					// These will be our dark pixels on which we will perform histogram stretching on. So we will be using these pixel values
					for (int row=0; row<256; row++)
					{
						
						if (current_pixel_value == histogram_roi3[row][0]) {
							// Increment the number of occurrences for that particular grey level
							histogram_roi3[row][1]++;
						} 
					}
				} else {
					// We are not going to use bright pixels!
					continue;
				}

			} else {
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
			}
		}

	// Step 3: Search in the computed histogram for the C (min intensity) and D (max intensity) values.
	
	// Call helper function to initialize C, D values from histogram. We will use these later to pass to MinMax functions to compute C-min and D-max in histogram. Only Dark Pixels!
	initializeMinMax(histogram_roi1, ROI1_C1, ROI1_D1);
	initializeMinMax(histogram_roi2, ROI2_C2, ROI2_D2);
	initializeMinMax(histogram_roi3, ROI3_C3, ROI3_D3);
    
	// Call helper function to find C-min and D-max from histogram
	findMinMax(histogram_roi1, ROI1_C1, ROI1_D1);
	findMinMax(histogram_roi2, ROI2_C2, ROI2_D2);
	findMinMax(histogram_roi3, ROI3_C3, ROI3_D3);

	// Step 4: Now that we have the histogram and the C, D values for each ROI we can start stretching the histogram by modifying the pixel values using the formula new_p = (P-C) * ((B-A)/(D-C)) + A.
 
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				if (src.getPixel(i,j) < threshold1) {
					// Perform histogram stretching on Dark Pixels only!
					
					// 5th and 95th percentile condition
					if (src.getPixel(i,j) < 0.05 * 255) {
						// If pixel value is in interval [0, 12.75] we update it to A
						tgt.setPixel(i,j,checkValue(A1));
					} else if (src.getPixel(i,j) > 0.95 * 255) {
						// If pixel value is in interval [242.55, 255] we update it to B
						tgt.setPixel(i,j,checkValue(B1));
					} else {
						// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
						int new_value = src.getPixel(i,j) - ROI1_C1;
						new_value *= (B1 - A1); 
						new_value /= (ROI1_D1 - ROI1_C1);
						new_value += A1;
						tgt.setPixel(i,j,checkValue(new_value));
					}

					// Now we also need to update the Histogram after the Stretching of the Pixel
					int current_pixel_value = tgt.getPixel(i,j);
					//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
					for (int row=0; row<256; row++)
					{	
						
						if (current_pixel_value == histogram_roi1_post[row][0]) {
							// Increment the number of occurrences for that particular grey level
							histogram_roi1_post[row][1]++;
						} 
					}

				} else {
					// We won't be performing histogram stretching on Bright Pixels!
					tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {

				
				if (src.getPixel(i,j) < threshold2) {
					// Perform histogram stretching on Dark Pixels only!
					// 5th and 95th percentile condition
					if (src.getPixel(i,j) < 0.05 * 255) {
						// If pixel value is in interval [0, 12.75] we update it to A
						tgt.setPixel(i,j,checkValue(A2));
					} else if (src.getPixel(i,j) > 0.95 * 255) {
						// If pixel value is in interval [242.55, 255] we update it to B
						tgt.setPixel(i,j,checkValue(B2));
					} else {
						// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
						int new_value = src.getPixel(i,j) - ROI2_C2;
						new_value *= (B2 - A2); 
						new_value /= (ROI2_D2 - ROI2_C2);
						new_value += A2;
						tgt.setPixel(i,j,checkValue(new_value));
					}

					// Now we also need to update the Histogram after the Stretching of the Pixel
					int current_pixel_value = tgt.getPixel(i,j);
					//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
					for (int row=0; row<256; row++)
					{	
						
						if (current_pixel_value == histogram_roi2_post[row][0]) {
							// Increment the number of occurrences for that particular grey level
							histogram_roi2_post[row][1]++;
						} 
					}

				} else {
					// We won't be performing histogram stretching on Bright Pixels!
					tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
				}
			

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				

				if (src.getPixel(i,j) < threshold3) {
					// Perform histogram stretching on Dark Pixels only!

					// 5th and 95th percentile condition
					if (src.getPixel(i,j) < 0.05 * 255) {
						// If pixel value is in interval [0, 12.75] we update it to A
						tgt.setPixel(i,j,checkValue(A3));
					} else if (src.getPixel(i,j) > 0.95 * 255) {
						// If pixel value is in interval [242.55, 255] we update it to B
						tgt.setPixel(i,j,checkValue(B3));
					} else {
						// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
						int new_value = src.getPixel(i,j) - ROI3_C3;
						new_value *= (B3 - A3); 
						new_value /= (ROI3_D3 - ROI3_C3);
						new_value += A3;
						tgt.setPixel(i,j,checkValue(new_value));
					}	

					// Now we also need to update the Histogram after the Stretching of the Pixel
					int current_pixel_value = tgt.getPixel(i,j);
					//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
					for (int row=0; row<256; row++)
					{	
						
						if (current_pixel_value == histogram_roi3_post[row][0]) {
							// Increment the number of occurrences for that particular grey level
							histogram_roi3_post[row][1]++;
						} 
					}
				} else {
					// We won't be performing histogram stretching on Bright Pixels!
					tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
				}

			} else {
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j))); 
			}
		}

     

	// Step 5. Compute and Plot the New Histograms. 

    // You can call the Histrostretch function
}

/*-----------------------------------------------------------------------**/
void utility::percchastrech(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, char channel1, int A1, int B1, int x2, int y2, int Sx2, int Sy2, char channel2, int A2, int B2, int x3, int y3, int Sx3, int Sy3, char channel3, int A3, int B3)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	int histogram_roi1[256][2];
	int histogram_roi2[256][2];
	int histogram_roi3[256][2];
	
	int histogram_roi1_post[256][2];
	int histogram_roi2_post[256][2];
	int histogram_roi3_post[256][2];

	int ROI1_C1, ROI1_D1;
	int ROI2_C2, ROI2_D2;
	int ROI3_C3, ROI3_D3;
	// Firstly, we need to initialize these arrays

	for (int i =0; i<256; i++)
		for (int j=0; j<2; j++)
		{
			if (j == 0) {
				// This is the intensity level (range 0 - 255)
				histogram_roi1[i][j] = i;
				histogram_roi2[i][j] = i;
				histogram_roi3[i][j] = i;
				histogram_roi1_post[i][j] = i;
				histogram_roi2_post[i][j] = i;
				histogram_roi3_post[i][j] = i;
			} else if (j == 1) {
				// Corresponding frequency per intensity level. Set to 0 initially
				histogram_roi1[i][j] = 0;
				histogram_roi2[i][j] = 0;
				histogram_roi3[i][j] = 0;
				histogram_roi1_post[i][j] = 0;
				histogram_roi2_post[i][j] = 0;
				histogram_roi3_post[i][j] = 0;

			}
		}
	
	// Mid-Step: Translate channels into int levels to be used
	int channel1_index, channel2_index, channel3_index;
	//cout << "\nChannel 1" << channel1;
	channel1_index = checkChannel(channel1);
	channel2_index = checkChannel(channel2);
	channel3_index = checkChannel(channel3);

	//cout << "\nChannel index " << channel1_index;
	// Secondly, we need to populate these arrays. 
	

	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// We collect pixels of corresponding user provided channel

				int current_pixel_value = src.getPixel(i,j,channel1_index);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<256; row++)
				{	
					
					if (current_pixel_value == histogram_roi1[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1[row][1]++;
					} 
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// We collect pixels of corresponding user provided channel
				
				int current_pixel_value = src.getPixel(i,j,channel2_index);
				
				for (int row=0; row<256; row++)
				{
					if (current_pixel_value == histogram_roi2[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2[row][1]++;
					} 
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// We collect pixels of corresponding user provided channel
				
				int current_pixel_value = src.getPixel(i,j,channel3_index);
				
				for (int row=0; row<256; row++)
				{
					if (current_pixel_value == histogram_roi3[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3[row][1]++;
					} 
				}
			} else {
				continue;
			}
		}

	// Step 3: Search in the computed histogram for the C (min intensity) and D (max intensity) values.
	
	// Call helper function to initialize C, D values from histogram. We will use these later to pass to MinMax functions to compute C-min and D-max in histogram
	initializeMinMax(histogram_roi1, ROI1_C1, ROI1_D1);
	initializeMinMax(histogram_roi2, ROI2_C2, ROI2_D2);
	initializeMinMax(histogram_roi3, ROI3_C3, ROI3_D3);
    
	// Call helper function to find C-min and D-max from histogram
	findMinMax(histogram_roi1, ROI1_C1, ROI1_D1);
	findMinMax(histogram_roi2, ROI2_C2, ROI2_D2);
	findMinMax(histogram_roi3, ROI3_C3, ROI3_D3);

	// Step 4: Now that we have the histogram and the C, D values for each ROI we can start stretching the histogram by modifying the pixel values using the formula new_p = (P-C) * ((B-A)/(D-C)) + A.
	// Variable to keep track of the channel index in a ROI which identifies the channel on which stretching will be performed.
	//int channel_index; 
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// We apply stretching only on pixels of corresponding user provided channel
				// 5th and 95th percentile condition
				if (src.getPixel(i,j,channel1_index) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,channel1_index, checkValue(A1));
				} else if (src.getPixel(i,j,channel1_index) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,channel1_index, checkValue(B1));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j,channel1_index) - ROI1_C1;
					new_value *= (B1 - A1); 
					new_value /= (ROI1_D1 - ROI1_C1);
					new_value += A1;
					tgt.setPixel(i,j,channel1_index, checkValue(new_value));
				}
				
				if (channel1_index == 0) {
					tgt.setPixel(i,j,1,src.getPixel(i,j,1));
					tgt.setPixel(i,j,2,src.getPixel(i,j,2));

				} else if (channel1_index == 1) {
					tgt.setPixel(i,j,0,src.getPixel(i,j,0));
					tgt.setPixel(i,j,2,src.getPixel(i,j,2));

				} else if (channel1_index == 2) {
					tgt.setPixel(i,j,0,src.getPixel(i,j,0));
					tgt.setPixel(i,j,1,src.getPixel(i,j,1));
				}
				
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = tgt.getPixel(i,j,channel1_index);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<256; row++)
				{	
					
					if (current_pixel_value == histogram_roi1_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_post[row][1]++;
					} 
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// 5th and 95th percentile condition
				if (src.getPixel(i,j, channel2_index) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,channel2_index, checkValue(A2));
				} else if (src.getPixel(i,j, channel2_index) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,channel2_index, checkValue(B2));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j, channel2_index) - ROI2_C2;
					new_value *= (B2 - A2); 
					new_value /= (ROI2_D2 - ROI2_C2);
					new_value += A2;
					tgt.setPixel(i,j,channel2_index, checkValue(new_value));
				}

				if (channel2_index == 0) {
					tgt.setPixel(i,j,1, src.getPixel(i,j,1));
					tgt.setPixel(i,j,2, src.getPixel(i,j,2));

				} else if (channel2_index == 1) {
					tgt.setPixel(i,j,0, src.getPixel(i,j,0));
					tgt.setPixel(i,j,2, src.getPixel(i,j,2));

				} else if (channel2_index == 2) {
					tgt.setPixel(i,j,0, src.getPixel(i,j,0));
					tgt.setPixel(i,j,1, src.getPixel(i,j,1));
				}
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = tgt.getPixel(i,j, channel2_index);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<256; row++)
				{	
					
					if (current_pixel_value == histogram_roi2_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_post[row][1]++;
					} 
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// 5th and 95th percentile condition
				if (src.getPixel(i,j, channel3_index) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,channel3_index, checkValue(A3));
				} else if (src.getPixel(i,j, channel3_index) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,channel3_index, checkValue(B3));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j, channel3_index) - ROI3_C3;
					new_value *= (B3 - A3); 
					new_value /= (ROI3_D3 - ROI3_C3);
					new_value += A3;
					tgt.setPixel(i,j,channel3_index, checkValue(new_value));
				}	
                
				if (channel3_index == 0) {
					tgt.setPixel(i,j,1, src.getPixel(i,j,1));
					tgt.setPixel(i,j,2, src.getPixel(i,j,2));

				} else if (channel3_index == 1) {
					tgt.setPixel(i,j,0, src.getPixel(i,j,0));
					tgt.setPixel(i,j,2, src.getPixel(i,j,2));

				} else if (channel3_index == 2) {
					tgt.setPixel(i,j,0, src.getPixel(i,j,0));
					tgt.setPixel(i,j,1, src.getPixel(i,j,1));
				}
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = tgt.getPixel(i,j, channel3_index);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<256; row++)
				{	
					
					if (current_pixel_value == histogram_roi3_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_post[row][1]++;
					} 
				}

			} else {
				tgt.setPixel(i,j,RED,checkValue(src.getPixel(i,j,RED)));
				tgt.setPixel(i,j,GREEN,checkValue(src.getPixel(i,j,GREEN)));
				tgt.setPixel(i,j,BLUE,checkValue(src.getPixel(i,j,BLUE)));
			}
		}

     

	// Step 5. Compute and Plot the New Histograms. 

    // Call Histostretch function
}

/*-----------------------------------------------------------------------**/
void utility::rgbstretch(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, char channel1, int A1, int B1, int x2, int y2, int Sx2, int Sy2, char channel2, int A2, int B2, int x3, int y3, int Sx3, int Sy3, char channel3, int A3, int B3)
{

	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	

	// Declare 2D arrays for each channel (R, G, B) for each ROI. Original Histogram
	int histogram_roi1_R[256][2];
	int histogram_roi1_G[256][2];
	int histogram_roi1_B[256][2];

	int histogram_roi2_R[256][2];
	int histogram_roi2_G[256][2];
	int histogram_roi2_B[256][2];

	int histogram_roi3_R[256][2];
	int histogram_roi3_G[256][2];
	int histogram_roi3_B[256][2];
	
	// Histograms after stretching has been performed. 
	int histogram_roi1_R_post[256][2];
	int histogram_roi1_G_post[256][2];
	int histogram_roi1_B_post[256][2];

	int histogram_roi2_R_post[256][2];
	int histogram_roi2_G_post[256][2];
	int histogram_roi2_B_post[256][2];

	int histogram_roi3_R_post[256][2];
	int histogram_roi3_G_post[256][2];
	int histogram_roi3_B_post[256][2];
    
	// C, D (min, max) values for each channel's histogram
	int ROI1_R_C1, ROI1_R_D1;
	int ROI1_G_C1, ROI1_G_D1;
	int ROI1_B_C1, ROI1_B_D1;

	int ROI2_R_C2, ROI2_R_D2;
	int ROI2_G_C2, ROI2_G_D2;
	int ROI2_B_C2, ROI2_B_D2;

	int ROI3_R_C3, ROI3_R_D3;
	int ROI3_G_C3, ROI3_G_D3;
	int ROI3_B_C3, ROI3_B_D3;

	// Firstly, we need to initialize these arrays

	for (int i =0; i<256; i++)
		for (int j=0; j<2; j++)
		{
			if (j == 0) {
				// This is the intensity level (range 0 - 255)
				histogram_roi1_R[i][j] = i;
				histogram_roi1_G[i][j] = i;
				histogram_roi1_B[i][j] = i;

				histogram_roi2_R[i][j] = i;
				histogram_roi2_G[i][j] = i;
				histogram_roi2_B[i][j] = i;

				histogram_roi3_R[i][j] = i;
				histogram_roi3_G[i][j] = i;
				histogram_roi3_B[i][j] = i;

				histogram_roi1_R_post[i][j] = i;
				histogram_roi1_G_post[i][j] = i;
				histogram_roi1_B_post[i][j] = i;

				histogram_roi2_R_post[i][j] = i;
				histogram_roi2_G_post[i][j] = i;
				histogram_roi2_B_post[i][j] = i;

				histogram_roi3_R_post[i][j] = i;
				histogram_roi3_G_post[i][j] = i;
				histogram_roi3_B_post[i][j] = i;

			} else if (j == 1) {
				// Corresponding frequency per intensity level. Set to 0 initially
				histogram_roi1_R[i][j] = 0;
				histogram_roi1_G[i][j] = 0;
				histogram_roi1_B[i][j] = 0;

				histogram_roi2_R[i][j] = 0;
				histogram_roi2_G[i][j] = 0;
				histogram_roi2_B[i][j] = 0;

				histogram_roi3_R[i][j] = 0;
				histogram_roi3_G[i][j] = 0;
				histogram_roi3_B[i][j] = 0;

				histogram_roi1_R_post[i][j] = 0;
				histogram_roi1_G_post[i][j] = 0;
				histogram_roi1_B_post[i][j] = 0;

				histogram_roi2_R_post[i][j] = 0;
				histogram_roi2_G_post[i][j] = 0;
				histogram_roi2_B_post[i][j] = 0;

				histogram_roi3_R_post[i][j] = 0;
				histogram_roi3_G_post[i][j] = 0;
				histogram_roi3_B_post[i][j] = 0;

			}
		}
	
	// Mid-Step: Translate channels into int levels to be used
	//int channel1_index, channel2_index, channel3_index;
	
	//channel1_index = checkChannel(channel1);
	//channel2_index = checkChannel(channel2);
	//channel3_index = checkChannel(channel3);

	// Secondly, we need to populate these arrays. 
	
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// We collect pixels of corresponding R, G, B channel

				int current_pixel_value_R = src.getPixel(i,j,0);
				int current_pixel_value_G = src.getPixel(i,j,1);
				int current_pixel_value_B = src.getPixel(i,j,2);

				for (int row=0; row<256; row++)
				{	

					if (current_pixel_value_R == histogram_roi1_R[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_R[row][1]++;
					}

					if (current_pixel_value_G == histogram_roi1_G[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_G[row][1]++;
					}

					if (current_pixel_value_B == histogram_roi1_B[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_B[row][1]++;
					}
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// We collect pixels of corresponding R, G, B channel

				int current_pixel_value_R = src.getPixel(i,j,0);
				int current_pixel_value_G = src.getPixel(i,j,1);
				int current_pixel_value_B = src.getPixel(i,j,2);

				for (int row=0; row<256; row++)
				{	

					if (current_pixel_value_R == histogram_roi2_R[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_R[row][1]++;
					}

					if (current_pixel_value_G == histogram_roi2_G[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_G[row][1]++;
					}

					if (current_pixel_value_B == histogram_roi2_B[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_B[row][1]++;
					}
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// We collect pixels of corresponding R, G, B channel

				int current_pixel_value_R = src.getPixel(i,j,0);
				int current_pixel_value_G = src.getPixel(i,j,1);
				int current_pixel_value_B = src.getPixel(i,j,2);

				for (int row=0; row<256; row++)
				{	

					if (current_pixel_value_R == histogram_roi3_R[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_R[row][1]++;
					}

					if (current_pixel_value_G == histogram_roi3_G[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_G[row][1]++;
					}

					if (current_pixel_value_B == histogram_roi3_B[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_B[row][1]++;
					}
				}

			} else {
				continue;
			}
		}

	// Step 3: Search in the computed histogram for the C (min intensity) and D (max intensity) values.
	
	// Call helper function to initialize C, D values from histogram. We will use these later to pass to MinMax functions to compute C-min and D-max in histogram
	initializeMinMax(histogram_roi1_R, ROI1_R_C1, ROI1_R_D1);
	initializeMinMax(histogram_roi1_G, ROI1_G_C1, ROI1_G_D1);
	initializeMinMax(histogram_roi1_B, ROI1_B_C1, ROI1_B_D1);

	initializeMinMax(histogram_roi2_R, ROI2_R_C2, ROI2_R_D2);
	initializeMinMax(histogram_roi2_G, ROI2_G_C2, ROI2_G_D2);
	initializeMinMax(histogram_roi2_B, ROI2_B_C2, ROI2_B_D2);

	initializeMinMax(histogram_roi3_R, ROI3_R_C3, ROI3_R_D3);
	initializeMinMax(histogram_roi3_G, ROI3_G_C3, ROI3_G_D3);
	initializeMinMax(histogram_roi3_B, ROI3_B_C3, ROI3_B_D3);
    
	// Call helper function to find C-min and D-max from histogram
	findMinMax(histogram_roi1_R, ROI1_R_C1, ROI1_R_D1);
	findMinMax(histogram_roi1_G, ROI1_G_C1, ROI1_G_D1);
	findMinMax(histogram_roi1_B, ROI1_B_C1, ROI1_B_D1);

	findMinMax(histogram_roi2_R, ROI2_R_C2, ROI2_R_D2);
	findMinMax(histogram_roi2_G, ROI2_G_C2, ROI2_G_D2);
	findMinMax(histogram_roi2_B, ROI2_B_C2, ROI2_B_D2);

	findMinMax(histogram_roi3_R, ROI3_R_C3, ROI3_R_D3);
	findMinMax(histogram_roi3_G, ROI3_G_C3, ROI3_G_D3);
	findMinMax(histogram_roi3_B, ROI3_B_C3, ROI3_B_D3);

	// Step 4: Now that we have the histogram and the C, D values for each ROI we can start stretching the histogram by modifying the pixel values using the formula new_p = (P-C) * ((B-A)/(D-C)) + A for each channel.
	// Variable to keep track of the channel index in a ROI which identifies the channel on which stretching will be performed.
	int channel_index; 
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// !!! We apply stretching on pixels of each channel!!!

				// 5th and 95th percentile condition for RED CHANNEL
				if (src.getPixel(i,j,0) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,0, checkValue(A1));
				} else if (src.getPixel(i,j,0) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,0, checkValue(B1));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j,0) - ROI1_R_C1;
					new_value *= (B1 - A1); 
					new_value /= (ROI1_R_D1 - ROI1_R_C1);
					new_value += A1;
					tgt.setPixel(i,j,0, checkValue(new_value));
				}

				// 5th and 95th percentile condition for GREEN CHANNEL
				if (src.getPixel(i,j,1) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,1, checkValue(A1));
				} else if (src.getPixel(i,j,1) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,1, checkValue(B1));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j,1) - ROI1_G_C1;
					new_value *= (B1 - A1); 
					new_value /= (ROI1_G_D1 - ROI1_G_C1);
					new_value += A1;
					tgt.setPixel(i,j,1, checkValue(new_value));
				}

				// 5th and 95th percentile condition for BLUE CHANNEL
				if (src.getPixel(i,j,2) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,2, checkValue(A1));
				} else if (src.getPixel(i,j,2) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,2, checkValue(B1));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j,2) - ROI1_B_C1;
					new_value *= (B1 - A1); 
					new_value /= (ROI1_B_D1 - ROI1_B_C1);
					new_value += A1;
					tgt.setPixel(i,j,2, checkValue(new_value));
				}

				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value_R = tgt.getPixel(i,j,0);
				int current_pixel_value_G = tgt.getPixel(i,j,1);
				int current_pixel_value_B = tgt.getPixel(i,j,2);

				for (int row=0; row<256; row++)
				{	

					if (current_pixel_value_R == histogram_roi1_R_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_R_post[row][1]++;
					}

					if (current_pixel_value_G == histogram_roi1_G_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_G_post[row][1]++;
					}

					if (current_pixel_value_B == histogram_roi1_B_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_B_post[row][1]++;
					}
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// !!! We apply stretching on pixels of each channel!!!

				// 5th and 95th percentile condition for RED CHANNEL
				if (src.getPixel(i,j,0) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,0, checkValue(A2));
				} else if (src.getPixel(i,j,0) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,0, checkValue(B2));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j,0) - ROI2_R_C2;
					new_value *= (B2 - A2); 
					new_value /= (ROI2_R_D2 - ROI2_R_C2);
					new_value += A2;
					tgt.setPixel(i,j,0, checkValue(new_value));
				}

				// 5th and 95th percentile condition for GREEN CHANNEL
				if (src.getPixel(i,j,1) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,1, checkValue(A2));
				} else if (src.getPixel(i,j,1) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,1, checkValue(B2));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j,1) - ROI2_G_C2;
					new_value *= (B2 - A2); 
					new_value /= (ROI2_G_D2 - ROI2_G_C2);
					new_value += A2;
					tgt.setPixel(i,j,1, checkValue(new_value));
				}

				// 5th and 95th percentile condition for BLUE CHANNEL
				if (src.getPixel(i,j,2) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,2, checkValue(A2));
				} else if (src.getPixel(i,j,2) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,2, checkValue(B2));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j,2) - ROI2_B_C2;
					new_value *= (B2 - A2); 
					new_value /= (ROI2_B_D2 - ROI2_B_C2);
					new_value += A2;
					tgt.setPixel(i,j,2, checkValue(new_value));
				}

				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value_R = tgt.getPixel(i,j,0);
				int current_pixel_value_G = tgt.getPixel(i,j,1);
				int current_pixel_value_B = tgt.getPixel(i,j,2);

				for (int row=0; row<256; row++)
				{	

					if (current_pixel_value_R == histogram_roi2_R_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_R_post[row][1]++;
					}

					if (current_pixel_value_G == histogram_roi2_G_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_G_post[row][1]++;
					}

					if (current_pixel_value_B == histogram_roi2_B_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_B_post[row][1]++;
					}
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// !!! We apply stretching on pixels of each channel!!!

				// 5th and 95th percentile condition for RED CHANNEL
				if (src.getPixel(i,j,0) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,0, checkValue(A3));
				} else if (src.getPixel(i,j,0) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,0, checkValue(B3));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j,0) - ROI3_R_C3;
					new_value *= (B3 - A3); 
					new_value /= (ROI3_R_D3 - ROI3_R_C3);
					new_value += A3;
					tgt.setPixel(i,j,0, checkValue(new_value));
				}

				// 5th and 95th percentile condition for GREEN CHANNEL
				if (src.getPixel(i,j,1) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,1, checkValue(A3));
				} else if (src.getPixel(i,j,1) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,1, checkValue(B3));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j,1) - ROI3_G_C3;
					new_value *= (B3 - A3); 
					new_value /= (ROI3_G_D3 - ROI3_G_C3);
					new_value += A3;
					tgt.setPixel(i,j,1, checkValue(new_value));
				}

				// 5th and 95th percentile condition for BLUE CHANNEL
				if (src.getPixel(i,j,2) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					tgt.setPixel(i,j,2, checkValue(A3));
				} else if (src.getPixel(i,j,2) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					tgt.setPixel(i,j,2, checkValue(B3));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = src.getPixel(i,j,2) - ROI3_B_C3;
					new_value *= (B3 - A3); 
					new_value /= (ROI3_B_D3 - ROI3_B_C3);
					new_value += A3;
					tgt.setPixel(i,j,2, checkValue(new_value));
				}

				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value_R = tgt.getPixel(i,j,0);
				int current_pixel_value_G = tgt.getPixel(i,j,1);
				int current_pixel_value_B = tgt.getPixel(i,j,2);

				for (int row=0; row<256; row++)
				{	

					if (current_pixel_value_R == histogram_roi3_R_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_R_post[row][1]++;
					}

					if (current_pixel_value_G == histogram_roi3_G_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_G_post[row][1]++;
					}

					if (current_pixel_value_B == histogram_roi3_B_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_B_post[row][1]++;
					}
				}

			} else {
				tgt.setPixel(i,j,RED,checkValue(src.getPixel(i,j,RED)));
				tgt.setPixel(i,j,GREEN,checkValue(src.getPixel(i,j,GREEN)));
				tgt.setPixel(i,j,BLUE,checkValue(src.getPixel(i,j,BLUE)));
			}
		}

     

	// Step 5. Compute and Plot the New Histograms. 

    // Call Histostretch function
}

/*-----------------------------------------------------------------------**/
void utility::istretch(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, char channel1, int A1, int B1, int x2, int y2, int Sx2, int Sy2, char channel2, int A2, int B2, int x3, int y3, int Sx3, int Sy3, char channel3, int A3, int B3)
{
	// I will create another image of same size as src
	image conversion_image;
	
	/* FOR DEBUGGING PURPOSES 
	image test_img;
	image con_img;
	image test_img_post;
	test_img_post.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	con_img.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	test_img.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			test_img.setPixel(i,j,0,src.getPixel(i,j,0));
			test_img.setPixel(i,j,1,src.getPixel(i,j,1));
			test_img.setPixel(i,j,2,src.getPixel(i,j,2));
		}
	
	rgbtohsi(src, con_img);

	hsitorgb(con_img, test_img_post);

	// Test that we are converting RGB to HSI and then back to RGB correctly. This shows that our RGB->HSI and HSI->RGB conversion functions are working correctly.
	
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			cout << "\n R value: "<< src.getPixel(i,j,0);
			cout << "\n G value: "<< src.getPixel(i,j,1);
			cout << "\n B value: "<< src.getPixel(i,j,2);
			cout << "\n H value: "<< con_img.getPixel(i,j,0);
			cout << "\n S value: "<< con_img.getPixel(i,j,1);
			cout << "\n I value: "<< con_img.getPixel(i,j,2);
			cout << "\n R post value: "<< test_img_post.getPixel(i,j,0);
			cout << "\n G post value: "<< test_img_post.getPixel(i,j,1);
			cout << "\n B post value: "<< test_img_post.getPixel(i,j,2);


			cout << endl;
		}
    */

	// Conversion Image will be used to contain the H,S,I values that we get from conversion from RGB to HSI
	conversion_image.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    
	// We need to populate Conversion Image with the H,S,I values so we call the RGB->HSI conversion helper function
	rgbtohsi(src, conversion_image);
	
	int histogram_roi1[256][2];
	int histogram_roi2[256][2];
	int histogram_roi3[256][2];
	
	int histogram_roi1_post[256][2];
	int histogram_roi2_post[256][2];
	int histogram_roi3_post[256][2];

	int ROI1_C1, ROI1_D1;
	int ROI2_C2, ROI2_D2;
	int ROI3_C3, ROI3_D3;

	// Firstly, we need to initialize these arrays

	for (int i =0; i<256; i++)
		for (int j=0; j<2; j++)
		{
			if (j == 0) {
				// This is the intensity level (range 0 - 255)
				histogram_roi1[i][j] = i;
				histogram_roi2[i][j] = i;
				histogram_roi3[i][j] = i;
				histogram_roi1_post[i][j] = i;
				histogram_roi2_post[i][j] = i;
				histogram_roi3_post[i][j] = i;
			} else if (j == 1) {
				// Corresponding frequency per intensity level. Set to 0 initially
				histogram_roi1[i][j] = 0;
				histogram_roi2[i][j] = 0;
				histogram_roi3[i][j] = 0;
				histogram_roi1_post[i][j] = 0;
				histogram_roi2_post[i][j] = 0;
				histogram_roi3_post[i][j] = 0;

			}
		}
	
	// Mid-Step: Translate channels into int levels to be used
	//int channel1_index, channel2_index, channel3_index;
	//cout << "\nChannel 1" << channel1;
	//channel1_index = checkChannel(channel1);
	//channel2_index = checkChannel(channel2);
	//channel3_index = checkChannel(channel3);

	//cout << "\nChannel index " << channel1_index;
	

	// Secondly, we need to populate these arrays. 
		for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// We collect pixels of the B channel which correspongs to I channel of HSI conversion image

				int current_pixel_value = conversion_image.getPixel(i,j,2);
				for (int row=0; row<256; row++)
				{	
					
					if (current_pixel_value == histogram_roi1[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1[row][1]++;
					} 
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// We collect pixels of the B channel which correspongs to I channel of HSI conversion image

				int current_pixel_value = conversion_image.getPixel(i,j,2);
				
				for (int row=0; row<256; row++)
				{
					if (current_pixel_value == histogram_roi2[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2[row][1]++;
					} 
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// We collect pixels of the B channel which correspongs to I channel of HSI conversion image

				int current_pixel_value = conversion_image.getPixel(i,j,2);
				
				for (int row=0; row<256; row++)
				{
					if (current_pixel_value == histogram_roi3[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3[row][1]++;
					} 
				}
			} else {
				continue;
			}
		}

	// Step 3: Search in the computed histogram for the C (min intensity) and D (max intensity) values.
	
	// Call helper function to initialize C, D values from histogram. We will use these later to pass to MinMax functions to compute C-min and D-max in histogram
	initializeMinMax(histogram_roi1, ROI1_C1, ROI1_D1);
	initializeMinMax(histogram_roi2, ROI2_C2, ROI2_D2);
	initializeMinMax(histogram_roi3, ROI3_C3, ROI3_D3);
    
	// Call helper function to find C-min and D-max from histogram
	findMinMax(histogram_roi1, ROI1_C1, ROI1_D1);
	findMinMax(histogram_roi2, ROI2_C2, ROI2_D2);
	findMinMax(histogram_roi3, ROI3_C3, ROI3_D3);


	// Step 4: Now that we have the histogram and the C, D values for each ROI we can start stretching the histogram by modifying the pixel values using the formula new_p = (P-C) * ((B-A)/(D-C)) + A.
	// Variable to keep track of the channel index in a ROI which identifies the channel on which stretching will be performed.
	//int channel_index; 
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// We apply stretching only on pixels of I channel of conversion_image
				// 5th and 95th percentile condition
				if (conversion_image.getPixel(i,j,2) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					conversion_image.setPixel(i,j,2, checkValue(A1));
				} else if (conversion_image.getPixel(i,j,2) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					conversion_image.setPixel(i,j,2, checkValue(B1));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,2) - ROI1_C1;
					new_value *= (B1 - A1); 
					new_value /= (ROI1_D1 - ROI1_C1);
					new_value += A1;
					conversion_image.setPixel(i,j,2, checkValue(new_value));
				}
				
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = conversion_image.getPixel(i,j,2);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<256; row++)
				{	
					
					if (current_pixel_value == histogram_roi1_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_post[row][1]++;
					} 
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// We apply stretching only on pixels of I channel of conversion_image
				// 5th and 95th percentile condition
				if (conversion_image.getPixel(i,j,2) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					conversion_image.setPixel(i,j,2, checkValue(A2));
				} else if (conversion_image.getPixel(i,j,2) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					conversion_image.setPixel(i,j,2, checkValue(B2));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,2) - ROI2_C2;
					new_value *= (B2 - A2); 
					new_value /= (ROI2_D2 - ROI2_C2);
					new_value += A2;
					conversion_image.setPixel(i,j,2, checkValue(new_value));
				}
				
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = conversion_image.getPixel(i,j,2);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<256; row++)
				{	
					
					if (current_pixel_value == histogram_roi2_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_post[row][1]++;
					} 
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// We apply stretching only on pixels of I channel of conversion_image
				// 5th and 95th percentile condition
				if (conversion_image.getPixel(i,j,2) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					conversion_image.setPixel(i,j,2, checkValue(A3));
				} else if (conversion_image.getPixel(i,j,2) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					conversion_image.setPixel(i,j,2, checkValue(B3));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,2) - ROI3_C3;
					new_value *= (B3 - A3); 
					new_value /= (ROI3_D3 - ROI3_C3);
					new_value += A3;
					conversion_image.setPixel(i,j,2, checkValue(new_value));
				}
				
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = conversion_image.getPixel(i,j,2);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<256; row++)
				{	
					
					if (current_pixel_value == histogram_roi3_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_post[row][1]++;
					} 
				}

			} else {
				continue;
			}
		}

    // Step 5: Now we need to convert HSI back to RGB so we can output it
	hsitorgb(conversion_image, tgt);

	// Step 6. Compute and Plot the New Histograms. 

    // Call Histostretch function
}

/*-----------------------------------------------------------------------**/
void utility::hstretch(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, char channel1, int A1, int B1, int x2, int y2, int Sx2, int Sy2, char channel2, int A2, int B2, int x3, int y3, int Sx3, int Sy3, char channel3, int A3, int B3)
{
	// I will create another image of same size as src
	image conversion_image;
	
	/* FOR DEBUGGING PURPOSES 
	image test_img;
	image con_img;
	image test_img_post;
	test_img_post.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	con_img.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	test_img.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			test_img.setPixel(i,j,0,src.getPixel(i,j,0));
			test_img.setPixel(i,j,1,src.getPixel(i,j,1));
			test_img.setPixel(i,j,2,src.getPixel(i,j,2));
		}
	
	rgbtohsi(src, con_img);

	hsitorgb(con_img, test_img_post);

	// Test that we are converting RGB to HSI and then back to RGB correctly. This shows that our RGB->HSI and HSI->RGB conversion functions are working correctly.
	
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			cout << "\n R value: "<< src.getPixel(i,j,0);
			cout << "\n G value: "<< src.getPixel(i,j,1);
			cout << "\n B value: "<< src.getPixel(i,j,2);
			cout << "\n H value: "<< con_img.getPixel(i,j,0);
			cout << "\n S value: "<< con_img.getPixel(i,j,1);
			cout << "\n I value: "<< con_img.getPixel(i,j,2);
			cout << "\n R post value: "<< test_img_post.getPixel(i,j,0);
			cout << "\n G post value: "<< test_img_post.getPixel(i,j,1);
			cout << "\n B post value: "<< test_img_post.getPixel(i,j,2);


			cout << endl;
		}
    */

	// Conversion Image will be used to contain the H,S,I values that we get from conversion from RGB to HSI
	conversion_image.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    
	// We need to populate Conversion Image with the H,S,I values so we call the RGB->HSI conversion helper function
	rgbtohsi(src, conversion_image);
	
	// Since we are stretching H now and the range is 0 - 360 we need to increase histogram size
	int histogram_roi1[361][2];
	int histogram_roi2[361][2];
	int histogram_roi3[361][2];
	
	int histogram_roi1_post[361][2];
	int histogram_roi2_post[361][2];
	int histogram_roi3_post[361][2];

	int ROI1_C1, ROI1_D1;
	int ROI2_C2, ROI2_D2;
	int ROI3_C3, ROI3_D3;

	// Firstly, we need to initialize these arrays

	for (int i =0; i<361; i++)
		for (int j=0; j<2; j++)
		{
			if (j == 0) {
				// This is the intensity level (range 0 - 360)
				histogram_roi1[i][j] = i;
				histogram_roi2[i][j] = i;
				histogram_roi3[i][j] = i;
				histogram_roi1_post[i][j] = i;
				histogram_roi2_post[i][j] = i;
				histogram_roi3_post[i][j] = i;
			} else if (j == 1) {
				// Corresponding frequency per intensity level. Set to 0 initially
				histogram_roi1[i][j] = 0;
				histogram_roi2[i][j] = 0;
				histogram_roi3[i][j] = 0;
				histogram_roi1_post[i][j] = 0;
				histogram_roi2_post[i][j] = 0;
				histogram_roi3_post[i][j] = 0;

			}
		}
	
	// Mid-Step: Translate channels into int levels to be used
	//int channel1_index, channel2_index, channel3_index;
	//cout << "\nChannel 1" << channel1;
	//channel1_index = checkChannel(channel1);
	//channel2_index = checkChannel(channel2);
	//channel3_index = checkChannel(channel3);

	//cout << "\nChannel index " << channel1_index;
	

	// Secondly, we need to populate these arrays. 
		for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// We collect pixels of the R channel which correspongs to H channel of HSI conversion image

				int current_pixel_value = conversion_image.getPixel(i,j,0);
				for (int row=0; row<361; row++)
				{	
					
					if (current_pixel_value == histogram_roi1[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1[row][1]++;
					} 
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// We collect pixels of the R channel which correspongs to H channel of HSI conversion image

				int current_pixel_value = conversion_image.getPixel(i,j,0);
				
				for (int row=0; row<361; row++)
				{
					if (current_pixel_value == histogram_roi2[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2[row][1]++;
					} 
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// We collect pixels of the R channel which correspongs to H channel of HSI conversion image

				int current_pixel_value = conversion_image.getPixel(i,j,0);
				
				for (int row=0; row<361; row++)
				{
					if (current_pixel_value == histogram_roi3[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3[row][1]++;
					} 
				}
			} else {
				continue;
			}
		}

	// Step 3: Search in the computed histogram for the C (min intensity) and D (max intensity) values.
	
	// Call helper function to initialize C, D values from histogram. We will use these later to pass to MinMax functions to compute C-min and D-max in histogram
	initializeMinMax(histogram_roi1, ROI1_C1, ROI1_D1);
	initializeMinMax(histogram_roi2, ROI2_C2, ROI2_D2);
	initializeMinMax(histogram_roi3, ROI3_C3, ROI3_D3);
    
	// Call helper function to find C-min and D-max from histogram
	findMinMax(histogram_roi1, ROI1_C1, ROI1_D1);
	findMinMax(histogram_roi2, ROI2_C2, ROI2_D2);
	findMinMax(histogram_roi3, ROI3_C3, ROI3_D3);


	// Step 4: Now that we have the histogram and the C, D values for each ROI we can start stretching the histogram by modifying the pixel values using the formula new_p = (P-C) * ((B-A)/(D-C)) + A.
	// Variable to keep track of the channel index in a ROI which identifies the channel on which stretching will be performed.
	int channel_index; 
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// We apply stretching only on pixels of H channel of conversion_image
				// 5th and 95th percentile condition
				if (conversion_image.getPixel(i,j,0) < 0.05 * 360) {
					// If pixel value is in interval [0, 18] we update it to A
					conversion_image.setPixel(i,j,0, A1);
				} else if (conversion_image.getPixel(i,j,0) > 0.95 * 360) {
					// If pixel value is in interval [342, 255] we update it to B
					conversion_image.setPixel(i,j,0, B1);
				} else {
					// If the pixel value is in interval [18, 342] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,0) - ROI1_C1;
					new_value *= (B1 - A1); 
					new_value /= (ROI1_D1 - ROI1_C1);
					new_value += A1;
					conversion_image.setPixel(i,j,0, new_value);
				}
				
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = conversion_image.getPixel(i,j,0);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<361; row++)
				{	
					
					if (current_pixel_value == histogram_roi1_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_post[row][1]++;
					} 
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// We apply stretching only on pixels of H channel of conversion_image
				// 5th and 95th percentile condition
				if (conversion_image.getPixel(i,j,0) < 0.05 * 360) {
					// If pixel value is in interval [0, 18] we update it to A
					conversion_image.setPixel(i,j,0, A2);
				} else if (conversion_image.getPixel(i,j,0) > 0.95 * 360) {
					// If pixel value is in interval [342, 360] we update it to B
					conversion_image.setPixel(i,j,0, B2);
				} else {
					// If the pixel value is in interval [18, 342] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,0) - ROI2_C2;
					new_value *= (B2 - A2); 
					new_value /= (ROI2_D2 - ROI2_C2);
					new_value += A2;
					conversion_image.setPixel(i,j,0, new_value);
				}
				
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = conversion_image.getPixel(i,j,0);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<361; row++)
				{	
					
					if (current_pixel_value == histogram_roi2_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_post[row][1]++;
					} 
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// We apply stretching only on pixels of H channel of conversion_image
				// 5th and 95th percentile condition
				if (conversion_image.getPixel(i,j,0) < 0.05 * 360) {
					// If pixel value is in interval [0, 18] we update it to A
					conversion_image.setPixel(i,j,0, A3);
				} else if (conversion_image.getPixel(i,j,0) > 0.95 * 360) {
					// If pixel value is in interval [342, 360] we update it to B
					conversion_image.setPixel(i,j,0, B3);
				} else {
					// If the pixel value is in interval [18, 342] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,0) - ROI3_C3;
					new_value *= (B3 - A3); 
					new_value /= (ROI3_D3 - ROI3_C3);
					new_value += A3;
					conversion_image.setPixel(i,j,0, new_value);
				}
				
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = conversion_image.getPixel(i,j,0);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<361; row++)
				{	
					
					if (current_pixel_value == histogram_roi3_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_post[row][1]++;
					} 
				}

			} else {
				continue;
			}
		}

    // Step 5: Now we need to convert HSI back to RGB so we can output it
	hsitorgb(conversion_image, tgt);

	// Step 6. Compute and Plot the New Histograms. 

    // Call Histostretch function
}

/*-----------------------------------------------------------------------**/
void utility::sstretch(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, char channel1, int A1, int B1, int x2, int y2, int Sx2, int Sy2, char channel2, int A2, int B2, int x3, int y3, int Sx3, int Sy3, char channel3, int A3, int B3)
{
	// I will create another image of same size as src
	image conversion_image;
	
	/* FOR DEBUGGING PURPOSES 
	image test_img;
	image con_img;
	image test_img_post;
	test_img_post.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	con_img.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	test_img.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			test_img.setPixel(i,j,0,src.getPixel(i,j,0));
			test_img.setPixel(i,j,1,src.getPixel(i,j,1));
			test_img.setPixel(i,j,2,src.getPixel(i,j,2));
		}
	
	rgbtohsi(src, con_img);

	hsitorgb(con_img, test_img_post);

	// Test that we are converting RGB to HSI and then back to RGB correctly. This shows that our RGB->HSI and HSI->RGB conversion functions are working correctly.
	
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			cout << "\n R value: "<< src.getPixel(i,j,0);
			cout << "\n G value: "<< src.getPixel(i,j,1);
			cout << "\n B value: "<< src.getPixel(i,j,2);
			cout << "\n H value: "<< con_img.getPixel(i,j,0);
			cout << "\n S value: "<< con_img.getPixel(i,j,1);
			cout << "\n I value: "<< con_img.getPixel(i,j,2);
			cout << "\n R post value: "<< test_img_post.getPixel(i,j,0);
			cout << "\n G post value: "<< test_img_post.getPixel(i,j,1);
			cout << "\n B post value: "<< test_img_post.getPixel(i,j,2);


			cout << endl;
		}
    */

	// Conversion Image will be used to contain the H,S,I values that we get from conversion from RGB to HSI
	conversion_image.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    
	// We need to populate Conversion Image with the H,S,I values so we call the RGB->HSI conversion helper function
	rgbtohsi(src, conversion_image);
	
	// Since we are stretching S now and the range is 0 - 100 we need to increase histogram size
	int histogram_roi1[101][2];
	int histogram_roi2[101][2];
	int histogram_roi3[101][2];
	
	int histogram_roi1_post[101][2];
	int histogram_roi2_post[101][2];
	int histogram_roi3_post[101][2];

	int ROI1_C1, ROI1_D1;
	int ROI2_C2, ROI2_D2;
	int ROI3_C3, ROI3_D3;

	// Firstly, we need to initialize these arrays

	for (int i =0; i<101; i++)
		for (int j=0; j<2; j++)
		{
			if (j == 0) {
				// This is the intensity level (range 0 - 360)
				histogram_roi1[i][j] = i;
				histogram_roi2[i][j] = i;
				histogram_roi3[i][j] = i;
				histogram_roi1_post[i][j] = i;
				histogram_roi2_post[i][j] = i;
				histogram_roi3_post[i][j] = i;
			} else if (j == 1) {
				// Corresponding frequency per intensity level. Set to 0 initially
				histogram_roi1[i][j] = 0;
				histogram_roi2[i][j] = 0;
				histogram_roi3[i][j] = 0;
				histogram_roi1_post[i][j] = 0;
				histogram_roi2_post[i][j] = 0;
				histogram_roi3_post[i][j] = 0;

			}
		}
	
	// Mid-Step: Translate channels into int levels to be used
	//int channel1_index, channel2_index, channel3_index;
	//cout << "\nChannel 1" << channel1;
	//channel1_index = checkChannel(channel1);
	//channel2_index = checkChannel(channel2);
	//channel3_index = checkChannel(channel3);

	//cout << "\nChannel index " << channel1_index;
	

	// Secondly, we need to populate these arrays. 
		for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// We collect pixels of the G channel which correspongs to S channel of HSI conversion image

				int current_pixel_value = conversion_image.getPixel(i,j,1);
				for (int row=0; row<101; row++)
				{	
					
					if (current_pixel_value == histogram_roi1[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1[row][1]++;
					} 
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// We collect pixels of the G channel which correspongs to S channel of HSI conversion image

				int current_pixel_value = conversion_image.getPixel(i,j,1);
				
				for (int row=0; row<101; row++)
				{
					if (current_pixel_value == histogram_roi2[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2[row][1]++;
					} 
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// We collect pixels of the G channel which correspongs to S channel of HSI conversion image

				int current_pixel_value = conversion_image.getPixel(i,j,1);
				
				for (int row=0; row<101; row++)
				{
					if (current_pixel_value == histogram_roi3[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3[row][1]++;
					} 
				}
			} else {
				continue;
			}
		}

	// Step 3: Search in the computed histogram for the C (min intensity) and D (max intensity) values.
	
	// Call helper function to initialize C, D values from histogram. We will use these later to pass to MinMax functions to compute C-min and D-max in histogram
	initializeMinMax(histogram_roi1, ROI1_C1, ROI1_D1);
	initializeMinMax(histogram_roi2, ROI2_C2, ROI2_D2);
	initializeMinMax(histogram_roi3, ROI3_C3, ROI3_D3);
    
	// Call helper function to find C-min and D-max from histogram
	findMinMax(histogram_roi1, ROI1_C1, ROI1_D1);
	findMinMax(histogram_roi2, ROI2_C2, ROI2_D2);
	findMinMax(histogram_roi3, ROI3_C3, ROI3_D3);


	// Step 4: Now that we have the histogram and the C, D values for each ROI we can start stretching the histogram by modifying the pixel values using the formula new_p = (P-C) * ((B-A)/(D-C)) + A.
	// Variable to keep track of the channel index in a ROI which identifies the channel on which stretching will be performed.
	//int channel_index; 
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// We apply stretching only on pixels of S channel of conversion_image
				// 5th and 95th percentile condition
				if (conversion_image.getPixel(i,j,1) < 0.05 * 100) {
					// If pixel value is in interval [0, 5] we update it to A
					conversion_image.setPixel(i,j,1, checkValue(A1));
				} else if (conversion_image.getPixel(i,j,1) > 0.95 * 100) {
					// If pixel value is in interval [95, 100] we update it to B
					conversion_image.setPixel(i,j,1, checkValue(B1));
				} else {
					// If the pixel value is in interval [5, 95] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,1) - ROI1_C1;
					new_value *= (B1 - A1); 
					new_value /= (ROI1_D1 - ROI1_C1);
					new_value += A1;
					conversion_image.setPixel(i,j,1, checkValue(new_value));
				}
				
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = conversion_image.getPixel(i,j,1);
				//cout << "The ROI 1 Current Pixel Value is: \n" << current_pixel_value;
				for (int row=0; row<101; row++)
				{	
					
					if (current_pixel_value == histogram_roi1_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_post[row][1]++;
					} 
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// We apply stretching only on pixels of S channel of conversion_image
				// 5th and 95th percentile condition
				if (conversion_image.getPixel(i,j,1) < 0.05 * 100) {
					// If pixel value is in interval [0, 5] we update it to A
					conversion_image.setPixel(i,j,1, checkValue(A2));
				} else if (conversion_image.getPixel(i,j,1) > 0.95 * 100) {
					// If pixel value is in interval [95, 100] we update it to B
					conversion_image.setPixel(i,j,1, checkValue(B2));
				} else {
					// If the pixel value is in interval [5, 95] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,1) - ROI2_C2;
					new_value *= (B2 - A2); 
					new_value /= (ROI2_D2 - ROI2_C2);
					new_value += A2;
					conversion_image.setPixel(i,j,1, checkValue(new_value));
				}
				
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = conversion_image.getPixel(i,j,1);
				
				for (int row=0; row<101; row++)
				{	
					
					if (current_pixel_value == histogram_roi2_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_post[row][1]++;
					} 
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// We apply stretching only on pixels of S channel of conversion_image
				// 5th and 95th percentile condition
				if (conversion_image.getPixel(i,j,1) < 0.05 * 100) {
					// If pixel value is in interval [0, 5] we update it to A
					conversion_image.setPixel(i,j,1, checkValue(A3));
				} else if (conversion_image.getPixel(i,j,1) > 0.95 * 100) {
					// If pixel value is in interval [95, 100] we update it to B
					conversion_image.setPixel(i,j,1, checkValue(B3));
				} else {
					// If the pixel value is in interval [5, 95] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,1) - ROI3_C3;
					new_value *= (B3 - A3); 
					new_value /= (ROI3_D3 - ROI3_C3);
					new_value += A3;
					conversion_image.setPixel(i,j,1, checkValue(new_value));
				}
				
				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value = conversion_image.getPixel(i,j,1);
				
				for (int row=0; row<101; row++)
				{	
					
					if (current_pixel_value == histogram_roi3_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_post[row][1]++;
					} 
				}

			} else {
				continue;
			}
		}

    // Step 5: Now we need to convert HSI back to RGB so we can output it
	hsitorgb(conversion_image, tgt);

	// Step 6. Compute and Plot the New Histograms. 

    // Call Histostretch
}

/*-----------------------------------------------------------------------**/
void utility::fullhsistretch(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int A1_H, int B1_H, int A1_S, int B1_S, int A1_I, int B1_I, int x2, int y2, int Sx2, int Sy2, int A2_H, int B2_H, int A2_S, int B2_S, int A2_I, int B2_I, int x3, int y3, int Sx3, int Sy3, int A3_H, int B3_H, int A3_S, int B3_S, int A3_I, int B3_I)
{

	// I will create another image of same size as src
	image conversion_image;
	/*
	cout << "\nA1_H" << A1_H;
	cout << "\nB1_H" << B1_H;
	cout << "\nA1_S" << A1_S;
	cout << "\nB1_S" << B1_S;
	cout << "\nA1_I" << A1_I;
	cout << "\nB1_I" << B1_I;

	cout << "\nA2_H" << A2_H;
	cout << "\nB2_H" << B2_H;
	cout << "\nA2_S" << A2_S;
	cout << "\nB2_S" << B2_S;
	cout << "\nA2_I" << A2_I;
	cout << "\nB2_I" << B2_I;

	cout << "\nA3_H" << A3_H;
	cout << "\nB3_H" << B3_H;
	cout << "\nA3_S" << A3_S;
	cout << "\nB3_S" << B3_S;
	cout << "\nA3_I" << A3_I;
	cout << "\nB3_I" << B3_I;
    */
	/* FOR DEBUGGING PURPOSES 
	image test_img;
	image con_img;
	image test_img_post;
	test_img_post.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	con_img.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	test_img.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			test_img.setPixel(i,j,0,src.getPixel(i,j,0));
			test_img.setPixel(i,j,1,src.getPixel(i,j,1));
			test_img.setPixel(i,j,2,src.getPixel(i,j,2));
		}
	
	rgbtohsi(src, con_img);

	hsitorgb(con_img, test_img_post);

	// Test that we are converting RGB to HSI and then back to RGB correctly. This shows that our RGB->HSI and HSI->RGB conversion functions are working correctly.
	
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			cout << "\n R value: "<< src.getPixel(i,j,0);
			cout << "\n G value: "<< src.getPixel(i,j,1);
			cout << "\n B value: "<< src.getPixel(i,j,2);
			cout << "\n H value: "<< con_img.getPixel(i,j,0);
			cout << "\n S value: "<< con_img.getPixel(i,j,1);
			cout << "\n I value: "<< con_img.getPixel(i,j,2);
			cout << "\n R post value: "<< test_img_post.getPixel(i,j,0);
			cout << "\n G post value: "<< test_img_post.getPixel(i,j,1);
			cout << "\n B post value: "<< test_img_post.getPixel(i,j,2);


			cout << endl;
		}
    */

	// Conversion Image will be used to contain the H,S,I values that we get from conversion from RGB to HSI
	conversion_image.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
    
	// We need to populate Conversion Image with the H,S,I values so we call the RGB->HSI conversion helper function
	rgbtohsi(src, conversion_image);
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	

	// Declare 2D arrays for each channel (H, S, I) for each ROI. Original Histogram
	int histogram_roi1_H[361][2];
	int histogram_roi1_S[101][2];
	int histogram_roi1_I[256][2];

	int histogram_roi2_H[361][2];
	int histogram_roi2_S[101][2];
	int histogram_roi2_I[256][2];

	int histogram_roi3_H[361][2];
	int histogram_roi3_S[101][2];
	int histogram_roi3_I[256][2];
	
	// Histograms after stretching has been performed. 
	int histogram_roi1_H_post[361][2];
	int histogram_roi1_S_post[101][2];
	int histogram_roi1_I_post[256][2];

	int histogram_roi2_H_post[361][2];
	int histogram_roi2_S_post[101][2];
	int histogram_roi2_I_post[256][2];

	int histogram_roi3_H_post[361][2];
	int histogram_roi3_S_post[101][2];
	int histogram_roi3_I_post[256][2];
    
	// C, D (min, max) values for each channel's histogram
	int ROI1_H_C1, ROI1_H_D1;
	int ROI1_S_C1, ROI1_S_D1;
	int ROI1_I_C1, ROI1_I_D1;

	int ROI2_H_C2, ROI2_H_D2;
	int ROI2_S_C2, ROI2_S_D2;
	int ROI2_I_C2, ROI2_I_D2;

	int ROI3_H_C3, ROI3_H_D3;
	int ROI3_S_C3, ROI3_S_D3;
	int ROI3_I_C3, ROI3_I_D3;

	// Firstly, we need to initialize these arrays
	for (int i =0; i<361; i++)
		for (int j=0; j<2; j++)
		{
			if (j == 0) {
				// This is the intensity level (range 0 - 360)
				histogram_roi1_H[i][j] = i;
				histogram_roi2_H[i][j] = i;
				histogram_roi3_H[i][j] = i;
				histogram_roi1_H_post[i][j] = i;
				histogram_roi2_H_post[i][j] = i;
				histogram_roi3_H_post[i][j] = i;
			} else if (j == 1) {
				// Corresponding frequency per intensity level. Set to 0 initially
				histogram_roi1_H[i][j] = 0;
				histogram_roi2_H[i][j] = 0;
				histogram_roi3_H[i][j] = 0;
				histogram_roi1_H_post[i][j] = 0;
				histogram_roi2_H_post[i][j] = 0;
				histogram_roi3_H_post[i][j] = 0;

			}
		}
	
	for (int i =0; i<101; i++)
		for (int j=0; j<2; j++)
		{
			if (j == 0) {
				// This is the intensity level (range 0 - 360)
				histogram_roi1_S[i][j] = i;
				histogram_roi2_S[i][j] = i;
				histogram_roi3_S[i][j] = i;
				histogram_roi1_S_post[i][j] = i;
				histogram_roi2_S_post[i][j] = i;
				histogram_roi3_S_post[i][j] = i;
			} else if (j == 1) {
				// Corresponding frequency per intensity level. Set to 0 initially
				histogram_roi1_S[i][j] = 0;
				histogram_roi2_S[i][j] = 0;
				histogram_roi3_S[i][j] = 0;
				histogram_roi1_S_post[i][j] = 0;
				histogram_roi2_S_post[i][j] = 0;
				histogram_roi3_S_post[i][j] = 0;

			}
		}

	for (int i =0; i<256; i++)
		for (int j=0; j<2; j++)
		{
			if (j == 0) {
				// This is the intensity level (range 0 - 360)
				histogram_roi1_I[i][j] = i;
				histogram_roi2_I[i][j] = i;
				histogram_roi3_I[i][j] = i;
				histogram_roi1_I_post[i][j] = i;
				histogram_roi2_I_post[i][j] = i;
				histogram_roi3_I_post[i][j] = i;
			} else if (j == 1) {
				// Corresponding frequency per intensity level. Set to 0 initially
				histogram_roi1_I[i][j] = 0;
				histogram_roi2_I[i][j] = 0;
				histogram_roi3_I[i][j] = 0;
				histogram_roi1_I_post[i][j] = 0;
				histogram_roi2_I_post[i][j] = 0;
				histogram_roi3_I_post[i][j] = 0;

			}
		}

	// Secondly, we need to populate these arrays. 
	
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// We collect pixels of corresponding H, S, I channel

				int current_pixel_value_H = conversion_image.getPixel(i,j,0);
				int current_pixel_value_S = conversion_image.getPixel(i,j,1);
				int current_pixel_value_I = conversion_image.getPixel(i,j,2);

				for (int row=0; row<361; row++)
				{	

					if (current_pixel_value_H == histogram_roi1_H[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_H[row][1]++;
					}
				}

				for (int row=0; row<101; row++)
				{	

					if (current_pixel_value_S == histogram_roi1_S[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_S[row][1]++;
					}
				}

				for (int row=0; row<256; row++)
				{	

					if (current_pixel_value_I == histogram_roi1_I[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_I[row][1]++;
					}
				}

				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// We collect pixels of corresponding H, S, I channel

				int current_pixel_value_H = conversion_image.getPixel(i,j,0);
				int current_pixel_value_S = conversion_image.getPixel(i,j,1);
				int current_pixel_value_I = conversion_image.getPixel(i,j,2);

				for (int row=0; row<361; row++)
				{	

					if (current_pixel_value_H == histogram_roi2_H[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_H[row][1]++;
					}
				}

				for (int row=0; row<101; row++)
				{	

					if (current_pixel_value_S == histogram_roi2_S[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_S[row][1]++;
					}
				}

				for (int row=0; row<256; row++)
				{	

					if (current_pixel_value_I == histogram_roi2_I[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_I[row][1]++;
					}
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// We collect pixels of corresponding H, S, I channel

				int current_pixel_value_H = conversion_image.getPixel(i,j,0);
				int current_pixel_value_S = conversion_image.getPixel(i,j,1);
				int current_pixel_value_I = conversion_image.getPixel(i,j,2);

				for (int row=0; row<361; row++)
				{	

					if (current_pixel_value_H == histogram_roi3_H[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_H[row][1]++;
					}
				}

				for (int row=0; row<101; row++)
				{	

					if (current_pixel_value_S == histogram_roi3_S[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_S[row][1]++;
					}
				}

				for (int row=0; row<256; row++)
				{	

					if (current_pixel_value_I == histogram_roi3_I[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_I[row][1]++;
					}
				}

			} else {
				continue;
			}
		}

	// Step 3: Search in the computed histogram for the C (min intensity) and D (max intensity) values.
	
	// Call helper function to initialize C, D values from histogram. We will use these later to pass to MinMax functions to compute C-min and D-max in histogram
	initializeMinMax(histogram_roi1_H, ROI1_H_C1, ROI1_H_D1);
	initializeMinMax(histogram_roi1_S, ROI1_S_C1, ROI1_S_D1);
	initializeMinMax(histogram_roi1_I, ROI1_I_C1, ROI1_I_D1);

	initializeMinMax(histogram_roi2_H, ROI2_H_C2, ROI2_H_D2);
	initializeMinMax(histogram_roi2_S, ROI2_S_C2, ROI2_S_D2);
	initializeMinMax(histogram_roi2_I, ROI2_I_C2, ROI2_I_D2);

	initializeMinMax(histogram_roi3_H, ROI3_H_C3, ROI3_H_D3);
	initializeMinMax(histogram_roi3_S, ROI3_S_C3, ROI3_S_D3);
	initializeMinMax(histogram_roi3_I, ROI3_I_C3, ROI3_I_D3);
    
	// Call helper function to find C-min and D-max from histogram
	findMinMax(histogram_roi1_H, ROI1_H_C1, ROI1_H_D1);
	findMinMax(histogram_roi1_S, ROI1_S_C1, ROI1_S_D1);
	findMinMax(histogram_roi1_I, ROI1_I_C1, ROI1_I_D1);

	findMinMax(histogram_roi2_H, ROI2_H_C2, ROI2_H_D2);
	findMinMax(histogram_roi2_S, ROI2_S_C2, ROI2_S_D2);
	findMinMax(histogram_roi2_I, ROI2_I_C2, ROI2_I_D2);

	findMinMax(histogram_roi3_H, ROI3_H_C3, ROI3_H_D3);
	findMinMax(histogram_roi3_S, ROI3_S_C3, ROI3_S_D3);
	findMinMax(histogram_roi3_I, ROI3_I_C3, ROI3_I_D3);

	// Step 4: Now that we have the histogram and the C, D values for each ROI we can start stretching the histogram by modifying the pixel values using the formula new_p = (P-C) * ((B-A)/(D-C)) + A for each channel.
	// Variable to keep track of the channel index in a ROI which identifies the channel on which stretching will be performed.
	//int channel_index; 
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			// If-else conditions for determining the ROIs and making respective adjustments.
			if ((i >= x1 && i < x1 + Sx1) && (j >= y1 && j < y1 + Sy1)) {
				
				// !!! We apply stretching on pixels of each channel!!!

				// 5th and 95th percentile condition for H
				if (conversion_image.getPixel(i,j,0) < 0.05 * 360) {
					// If pixel value is in interval [0, 18] we update it to A
					conversion_image.setPixel(i,j,0, A1_H);
				} else if (src.getPixel(i,j,0) > 0.95 * 360) {
					// If pixel value is in interval [18, 342] we update it to B
					conversion_image.setPixel(i,j,0, B1_H);
				} else {
					// If the pixel value is in interval [342, 360] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,0) - ROI1_H_C1;
					new_value *= (B1_H - A1_H); 
					new_value /= (ROI1_H_D1 - ROI1_H_C1);
					new_value += A1_H;
					conversion_image.setPixel(i,j,0, new_value);
				}

				// 5th and 95th percentile condition for S
				if (conversion_image.getPixel(i,j,1) < 0.05 * 100) {
					// If pixel value is in interval [0, 5] we update it to A
					conversion_image.setPixel(i,j,1, checkValue(A1_S));
				} else if (src.getPixel(i,j,1) > 0.95 * 100) {
					// If pixel value is in interval [5, 95] we update it to B
					conversion_image.setPixel(i,j,1, checkValue(B1_S));
				} else {
					// If the pixel value is in interval [5, 95] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,1) - ROI1_S_C1;
					new_value *= (B1_S - A1_S); 
					new_value /= (ROI1_S_D1 - ROI1_S_C1);
					new_value += A1_S;
					conversion_image.setPixel(i,j,1, checkValue(new_value));
				}

				// 5th and 95th percentile condition for I
				if (conversion_image.getPixel(i,j,2) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					conversion_image.setPixel(i,j,2, checkValue(A1_I));
				} else if (conversion_image.getPixel(i,j,2) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					conversion_image.setPixel(i,j,2, checkValue(B1_I));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,2) - ROI1_I_C1;
					new_value *= (B1_I - A1_I); 
					new_value /= (ROI1_I_D1 - ROI1_I_C1);
					new_value += A1_I;
					conversion_image.setPixel(i,j,2, checkValue(new_value));
				}

				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value_H = conversion_image.getPixel(i,j,0);
				int current_pixel_value_S = conversion_image.getPixel(i,j,1);
				int current_pixel_value_I = conversion_image.getPixel(i,j,2);

				for (int row=0; row<361; row++)
				{	

					if (current_pixel_value_H == histogram_roi1_H_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_H_post[row][1]++;
					}
				}

				for (int row=0; row<101; row++)
				{	

					if (current_pixel_value_S == histogram_roi1_S_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_S_post[row][1]++;
					}
				}

				for (int row=0; row<256; row++)
				{	

					if (current_pixel_value_I == histogram_roi1_I[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi1_I_post[row][1]++;
					}
				}
				
			} else if (x2 != DEFAULT && ((i >= x2 && i < x2 + Sx2) && (j >= y2 && j < y2 + Sy2))) {
				
				// !!! We apply stretching on pixels of each channel!!!

				// 5th and 95th percentile condition for H
				if (conversion_image.getPixel(i,j,0) < 0.05 * 360) {
					// If pixel value is in interval [0, 18] we update it to A
					conversion_image.setPixel(i,j,0, A2_H);
				} else if (src.getPixel(i,j,0) > 0.95 * 360) {
					// If pixel value is in interval [18, 342] we update it to B
					conversion_image.setPixel(i,j,0, B2_H);
				} else {
					// If the pixel value is in interval [342, 360] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,0) - ROI2_H_C2;
					new_value *= (B2_H - A2_H); 
					new_value /= (ROI2_H_D2 - ROI2_H_C2);
					new_value += A2_H;
					conversion_image.setPixel(i,j,0, new_value);
				}

				// 5th and 95th percentile condition for S
				if (conversion_image.getPixel(i,j,1) < 0.05 * 100) {
					// If pixel value is in interval [0, 5] we update it to A
					conversion_image.setPixel(i,j,1, checkValue(A2_S));
				} else if (src.getPixel(i,j,1) > 0.95 * 100) {
					// If pixel value is in interval [5, 95] we update it to B
					conversion_image.setPixel(i,j,1, checkValue(B2_S));
				} else {
					// If the pixel value is in interval [5, 95] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,1) - ROI2_S_C2;
					new_value *= (B2_S - A2_S); 
					new_value /= (ROI2_S_D2 - ROI2_S_C2);
					new_value += A2_S;
					conversion_image.setPixel(i,j,1, checkValue(new_value));
				}

				// 5th and 95th percentile condition for I
				if (conversion_image.getPixel(i,j,2) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					conversion_image.setPixel(i,j,2, checkValue(A2_I));
				} else if (conversion_image.getPixel(i,j,2) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					conversion_image.setPixel(i,j,2, checkValue(B2_I));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,2) - ROI2_I_C2;
					new_value *= (B2_I - A2_I); 
					new_value /= (ROI2_I_D2 - ROI2_I_C2);
					new_value += A2_I;
					conversion_image.setPixel(i,j,2, checkValue(new_value));
				}

				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value_H = conversion_image.getPixel(i,j,0);
				int current_pixel_value_S = conversion_image.getPixel(i,j,1);
				int current_pixel_value_I = conversion_image.getPixel(i,j,2);

				for (int row=0; row<361; row++)
				{	

					if (current_pixel_value_H == histogram_roi2_H_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_H_post[row][1]++;
					}
				}

				for (int row=0; row<101; row++)
				{	

					if (current_pixel_value_S == histogram_roi2_S_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_S_post[row][1]++;
					}
				}

				for (int row=0; row<256; row++)
				{	

					if (current_pixel_value_I == histogram_roi2_I[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi2_I_post[row][1]++;
					}
				}

			} else if (x3 != DEFAULT && (i >= x3 && i < x3 + Sx3) && (j >= y3 && j < y3 + Sy3)) {
				
				// !!! We apply stretching on pixels of each channel!!!

				// 5th and 95th percentile condition for H
				if (conversion_image.getPixel(i,j,0) < 0.05 * 360) {
					// If pixel value is in interval [0, 18] we update it to A
					conversion_image.setPixel(i,j,0, A3_H);
				} else if (src.getPixel(i,j,0) > 0.95 * 360) {
					// If pixel value is in interval [18, 342] we update it to B
					conversion_image.setPixel(i,j,0, B3_H);
				} else {
					// If the pixel value is in interval [342, 360] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,0) - ROI3_H_C3;
					new_value *= (B3_H - A3_H); 
					new_value /= (ROI3_H_D3 - ROI3_H_C3);
					new_value += A3_H;
					conversion_image.setPixel(i,j,0, new_value);
				}

				// 5th and 95th percentile condition for S
				if (conversion_image.getPixel(i,j,1) < 0.05 * 100) {
					// If pixel value is in interval [0, 5] we update it to A
					conversion_image.setPixel(i,j,1, checkValue(A3_S));
				} else if (src.getPixel(i,j,1) > 0.95 * 100) {
					// If pixel value is in interval [5, 95] we update it to B
					conversion_image.setPixel(i,j,1, checkValue(B3_S));
				} else {
					// If the pixel value is in interval [5, 95] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,1) - ROI3_S_C3;
					new_value *= (B3_S - A3_S); 
					new_value /= (ROI3_S_D3 - ROI3_S_C3);
					new_value += A3_S;
					conversion_image.setPixel(i,j,1, checkValue(new_value));
				}

				// 5th and 95th percentile condition for I
				if (conversion_image.getPixel(i,j,2) < 0.05 * 255) {
					// If pixel value is in interval [0, 12.75] we update it to A
					conversion_image.setPixel(i,j,2, checkValue(A3_I));
				} else if (conversion_image.getPixel(i,j,2) > 0.95 * 255) {
					// If pixel value is in interval [242.55, 255] we update it to B
					conversion_image.setPixel(i,j,2, checkValue(B3_I));
				} else {
					// If the pixel value is in interval [12.75, 242,55] we stretch it to (A,B)
					int new_value = conversion_image.getPixel(i,j,2) - ROI3_I_C3;
					new_value *= (B3_I - A3_I); 
					new_value /= (ROI3_I_D3 - ROI3_I_C3);
					new_value += A3_I;
					conversion_image.setPixel(i,j,2, checkValue(new_value));
				}

				// Now we also need to update the Histogram after the Stretching of the Pixel
				int current_pixel_value_H = conversion_image.getPixel(i,j,0);
				int current_pixel_value_S = conversion_image.getPixel(i,j,1);
				int current_pixel_value_I = conversion_image.getPixel(i,j,2);

				for (int row=0; row<361; row++)
				{	

					if (current_pixel_value_H == histogram_roi3_H_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_H_post[row][1]++;
					}
				}

				for (int row=0; row<101; row++)
				{	

					if (current_pixel_value_S == histogram_roi3_S_post[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_S_post[row][1]++;
					}
				}

				for (int row=0; row<256; row++)
				{	

					if (current_pixel_value_I == histogram_roi3_I[row][0]) {
						// Increment the number of occurrences for that particular grey level
						histogram_roi3_I_post[row][1]++;
					}
				}

			} else {
				continue;
			}
		}

     

	 // Step 5: Now we need to convert HSI back to RGB so we can output it
	hsitorgb(conversion_image, tgt);
	
	// Step 6. Compute and Plot the New Histograms.
    // Call Histostretch to visualize histograms
	
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
