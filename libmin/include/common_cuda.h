
#ifdef USE_CUDA
#ifndef DEF_MAIN_CUDA
	#define DEF_MAIN_CUDA


	#include <cuda.h>    
	#include <cudaGL.h>

	#define DEV_FIRST		-1
	#define DEV_CURRENT		-2
	#define DEV_EXISTING	-3

	HELPAPI bool cuCheck(CUresult launch_stat, char* method, char* apicall, char* arg, bool bDebug);
	HELPAPI void cuStart(int devsel, CUcontext ctxsel, CUdevice& dev, CUcontext& ctx, CUstream* strm, bool verbose);
	HELPAPI void cuGetMemUsage(int& total, int& used, int& free);

#endif
#endif