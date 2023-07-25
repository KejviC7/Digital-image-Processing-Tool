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
	int multiplier1, multiplier2, multiplier3;

	// Channel: 0 -> H; 1 -> S; 2 - > V
	int channel1, channel2, channel3;
	
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
			
			else if (strncmp(pch,"mag",MAXLEN)==0) {
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
					utility::cv_DisplayAmplitude(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

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
					utility::cv_DisplayAmplitude(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_DisplayAmplitude(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"invdft",MAXLEN)==0) {
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
					utility::cv_Inverse_Fourier(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2);

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
					utility::cv_Inverse_Fourier(I,I2,roi,x1,y1,Sx1,Sy1,x2,y2,Sx2,Sy2,x3,y3,Sx3,Sy3);
					
				} else {
					utility::cv_Inverse_Fourier(I, I2, roi, x1, y1, Sx1, Sy1);
				}
			}

			else if (strncmp(pch,"lowpass",MAXLEN)==0) {
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
				
				threshold1 = atoi(strtok(NULL, " "));
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					threshold2 = atoi(strtok(NULL, " "));
					// Call function
					utility::low_pass(I,I2,roi,x1,y1,Sx1,Sy1,threshold1, x2,y2,Sx2,Sy2, threshold2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					threshold2 = atoi(strtok(NULL, " "));

					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
					threshold3 = atoi(strtok(NULL, " "));
					
					// Call function
					utility::low_pass(I,I2,roi,x1,y1,Sx1,Sy1,threshold1,x2,y2,Sx2,Sy2,threshold2,x3,y3,Sx3,Sy3,threshold3);
					
				} else {
					utility::low_pass(I, I2, roi, x1, y1, Sx1, Sy1, threshold1);
				}
			}

			else if (strncmp(pch,"highpass",MAXLEN)==0) {
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
				threshold1 = atoi(strtok(NULL, " "));
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					threshold2 = atoi(strtok(NULL, " "));
					// Call function
					utility::high_pass(I,I2,roi,x1,y1,Sx1,Sy1,threshold1, x2,y2,Sx2,Sy2, threshold2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));

					threshold2 = atoi(strtok(NULL, " "));

					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
					threshold3 = atoi(strtok(NULL, " "));
					// Call function
					utility::high_pass(I,I2,roi,x1,y1,Sx1,Sy1,threshold1,x2,y2,Sx2,Sy2,threshold2,x3,y3,Sx3,Sy3,threshold3);
					
				} else {
					utility::high_pass(I, I2, roi, x1, y1, Sx1, Sy1, threshold1);
				}
			}

			else if (strncmp(pch,"low_pass_color",MAXLEN)==0) {
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
				threshold1 = atoi(strtok(NULL, " "));
				channel1 = atoi(strtok(NULL, " "));

				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					threshold2 = atoi(strtok(NULL, " "));
					channel2 = atoi(strtok(NULL, " "));
					// Call function
					utility::low_pass_color(I,I2,roi,x1,y1,Sx1,Sy1,threshold1,channel1, x2,y2,Sx2,Sy2, threshold2,channel2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));

					threshold2 = atoi(strtok(NULL, " "));
					channel2 = atoi(strtok(NULL, " "));

					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
					threshold3 = atoi(strtok(NULL, " "));
					channel3 = atoi(strtok(NULL, " "));
					// Call function
					utility::low_pass_color(I,I2,roi,x1,y1,Sx1,Sy1,threshold1,channel1,x2,y2,Sx2,Sy2,threshold2,channel2,x3,y3,Sx3,Sy3,threshold3,channel3);
					
				} else {
					utility::low_pass_color(I, I2, roi, x1, y1, Sx1, Sy1, threshold1,channel1);
				}
			}

			else if (strncmp(pch,"high_pass_color",MAXLEN)==0) {
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
				threshold1 = atoi(strtok(NULL, " "));
				channel1 = atoi(strtok(NULL, " "));
				
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					threshold2 = atoi(strtok(NULL, " "));
					channel2 = atoi(strtok(NULL, " "));
					// Call function
					utility::high_pass_color(I,I2,roi,x1,y1,Sx1,Sy1,threshold1,channel1, x2,y2,Sx2,Sy2, threshold2,channel2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));

					threshold2 = atoi(strtok(NULL, " "));
					channel2 = atoi(strtok(NULL, " "));

					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
					threshold3 = atoi(strtok(NULL, " "));
					channel3 = atoi(strtok(NULL, " "));
					// Call function
					utility::high_pass_color(I,I2,roi,x1,y1,Sx1,Sy1,threshold1,channel1,x2,y2,Sx2,Sy2,threshold2,channel2,x3,y3,Sx3,Sy3,threshold3,channel3);
					
				} else {
					utility::high_pass_color(I, I2, roi, x1, y1, Sx1, Sy1, threshold1,channel1);
				}
			}

			else if (strncmp(pch,"edgesharp",MAXLEN)==0) {
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
				multiplier1 = atoi(strtok(NULL, " "));
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					multiplier2 = atoi(strtok(NULL, " "));
					// Call function
					utility::unsharp_mask(I,I2,roi,x1,y1,Sx1,Sy1,multiplier1, x2,y2,Sx2,Sy2, multiplier2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));

					multiplier2 = atoi(strtok(NULL, " "));

					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
					multiplier3 = atoi(strtok(NULL, " "));
					// Call function
					utility::unsharp_mask(I,I2,roi,x1,y1,Sx1,Sy1,multiplier1,x2,y2,Sx2,Sy2,multiplier2,x3,y3,Sx3,Sy3,multiplier3);
					
				} else {
					utility::unsharp_mask(I, I2, roi, x1, y1, Sx1, Sy1, multiplier1);
				}
			}

			else if (strncmp(pch,"edgesharp_low",MAXLEN)==0) {
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
				multiplier1 = atoi(strtok(NULL, " "));
				if (roi == 2) {
					// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));
					multiplier2 = atoi(strtok(NULL, " "));
					// Call function
					utility::unsharp_mask_low(I,I2,roi,x1,y1,Sx1,Sy1,multiplier1, x2,y2,Sx2,Sy2, multiplier2);

				} else if (roi == 3) {
					//cout << "The roi is: " << roi;
					x2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y2 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx2 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy2 = atoi(strtok(NULL, " "));

					multiplier2 = atoi(strtok(NULL, " "));

					x3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					y3 = atoi(strtok(NULL, " "));
					/* Add initial row */
					Sx3 = atoi(strtok(NULL, " "));
					/* Add initial col */
					Sy3 = atoi(strtok(NULL, " "));
					multiplier3 = atoi(strtok(NULL, " "));
					// Call function
					utility::unsharp_mask_low(I,I2,roi,x1,y1,Sx1,Sy1,multiplier1,x2,y2,Sx2,Sy2,multiplier2,x3,y3,Sx3,Sy3,multiplier3);
					
				} else {
					utility::unsharp_mask_low(I, I2, roi, x1, y1, Sx1, Sy1, multiplier1);
				}
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

