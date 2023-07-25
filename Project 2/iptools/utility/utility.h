#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>
#define DEFAULT 999999999
#define DEFAULT_CHAR 'K'
class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static int checkChannel(char channel);
		static void rgbtohsi(image &src, image &conversion_image);
		static void hsitorgb(image &conversion_image, image &tgt);
		static void findMinMax(int histogram[256][2], int &C, int &D);
		static void initializeMinMax(int histogram[256][2], int &C, int &D);
		static void visualizeHistogram(int histogram[256][2]);
		static void visualizeHistogramHorizontally(int histogram[256][2]);



		static void scale(image &src, image &tgt, float ratio);
		static void Histostretch(int histogram[256][2], string name);
		static void althistostretch(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int A1, int B1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int A2=DEFAULT, int B2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int A3=DEFAULT, int B3=DEFAULT);
		static void histothres(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int threshold1, int A1, int B1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int threshold2=DEFAULT, int A2=DEFAULT, int B2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int threshold3=DEFAULT, int A3=DEFAULT, int B3=DEFAULT);
		static void percchastrech(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, char channel1, int A1, int B1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, char channel2=DEFAULT_CHAR, int A2=DEFAULT, int B2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, char channel3=DEFAULT_CHAR, int A3=DEFAULT, int B3=DEFAULT);
		static void rgbstretch(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, char channel1, int A1, int B1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, char channel2=DEFAULT_CHAR, int A2=DEFAULT, int B2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, char channel3=DEFAULT_CHAR, int A3=DEFAULT, int B3=DEFAULT);
		static void istretch(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, char channel1, int A1, int B1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, char channel2=DEFAULT_CHAR, int A2=DEFAULT, int B2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, char channel3=DEFAULT_CHAR, int A3=DEFAULT, int B3=DEFAULT);
		static void hstretch(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, char channel1, int A1, int B1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, char channel2=DEFAULT_CHAR, int A2=DEFAULT, int B2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, char channel3=DEFAULT_CHAR, int A3=DEFAULT, int B3=DEFAULT);
		static void sstretch(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, char channel1, int A1, int B1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, char channel2=DEFAULT_CHAR, int A2=DEFAULT, int B2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, char channel3=DEFAULT_CHAR, int A3=DEFAULT, int B3=DEFAULT);
		static void fullhsistretch(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int A1_H, int B1_H, int A1_S, int B1_S, int A1_I, int B1_I, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int A2_H=DEFAULT, int B2_H=DEFAULT, int A2_S=DEFAULT, int B2_S=DEFAULT, int A2_I=DEFAULT, int B2_I=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int A3_H=DEFAULT, int B3_H=DEFAULT, int A3_S=DEFAULT, int B3_S=DEFAULT, int A3_I=DEFAULT, int B3_I=DEFAULT);
};

#endif

