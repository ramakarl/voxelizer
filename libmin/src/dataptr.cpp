

#include "common_defs.h"
#include "dataptr.h"

#include <GL/glew.h>

#ifdef USE_CUDA
	#include "common_cuda.h"
#endif

int DataPtr::mFBO = -1;

DataPtr::~DataPtr()
{	
	// must be explicitly freed 
}

void DataPtr::Clear ()
{
	if ( mCpu != 0x0 ) free (mCpu);					// free cpu memory
	
	if ( mUseFlags & DT_GLTEX ) {				
		#ifdef USE_CUDA
			if ( mUseFlags & DT_CUMEM ) {				// release cuda-gl interop
				if ( mGrsc != 0x0 ) cuGraphicsUnregisterResource ( mGrsc );
				mGrsc=0; mGpu = 0;
			}
		#endif
		glDeleteTextures ( 1, (GLuint*) &mGLID );	// free from gl

	} else if ( mUseFlags & DT_GLVBO ) {
		#ifdef USE_CUDA
			if ( mUseFlags & DT_CUMEM ) {				// release cuda-gl interop
				if ( mGrsc != 0x0 ) cuGraphicsUnregisterResource ( mGrsc );
				mGrsc=0; mGpu = 0;
			}
		#endif	
		glDeleteBuffers ( 1, (GLuint*) &mGLID );	// free from gl

	} else if ( mUseFlags & DT_CUMEM ) {		// release cuda linear memory
		#ifdef USE_CUDA
			cuCheck ( cuMemFree (mGpu), "DataPtr:Clear", "cuMemFree", "", false );		
			mGpu = 0;
		#endif
	}
	mCpu = 0; 
	mGLID = -1;
	mNum = 0; mMax = 0; mSize = 0;	
}

int DataPtr::getStride ( uchar dtype )
{
	int bpp = dtype;
	switch (dtype) {
	case DT_UCHAR:	bpp = 1; break;
	case DT_UCHAR3:	bpp = 3; break;
	case DT_UCHAR4:	bpp = 4; break;
	case DT_USHORT: bpp = 2; break;
	case DT_UINT:	bpp = 4; break;
	case DT_FLOAT:	bpp = 4; break;
	case DT_FLOAT4:	bpp = 16; break;
	};
	return bpp;
}

void DataPtr::SetUsage ( uchar dt, uchar flags, Vector3DI res )
{
	mUseType = dt;
	if ( flags != DT_MISC ) mUseFlags = flags;
	if ( res.x != -1) mUseRes = res;	
}

void DataPtr::UpdateUsage ( uchar flags )
{
	mUseFlags |= flags;							// append usage, e.g. GPU
	SetUsage ( mUseType, mUseFlags );
	Append ( mStride, 0, 0x0, mUseFlags );		// reallocate on new usage
	Commit ();									// commit to new usage
}

void DataPtr::ReallocateCPU ( uint64_t oldsz, uint64_t newsz )
{
	if ( oldsz == newsz ) return;
	char* newdata = (char*) malloc ( newsz );
	if ( mCpu != 0x0 ) {
		memcpy ( newdata, mCpu, oldsz );	
		free ( mCpu );		
	}
	mCpu = newdata;
}

void DataPtr::Resize ( int stride, uint64_t cnt, char* dat, uchar dest_flags )
{
	Clear();
	Append ( stride, cnt, dat, dest_flags );
}

int DataPtr::Append ( int stride, uint64_t added_cnt, char* dat, uchar dest_flags )
{ 
	bool mbDebug = false;

	mStride = stride;

	// Update size
	DataPtr newdat;	
	uint64_t old_size = mSize;
	uint64_t added_size = getDataSz ( added_cnt, stride );	
	uint64_t new_size = old_size + added_size;
	newdat.mCpu = 0x0;
	newdat.mGLID = -1;
	#ifdef USE_CUDA
		newdat.mGpu = 0;
	#endif
	mMax += added_cnt;
	mSize = new_size;

	if ( new_size==0 ) return 0;

	// CPU allocation
	if ( dest_flags & DT_CPU ) {
		ReallocateCPU ( old_size, new_size );		
		if ( dat != 0x0 ) {
			memcpy ( mCpu + old_size, dat, added_size );
		}		
	}
	char* src = (dat!=0) ? dat : mCpu;

	// GPU allocation
	if ( dest_flags & DT_GLTEX ) {		
			// OpenGL Texture
			checkGL ( "start create DT_GLTEX" );
			if ( mGLID==-1 ) glGenTextures( 1, (GLuint*) &mGLID );
			checkGL ( "glGenTextures (DataPtr::Append)" );
			glBindTexture ( GL_TEXTURE_2D, mGLID );			
			glPixelStorei ( GL_PACK_ALIGNMENT, 4 );	
			glPixelStorei ( GL_UNPACK_ALIGNMENT, 4 );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);	

			checkGL ( "glBindTexture (DataPtr::Append)" );
			switch (mUseType) {
			case DT_UCHAR:	glTexImage2D ( GL_TEXTURE_2D, 0, GL_R8,		mUseRes.x, mUseRes.y, 0, GL_RED,	GL_UNSIGNED_BYTE, src );	break;			
			case DT_UCHAR3:	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA8,	mUseRes.x, mUseRes.y, 0, GL_RGB,	GL_UNSIGNED_BYTE, src );	break; // <-- NOTE: GL_RGBA8 (4 chan) as GL_RGB8 (3 chan) not supported by CUDA interop
			case DT_UCHAR4:	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA8,	mUseRes.x, mUseRes.y, 0, GL_RGBA,	GL_UNSIGNED_BYTE, src );	break;
			case DT_USHORT: glTexImage2D ( GL_TEXTURE_2D, 0, GL_R32F,   mUseRes.x, mUseRes.y, 0, GL_RED,	GL_UNSIGNED_SHORT,src );	break;
			case DT_INT:	glTexImage2D ( GL_TEXTURE_2D, 0, GL_R32F,   mUseRes.x, mUseRes.y, 0, GL_RED,	GL_UNSIGNED_INT,  src );	break;
			case DT_FLOAT:	glTexImage2D ( GL_TEXTURE_2D, 0, GL_R32F,	mUseRes.x, mUseRes.y, 0, GL_RED,	GL_FLOAT, src);				break;
			case DT_FLOAT4:	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA32F, mUseRes.x, mUseRes.y, 0, GL_RGBA,	GL_FLOAT, src);				break;
			};			
			checkGL ( "glTexImage2D (DataPtr::Append)" );		
			
			#ifdef USE_CUDA

			// CUDA-GL interop
			if ( dest_flags & DT_CUMEM ) {
				dbgprintf ( "ERROR: CUDA does not allow GL texture interop with linear memory. Must use DT_CUARRAY.\n" );
				exit(-3);
			}
			if ( dest_flags & DT_CUARRAY ) {
				// Get array from OpenGL ID
				cuCheck ( cuCtxSynchronize(), "sync", "", "", false );

				if ( mGrsc != 0 ) cuCheck ( cuGraphicsUnregisterResource ( mGrsc ), "DataPtr::Append", "cuGraphicsUnregisterResrc", "", false );
				cuCheck ( cuGraphicsGLRegisterImage( &mGrsc, (GLuint) mGLID, GL_TEXTURE_2D, CU_GRAPHICS_MAP_RESOURCE_FLAGS_NONE ), "DataPtr::Append", "cuGraphicsRegisterImage", "", false );
				cuCheck ( cuGraphicsMapResources(1, &mGrsc, 0), "DataPtr::Append", "cuGraphicsMapResources", "", false);
				cuCheck ( cuGraphicsSubResourceGetMappedArray( &mGarray, mGrsc, 0, 0), "DataPtr::Append", "cuGraphicsSubResourceGetMappedArray", "", false );				

				// CUDA texture/surface interop - resource descripion
				CUDA_RESOURCE_DESC resDesc;
				memset ( &resDesc, 0, sizeof(resDesc) );
				resDesc.resType = CU_RESOURCE_TYPE_ARRAY;
				resDesc.res.array.hArray = mGarray;			// <---- CUDA 2D array
				resDesc.flags = 0;		

				CUDA_TEXTURE_DESC texDesc;
				memset ( &texDesc, 0, sizeof(texDesc) );					
				texDesc.filterMode = CU_TR_FILTER_MODE_LINEAR;		// filter mode 				
				// texDesc.filterMode = CU_TR_FILTER_MODE_POINT;				
				switch ( mUseType ) {								// pixel format
				case DT_FLOAT: case DT_FLOAT3: case DT_FLOAT4: case DT_USHORT: case DT_INT: texDesc.flags = 0;	break;	// float 2D array					
				case DT_UCHAR: case DT_UCHAR3: case DT_UCHAR4:		texDesc.flags = CU_TRSF_READ_AS_INTEGER; break;		// int 2D array				
				};								
				texDesc.addressMode[0] = CU_TR_ADDRESS_MODE_CLAMP;	// border mode
				texDesc.addressMode[1] = CU_TR_ADDRESS_MODE_CLAMP;
				texDesc.addressMode[2] = CU_TR_ADDRESS_MODE_CLAMP;				

				if ( mGtex != -1 ) cuCheck ( cuTexObjectDestroy ( mGtex ), "DataPtr::Append", "cuTexObjectDestroy", "", false );
				if ( mGsurf != -1) cuCheck ( cuSurfObjectDestroy ( mGsurf ), "DataPtr::Append", "cuSurfObjectDestroy", "", false );
				cuCheck ( cuTexObjectCreate ( &mGtex, &resDesc, &texDesc, NULL ), "DataPtr::Append", "cuTexObjectCreate", "", false );
				cuCheck ( cuSurfObjectCreate ( &mGsurf, &resDesc ), "DataPtr::Append", "cuSurfObjectCreate", "", false );	

				cuCheck ( cuGraphicsUnmapResources (1, &mGrsc, 0), "DataPtr::Append", "cuGraphicsUnmapResrc", "", false );
			}
			#endif		

	} else if ( dest_flags & DT_GLVBO ) {
			// OpenGL VBO
			if ( mGLID == -1 ) glGenBuffers ( 1, (GLuint*) &mGLID );               // GLID = VBO
			glBindBuffer ( GL_ARRAY_BUFFER, mGLID );						
			glBufferData ( GL_ARRAY_BUFFER, new_size, src, GL_STATIC_DRAW );

			#ifdef USE_CUDA
			// CUDA-GL interop
			if ( dest_flags & DT_CUMEM ) {
				if ( mGrsc != 0 ) cuCheck ( cuGraphicsUnregisterResource ( mGrsc ), "DataPtr::Append", "cuGraphicsUnregisterResource", "", false );
				cuCheck ( cuGraphicsGLRegisterBuffer ( &mGrsc, mGLID, CU_GRAPHICS_REGISTER_FLAGS_NONE ), "DataPtr::Append", "cuGraphicsGLReg", "", false );
				cuCheck ( cuGraphicsMapResources(1, &mGrsc, 0), "", "cuGraphicsMapResrc", "", false );
				size_t sz = 0;
				cuCheck ( cuGraphicsResourceGetMappedPointer ( &mGpu, &sz, mGrsc ),  "", "cuGraphicsResrcGetMappedPtr", "", false );	
				cuCheck ( cuGraphicsUnmapResources (1, &mGrsc, 0), "DataPtr::Append", "cuGraphicsUnmapResrc", "", false );
			}
			#endif
	
	} else if ( dest_flags & DT_CUARRAY ) {
			// CUarray using CUDA		

	} else if ( dest_flags & DT_CUMEM ) {	
			#ifdef USE_CUDA
			// CUDA Linear Memory			
			cuCheck ( cuMemAlloc ( &newdat.mGpu, new_size ), "DataPtr::Append", "cuMemAlloc", "", false );
			if ( dat != 0x0 ) {
				cuCheck ( cuMemcpyHtoD ( newdat.mGpu + old_size, src, added_size), "DataPtr::Append", "cuMemcpyHtoD", "", false);
			}
			if ( mGpu != 0x0 ) 
				cuCheck ( cuMemFree ( mGpu ), "DataPtr::Append", "cuMemFree", "", false);			
			mGpu = newdat.mGpu;
			#endif
	}
	return mSize / mStride;
}

void DataPtr::Commit ()
{
	int sz = mNum * mStride;		// only copy in-use elements
	if ( sz==0 ) return;

	if ( mUseFlags & DT_GLTEX ) {
			//-- need to save x for this to work
			glBindTexture ( GL_TEXTURE_2D, mGLID );				
			switch (mUseType) {
			case DT_UCHAR:	glTexImage2D ( GL_TEXTURE_2D, 0, GL_R8,		mUseRes.x, mUseRes.y, 0, GL_RED,	GL_UNSIGNED_BYTE, mCpu );	break;
			case DT_USHORT: glTexImage2D ( GL_TEXTURE_2D, 0, GL_R16F,   mUseRes.x, mUseRes.y, 0, GL_RED,	GL_UNSIGNED_SHORT,mCpu );	break;
			case DT_INT:	glTexImage2D ( GL_TEXTURE_2D, 0, GL_R32F,   mUseRes.x, mUseRes.y, 0, GL_RED,	GL_UNSIGNED_INT,  mCpu );	break;
			case DT_UCHAR3:	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA8,	mUseRes.x, mUseRes.y, 0, GL_RGB,	GL_UNSIGNED_BYTE, mCpu );	break;
			case DT_UCHAR4:	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA8,	mUseRes.x, mUseRes.y, 0, GL_RGBA,	GL_UNSIGNED_BYTE, mCpu );	break;
			case DT_FLOAT:	glTexImage2D ( GL_TEXTURE_2D, 0, GL_R32F,	mUseRes.x, mUseRes.y, 0, GL_RED,	GL_FLOAT, mCpu);			break;
			case DT_FLOAT4:	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA32F, mUseRes.x, mUseRes.y, 0, GL_RGBA,	GL_FLOAT, mCpu);		break;
			}; 
	} else if ( mUseFlags & DT_GLVBO ) {
		#ifdef USE_CUDA
		if ( mUseFlags & DT_CUMEM ) {
			// CUDA-GL Interop
			cuCheck ( cuMemcpyHtoD ( mGpu, mCpu, sz), "DataPtr::Commit", "cuMemcpyHtoD", "", false );
			return;
		} 		
		#endif
		// OpenGL VBO 			
		glBindBuffer ( GL_ARRAY_BUFFER, mGLID );						
		glBufferData ( GL_ARRAY_BUFFER, sz, mCpu, GL_STATIC_DRAW );		

	} else if ( mUseFlags & DT_CUMEM ) {
		#ifdef USE_CUDA		
		cuCheck ( cuMemcpyHtoD ( mGpu, mCpu, sz), "DataPtr::Commit", "cuMemcpyHtoD", "", false );	// CUDA Linear memory		
		#endif
	} 
}


void DataPtr::Retrieve ()
{
	if ( mCpu == 0 ) {		
		ReallocateCPU ( 0, mSize );				// ensure space exists on cpu
	}

	if ( mUseFlags & DT_GLTEX ) {

		int w, h;
		glBindTexture(GL_TEXTURE_2D, mGLID );		
		glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w );
		glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h ); 

		if (mFBO==-1) glGenFramebuffers (1, (GLuint*) &mFBO);
		glBindFramebuffer ( GL_FRAMEBUFFER, mFBO);
		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGLID, 0);
		GLenum status = glCheckFramebufferStatus (GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) 	{
			dbgprintf ( "ERROR: Binding frame buffer for texture read pixels.\n" );
			return;
		}	

		switch (mUseType) {
		case DT_UCHAR:	glReadPixels(0, 0, w, h, GL_RED, GL_UNSIGNED_BYTE, mCpu );		break;
		case DT_UCHAR3:	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, mCpu );		break;
		case DT_UCHAR4:	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, mCpu );		break;
		case DT_USHORT: glReadPixels(0, 0, w, h, GL_RED, GL_UNSIGNED_SHORT, mCpu );		break;
		case DT_FLOAT:	glReadPixels(0, 0, w, h, GL_RED,  GL_FLOAT, mCpu );				break;
		case DT_FLOAT4:	glReadPixels(0, 0, w, h, GL_RGBA, GL_FLOAT, mCpu );				break;
		}; 

		// Flip Y
		/*int pitch = w * 3;
		unsigned char* buf = (unsigned char*)malloc(pitch);
		for (int y = 0; y < h / 2; y++) {
			memcpy(buf, pixbuf + (y * pitch), pitch);
			memcpy(pixbuf + (y * pitch), pixbuf + ((h - y - 1) * pitch), pitch);
			memcpy(pixbuf + ((h - y - 1) * pitch), buf, pitch);
		}*/

	} else if ( mUseFlags & DT_GLVBO ) {
		#ifdef USE_CUDA			
		if ( mUseFlags & DT_CUMEM ) cuCheck ( cuMemcpyDtoH ( mCpu, mGpu, mSize ), "DataPtr::Retrieve", "cuMemcpyDtoH", "DT_GLVBO", false );			// cuda-interop
		#endif		
	} else if ( mUseFlags & DT_CUMEM ) {
		#ifdef USE_CUDA			
		cuCheck ( cuMemcpyDtoH ( mCpu, mGpu, mSize ), "DataPtr::Retrieve", "cuMemcpyDtoH", "DT_CUMEM", false );		
		#endif
	}
}

void DataPtr::CopyTo ( DataPtr* dest, uchar dest_flags )
{
	if ( mSize != dest->mSize ) {
		dbgprintf ( "ERROR: CopyTo sizes don't match.\n" );
		exit(-11);
	}	
	if ( (mUseFlags & DT_CPU) && (dest_flags & DT_CPU) )
		memcpy ( dest->mCpu, mCpu, mSize );

	if ( (mUseFlags & DT_CUMEM) && (dest_flags & DT_CUMEM) ) {
		#ifdef USE_CUDA			
		cuCheck ( cuMemcpyDtoD ( dest->mGpu, mGpu, mSize), "DataPtr::CopyTo", "cuMemcpyDtoD", "", false );		// also covers cuda-interop cases
		#endif
	}

	//if ( (mUseFlags & DT_GLVBO) && (dest_flags & DT_GLVBO) )
}

void DataPtr::FillBuffer ( uchar v )
{
	if ( mUseFlags & DT_CPU )
		memset ( mCpu, v, mSize );
	
	if ( mUseFlags & DT_CUMEM ) {
		#ifdef USE_CUDA			
		cuCheck ( cuMemsetD8 ( mGpu, v, mSize), "FillBuffer", "cuMemsetD8", "", false );		// also covers cuda-interop cases
		#endif
	}

}


