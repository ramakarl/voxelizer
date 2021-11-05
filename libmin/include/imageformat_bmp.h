
#ifdef BUILD_BMP

#ifndef DEF_IMAGEFORMAT_BMP
	#define	DEF_IMAGEFORMAT_BMP

	#include "imageformat.h"
	
	class HELPAPI CImageFormatBmp : public CImageFormat {
	public:		
		virtual bool Load (char *filename, Image* pImg );
		virtual bool Save (char *filename, Image* pImg );

		int readPaletteBMP ( FILE* fp, RGBQUAD*& palette, int bit_count );
	
	private:
		bool LoadBmp (char *filename);
		bool SaveBmp (char *filename);		
	};

#endif

#endif


