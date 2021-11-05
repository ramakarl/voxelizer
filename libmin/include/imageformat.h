
#ifndef DEF_IMAGEFORMAT
	#define	DEF_IMAGEFORMAT

	#include "image.h"
	#include "image_info.h"
	#include <stdio.h>
	#include <windows.h>
	#include <conio.h>

	#define FILE_NAMELEN	512

	class HELPAPI CImageFormat {
	public:
		CImageFormat ();
		~CImageFormat ();

		virtual bool Save (char *filename, Image* pImg) {return false;}		
		virtual bool Load (char *filename, Image* pImg) {return false;}		
		virtual bool LoadIncremental ( const char* filename, Image* pImg) { return false; }
		virtual ImageOp::FormatStatus LoadNextRow () { return ImageOp::LoadNotReady; }		

#if defined(BUILD_VC)
		bool TransferBitmap ( HBITMAP hBmp );
#endif
		// Helper functions		
		int GetWidth ( Image* img )				{ return img->GetWidth();  }
		int GetHeight ( Image* img )				{ return img->GetHeight(); }
		ImageOp::Format GetFormat ( Image* img )	{ return img->GetFormat(); }
		int GetBitsPerPixel ( Image* img )		{ return img->GetBitsPerPixel ();}
		int GetBytesPerPixel ( Image* img )		{ return img->GetBytesPerPixel ();}
		int GetBytesPerRow ( Image* img )			{ return img->GetBytesPerRow ();}
		uchar* GetData ( Image* img )				{ return img->GetData  (); }
		void CreateImage ( Image*& img, int xr, int yr, ImageOp::Format fm );

		ImageOp::FormatStatus GetStatus ()	{ return m_eStatus; }
		void GetStatusMsg (char* buf);

		void StartLoad ( char* filename, Image* pImg );		
		void FinishLoad ();

	protected:
		Image*				m_pOrigImage;			// Original image. (ImageFormat does not own it)
		Image*				m_pNewImage;
		char				m_Filename[FILE_NAMELEN];
		ImageOp::FormatStatus 	m_eStatus;		

		// General format data
		int					m_Xres, m_Yres;		
		int					m_BytesPerRow;
		int					m_BitsPerPixel;
	};


#endif


