#include "../iptools/core.h"
#include <string.h>

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char infile[MAXLEN];
	char outfile[MAXLEN];
	char *pch;
	int x1, y1, Sx1, Sy1, value1;
	int x2, y2, Sx2, Sy2, value2;
	int x3, y3, Sx3, Sy3, value3;
	int roi;
	int threshold1, threshold2, threshold3;
	
	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	while(fgets(str,MAXLEN,fp) != NULL) {
		if (strncmp(str,"#",1)==0) continue;
		int NumParameters = 0;
		
		pch = strtok(str, " ");
		strcpy(infile, pch);

		pch = strtok(NULL, " ");
		strcpy(outfile, pch);

		pch = strtok(NULL, " ");
		if (strncmp(pch,"opencv",6)==0) {
			cv::Mat I = cv::imread(infile);
			cv::Mat I2;
			
			if( I.empty()) {
				cout << "Could not open or find the image: " << infile << endl;
				return -1;
			}
			
			pch = strtok(NULL, " ");
			if (strncmp(pch,"gray",4)==0) {
				utility::cv_gray(I, I2);
			}
			
			else if (strncmp(pch,"sobel3",MAXLEN)==0) {
				/* Very Important #ROI*/
				roi = atoi(strtok(NULL, " "));

				/* At least 1 ROI is required so these parameters will be required. */
				x1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y1 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy1 = atoi(strtok(NULL, " "));
				
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					
					// Call function
					utility::cv_sobel3(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
										
					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
						
					// Call function
					utility::cv_sobel3(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_sobel3(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"sobel5",MAXLEN)==0) {
				/* Very Important #ROI*/
				roi = atoi(strtok(NULL, " "));

				/* At least 1 ROI is required so these parameters will be required. */
				x1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y1 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy1 = atoi(strtok(NULL, " "));
				
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					
					// Call function
					utility::cv_sobel5(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
										
					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
						
					// Call function
					utility::cv_sobel5(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_sobel5(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"coloredge3",MAXLEN)==0) {
				/* Very Important #ROI*/
				roi = atoi(strtok(NULL, " "));

				/* At least 1 ROI is required so these parameters will be required. */
				x1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y1 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy1 = atoi(strtok(NULL, " "));
				
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					
					// Call function
					utility::cv_sobel3_color(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
										
					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
						
					// Call function
					utility::cv_sobel3_color(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_sobel3_color(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"coloredge5",MAXLEN)==0) {
				/* Very Important #ROI*/
				roi = atoi(strtok(NULL, " "));

				/* At least 1 ROI is required so these parameters will be required. */
				x1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y1 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy1 = atoi(strtok(NULL, " "));
				
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					
					// Call function
					utility::cv_sobel5_color(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
										
					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
						
					// Call function
					utility::cv_sobel5_color(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_sobel5_color(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"binaryedge",MAXLEN)==0) {
				/* Very Important #ROI*/
				roi = atoi(strtok(NULL, " "));

				/* At least 1 ROI is required so these parameters will be required. */
				x1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y1 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy1 = atoi(strtok(NULL, " "));
				
				//threshold1 = atoi(strtok(NULL, " "));

				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));

					//threshold2 = atoi(strtok(NULL, " "));
					
					// Call function
					utility::cv_binaryedge(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					
					//threshold2 = atoi(strtok(NULL, " "));

					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));

					//threshold3 = atoi(strtok(NULL, " "));
						
					// Call function
					utility::cv_binaryedge(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_binaryedge(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"direction_binaryedge",MAXLEN)==0) {
				/* Very Important #ROI*/
				roi = atoi(strtok(NULL, " "));

				/* At least 1 ROI is required so these parameters will be required. */
				x1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y1 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy1 = atoi(strtok(NULL, " "));
				
				//threshold1 = atoi(strtok(NULL, " "));

				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));

					//threshold2 = atoi(strtok(NULL, " "));
					
					// Call function
					utility::cv_binaryedge_direction(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					
					//threshold2 = atoi(strtok(NULL, " "));

					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));

					//threshold3 = atoi(strtok(NULL, " "));
						
					// Call function
					utility::cv_binaryedge_direction(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_binaryedge_direction(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"cannycv",MAXLEN)==0) {
				/* Very Important #ROI*/
				roi = atoi(strtok(NULL, " "));

				/* At least 1 ROI is required so these parameters will be required. */
				x1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y1 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy1 = atoi(strtok(NULL, " "));
				
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					
					// Call function
					utility::cv_canny(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
										
					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
						
					// Call function
					utility::cv_canny(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_canny(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"histoeq",MAXLEN)==0) {
				/* Very Important #ROI*/
				roi = atoi(strtok(NULL, " "));

				/* At least 1 ROI is required so these parameters will be required. */
				x1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y1 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy1 = atoi(strtok(NULL, " "));
				
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					
					// Call function
					utility::cv_histoeq(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
										
					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
						
					// Call function
					utility::cv_histoeq(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_histoeq(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"cannyhistoeq",MAXLEN)==0) {
				/* Very Important #ROI*/
				roi = atoi(strtok(NULL, " "));

				/* At least 1 ROI is required so these parameters will be required. */
				x1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y1 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy1 = atoi(strtok(NULL, " "));
				
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					
					// Call function
					utility::cv_canny_hist(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
										
					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
						
					// Call function
					utility::cv_canny_hist(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_canny_hist(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"otsucv",MAXLEN)==0) {
				/* Very Important #ROI*/
				roi = atoi(strtok(NULL, " "));

				/* At least 1 ROI is required so these parameters will be required. */
				x1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y1 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy1 = atoi(strtok(NULL, " "));
				
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					
					// Call function
					utility::cv_otsucv(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
										
					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
						
					// Call function
					utility::cv_otsucv(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_otsucv(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"otsuhisto",MAXLEN)==0) {
				/* Very Important #ROI*/
				roi = atoi(strtok(NULL, " "));

				/* At least 1 ROI is required so these parameters will be required. */
				x1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y1 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx1 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy1 = atoi(strtok(NULL, " "));
				
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					
					// Call function
					utility::cv_otsuhisto(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
										
					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
						
					// Call function
					utility::cv_otsuhisto(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_otsuhisto(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"QRcode",MAXLEN)==0) {
				//roi = atoi(strtok(NULL, " "));
				utility::cv_QRcode(I,I2);

			}

			else if (strncmp(pch,"QRcodequl",MAXLEN)==0) {
				//roi = atoi(strtok(NULL, " "));
				utility::cv_QRcodequl(I,I2);

			}

			else if (strncmp(pch,"blur_avg",8)==0) {
				pch = strtok(NULL, " ");
				utility::cv_avgblur(I, I2, atoi(pch));
			}
			else {
				printf("No function: %s\n", pch);
				continue;
			}
			
			cv::imwrite(outfile, I2);
		}
		else {
			src.read(infile);
			if (strncmp(pch,"add",3)==0) {
				pch = strtok(NULL, " ");
				utility::addGrey(src,tgt,atoi(pch));
			}

			else if (strncmp(pch,"binarize",8)==0) {
				pch = strtok(NULL, " ");
				utility::binarize(src,tgt,atoi(pch));
			}

			else if (strncmp(pch,"scale",5)==0) {
				pch = strtok(NULL, " ");
				utility::scale(src,tgt,atof(pch));
			}

			else {
				printf("No function: %s\n", pch);
				continue;
			}
			
			tgt.save(outfile);
		}
	}
	fclose(fp);
	return 0;
}

