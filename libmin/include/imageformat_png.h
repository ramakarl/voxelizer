

#ifndef DEF_IMAGEFORMAT_PNG
	#define	DEF_IMAGEFORMAT_PNG

	#include "imageformat.h"

    #define PNG_SKIP_SETJMP_CHECK
	#include "file_png.h"				// libpng. To install: apt-get install libpng12-dev

	class HELPAPI CImageFormatPng : public CImageFormat {
	public:		
		virtual bool Load (char *filename, Image* pImg );
		virtual bool Save (char *filename, Image* pImg );	

	private:
		bool LoadPng (char *filename );
		bool SavePng (char *filename );

		FILE*			m_png_file;
	};

#endif
	

