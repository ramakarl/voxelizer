//**************************************
//
// GENERIC Format
//
// NOTE: CURRENT CAPABILITIES:
//		- Uses OLE to load JPG and GIF foramts
//
//**************************************

#include <olectl.h>

#include "imageformat_generic.h"

//--------- MUST BE UPDATED
/*
bool CImageFormatGeneric::Load (char *filename, const Image* pOrigImg, Image*& pNewImg)
{
	m_pOrigImage = (Image*) pOrigImg;
	m_pNewImage = 0x0;
	m_eStatus = FormatStatus::Loading;
	strcpy (m_Filename, filename);	
	bool result = LoadGeneric ( filename );
	pNewImg = m_pNewImage;
	return result;	
}

bool CImageFormatGeneric::Save (char *filename, const Image* pOrigImg)
{
	m_pOrigImage = (Image*) pOrigImg;
	m_eStatus = FormatStatus::Saving;
	strcpy (m_Filename, filename);
	return SaveGeneric ( filename );
}

CImageFormatGeneric::CImageFormatGeneric ()
{
}

bool CImageFormatGeneric::LoadGeneric ( char *filename )
{
	IPicture* picture;
	IStream* stream;
	HGLOBAL hGlobal;
	FILE* file;

	// Open file to load
	file = fopen ( filename, "rb" ); // open file in read only mode
	if (file == NULL) {
		m_eStatus = FormatStatus::FileNotFound;
		return false;
	}

	// Allocates global memory same size as file to be loaded
	fseek ( file, 0, SEEK_END);
	int iFileSize = ftell ( file );
	fseek ( file, 0, SEEK_SET);
	hGlobal = GlobalAlloc(GPTR, iFileSize);			
	if ( hGlobal == NULL) {
		fclose ( file );
        m_eStatus = FormatStatus::InvalidFile;
		return false;
	}	
	// Read file data into global memory
	fread ( (void*) hGlobal, 1, iFileSize, file);
	fclose ( file );

	// Create a stream on global memory
	CreateStreamOnHGlobal( hGlobal, false, &stream);
	if ( stream == NULL ) {
		GlobalFree ( hGlobal );
		m_eStatus = FormatStatus::InvalidFile;
		return false;
	}

	// Decompress and load the JPG or GIF into Picture COM object
	OleLoadPicture( stream, 0, false, IID_IPicture, (void**)&picture );
	if ( picture == NULL ) { 
		stream->Release();
		GlobalFree( hGlobal );
		m_eStatus = FormatStatus::InvalidFile;
		return false;
	}

	// Release the stream 
	stream->Release();
	
	// Free the global memory
	GlobalFree( hGlobal );

	// Get a Windows bitmap from the Picture COM object
	HBITMAP hSrcBitmap = 0;
	picture->get_Handle( (unsigned int*)&hSrcBitmap );
	if ( hSrcBitmap == 0 ) {
		m_eStatus = FormatStatus::InvalidFile;
		return false;
	}

	// Copy Windows bitmap into a new one (why?)
	HBITMAP hDestBitmap = (HBITMAP) CopyImage ( hSrcBitmap, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
	if ( hDestBitmap == 0) {
		m_eStatus = FormatStatus::InvalidFile;
		return false;
	}

	// Success up to this point!
	// Release the Picture COM object
	picture->Release();

	// Transfer Windows bitmap into ImageX
	// (note: Format status will be set by TransferBitmap)
	bool bSuccess = TransferBitmap ( hDestBitmap );

	// Delete Windows bitmaps
	if ( hSrcBitmap ) DeleteObject ( hSrcBitmap );
	if ( hDestBitmap ) DeleteObject ( hDestBitmap );

	return bSuccess;
}

bool CImageFormatGeneric::SaveGeneric ( char *filename )
{
	// Not implemented.
	m_eStatus = FormatStatus::NotImplemented;
	return false;
}
*/
