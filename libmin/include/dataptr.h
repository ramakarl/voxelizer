
#ifndef DEF_DATAPTR_H
	#define DEF_DATAPTR_H

	#include "common_defs.h"
	#include <assert.h>
	#include <vector>
	#include <string>		
	#include "vec.h"	
	#ifdef USE_CUDA
		#include "cuda.h"	
		#define PUSH_CTX		cuCtxPushCurrent(cuCtx);
		#define POP_CTX			CUcontext pctx; cuCtxPopCurrent(&pctx);
	#else
		#define PUSH_CTX
		#define POP_CTX		
	#endif

	#define DT_MISC			0
	#define DT_UCHAR		1		// 8-bit
	#define DT_USHORT		2		// 16-bit
	#define DT_UCHAR3		3		// 24-bit
	
	#define DT_UCHAR4		4		// 32-bit, 4 bytes, 4*sizeof(char)
	#define DT_INT			5		// 32-bit, 4 bytes, 1*sizeof(int32_t)	
	#define DT_UINT			6		// 32-bit, 4 bytes, 1*sizeof(uint32_t)	
	#define DT_FLOAT		7		// 32-bit, 4 bytes, 1*sizeof(float)

	#define DT_UINT64		8		//  64-bit,  8 bytes, 1*sizeof(uint64_t)
	#define DT_FLOAT3		12		//  96-bit, 12 bytes, 3*sizeof(float)
	#define DT_FLOAT4		16	    // 128-bit, 16 bytes, 4*sizeof(float)
	
	#define DT_CPU			1		// use flags
	#define DT_CUMEM		2
	#define DT_CUARRAY		4
	#define DT_GLTEX		8	
	#define DT_GLVBO		16

	class HELPAPI DataPtr {
	public:
		DataPtr() { mNum=0; mMax=0; mStride=0; mUseRes.Set(0,0,0); mUseType=DT_MISC; mUseFlags=DT_MISC; 
					mSize=0; mCpu=0; 
					#ifdef USE_CUDA
						mGpu=0; mGrsc=0; mGarray=0; mGLID=-1; mGtex = -1; mGsurf = -1; 
					#endif			
					}
		~DataPtr();

		void			Resize ( int stride, uint64_t cnt, char* dat=0x0, uchar dest_flags=DT_CPU );
		int				Append ( int stride, uint64_t cnt, char* dat=0x0, uchar dest_flags=DT_CPU );
		void			SetUsage ( uchar dt, uchar flags=DT_MISC, Vector3DI res = Vector3DI(-1,-1,-1) );		// special usage (2D,3D,GLtex,GLvbo,etc.)
		void			UpdateUsage ( uchar flags );		
		void			ReallocateCPU ( uint64_t oldsz, uint64_t newsz );
		void			FillBuffer ( uchar v );
		void			CopyTo ( DataPtr* dest, uchar dest_flags );
		void			Commit ();		
		void			Retrieve ();		
		void			Clear ();

		int				getStride ( uchar dtype );
		uint64_t		getDataSz ( int cnt, int stride )	{ return (uint64_t) cnt * stride; }

		int				getNum()	{ return mNum; }
		int				getMax()	{ return mMax; }
		char*			getData()	{ return mCpu; }
		#ifdef USE_CUDA
			CUdeviceptr		getGPU()	{ return mGpu; }		
		#endif
		char*			getPtr(int n)	{ return mCpu + n*mStride; }

	public:
		uint64_t		mNum, mMax, mSize;
		int				mStride;
		uchar			mRefID, mUseType, mUseFlags;	// usage
		Vector3DI		mUseRes;		
		bool			bCpu, bGpu;
		char*			mCpu;			
		
		int				mGLID;				// OpenGL

		#ifdef USE_CUDA
			CUdeviceptr		mGpu;			// CUDA
			CUgraphicsResource	mGrsc;		// CUDA-GL interop
			CUarray			mGarray;	
			CUtexObject		mGtex;			// CUDA texture/surface interop
			CUsurfObject	mGsurf;
		#endif

		static int		mFBO;
	};

#endif




