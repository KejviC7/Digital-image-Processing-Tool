#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>
#define DEFAULT 999999999

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
	
		static void scale(image &src, image &tgt, float ratio);
		static void add(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int value1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int value2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int value3=DEFAULT);
		static void binarize(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int threshhold1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int threshhold2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int threshhold3=DEFAULT);
		static void uniformsmooth(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int windowsize1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int windowssize2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int windowssize3=DEFAULT);
		static void adptvsmooth(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, int windowsize1, int threshhold1, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, int windowssize2=DEFAULT, int threshhold2=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, int windowssize3=DEFAULT, int threshhold3=DEFAULT);
		static void mulcolorbright(image &src, image &tgt, int roi, int x1, int y1, int Sx1, int Sy1, float DR, int x2=DEFAULT, int y2=DEFAULT, int Sx2=DEFAULT, int Sy2=DEFAULT, float DG=DEFAULT, int x3=DEFAULT, int y3=DEFAULT, int Sx3=DEFAULT, int Sy3=DEFAULT, float DB=DEFAULT);
};

#endif

