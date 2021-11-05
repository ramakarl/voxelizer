
#ifdef BUILD_JPG

#ifndef DEF_IMAGEFORMAT_JPG
	#define	DEF_IMAGEFORMAT_JPG

	#include "imageformat.h"	

	#include "libjpg/jpeglib.h"
	#include "libjpg/jerror.h"
	#include <setjmp.h>

	struct extended_error_mgr {
		struct jpeg_error_mgr pub;
		jmp_buf setjmp_buffer;
	};

	typedef struct extended_error_mgr * extended_error_ptr;

	class HELPAPI CImageFormatJpg : public CImageFormat {
	public:		
		virtual bool Load (char *filename, Image* pImg );
		virtual bool Save (char *filename, Image* pImg );	
		virtual bool LoadIncremental ( char* filename, Image* pImg );
		virtual ImageOp::FormatStatus LoadNextRow ();

	private:
		bool LoadJpg (char *filename, bool bIncremental);
		bool SaveJpg (char *filename, int iQuality);		

		jpeg_compress_struct			m_jpeg_cinfo;
		jpeg_decompress_struct			m_jpeg_dinfo;
		extended_error_mgr				m_jerr;
		FILE*							m_jpeg_file;
		XBYTE*							m_pDest;	
	};

#endif
	
#endif