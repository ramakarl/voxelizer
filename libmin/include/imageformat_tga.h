
#ifndef DEF_IMAGEFORMAT_TGA
	#define	DEF_IMAGEFORMAT_TGA

	#include "imageformat.h"

	class HELPAPI CImageFormatTga : public CImageFormat {
	public:		
		virtual bool Load (char *filename, Image* pImg );
		virtual bool Save (char *filename, Image* pImg );
	
	private:
		unsigned char *getRGBA(FILE *s, unsigned char* rgba, int size);
		unsigned char *getRGB(FILE *s, unsigned char* rgb, int size);
		unsigned char *getGray(FILE *s, unsigned char* grayData, int size);
		void           writeRGBA(FILE *s, const unsigned char *externalImage, int size);
		void           writeRGB(FILE *s, const unsigned char *externalImage, int size);
		void           writeGrayAsRGB(FILE *s, const unsigned char *externalImage, int size);
		void           writeGray(FILE *s, const unsigned char *externalImage, int size);

		bool LoadTga (char *filename);
		//bool SaveTga (char *filename);		
	};

#endif


