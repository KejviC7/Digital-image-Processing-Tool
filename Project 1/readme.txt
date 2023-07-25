This software is architectured as follows. This software can work on grad server.

iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory.



*** INSTALATION ***

On Linux

Enter the project directory in terminal and run make

As a result you should get iptool in project/bin directory.

*** FUNCTIONS ***

1. Add intensity: add
Increase the intensity for a gray-level image within the specified ROIs (up to 3).
For each ROI the user can provide a different intensity value.

2. Binarization: binarize
Binarize the pixels with the threshold on each ROI(region of interest, up to 3).
For each ROI the user can provide a different threshold value. 

3. Smoothing: uniformsmooth
Apply uniform smoothing to the ROIs(up to 3) of the image, using odd windows size which is 
also known as kernel size. The avg value of all pixels in the kernel is used to replace the 
current value of the pixel. The user can specify a window size parameter for each ROI.

4. Smoothing with Threshold: adptvsmooth
Apply uniform smoothing to the ROIs(up to 3) of the image, using odd windows size which is 
also known as kernel size. The avg value of all pixels in the kernel is used to replace the 
current value of the pixel. However, the user provides a threshold value and if the change of
the pixel value due to uniform smoothing is larger than the threshold the pixel value will
remain unchanged. The user can specify a window size parameter for each ROI.

5. Change intensity for Color Images: mulcolorbright
Add multiplicative color brightness adjustment on the ROIs(up to 3) of a Color image. The user will 
provide the threshold/value that will be used to multiply with the current pixel's channel value. The new
value must be in the range 0-255. A different threshold can be used for each ROI.

6. Scaling: Scale
Reduce or expand the heigh and width with two scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.


*** Conversion of PGM TO JPG ***
We will be using ImageMagick
Run the following command:
mogrify -format jpg *.pgm

*** Conversion of PPM TO JPG (Color Images)***
We will be using ImageMagick
Run the following command:
mogrify -format jpg *.ppm

*** PARAMETERS FILE ***

There are nine - twenty-two parameters:
1. the input file name;
2. the output file name;
3. the name of the filter. Use "add", "binarize", "uniformsmooth", "adptvsmooth", "mulcolorbright" for your filters;
4. the number of ROIs being used
5. initial row, this means the first pixel in the leftmost position to begin the ROI 
calculation 
6. initial column, this means the first pixel in the top most position to begin the ROI 
calculation. Unlike x (which determines left to right), y axis is responsible for  up and 
down.  
7. Sx - total number of pixels in the x axis
8. Sy - total number of pixels in the y axis
9. p1, ...pX - parameters needed for function. (p1 - intensity or threshhold or windows size depending on the function)
					       (p1, p2 - window size, threshold for "adptvsmooth" function
10-22. Repeat of parameters 5-9 with different value which are specific to their respective ROI. (Up to 3 ROIs)
  

*** Run the program: ./iptool parameters.txt
