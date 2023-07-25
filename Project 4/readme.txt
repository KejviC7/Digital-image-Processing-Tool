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

1. Display Image Frequency Amplitude from Frequency Domain: cv_DisplayAmplitude
Perform Discrete Fourier Transformation to get the image to frequency domain. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

2. Convert Image from Frequency Domain to image space: cv_Inverse_Fourier
Perform Inverse Discrete Fourier Transform to convert image from frequency domain to image space. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 x2 y2 Sx2 Sy2 x3 y3 Sx3 Sy3

3. Low Pass Filtering in Frequency Domain: lowpass 
After converting image to frequency domain apply low pass filtering to it. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 threshold1 x2 y2 Sx2 Sy2 threshold2 x3 y3 Sx3 Sy3 threshold3

4. High Pass Filtering in Frequency Domain: highpass 
After converting image to frequency domain apply high pass filtering to it. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 threshold1 x2 y2 Sx2 Sy2 threshold2 x3 y3 Sx3 Sy3 threshold3

5. Edge sharpening on high frequency pixels in frequency domain: edgesharp -> unsharp_mask
Apply edge sharpening on the high frequency pixels as filtered by the high pass function and modify pixels based on unsharp masking formula for each ROI. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 threshold1 multiplier1 x2 y2 Sx2 Sy2 threshold2 multiplier2 x3 y3 Sx3 Sy3 threshold3 multiplier3

6. Edge sharpening on low frequency pixels in frequency domain: edgesharp_low -> unsharp_mask_low
Apply edge sharpening on the high frequency pixels as filtered by the high pass function and modify pixels based on unsharp masking formula for each ROI. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 threshold1 multiplier1 x2 y2 Sx2 Sy2 threshold2 multiplier2 x3 y3 Sx3 Sy3 threshold3 multiplier3

7. Low Pass Filtering in Frequency Domain on Color Images of HSV color space: lowpass_color
After converting image to frequency domain apply low pass filtering only to H (channel = 0), S(channel = 1), or V(channel = 2) channel of it as specified by the user. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 threshold1 channel1 x2 y2 Sx2 Sy2 threshold2 channel2 x3 y3 Sx3 Sy3 threshold3 channel3

8. High Pass Filtering in Frequency Domain on Color Images of HSV color space: highpass_color
After converting image to frequency domain apply high pass filtering only to H (channel = 0), S(channel = 1), or V(channel = 2) channel of it as specified by the user. Up to 3 ROIs can be provided.
parameter: roi x1 y1 Sx1 Sy1 threshold1 channel1 x2 y2 Sx2 Sy2 threshold2 channel2 x3 y3 Sx3 Sy3 threshold3 channel3

9. Gray Scale: gray
Change whole image to gray-level.
parameter: no

10. Average Blur: blur_avg
Uniform smoothing.
parameter: window_size

*** HELPER FUNCTIONS ***
1. crop_and_rearrange
This helper function is used to cut the extra levels added by optimal image sizing, and swap the quadrants of the frequency domain for proper display. Returns the updated ROI

2. fourierTransform
This helper function will apply Discrete Fourier Transform from OpenCV, and make necessary quadrant swapping, computations, and normalizations in order to take the output from DFT and display the amplitude
spectrum of frequency domain to the human eye. Returns the updated ROI

3. inverseFourierTransform
This helper function will take the original image in image space, convert it to frequency domain using DFT and then convert it back to image space using inverse discrete fourier transform. 
Returns the ROI

4. low_pass_fourier
This helper function will convert the passed ROI to frequency domain and then apply low pass filtering to it bu utilizing a mask. Then it will convert image back to image space and update the ROI. 
Returns the updated ROI.

5. high_pass_fourier
This helper function will convert the passed ROI to frequency domain and then apply high pass filtering to it bu utilizing a mask. Then it will convert image back to image space and update the ROI. 
Returns the updated ROI.

6. low_pass_color_fourier
This helper function will convert the passed ROI to frequency domain and then apply low pass filtering to H,S, or V component of the color image by utilizing a mask. Then it will convert image back to image space and update the ROI. 
Returns the updated ROI.

7. high_pass_color_fourier
This helper function will convert the passed ROI to frequency domain and then apply high pass filtering to H,S, or V component of the color image by utilizing a mask. Then it will convert image back to image space and update the ROI. 
Returns the updated ROI.

8. unsharp_mask_compute
This helper function will perform edge sharpening by converting image to frequency domain and apply high pass filter and apply further processing. Then it will update pixels based on the formula high-frequency -> mask = multiplier else mask = 1
Returns the updated ROI. 

9. unsharp_mask_low_compute
This helper function will perform edge sharpening by converting image to frequency domain and apply low pass filter and apply further processing. Then it will update pixels based on the formula high-frequency -> mask = multiplier else mask = 1
Returns the updated ROI. 

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


