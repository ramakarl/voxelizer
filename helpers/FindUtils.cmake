

#####################################################################################
# Find Sample Utils
#
unset( UTILS_FOUND CACHE)

message ( STATUS "--> Find Sample Utils") 

set ( SAMPLE_UTIL_DIR "${CMAKE_HELPERS_PATH}" CACHE PATH "Path to sample_utils" )

set ( OK_UTIL "0" )
_FIND_FILE ( UTIL_FILES SAMPLE_UTIL_DIR "nv_gui.h" "nv_gui.h" OK_UTIL )

if ( OK_UTIL EQUAL 1 )
    set ( UTILS_FOUND "YES" )    
	include_directories( ${SAMPLE_UTIL_DIR} )	
	
	# OpenGL always required
	if ( REQUIRE_OPENGL )
		find_package(OpenGL)		
		message ( STATUS " Using OpenGL")
	endif()

	#-------------------------------- Image formats
	if ( REQUIRE_PNG )				
	    # Add PNG to build
		message ( STATUS " Using PNG")
		add_definitions(-DBUILD_PNG)
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/file_png.cpp" )
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/file_png.h" )
	endif()
	if ( REQUIRE_TGA )
		# Add TGA to build
		message ( STATUS " Using TGA")
		add_definitions(-DBUILD_TGA)
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/file_tga.cpp" )	
	endif()
	if ( REQUIRE_JPG )
		message( STATUS " Using JPG") 
		add_definitions(-DBUILD_JPG)
		LIST(APPEND LIBRARIES_OPTIMIZED "${COMBINED_LIB_PATH}/libjpg_2015x64.lib" )
		LIST(APPEND LIBRARIES_DEBUG "${COMBINED_LIB_PATH}/libjpg_2015x64d.lib"  )	
	endif()
	#---------------------------------------- Graphics Libs	
	if ( REQUIRE_OPTIX )
		# Add OptiX to build
	    	# Collect all OptiX utility files
		add_definitions(-DUSE_OPTIX_UTILS)
		message ( STATUS " Using OPTIX_UTILS")
		FILE( GLOB UTIL_OPTIX_FILES "${SAMPLE_UTIL_DIR}/optix*.cpp" "${SAMPLE_UTIL_DIR}/optix*.hpp")
		FILE( GLOB UTIL_OPTIX_KERNELS "${SAMPLE_UTIL_DIR}/optix*.cu" "${SAMPLE_UTIL_DIR}/optix*.cuh")
	endif()
	if ( REQUIRE_GLEW )
		# Add GLEW to build
		message ( STATUS " Using GLEW")
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/glew.c" )	
	endif()
	if ( REQUIRE_NVGUI )
		# Add NVGUI to build
		message ( STATUS " Using NVGUI")
		add_definitions(-DUSE_NVGUI)
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/nv_gui.cpp" )	
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/nv_gui.h" )				
	endif()
	if ( REQUIRE_CAM )
		# Add Camera to build
		message ( STATUS " Using CAM")
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/camera.cpp" )	
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/camera.h" )				
	endif()
	if ( REQUIRE_VEC )
		# Add Vec to build
		message ( STATUS " Using VEC")
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/vec.cpp" )	
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/vec.h" )				
	endif()
	#---------------------------------------- Strings & Dirs
	if ( REQUIRE_STRHELPER )
		message ( STATUS " Using STR_HELPER")
		add_definitions(-DUSE_STR_HELPER)
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/string_helper.cpp" )	
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/string_helper.h" )		
	endif()
	if ( REQUIRE_DIRECTORY )
		message ( STATUS " Using DIRECTORY")
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/directory.cpp" )	
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/directory.h" )		
	endif()
	#---------------------------------------- Main
	if ( REQUIRE_MAIN )
		# Add Main to build
		IF(WIN32)
  			LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/main_win.cpp" )
	    ELSE()
	  		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/main_x11.cpp" )
		ENDIF()
		LIST( APPEND UTIL_SOURCE_FILES "${SAMPLE_UTIL_DIR}/main.h" )
	endif()

	message ( STATUS "--> Using Sample Utils. ${SAMPLE_UTIL_DIR}\n") 
else()
    set ( UTILS_FOUND "NO" )	
	message ( FATAL_ERROR "
Sample Utils not found. Please set the CMAKE_MODULE_PATH  \
and the SAMPLE_UTIL_DIR to the location of sample_utils path,  \
which contains nv_gui, file_png, file_tga, main_win, main_x11, \
and cmake helpers. \n
     ") 
endif()

mark_as_advanced( UTILS_FOUND )
