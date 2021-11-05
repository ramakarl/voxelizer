
#include "image.h"

#include <assert.h>


void Image::SetPixelBW32 (int x, int y, XBYTE r, XBYTE g, XBYTE b, XBYTE a)
{
	if ( x>=0 && y>=0 && x < getInfo()->mXres && y < getInfo()->mYres ) {
		XBYTE4* pix = (XBYTE4*) (GetData() + ( (y * getInfo()->mXres + x) * getInfo()->GetBytesPerPix() ));
		*pix = r;
		// setNotify ( 1 ); // *** NOTIFY(1)
	}	
}
void Image::GetPixelBW32 (int x, int y, XBYTE& r, XBYTE& g, XBYTE& b, XBYTE& a )
{
	if ( x>=0 && y>=0 && x < getInfo()->mXres && y < getInfo()->mYres ) {
		XBYTE* pix = (XBYTE*) GetData() + ( (y * getInfo()->mXres + x) * getInfo()->GetBytesPerPix() );
        r = *(pix+3);
        g = *(pix+2);
		b = *(pix+1);
		a = *pix;
	}
}

void Image::FillBW32 (XBYTE r, XBYTE g, XBYTE b, XBYTE a)
{
	// Temporary fill buffer
	assert ( getInfo()->GetBytesPerRow() <= 16384 );
	for (uint x=0; x < getInfo()->GetBytesPerRow();)
		fillbuf[x++] = r; 		
    
    XBYTE2 *dest_pix, *dest_pix_stop;
	dest_pix = (XBYTE2*) GetData();
	dest_pix_stop = dest_pix + getInfo()->GetSize();
	for (; dest_pix < dest_pix_stop;) {
		memcpy (dest_pix, fillbuf, getInfo()->GetBytesPerRow() );
		dest_pix += getInfo()->GetBytesPerRow();
	}	
	// setNotify ( 1 ); // *** NOTIFY(1)
}

void Image::RemapBW32 ( unsigned int vmin, unsigned int vmax )
{
	XBYTE4* src = (XBYTE4*) GetData();
	XBYTE4* src_stop = src + (getInfo()->mXres*getInfo()->mYres);	
	
	unsigned long mMin, mMax;
	mMin = ((unsigned long) 1 << 16)-1;
	mMax = 0;
	for (; src < src_stop; ) {
		if ( *src < mMin ) mMin = *src;
		if ( *src > mMax ) mMax = *src;
		src++;
	}
	if ( vmin == vmax ) return;
	
	src = (XBYTE4*) GetData();
	unsigned int vdelta = vmax-vmin;
	for (; src < src_stop; ) {
		*src = (XBYTE4) ( float(vmin) + float(*src - mMin)*vdelta / (mMax-mMin) );
		src++;
	}
}


// Reformat - Reformats given pixel format to
// another pixel format
void Image::ReformatBW32 ( ImageOp::Format eFormat )
{
	Image* new_img = new Image ( getInfo()->mXres, getInfo()->mYres, eFormat );
	
	XBYTE4* src = (XBYTE4*) GetData();
	XBYTE4* src_stop = src + (getInfo()->mXres*getInfo()->mYres);	
	
	switch ( eFormat ) {
	case ImageOp::BW16: {
		XBYTE2* dest = (XBYTE2*) new_img->GetData ();		
		for (; src < src_stop; ) {
			*dest++ = (XBYTE2) *src++;			
		}		
		} break;
	case ImageOp::RGB24: {
		XBYTE* dest = (XBYTE*) new_img->GetData ();		
		for (; src < src_stop; ) {
			*dest++ = (XBYTE) *src;
			*dest++ = (XBYTE) *src;
			*dest++ = (XBYTE) *src++;			
		}		
		} break;
	case ImageOp::BGR24: {
		XBYTE* dest = (XBYTE*) new_img->GetData ();		
		for (; src < src_stop; ) {
			*dest++ = (XBYTE) *src;
			*dest++ = (XBYTE) *src;
			*dest++ = (XBYTE) *src++;
		}	
		} break;
	case ImageOp::RGBA32: {
		XBYTE* dest = (XBYTE*) new_img->GetData ();
		for (; src < src_stop; ) {
			*dest++ = (XBYTE) *src;
			*dest++ = (XBYTE) *src;
			*dest++ = (XBYTE) *src++;
			*dest++ = 255;
		}
		} break;
	};
	TransferFrom ( new_img );
	delete ( new_img );
}

// Paste - Pastes a portion of an image into another image.
// Subselection: yes	- Allows sub-selection of source
// Cropping:     yes	- Allows cropping into target
// Offsetting:   yes	- Allows offsetting into target
// Scaling:      no		- Allows rescaling of source
// Filtering:    no		- Allows filtering of source
// Rotation:	 no		- Allows rotation of source
void Image::PasteBW32 ( int x1, int y1, int x2, int y2, int offx, int offy, XBYTE* dest, ImageOp::Format dest_format, int destx, int desty )
{
	XBYTE *src, *src_start, *src_end;
	XBYTE *dest_start, *dest_end, *dest_row;
	int src_wid, src_pitch;
	int dest_wid, dest_pitch, dest_bpr;

	if ( getInfo()->QueryPaste ( GetFormat(), GetWidth(), GetHeight(), GetData(), x1, y1, x2, y2,
						  dest_format, destx, desty, dest, offx, offy,
						  src_start, src_end, src_wid, src_pitch,
						  dest_start, dest_end, dest_wid, dest_pitch) ) {
		if ( dest_format==ImageOp::BW8 ) {
			dest_bpr = getInfo()->GetBytesPerRow();
			for (src = src_start, dest = dest_start; dest < dest_end;) {
				memcpy (dest, src, src_wid);		
				src += GetBytesPerRow ();
				dest += dest_bpr;
			}
		} else if ( dest_format==ImageOp::RGB24 ) {	
			dest_bpr = getInfo()->GetBytesPerRow();
			for (src = src_start, dest = dest_start, dest_row = dest_start+dest_wid; dest < dest_end;) {
				for ( ; dest < dest_row; ) {
					*dest++ = *src;
					*dest++ = *src;					
					*dest++ = *src++;
				}
				dest_row += dest_bpr;
				dest += dest_pitch;
				src += src_pitch;
			}
		} else if ( dest_format==ImageOp::RGBA32 ) {
			dest_bpr = getInfo()->GetBytesPerRow();
			for (src = src_start, dest = dest_start, dest_row = dest_start+dest_wid; dest < dest_end;) {
				for ( ; dest < dest_row; ) {
					*dest++ = *src;
					*dest++ = *src;					
					*dest++ = *src++;
					*dest++ = 255;
				}
				dest_row += dest_bpr;
				dest += dest_pitch;
				src += src_pitch;
			}
		}
	}	
}

// Scale - Rescales an image into another image
// Subselection: no		- Allows sub-selection of source
// Cropping:     no		- Allows cropping into target
// Offsetting:   no		- Allows offsetting into target
// Scaling:      yes	- Allows rescaling of source
// Filtering:    yes	- Allows filtering of source
// Rotation:	 no		- Allows rotation of source
// Limitations: Rescaled size must match target buffer

#define BPX		1				// bytes per pixel (fast define)

void Image::ScaleBW32 ( XBYTE* dest, ImageOp::Format dest_format, int destx, int desty )
{
	assert ( GetFormat()==dest_format );

	// Limitation: Rescaled size must match target buffer
	int nx = destx;
	int ny = desty;

	// Source addressing
	XBYTE* src;												// Current pixel in source
	XBYTE* src_row;											// Current row in source (start of row)	

	// Destination addressing
	int dest_bpr = GetBytesPerRow(destx,desty,dest_format);
	XBYTE* dest_rowend = dest + dest_bpr;						// End of row in dest
	XBYTE* dest_end = dest + GetSize(destx,desty,dest_format);	// End of entire image in dest
	double delta_x = double(getInfo()->mXres-2) / double(nx);	// Filtering distances
	double delta_y = double(getInfo()->mYres-2) / double(ny);
	int iDiffX = int(delta_x*0.5);							// Filtering deltas (in bytes of source data)
	int iDiffY = getInfo()->GetBytesPerRow();
	float sx, sy;											// Current pixel (with sub-pixel accuracy)

	sy = 1.0;	
	for (; dest < dest_end;) {
		src_row = (XBYTE*) GetData() + int(sy) * getInfo()->GetBytesPerRow();
		sx = 1.0;
		for (; dest < dest_rowend;) {
			src = src_row + (int) sx;
			// Filtering (Fast, poor quality but better than nothing)
			// BW
			*dest = *(src - iDiffX) >> 3;				// left
			*dest += *(src) >> 1;						// center
			*dest += *(src+iDiffX) >> 3;				// right
			*dest += *(src-iDiffY) >> 3;				// up
			*dest++ += *(src+iDiffY) >> 3;				// down				
			sx += (float) delta_x;
		}
		sy += (float) delta_y;				
		dest_rowend += dest_bpr;
	}	
}

// Alpha Paste - Copies alpha from another source
void Image::AlphaBW32 ( int x1, int y1, int x2, int y2, XBYTE* src, ImageOp::Format src_format, int src_x, int src_y )
{
	XBYTE *src_start, *src_end;
	XBYTE *dest, *dest_start, *dest_end, *dest_row;
	int src_wid, src_pitch;
	int dest_wid, dest_pitch, dest_bpr;

	// Source is the alpha image (any format)
	// Dest is 'this' image (RGBA32)
	if ( getInfo()->QueryPaste ( src_format, src_x, src_y, src, x1, y1, x2, y2,
		  GetFormat(), GetWidth(), GetHeight(), GetData(), 0, 0,
		  src_start, src_end, src_wid, src_pitch,
		  dest_start, dest_end, dest_wid, dest_pitch) ) {
		if ( src_format==ImageOp::BW8 ) {
			dest_bpr = getInfo()->GetBytesPerRow();
			for (src = src_start, dest = dest_start; dest < dest_end;) {
				memcpy (dest, src, src_wid);		
				src += GetBytesPerRow ();
				dest += dest_bpr;
			}
		} else if ( src_format==ImageOp::RGB24 ) {
			dest_bpr = getInfo()->GetBytesPerRow();
			for (src = src_start, dest = dest_start, dest_row = dest_start+dest_wid; dest < dest_end;) {
				for ( ; dest < dest_row; ) {
					*dest++ = *src;
					src += 3;					
				}
				dest_row += dest_bpr;
				dest += dest_pitch;
				src += src_pitch;
			}
		} else if ( src_format==ImageOp::RGBA32 ) {
			dest_bpr = getInfo()->GetBytesPerRow();
			for (src = src_start+3, dest = dest_start, dest_row = dest_start+dest_wid; dest < dest_end;) {
				for ( ; dest < dest_row; ) {
					*dest++ = *src;
					src += 4;
				}
				dest_row += dest_bpr;
				dest += dest_pitch;
				src += src_pitch;
			}
		}	
	}
}
