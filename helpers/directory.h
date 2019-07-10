
#ifndef DEF_DIR_OBJECT
	#define DEF_DIR_OBJECT

	//#include "common_defs.h"
	#include <vector>

	#define FILE_TYPE_DIR 0
	#define FILE_TYPE_FILE 1

	// Hmmm - have to factor this specific stuff out.
	#define FILE_TYPE_BLOCKSAVE 2
	#define FILE_TYPE_BLOCKLEV 3

	typedef struct {
		int type;
		std::string text;
		std::string extension;
		int length;
	} dir_list_element;

	typedef std::vector< dir_list_element > dir_list;

	typedef struct {
		std::string text;
		float length;
	} text_element_t;

	typedef std::vector< text_element_t > elem_vec_t;


	class Directory {
	public:	
		Directory ();

		void LoadDir ( std::string path, std::string filter );
		int CreateDir ( std::string path );
		int getNumFiles ()					{ return (int) mList.size(); }
		dir_list_element getFile ( int n )	{ return mList[n]; }
		bool isDir ( int n )				{ return mList[n].type==0; }

		// Static functions
		static dir_list DirList( std::string path, std::string filter );
		static bool FileExists( std::string filename );
		static std::string ws2s(const std::wstring& s); 
		static std::wstring s2ws(const std::string& s);		
		static dir_list GetFileItems( dir_list input);
		static dir_list GetDirectoryItems( dir_list input);
		static std::string NormalizeSlashies( std::string path );
		static std::string GetExtension( std::string path );
		static std::string GetExecutablePath();
		static std::string GetCollapsedPath( std::string path );
		static std::string gPathDelim;

		std::string getPath ()		{ return mPath; }
		std::string getFilter ()	{ return mFileFilter; }

	private:

		std::string		mPath;
		std::string		mFileFilter;	
		

		dir_list		mList;

	};

#endif
