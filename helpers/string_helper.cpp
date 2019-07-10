//--------------------------------------------------------------------------------
// NVIDIA(R) GVDB VOXELS
// Copyright 2017, NVIDIA Corporation. 
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
//    in the documentation and/or  other materials provided with the distribution.
// 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived 
//    from this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
// SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// Version 1.0: Rama Hoetzlein, 5/1/2017
//----------------------------------------------------------------------------------

#include "vec.h"
#include "string_helper.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include <windows.h>		// needed for WideCharToMultiByte

int strToI (std::string s) {
	//return ::atoi ( s.c_str() );
	std::istringstream str_stream ( s ); 
	int x; 
	if (str_stream >> x) return x;		// this is the correct way to convert std::string to int, do not use atoi
	return 0;
};
float strToF (std::string s) {
	//return ::atof ( s.c_str() );
	std::istringstream str_stream ( s ); 
	float x; 
	if (str_stream >> x) return x;		// this is the correct way to convert std::string to float, do not use atof
	return 0;
};
double strToD (std::string s) {
	//return ::atof ( s.c_str() );
	std::istringstream str_stream ( s ); 
	double x; 
	if (str_stream >> x) return x;		// this is the correct way to convert std::string to float, do not use atof
	return 0;
};
unsigned char strToC ( std::string s ) {
	char c;
	memcpy ( &c, s.c_str(), 1 );		// cannot use atoi here. atoi only returns numbers for strings containing ascii numbers.
	return c;
};
// Return 4-byte long int whose bytes
// match the first 4 ASCII chars of string given.
unsigned long strToID ( std::string str )
{
	str = str + "    ";	
	return (static_cast<unsigned long>(str.at(0)) << 24) | 
		   (static_cast<unsigned long>(str.at(1)) << 16) |
		   (static_cast<unsigned long>(str.at(2)) << 8) |
		   (static_cast<unsigned long>(str.at(3)) );
}

/*Vector4DF strToV4 ( std::string val )
{
	Vector4DF v(0,0,0,0);
	size_t pos1 = val.find (',');
	size_t pos2 = val.find (',', pos1+1);
	size_t pos3 = val.find (',', pos2+1);

	if ( pos1 != std::string::npos && pos2 != std::string::npos && pos3 != std::string::npos ) {
		v.x = atof ( val.substr ( 0, pos1 ).c_str() );
		v.y = atof ( val.substr ( pos1+1, pos2-pos1 ).c_str() );
		v.z = atof ( val.substr ( pos2+1, pos3-pos2 ).c_str() );
		v.w = atof ( val.substr( pos3+1 ).c_str() );
	}
	return v;
}*/

objType strToType (	std::string str )
{
	char buf[5];
	strcpy ( buf, str.c_str() );
	objType name;
	((char*) &name)[3] = buf[0];
	((char*) &name)[2] = buf[1];
	((char*) &name)[1] = buf[2];
	((char*) &name)[0] = buf[3];
	return name;
}

std::string typeToStr ( objType name )			// static function
{
	char buf[5];	
	buf[0] = ((char*) &name)[3];
	buf[1] = ((char*) &name)[2];
	buf[2] = ((char*) &name)[1];
	buf[3] = ((char*) &name)[0];
	buf[4] = '\0';
	return std::string ( buf );
}


std::string cToStr ( char c )
{
	char buf[2];
	buf[0] = c;
	buf[1] = '\0';
	return std::string ( buf );
}

std::string iToStr ( int i )
{
	std::ostringstream ss;
	ss << i;
	return ss.str();
}
std::string fToStr ( float f )
{
	std::ostringstream ss;
	ss << f;
	return ss.str();
}


std::string strFilebase ( std::string str )
{
	size_t pos = str.find_last_of ( '.' );
	if ( pos != std::string::npos ) 
		return str.substr ( 0, pos );
	return str;
}
std::string strFilepath ( std::string str )
{
	size_t pos = str.find_last_of ( '\\' );
	if ( pos != std::string::npos ) 
		return str.substr ( 0, pos+1 );
	return str;
}

// Parse string inside two separators. Input becomes output exterior. Return is interior.
//   e.g. object<car> --> str: object, return: 'car'
std::string strParse ( std::string& str, std::string lsep, std::string rsep )
{
	std::string result;
	size_t lfound, rfound;

	lfound = str.find_first_of ( lsep );
	if ( lfound != std::string::npos) {
		rfound = str.find_first_of ( rsep, lfound+1 );
		if ( rfound != std::string::npos ) {
			result = str.substr ( lfound+1, rfound-lfound-1 );					// return string strickly between lsep and rsep
			str = str.substr ( 0, lfound ) + str.substr ( rfound+1 );
			return result;
		} 
	}
	return str;
}

std::string strParse ( std::string str, std::string lstr, std::string rstr, std::string lsep, std::string rsep )
{
	std::string result;
	size_t lfound, rfound;

	lfound = str.find_first_of ( lsep );
	if ( lfound != std::string::npos) {
		rfound = str.find_first_of ( rsep, lfound+1 );
		if ( rfound != std::string::npos ) {
			result = str.substr ( lfound+1, rfound-lfound-1 );					// return string strickly between lsep and rsep
			lstr = str.substr ( 0, lfound );
			rstr = str.substr ( rfound+1 );
			return result;
		} 
	}
	return "";
}


// Get string from inside two separators. Input is unchanged.
//  e.g. object<car> --> result: car
bool strGet ( std::string str, std::string& result, std::string lsep, std::string rsep )
{	
	size_t lfound, rfound;

	lfound = str.find_first_of ( lsep );
	if ( lfound != std::string::npos) {
		rfound = str.find_first_of ( rsep, lfound+1 );
		if ( rfound != std::string::npos ) {
			result = str.substr ( lfound+1, rfound-lfound-1 );					// return string strickly between lsep and rsep			
			return true;
		} 
	}
	return false;
}

// strGet - return a substring between two separators, without modifying input string.
// input:   strGet ( "hello(world)", "(", ")", result, pos )
// output:  result = "world", pos=7
bool strGet ( const std::string& str, std::string lsep, std::string rsep, std::string& result, size_t& pos )
{
	size_t lfound, rfound;
	lfound = str.find_first_of ( lsep );
	if ( lfound != std::string::npos ) {
		rfound = str.find_first_of ( rsep, lfound+1 );
		if ( rfound != std::string::npos ) {
			result = str.substr ( lfound, rfound-lfound+1 );
			pos = lfound;
			return true;
		}
	}
	return false;
}


// Get string split on separator. Input becomes output right side. Return is left side.
//   e.g. object:car --> str: car, result: object
std::string strSplit ( std::string& str, std::string sep )
{
	std::string result;
	size_t f1, f2;

	f1 = str.find_first_not_of ( sep );
	if ( f1 == std::string::npos ) f1 = 0;
	f2 = str.find_first_of ( sep, f1 );
	if ( f2 != std::string::npos) {
		result = str.substr ( f1, f2-f1 );
		str = str.substr ( f2+1 );		
	} else {
		result = str;		
		str = "";
	}
	return result;
}

bool strSplit ( std::string& str, std::string result, std::string sep )
{
	size_t found;
	found = str.find_first_of ( sep );
	if ( found == std::string::npos) 
		return false;

	str = str.substr ( found+1 );	
	result = str.substr ( 0, found );
	return true;
}

// Split a string into multiple words deliminated by sep
std::vector<std::string> strSplitMultiple ( std::string str, std::string sep )
{
	std::vector<std::string> words;
	size_t f1;
	f1 = str.find_first_of ( sep );
	
	while ( f1 != std::string::npos  ) {
		words.push_back ( str.substr(0,f1) );
		str = str.substr ( f1+1 );
		f1 = str.find_first_of ( sep );		
	}	
	return words;
}

bool strFileSplit ( std::string str, std::string& path, std::string& name, std::string& ext )
{	
	size_t slash = str.find_last_of ( "/\\" );
	if ( slash != std::string::npos ) {
		path = str.substr ( 0, slash );
		str = str.substr ( slash+1 );
	} else {
		path = "";		
	}
	size_t dot = str.find_last_of ( '.' );
	if ( dot != std::string::npos ) {
		name = str.substr ( 0, dot );
		ext = str.substr ( dot+1 );		
	} else {
		name = str;
		ext = "";
	}
	return true;
}

// Get string split on separator. Input becomes output left side. Return is right side.
std::string strSplitArg ( std::string& str, std::string sep )
{
	std::string result;
	size_t f1, f2;

	f1 = str.find_first_not_of ( sep );
	if ( f1 == std::string::npos ) f1 = 0;
	f2 = str.find_first_of ( sep, f1 );
	if ( f2 != std::string::npos) {
		result = str.substr ( f2+1 );		
		str = str.substr ( f1, f2-f1 );		
	} else {		
		result = "";		
	}
	return result;
}
// Parse a list as a set of key-value arguments. Input becomes output right side. Return is arg value. Tag is arg tag.
//   e.g. object:cat, name:felix -> str=name:felix, return=cat, tag=object
std::string strParseArg ( std::string& tag, std::string argsep, std::string sep, std::string& str )
{
	std::string result;
	size_t f1, f2, f3;
	f1 = str.find_first_not_of ( " \t" );
	if ( f1 == std::string::npos ) f1=0;
	f2 = str.find_first_of ( argsep, f1 );
	if ( f2 == std::string::npos ) f2=f1;
	f3 = str.find_first_of ( sep, f2 );
	if ( f3 != std::string::npos ) {
		tag = strTrim ( str.substr ( f1, f2-f1 ) );
		result = strTrim ( str.substr ( f2+1, f3-f2-1 ) );
		str = str.substr ( f3+1 );
	} else {
		tag = "";
		str = "";
		result = "";
	}
	return result;
}


std::string strParseFirst ( std::string& str, std::string sep, std::string others, char& ch )
{
	std::string result;
	size_t lfound, ofound;

	lfound = str.find_first_of ( sep );	
	ofound = str.find_first_of ( others );
	lfound = ( lfound==std::string::npos || ( ofound!=std::string::npos && ofound<lfound) ) ? ofound : lfound;

	if ( lfound != std::string::npos ) {
		ch = str.at( lfound );
		result = str.substr ( 0, lfound );
		str = str.substr ( lfound+1 );
		return result;
	} else {
		ch = '\n';
		result = str;
		str = "";
		return result;
	}
}

std::string strReplace ( std::string str, std::string target, std::string replace )
{
	size_t found = str.find ( target );
	while ( found != std::string::npos ) {
		str = str.substr ( 0, found ) + replace + str.substr ( found + target.length() );
		found = str.find ( target );
	}
	return str;
}

bool strReplace ( std::string& str, std::string src, std::string dest, int& cnt )
{
	cnt = 0;
	size_t pos = 0;
	
	pos = str.find ( src, pos );
	if ( pos != std::string::npos ) return false;	
	while ( pos != std::string::npos ) {
		str = str.substr(0,pos) + dest + str.substr(pos + src.length() );
		pos += dest.length() - src.length() + 1;
		cnt++;
		pos = str.find ( src, pos );
	}
	return true;
}

bool strSub ( std::string str, int first, int cnt, std::string cmp )
{
	if ( str.substr ( first, cnt ).compare ( cmp ) == 0 ) return true;
	return false;
}

int strCount ( std::string& str, char ch )
{
	size_t pos = 0;
	int cnt = 0;
	while ( (pos=str.find_first_of ( ch, pos )) != std::string::npos ) {
		cnt++; pos++;
	}
	return cnt;
}

bool strEmpty ( const std::string& s)
{
	if ( s.empty() ) return true;
	if ( s.length()==0 ) return true;
	size_t pos = s.find_first_not_of ( " \n\b\t" );
	if (  pos == std::string::npos ) return true;
	return false;
}



int strFindFromList ( std::string str, std::vector<std::string>& list, int& pos )
{
	size_t posL;
	for (int n=0; n < list.size(); n++ ) {
		posL = str.find ( list[n] );
		if ( posL != std::string::npos ) {
			pos = (int) posL;
			return n;
		}
	}
	return -1;		// not found
}


bool strIsNum ( std::string str, float& f )
{
	if (str.empty()) return false;
	std::string::iterator it;	
	char ch;
	for (it = str.begin(); it != str.end(); it++ ) {
		ch = (*it);
		if ( ch!='.' && ch!='-' && ch!='0' && ch!='1' && ch!='2' && ch!='3' && ch!='4' && ch!='5' && ch!='6' && ch!='7' && ch!='8' &&	ch!='9'  )
			 break;
	}
	if ( it==str.end() ) {
		f = atof ( str.c_str() );
		return true;
	}
	return false;
}

float strToNum ( std::string str )
{
	return (float) atof ( str.c_str() );
}
bool strToVec ( std::string& str, std::string lsep, std::string insep, std::string rsep, float* vec, int cpt )
{
	std::string val;	
	char sep = insep.at(0);
	int vc = 0;

	std::string vstr = strParse ( str, lsep, rsep );
	if ( vstr.empty() ) return false;
	
	for (int j=0; j < vstr.length(); j++ ) {		
		if ( vstr[j]==sep )  {
			vec[vc++] = strToNum(val);
			if ( vc >= cpt ) return true;
			val = "";
		} else {
			val += vstr[j];			
		}
	}	
	if ( !val.empty() ) vec[vc] = strToNum(val);
	return true;
}

bool strToVec3 ( std::string& str, std::string lsep, std::string insep, std::string rsep, float* vec )
{
	return strToVec ( str, lsep, insep, rsep, vec, 3 );	
}
bool strToVec4 ( std::string& str, std::string lsep, std::string insep, std::string rsep, float* vec )
{
	return strToVec ( str, lsep, insep, rsep, vec, 4 );	
}


std::string wsToStr ( const std::wstring& str )
{
#ifdef _MSC_VER
	int len = WideCharToMultiByte ( CP_ACP, 0, str.c_str(), (int) str.length(), 0, 0, 0, 0);
	char* buf = new char[ len+1 ];
	memset ( buf, '\0', len+1 );
	WideCharToMultiByte ( CP_ACP, 0, str.c_str(), (int) str.length(), buf, len+1, 0, 0);	
#else
    int len = wcstombs( NULL, str.c_str(), 0 );
	char* buf = new char[ len ];
	wcstombs( buf, str.c_str(), len );
#endif
	std::string r(buf);
	delete[] buf;
	return r;
}

std::wstring strToWs (const std::string& s)
{
	wchar_t* buf = new wchar_t[ s.length()+1 ];

#ifdef _MSC_VER
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, buf, (int) s.length()+1 );
#else
    mbstowcs( buf, s.c_str(), s.length() + 1 );
#endif
	std::wstring r(buf);
	delete[] buf;
	return r;
}


// trim from start
#include <algorithm>
#include <cctype>

/*std::string strLTrim(std::string str) {
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return str;
}

// trim from end
std::string strRTrim(std::string str) {
        str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), str.end());
        return str;
} */

// trim from both ends
std::string strTrim(std::string str)
{
	size_t lft = str.find_first_not_of ( " \t\r\n" );
	size_t rgt = str.find_last_not_of ( " \t\r\n" );
	if ( lft == std::string::npos || rgt == std::string::npos ) return "";
	return str.substr ( lft, rgt-lft+1 );
}

std::string strTrim ( std::string str, std::string ch )
{
	size_t found1 = str.find_first_not_of ( ch );
	size_t found2;
	if ( found1 != std::string::npos ) {
		str = str.substr ( found1 );	
		found2 = str.find_last_not_of ( ch );
		if ( found2 != std::string::npos ) 
			str = str.substr ( 0, found2+1 );
	} else {	
		found2 = str.find_last_not_of ( ch );
		if ( found2 != std::string::npos ) 
			str = str.substr ( 0, found2+1 );
		else
			str = ""; 		
	}
	return str;
}


std::string strLeft ( std::string str, int n )
{
	return str.substr ( 0, n );
}
std::string strRight ( std::string str, int n )
{
	if ( str.length() < n ) return "";
	return str.substr ( str.length()-n, n );
}

std::string strLeftOf ( std::string str, std::string sep )
{
	size_t f0 = str.find_first_of(sep);
	if ( f0 == std::string::npos ) return str;
	return str.substr(0, f0);
}
std::string strMidOf ( std::string str, std::string sep )
{
	size_t f0 = str.find_first_of(sep);
	size_t f1 = str.find_last_of(sep);
	if ( f0 == std::string::npos || f1==std::string::npos) return "";
	return str.substr(f0+1, f1-f0-1);
}
std::string strRightOf ( std::string str, std::string sep )
{
	size_t f0 = str.find_last_of(sep);
	if ( f0 == std::string::npos ) return "";
	return str.substr(f0+sep.length());
}

int strExtract ( std::string& str, std::vector<std::string>& list )
{
	size_t found ;
	for (int n=0; n < list.size(); n++) {
		found = str.find ( list[n] );
		if ( found != std::string::npos ) {
			str = str.substr ( 0, found ) + str.substr ( found + list[n].length() );
			return n;
		}
	}
	return -1;
}

unsigned long getFileSize ( char* fname )
{
	FILE* fp;
	fopen_s ( &fp, fname, "rb" );
	fseek ( fp, 0, SEEK_END );
	return ftell ( fp );
}
unsigned long getFilePos ( FILE* fp )
{
	return ftell ( fp );
}

bool getFileLocation ( std::string filename, std::string& outpath )
{
	char instr[2048];
	char outstr[2048];
	strncpy_s (instr, filename.c_str(), 2048 );
	bool result = getFileLocation ( instr, outstr );
	outpath = outstr;
	return result;
}

bool getFileLocation ( char* filename, char* outpath )
{
	std::vector<std::string> paths;
	paths.push_back ( "./");
	paths.push_back ( ASSET_PATH );
	bool result = getFileLocation ( filename, outpath, paths );
	return result;
}

bool getFileLocation ( char* filename, char* outpath, std::vector<std::string> searchPaths )
{
	bool found = false;
	FILE* fp = 0;
	fopen_s ( &fp, filename, "rb" );
	if (fp) {
		found = true;
		strcpy ( outpath, filename );		
	} else {
		for (int i=0; i < searchPaths.size(); i++) {			
			if (searchPaths[i].empty() ) continue;  
			sprintf ( outpath, "%s%s", searchPaths[i].c_str(), filename );
			fp = fopen( outpath, "rb" );
			if (fp)	{ found = true;	break; }
		}		
	}
	if ( found ) fclose ( fp );
	return found;
}



std::string readword ( char* line, char delim )
{
	char word_buf[8192];

	if ( readword ( line, delim, word_buf, 8192 ) ) return word_buf;

	return "";
}


bool readword ( char *line, char delim, char *word, int max_size )
{
	char *buf_pos;
	char *start_pos;	

	// read past spaces/tabs, or until end of line/string
	for (buf_pos=line; (*buf_pos==' ' || *buf_pos=='\t') && *buf_pos!='\n' && *buf_pos!='\0';)
		buf_pos++;
	
	// if end of line/string found, then no words found, return null
	if (*buf_pos=='\n' || *buf_pos=='\0') {*word = '\0'; return false;}

	// mark beginning of word, read until end of word
	for (start_pos = buf_pos; *buf_pos != delim && *buf_pos!='\t' && *buf_pos!='\n' && *buf_pos!='\0';)
		buf_pos++;
	
	if (*buf_pos=='\n' || *buf_pos=='\0') {	// buf_pos now points to the end of buffer
        strncpy_s (word, max_size, start_pos, max_size);	// copy word to output string
		if ( *buf_pos=='\n') *(word + strlen(word)-1) = '\0';
		*line = '\0';						// clear input buffer
	} else {
											// buf_pos now points to the delimiter after word
		*buf_pos++ = '\0';					// replace delimiter with end-of-word marker
		strncpy_s ( word, max_size, start_pos, (long long) (buf_pos-line) );	// copy word(s) string to output string			
											// move start_pos to beginning of entire buffer
		strcpy ( start_pos, buf_pos );		// copy remainder of buffer to beginning of buffer
	}
	return true;						// return word(s) copied	
}



