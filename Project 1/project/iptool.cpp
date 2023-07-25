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
using namespace std;

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
	int threshhold1, threshhold2, threshhold3;
	int windowsize1, windowsize2, windowsize3;
	float DR, DG, DB;


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
        if (strncasecmp(pch,"add",MAXLEN)==0) {
			
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
			value1 = atoi(strtok(NULL, " "));

			if (roi == 2) {
				// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				value2 = atoi(strtok(NULL, " "));

				// Call function
				utility::add(src,tgt,roi,x1,y1,Sx1,Sy1,value1,x2,y2,Sx2,Sy2,value2);

			} else if (roi == 3) {
				//cout << "The roi is: " << roi;
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				value2 = atoi(strtok(NULL, " "));
				
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    value3 = atoi(strtok(NULL, " "));
 
				// Call function
				utility::add(src,tgt,roi,x1,y1,Sx1,Sy1,value1,x2,y2,Sx2,Sy2,value2,x3,y3,Sx3,Sy3,value3);

			} else {
				// If ROI is 1
				//utility::newAddGrey(src,tgt,x1,y1,Sx1,Sy1, value1);
				utility::add(src,tgt,roi,x1,y1,Sx1,Sy1,value1);
			}

        }

		else if (strncasecmp(pch,"binarize",MAXLEN)==0) {
			
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
			threshhold1 = atoi(strtok(NULL, " "));

			if (roi == 2) {
				// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				threshhold2 = atoi(strtok(NULL, " "));

				// Call function
				utility::binarize(src,tgt,roi,x1,y1,Sx1,Sy1,threshhold1,x2,y2,Sx2,Sy2,threshhold2);

			} else if (roi == 3) {
				//cout << "The roi is: " << roi;
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				threshhold2 = atoi(strtok(NULL, " "));
				
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    threshhold3 = atoi(strtok(NULL, " "));
 
				// Call function
				utility::binarize(src,tgt,roi,x1,y1,Sx1,Sy1,threshhold1,x2,y2,Sx2,Sy2,threshhold2,x3,y3,Sx3,Sy3,threshhold3);

			} else {
				// If ROI is 1
				utility::binarize(src,tgt,roi,x1,y1,Sx1,Sy1,threshhold1);
			}
			
		}
        
		else if (strncasecmp(pch,"uniformsmooth",MAXLEN)==0) {
			
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
			windowsize1 = atoi(strtok(NULL, " "));
            
			//cout << "x" << roi;
			//cout << "x" << x1;
			//cout << "x" << y1;
			//cout << "x" << Sx1;
			//cout << "x" << Sy1;
			//cout << "x" << windowsize1;
			if (roi == 2) {
				// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				windowsize2 = atoi(strtok(NULL, " "));

				// Call function
				utility::uniformsmooth(src,tgt,roi,x1,y1,Sx1,Sy1,windowsize1,x2,y2,Sx2,Sy2,windowsize2);

			} else if (roi == 3) {
				//cout << "The roi is: " << roi;
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				windowsize2 = atoi(strtok(NULL, " "));
				
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    windowsize3 = atoi(strtok(NULL, " "));
 
				// Call function
				utility::uniformsmooth(src,tgt,roi,x1,y1,Sx1,Sy1,windowsize1,x2,y2,Sx2,Sy2,windowsize2,x3,y3,Sx3,Sy3,windowsize3);

			} else {
				// If ROI is 1
				utility::uniformsmooth(src,tgt,roi,x1,y1,Sx1,Sy1,windowsize1);
			}
			
		}

		else if (strncasecmp(pch,"adptvsmooth",MAXLEN)==0) {
			
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
			windowsize1 = atoi(strtok(NULL, " "));
			threshhold1 = atoi(strtok(NULL, " "));

			if (roi == 2) {
				// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				windowsize2 = atoi(strtok(NULL, " "));
				threshhold2 = atoi(strtok(NULL, " "));
				// Call function
				utility::adptvsmooth(src,tgt,roi,x1,y1,Sx1,Sy1,windowsize1,threshhold1, x2,y2,Sx2,Sy2,windowsize2, threshhold2);

			} else if (roi == 3) {
				//cout << "The roi is: " << roi;
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				windowsize2 = atoi(strtok(NULL, " "));
				threshhold2 = atoi(strtok(NULL, " "));
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    windowsize3 = atoi(strtok(NULL, " "));
				threshhold3 = atoi(strtok(NULL, " "));
				// Call function
				utility::adptvsmooth(src,tgt,roi,x1,y1,Sx1,Sy1,windowsize1,threshhold1,x2,y2,Sx2,Sy2,windowsize2,threshhold2,x3,y3,Sx3,Sy3,windowsize3,threshhold3);

			} else {
				// If ROI is 1
				utility::adptvsmooth(src,tgt,roi,x1,y1,Sx1,Sy1,windowsize1,threshhold1);
			}
			
		}

		else if (strncasecmp(pch,"mulcolorbright",MAXLEN)==0) {
			
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
			DR = atof(strtok(NULL, " "));
            
			if (roi == 2) {
				// Since these parameters are optional we will only try to populate them from the file based on the #roi parameter
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				DG = atof(strtok(NULL, " "));

				// Call function
				utility::mulcolorbright(src,tgt,roi,x1,y1,Sx1,Sy1,DR,x2,y2,Sx2,Sy2,DG);

			} else if (roi == 3) {
				//cout << "The roi is: " << roi;
				x2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				y2 = atoi(strtok(NULL, " "));
				/* Add initial row */
				Sx2 = atoi(strtok(NULL, " "));
				/* Add initial col */
				Sy2 = atoi(strtok(NULL, " "));
				DG = atof(strtok(NULL, " "));
				
				x3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    y3 = atoi(strtok(NULL, " "));
				/* Add initial row */
		        Sx3 = atoi(strtok(NULL, " "));
				/* Add initial col */
			    Sy3 = atoi(strtok(NULL, " "));
			    DB = atof(strtok(NULL, " "));
 
				// Call function
				utility::mulcolorbright(src,tgt,roi,x1,y1,Sx1,Sy1,DR,x2,y2,Sx2,Sy2,DG,x3,y3,Sx3,Sy3,DB);

			} else {
				// If ROI is 1
				utility::mulcolorbright(src,tgt,roi,x1,y1,Sx1,Sy1,DR);
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

