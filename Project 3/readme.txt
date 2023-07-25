This software is architectured as follows.

iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image. (without OpenCV)
	utility - This folder hosts the implemented algorithms.
	roi - This folder hosts the roi structure
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory.



*** INSTALATION ***

On Linux

Enter the project directory in terminal and run make
The executable file will be created in bin/



*** Run the program:
cd bin/
./iptool parameters.txt



*** FUNCTIONS (without OpenCV) ***

1. Add intensity: add
Increase the intensity for a gray-level image.
parameter: intensity

2. Binarization: binarize
Binarize the pixels with the threshold.
parameter: threshold

3. Scaling: Scale
Reduce or expand the heigh and width with two scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.
parameter: scaling_factor



*** FUNCTIONS (with OpenCV) ***

1. Sobel Edge Detection on Gray Level Image: sobel3
Apply Sobel Edge detection with kernel size 3x3 on the user specified ROI. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

2. Sobel Edge Detection on Gray Level Image: sobel5
Apply Sobel Edge detection with kernel size 5x5 on the user specified ROI. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

3. Sobel Edge Detection on Color Image: coloredge3 -> sobel3_color
The RGB color image is converted to HSV color space using OpenCV. Then Apply Sobel Edge detection with kernel size 3x3 on the user specified ROI only on the V component. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

4. Sobel Edge Detection on Color Image: coloredge5 -> sobel5_color
The RGB color image is converted to HSV color space using OpenCV. Then Apply Sobel Edge detection with kernel size 5x5 on the user specified ROI only on the V component. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

5. Binary Edge Image on Gray level images: binaryedge
Perform binarification by applying internal thresholding after applying Sobel Edge detection with kernel size 3x3 on the user specified ROI. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

6. Binary Edge Image with Direction on Gray level images: direction_binaryedge -> binaryedge_direction
Perform binarification by applying internal thresholding based on pixel direction which is calculated with formula direction = atan(gradient_y / gradient_x), after applying Sobel Edge detection with kernel size 3x3 on the user specified ROI. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

7. Canny Edge Detection on Gray Level images: cannycv
Apply Canny edge detection with kernel size 3x3 on the user specified ROI. Up to 3 ROIs can be provided. 
The Canny algorithm is a multi-stage algorithm where blurring, edge thinning and double thresholding is applied after Sobel. 
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

8. Histogram Equalization on Gray Level images: histoeq
Perform Histogram Equalization as contrast enhancement to improve edge detection on the user specified ROI. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

9. Canny Edge Detection with Histogram Equalization: canny_hist
Perform Histogram Equalization on the ROI and then apply Canny edge detection to that ROI. Up to 3 ROIs can be provided. 
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

10. Otsu Thresholding: otsucv
Perform the Otsu algorithm thresholding for image segmentation. Brute-force algorithm to find the best threshold level which provides the smallest sum of variances between the background and foreground pixels as determined by the Otsu algorithm. 
This is applied on the ROI. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

11. Otsu Thresholding with Histogram Equalization: otsuhisto
Perform the Otsu algorithm thresholding for image segmentation. Brute-force algorithm to find the best threshold level which provides the smallest sum of variances between the background and foreground pixels as determined by the Otsu algorithm. 
Additionally, apply Histogram Equalization on the background pixels as determined by the Otsu algorithm. This is applied on the ROI. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

12. QR Code Detection and Decoding: QRcode
Utilize the QRCodeDetector from OpenCV. Then detectAndDecode method is utilized to scan the image for the QR code. After it has been detected it will be decoded.
The output image will be a rectified image of the QR code that was detected and the console will print out the content of the QR code.  
parameter: no

13. QR Code Detection and Decoding with Histogram Equalization: QRcodequl
Perform histogram equalization on the image first to enhance contrast and feature detection, in this case QR code detection. Utilize the QRCodeDetector from OpenCV. Then detectAndDecode method is utilized to scan the image for the QR code. After it has been detected it will be decoded.
The output image will be a rectified image of the QR code that was detected and the console will print out the content of the QR code.  
parameter: no


14. Gray Scale: gray
Change whole image to gray-level.
parameter: no

15. Average Blur: blur_avg
Uniform smoothing.
parameter: window_size

*** HELPER FUNCTIONS ***
1. sobel_grayscale
This helper function performs the sobel edge detection for each passed ROI. It converts the ROI to grayscale. Then calls the Sobel operator with kernel size (3 or 5) on the x and y direction to compute the x_gradient and y_gradient. 
Then the x_gradient and y_gradient are converted to 8U. addWeighted is used to add the gradients together to compute the gradient of the pixel. Then returns the updated pixels. 

2. canny_grayscale
This helper function performs canny edge detection for each passed ROI. It converts the ROI to grayscale. Then applies blurring and calls the Canny function from OpenCV. You pass the threshold values for the double thresholding. 
Returns the updated ROI.

3. canny_hist_grayscale
This helper function will apply histogram equalization to the passed ROI. Then it will perform Canny as described in the function above. 
Returns the updated ROI.

4. otsu_grayscale
This helper function will perform Otsu thresholding on the passed ROI using OpenCV method. 
Returns the updated ROI.

5. otsu_histo_grayscale
This helper function will perform Otsu thresholding on the passed ROI using OpenCV method and apply Histrogram Equalization on the background pixels as specified by the Otsu algorithm.
Returns the updated ROI. 

6. histoeq_grayscale
This helper function will perform Histogram Equalization on the passed ROI using the OpenCV method "equalizeHist". 
Returns the updated ROI. 

7. sobel_grayscale_threshold
This helper function will perform binarification of the image by applying fixed thresholding after the Sobel has been applied. Set pixel value to 255 if value >= threshold, or 0 if value < threshold
Returns the updated ROI.

8. sobel_grayscale_threshold_direction
This helper function will perform binarification of the image by applying thresholding via the direction of the pixel after the Sobel has been applied. Set pixel value to 255 if value >= direction, or 0 if value < direction
Returns the updated ROI.

9. sobel_color 
This helper function performs the sobel edge detection for each passed ROI. It converts the ROI to HSV and isolates the V channel. Then applies the Sobel operator with kernel size 3 on the x and y direction to compute the x_gradient and y_gradient only on the V component. 
Then the x_gradient and y_gradient are converted to 8U. addWeighted is used to add the gradients together to compute the gradient of the pixel. The V component is updated and the ROI is converted back to color space RGB/BGR for display.

*** IMPORTANT ***
You are passing a .jpg image than imread method OpenCV reads. Even if you pass a grayscale image as jpg it will have 3 channels.
Then it will be converted to 1 channel before applying the algorithms. If you were to pass the src image as 1 channel image by default you would run into channel size error.
So in that case either read image as .jpg and it will be recognized with 3 channels by default, or comment out the color conversion function call in the function. 


*** PARAMETERS FILE ***
(Please check parameters.txt)
There are four parameters:
1. the input file name
2. the output file name
3. opencv / FUNCTIONS (without OpenCV): skip to 5 for parameters
4. the name of the function with OpenCV. Check FUNCTIONS (with OpenCV).
5. parameters (Check FUNCTIONS (with OpenCV) for parameters)


