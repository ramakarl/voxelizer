//--------------------------------------------------------------------------------
// NVIDIA(R) GVDB VOXELS
// Copyright 2017, NVIDIA Corporation
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright notice, this 
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this 
//    list of conditions and the following disclaimer in the documentation and/or 
//    other materials provided with the distribution.
// 3. Neither the name of the copyright holder nor the names of its contributors may 
//    be used to endorse or promote products derived from this software without specific 
//   prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
// SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
// OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// Version 1.0: Rama Hoetzlein, 5/1/2017
//----------------------------------------------------------------------------------

#ifndef DEF_STRING_HELPER
	#define DEF_STRING_HELPER

	#include <string>
	#include <vector>

	#ifndef DEF_OBJTYPE
		typedef	uint32_t		objType;
	#endif
	std::string strFilebase ( std::string str );	// basename of a file (minus ext)
	std::string strFilepath ( std::string str );	// path of a file

	// convert
	int strToI (std::string s);
	float strToF (std::string s);
	double strToD (std::string s);
	unsigned char strToC ( std::string s );	
	unsigned long strToID ( std::string str );	
	bool strToVec ( std::string& str, std::string lsep, std::string insep, std::string rsep, float* vec, int cpt=3);	
	bool strToVec3 ( std::string& str, std::string lsep, std::string insep, std::string rsep, float* vec );	
	bool strToVec4 ( std::string& str, std::string lsep, std::string insep, std::string rsep, float* vec );	
	std::string cToStr ( char c );
	std::string iToStr ( int i );
	std::string fToStr ( float f );	
	objType strToType ( std::string str );
	std::string typeToStr ( objType t );
	std::string wsToStr ( const std::wstring& str );
	std::wstring strToWs (const std::string& s);	

	// parse
	std::string strParse ( std::string& str, std::string lsep, std::string rsep );
	std::string strParse ( std::string str, std::string lstr, std::string rstr, std::string lsep, std::string rsep );
	std::string strParseArg ( std::string& tag, std::string valsep, std::string sep, std::string& str );
	std::string strParseFirst ( std::string& str, std::string sep, std::string others, char& ch );
	bool strGet ( std::string str, std::string& result, std::string lsep, std::string rsep );	
	bool strGet ( const std::string& s, std::string lsep, std::string rsep, std::string& result, size_t& pos );
	std::string strSplit ( std::string& str, std::string sep );
	bool		strSplit ( std::string& str, std::string dest, std::string sep );	
	std::string strSplitArg ( std::string& str, std::string sep );
	bool strSub ( std::string str, int first, int cnt, std::string cmp );	
	std::string strReplace ( std::string str, std::string src, std::string dest );
	bool strReplace ( std::string& str, std::string src, std::string dest, int& cnt );
	int strExtract ( std::string& str, std::vector<std::string>& list );
	int strFindFromList ( std::string str, std::vector<std::string>& list, int& pos );
	bool strEmpty ( const std::string& s);
	std::vector<std::string> strSplitMultiple ( std::string str, std::string sep );
	bool strFileSplit ( std::string str, std::string& path, std::string& name, std::string& ext );

	// trim
	std::string strLTrim ( std::string str );
	std::string strRTrim ( std::string str );
	std::string strTrim ( std::string str );
	std::string strTrim ( std::string str, std::string ch );
	std::string strLeft ( std::string str, int n );
	std::string strRight ( std::string str, int n );	
	std::string strLeftOf ( std::string str, std::string sep );
	std::string strMidOf ( std::string str, std::string sep );
	std::string strRightOf ( std::string str, std::string sep );	
	
	// alphanumeric
	bool strIsNum ( std::string str, float& f );	
	float strToNum ( std::string str );	
	int strCount ( std::string& str, char ch );		

	bool readword ( char *line, char delim, char *word, int max_size  );
	std::string readword ( char *line, char delim );

	// file helpers	
	unsigned long getFileSize ( char* fname );
	unsigned long getFilePos ( FILE* fp );
	bool getFileLocation ( std::string filename, std::string& outpath );
	bool getFileLocation ( char* filename, char* outpath );
	bool getFileLocation ( char* filename, char* outpath, std::vector<std::string> paths );

	#define MAKECLR(r,g,b,a)	( (uint(a*255.0f)<<24) | (uint(b*255.0f)<<16) | (uint(g*255.0f)<<8) | uint(r*255.0f) )
	#define ALPH(c)			(float((c>>24) & 0xFF)/255.0)
	#define BLUE(c)			(float((c>>16) & 0xFF)/255.0)
	#define GRN(c)			(float((c>>8)  & 0xFF)/255.0)
	#define RED(c)			(float( c      & 0xFF)/255.0)

#endif
