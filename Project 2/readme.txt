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

1. Histogram Stretching: Histostretch
Take a 2D array representing the histogram data and output it as an image to show the visualization of intensity levels
and their frequency. You pass the histogram 2D array and the name you want to give to the output file. Don't forget to add
.pgm in the name. You can call this function from anywhere inside the other implemented algorithms.


2. ROI based Histogram Stretching: althistostretch
Perform contrast enhancement by performing Histogram Stretching on each ROI(region of interest, up to 3).
For each ROI the user can provide a different intensity range. 

3. Histogram Modification with Thresholding: histothres
Perform contrast enhancement by performing Histogram Stretching on each ROI(region of interest, up to 3). Additionally,
thresholding is used so Histogram Stretching is only applied to the dark pixels (< threshold).
For each ROI the user can provide a different threshold and intensity range. 

4. Color Image Histogram Stretching: percchastrech
Perform contrast enhancement by performing Histogram Stretching on each ROI(region of interest, up to 3) on Color Images of RGB color space.
Histogram Stretching is applied only on one channel (R,G,B) at a time. 
For each ROI the user can provide a different channel (R,G,B) and intensity range. 

5. Color Image Histogram Stretching: rgbstretch
Perform contrast enhancement by performing Histogram Stretching on each ROI(region of interest, up to 3) on Color Images of RGB color space.
Histogram Stretching is applied only on all channels (R,G,B) at once. 
For each ROI the user can provide a different intensity range. 

6. Color Image HSI Color Space I Histogram Stretching: istretch
Perform contrast enhancement by performing Histogram Stretching on each ROI(region of interest, up to 3) on Color Images of HSI color space.
Color image is converted from RGB to HSI color space and Histogram Stretching is applied only on the I component. Then the image is converted back to RGB for display. 
For each ROI the user can provide a different intensity range. 

7. Color Image HSI Color Space H Histogram Stretching: hstretch
Perform contrast enhancement by performing Histogram Stretching on each ROI(region of interest, up to 3) on Color Images of HSI color space.
Color image is converted from RGB to HSI color space and Histogram Stretching is applied only on the H component. Then the image is converted back to RGB for display. 
For each ROI the user can provide a different intensity range. 

8. Color Image HSI Color Space S Histogram Stretching: sstretch
Perform contrast enhancement by performing Histogram Stretching on each ROI(region of interest, up to 3) on Color Images of HSI color space.
Color image is converted from RGB to HSI color space and Histogram Stretching is applied only on the S component. Then the image is converted back to RGB for display. 
For each ROI the user can provide a different intensity range. 

9. Color Image HSI Color Space Histogram Stretching: fullhsistretch
Perform contrast enhancement by performing Histogram Stretching on each ROI(region of interest, up to 3) on Color Images of HSI color space.
Color image is converted from RGB to HSI color space and Histogram Stretching is applied on all components at once (H,S,I). Then the image is converted back to RGB for display. 
For each ROI the user can provide a different intensity range. 

10. Scaling: Scale
Reduce or expand the heigh and width with two scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.


*** HELPER FUNCTIONS ***

1. checkChannel: This function is able to convert user input of channel to integer (0, 1, 2) that can be used in the algorithm logic.

2. rgbtohsi: This function is very important as it converts a Color Image from RGB color space to HSI color space. 

3. hsitorgb: This function will convert a Color Image from HSI color space to RGB color space.

4. findMinMax: This function will find the Min and Max in a 2D array. It is used to find the C - Imin and D - Imax values in a histogram which are later used in the stretching formula.

5. initializeMinMax: This function will initialize the C,D values found in a histogram that have a frequency greater than 0. The output of this function is later used for findMinMax function.

6. visualizeHistogram: This function is used to provide a form of visualization for the histograms by printing it out on the terminal/console. It is not fancy but can be used to visualize how the histogram
has changed after stretching. Good for debugging.

7. visualizeHistogramHorizontally: This function is used to provide a form of visualization for the histograms by printing it out on the terminal/console. It is not fancy but can be used to visualize how the histogram
has changed after stretching. Good for debugging. Another way of visualization of the histogram. It was easier for me to use this one to visualize the histogram.

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
3. the name of the filter. Use "Histostretch", "althistostretch", "histothres", "percchastrech", "rgbstretch", "istretch", "hstretch", "sstretch", "fullhsistretch" for your filters;
4. the number of ROIs being used
5. initial row, this means the first pixel in the leftmost position to begin the ROI 
calculation 
6. initial column, this means the first pixel in the top most position to begin the ROI 
calculation. Unlike x (which determines left to right), y axis is responsible for  up and 
down.  
7. Sx - total number of pixels in the x axis
8. Sy - total number of pixels in the y axis
9. p1, ...pX - parameters needed for function. (p1 - threshhold or color channel or A of intensity range)
					       (p1, p2 - intensity range [A,B]. For hsistretch you will provide intensity range for each H, S, I)
10-22. Repeat of parameters 5-9 with different value which are specific to their respective ROI. (Up to 3 ROIs)
  

*** Run the program: ./iptool parameters.txt
