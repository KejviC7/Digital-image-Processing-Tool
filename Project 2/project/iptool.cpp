/************************************************************
 *															*
 * This sample project include three functions:				*
 * 1. Add intensity for gray-level image.					*
 *    Input: source image, output image name, value			*
 *															*
 * 2. Image thresholding: pixels will become black if the	*
 *    intensity is below the threshold, and white if above	*
 *    or equal the threhold.								*
 *    Input: source image, output image name, threshold		*
 *															*
 * 3. Image scaling: reduction/expansion of 2 for 			*
 *    the width and length. This project uses averaging 	*
 *    technique for reduction and pixel replication			*
 *    technique for expansion.								*
 *    Input: source image, output image name, scale factor	*
 *															*
 ************************************************************/

#include "../iptools/core.h"
#include <strings.h>
#include <string.h>
#include <iostream>
//#include <opencv2/opencv.hpp>

using namespace std;
//using namespace cv;

#define MAXLEN 256
#define DEFAULT 999999999
int main (int argc, char** argv)
{
	
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char outfile[MAXLEN];
	char *pch;
	int threshold;
	int intensity1;
	int intensity2;
	int threshold_one;
	int threshold_two;
	int roi;
	int x1, y1, Sx1, Sy1, value1;
	int x2, y2, Sx2, Sy2, value2;
	int x3, y3, Sx3, Sy3, value3;
	int threshold1, threshold2, threshold3;
	int windowsize1, windowsize2, windowsize3;
	float DR, DG, DB;
	int A1, B1, A2, B2, A3, B3;
	char channel1, channel2, channel3;
	int A1_H, B1_H, A1_S, B1_S, A1_I, B1_I;
	int A2_H, B2_H, A2_S, B2_S, A2_I, B2_I;
	int A3_H, B3_H, A3_S, B3_S, A3_I, B3_I;


	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}
	//cout << "The value of argc is: " << argc;
	while(fgets(str,MAXLEN,fp) != NULL) {
		//cout << "\n The length of the txt string is: " << strlen(str);
		pch = strtok(str, " ");
		src.read(pch);

		pch = strtok(NULL, " ");
		strcpy(outfile, pch);

		pch = strtok(NULL, " ");
        
		if (strncasecmp(pch,"althistostretch",MAXLEN)==0) {
			
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
			A1 = atoi(strtok(NULL, " "));
			B1 = atoi(strtok(NULL, " "));

			if (roi == 2) {
				// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));

				// Call function
				utility::althistostretch(src,tgt,roi,x1,y1,Sx1,Sy1,A1,B1,x2,y2,Sx2,Sy2,A2,B2);

			} else if (roi == 3) {
				//cout << "The roi is: " << roi;
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));
				
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    A3 = atoi(strtok(NULL, " "));
				B3 = atoi(strtok(NULL, " "));
 
				// Call function
				utility::althistostretch(src,tgt,roi,x1,y1,Sx1,Sy1,A1,B1,x2,y2,Sx2,Sy2,A2,B2,x3,y3,Sx3,Sy3,A3,B3);

			} else {
				// If ROI is 1
				utility::althistostretch(src,tgt,roi,x1,y1,Sx1,Sy1,A1,B1);
			}
			
		}

		else if (strncasecmp(pch,"histothres",MAXLEN)==0) {
			
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
			A1 = atoi(strtok(NULL, " "));
			B1 = atoi(strtok(NULL, " "));

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
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));

				// Call function
				utility::histothres(src,tgt,roi,x1,y1,Sx1,Sy1,threshold1,A1,B1,x2,y2,Sx2,Sy2,threshold2,A2,B2);

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
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));
				
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    threshold3 = atoi(strtok(NULL, " "));
				A3 = atoi(strtok(NULL, " "));
				B3 = atoi(strtok(NULL, " "));
 
				// Call function
				utility::histothres(src,tgt,roi,x1,y1,Sx1,Sy1,threshold1,A1,B1,x2,y2,Sx2,Sy2,threshold2,A2,B2,x3,y3,Sx3,Sy3,threshold3,A3,B3);

			} else {
				// If ROI is 1
				utility::histothres(src,tgt,roi,x1,y1,Sx1,Sy1,threshold1,A1,B1);
			}
			
		}

		else if (strncasecmp(pch,"percchastrech",MAXLEN)==0) {
			
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
			channel1 = *strtok(NULL, " ");
			A1 = atoi(strtok(NULL, " "));
			B1 = atoi(strtok(NULL, " "));

			if (roi == 2) {
				// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				channel2 = *strtok(NULL, " ");
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));

				// Call function
				utility::percchastrech(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1,x2,y2,Sx2,Sy2,channel2,A2,B2);

			} else if (roi == 3) {
				//cout << "The roi is: " << roi;
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				channel2 = *strtok(NULL, " ");
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));
				
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    channel3 = *strtok(NULL, " ");
				A3 = atoi(strtok(NULL, " "));
				B3 = atoi(strtok(NULL, " "));
 
				// Call function
				utility::percchastrech(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1,x2,y2,Sx2,Sy2,channel2,A2,B2,x3,y3,Sx3,Sy3,channel3,A3,B3);

			} else {
				// If ROI is 1
				utility::percchastrech(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1);
			}
			
		}

		else if (strncasecmp(pch,"rgbstretch",MAXLEN)==0) {
			
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
			channel1 = atoi(strtok(NULL, " "));
			A1 = atoi(strtok(NULL, " "));
			B1 = atoi(strtok(NULL, " "));

			if (roi == 2) {
				// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				channel2 = atoi(strtok(NULL, " "));
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));

				// Call function
				utility::rgbstretch(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1,x2,y2,Sx2,Sy2,channel2,A2,B2);

			} else if (roi == 3) {
				//cout << "The roi is: " << roi;
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				channel2 = atoi(strtok(NULL, " "));
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));
				
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    channel3 = atoi(strtok(NULL, " "));
				A3 = atoi(strtok(NULL, " "));
				B3 = atoi(strtok(NULL, " "));
 
				// Call function
				utility::rgbstretch(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1,x2,y2,Sx2,Sy2,channel2,A2,B2,x3,y3,Sx3,Sy3,channel3,A3,B3);

			} else {
				// If ROI is 1
				utility::rgbstretch(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1);
			}
			
		}

		else if (strncasecmp(pch,"istretch",MAXLEN)==0) {
			
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
			channel1 = *strtok(NULL, " ");
			A1 = atoi(strtok(NULL, " "));
			B1 = atoi(strtok(NULL, " "));

			if (roi == 2) {
				// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				channel2 = *strtok(NULL, " ");
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));

				// Call function
				utility::istretch(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1,x2,y2,Sx2,Sy2,channel2,A2,B2);

			} else if (roi == 3) {
				//cout << "The roi is: " << roi;
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				channel2 = *strtok(NULL, " ");
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));
				
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    channel3 = *strtok(NULL, " ");
				A3 = atoi(strtok(NULL, " "));
				B3 = atoi(strtok(NULL, " "));
 
				// Call function
				utility::istretch(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1,x2,y2,Sx2,Sy2,channel2,A2,B2,x3,y3,Sx3,Sy3,channel3,A3,B3);

			} else {
				// If ROI is 1
				utility::istretch(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1);
			}
			
		}

		else if (strncasecmp(pch,"hstretch",MAXLEN)==0) {
			
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
			channel1 = *strtok(NULL, " ");
			A1 = atoi(strtok(NULL, " "));
			B1 = atoi(strtok(NULL, " "));

			if (roi == 2) {
				// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				channel2 = *strtok(NULL, " ");
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));

				// Call function
				utility::hstretch(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1,x2,y2,Sx2,Sy2,channel2,A2,B2);

			} else if (roi == 3) {
				//cout << "The roi is: " << roi;
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				channel2 = *strtok(NULL, " ");
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));
				
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    channel3 = *strtok(NULL, " ");
				A3 = atoi(strtok(NULL, " "));
				B3 = atoi(strtok(NULL, " "));
 
				// Call function
				utility::hstretch(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1,x2,y2,Sx2,Sy2,channel2,A2,B2,x3,y3,Sx3,Sy3,channel3,A3,B3);

			} else {
				// If ROI is 1
				utility::hstretch(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1);
			}
			
		}

		else if (strncasecmp(pch,"sstretch",MAXLEN)==0) {
			
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
			channel1 = *strtok(NULL, " ");
			A1 = atoi(strtok(NULL, " "));
			B1 = atoi(strtok(NULL, " "));

			if (roi == 2) {
				// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				channel2 = *strtok(NULL, " ");
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));

				// Call function
				utility::sstretch(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1,x2,y2,Sx2,Sy2,channel2,A2,B2);

			} else if (roi == 3) {
				//cout << "The roi is: " << roi;
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				channel2 = *strtok(NULL, " ");
				A2 = atoi(strtok(NULL, " "));
				B2 = atoi(strtok(NULL, " "));
				
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    channel3 = *strtok(NULL, " ");
				A3 = atoi(strtok(NULL, " "));
				B3 = atoi(strtok(NULL, " "));
 
				// Call function
				utility::sstretch(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1,x2,y2,Sx2,Sy2,channel2,A2,B2,x3,y3,Sx3,Sy3,channel3,A3,B3);

			} else {
				// If ROI is 1
				utility::sstretch(src,tgt,roi,x1,y1,Sx1,Sy1,channel1,A1,B1);
			}
			
		}

		else if (strncasecmp(pch,"fullhsistretch",MAXLEN)==0) {
			
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
			//channel1 = *strtok(NULL, " ");
			A1_H = atoi(strtok(NULL, " "));
			B1_H = atoi(strtok(NULL, " "));
			A1_S = atoi(strtok(NULL, " "));
			B1_S = atoi(strtok(NULL, " "));
			A1_I = atoi(strtok(NULL, " "));
			B1_I = atoi(strtok(NULL, " "));

			if (roi == 2) {
				// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				//channel2 = *strtok(NULL, " ");
				A2_H = atoi(strtok(NULL, " "));
				B2_H = atoi(strtok(NULL, " "));
				A2_S = atoi(strtok(NULL, " "));
				B2_S = atoi(strtok(NULL, " "));
				A2_I = atoi(strtok(NULL, " "));
				B2_I = atoi(strtok(NULL, " "));

				// Call function
				utility::fullhsistretch(src,tgt,roi,x1,y1,Sx1,Sy1,A1_H,B1_H,A1_S,B1_S,A1_I,B1_I,x2,y2,Sx2,Sy2,A2_H,B2_H,A2_S,B2_S,A2_I,B2_I);

			} else if (roi == 3) {
				//cout << "The roi is: " << roi;
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				//channel2 = *strtok(NULL, " ");
				A2_H = atoi(strtok(NULL, " "));
				B2_H = atoi(strtok(NULL, " "));
				A2_S = atoi(strtok(NULL, " "));
				B2_S = atoi(strtok(NULL, " "));
				A2_I = atoi(strtok(NULL, " "));
				B2_I = atoi(strtok(NULL, " "));
				
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    //channel3 = *strtok(NULL, " ");
				A3_H = atoi(strtok(NULL, " "));
				B3_H = atoi(strtok(NULL, " "));
				A3_S = atoi(strtok(NULL, " "));
				B3_S = atoi(strtok(NULL, " "));
				A3_I = atoi(strtok(NULL, " "));
				B3_I = atoi(strtok(NULL, " "));
				// Call function
				utility::fullhsistretch(src,tgt,roi,x1,y1,Sx1,Sy1,A1_H,B1_H,A1_S,B1_S,A1_I,B1_I,x2,y2,Sx2,Sy2,A2_H,B2_H,A2_S,B2_S,A2_I,B2_I,x3,y3,Sx3,Sy3,A3_H,B3_H,A3_S,B3_S,A3_I,B3_I);

			} else {
				// If ROI is 1
				utility::fullhsistretch(src,tgt,roi,x1,y1,Sx1,Sy1,A1_H,B1_H,A1_S,B1_S,A1_I,B1_I);
			}
			
		}
    
		else if (strncasecmp(pch,"scale",MAXLEN)==0) {
			/* Image scaling */
			pch = strtok(NULL, " ");
			utility::scale(src,tgt,atof(pch));
		}

		else {
			printf("No function: %s\n", pch);
			continue;
		}
       
		tgt.save(outfile);
	}
	fclose(fp);
	return 0;
}

