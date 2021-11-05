cmake_minimum_required(VERSION 2.8)

set(PROJNAME libmin)
Project(${PROJNAME})

Message("-------------------------------")
Message("PROJECT NAME: ${PROJNAME}")

#####################################################################################
# Bootstrap
#
set( BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
find_path ( CMAKE_HELPERS_PATH "HelpersBootstrap.cmake" HINTS ${BASE_DIRECTORY}/cmake/ )
if ( ${CMAKE_HELPERS_PATH} STREQUAL "HELPERS-NOTFOUND" )
    message ( FATAL_ERROR "\n Please set the CMAKE_HELPERS_PATH to location of HelpersBootstrap.cmake" )
endif()
include( ${CMAKE_HELPERS_PATH}/HelpersBootstrap.cmake )		# Cross-Platform functions
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/src")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include")

#####################################################################################
#
# LIBMIN - Options
#
add_definitions ( -DLIBHELP_EXPORTS) 

add_definitions ( -DGLEW_STATIC) 

OPTION (BUILD_OPENGL "Build with OpenGL" ON)
if (BUILD_OPENGL)
   add_definitions(-DUSE_OPENGL)  		# Use OpenGL

   IF (WIN32)
     LIST(APPEND LIBRARIES_OPTIMIZED "opengl32.lib" )
     LIST(APPEND LIBRARIES_DEBUG "opengl32.lib" )
   ENDIF()

endif()

#####################################################################################
# Source files for this project
#
file(GLOB SOURCE_FILES src/*.cpp src/*.c)
file(GLOB INCLUDE_FILES include/*.hpp include/*.h)

#####################################################################################
# Library paths
#
# CMAKE_INSTALL_PREFIX -- path where library will be installed to

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   if ( WIN32 )
      get_filename_component ( _instpath "${CMAKE_CURRENT_BINARY_DIR}" REALPATH )
   else()
      get_filename_component ( _instpath "/usr/local/libhelp" REALPATH )
   endif()
   set ( CMAKE_INSTALL_PREFIX ${_instpath} CACHE PATH "default install path" FORCE)   
endif()

get_filename_component( LIB_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} REALPATH)
get_filename_component( INSTALL_BIN_PATH ${CMAKE_INSTALL_PREFIX}/bin REALPATH)
get_filename_component( INSTALL_INC_PATH ${CMAKE_INSTALL_PREFIX}/include REALPATH)
get_filename_component( SHARE_PATH ${CMAKE_INSTALL_PREFIX}/lib REALPATH)

set ( EXECUTABLE_OUTPUT_PATH ${INSTALL_BIN_PATH} CACHE PATH "" FORCE )


#####################################################################################
# Library output
#
unset ( ALL_SOURCE_FILES )
list( APPEND ALL_SOURCE_FILES ${SOURCE_FILES} )
list( APPEND ALL_SOURCE_FILES ${INCLUDE_FILES} )
list( APPEND ALL_SOURCE_FILES ${PACKAGE_SOURCE_FILES} )

# Definitions
add_definitions(-DLIBHELP_EXPORTS)	# Export dll symbols

# Set the library type
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" ON)
set (LIB_TYPE STATIC)
if (BUILD_SHARED_LIBS)
  set (LIB_TYPE SHARED)
endif()

if( WIN32 AND NOT GLUT_FOUND)
  add_definitions(/wd4267) #remove size_t to int warning
  add_definitions(/wd4996) #remove printf warning
  add_definitions(/wd4244) #remove double to float conversion warning
  add_definitions(/wd4305) #remove double to float truncation warning
  add_library (${PROJNAME} ${LIB_TYPE} ${ALL_SOURCE_FILES} ${CUDA_FILES} )
else()
  add_library (${PROJNAME} ${LIB_TYPE} ${ALL_SOURCE_FILES} ${CUDA_FILES} ${PTX_FILES} )
endif()

# debug and relase libs
set ( CMAKE_DEBUG_POSTFIX "d" CACHE STRING "" )
set_target_properties( ${PROJNAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

set_target_properties( ${PROJNAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIB_OUTPUT_PATH} )
set_target_properties( ${PROJNAME} PROPERTIES VS_INTERMEDIATE_DIRECTORY_DEBUG ${LIB_OUTPUT_PATH}/Debug )
set_target_properties( ${PROJNAME} PROPERTIES VS_INTERMEDIATE_DIRECTORY_RELEASE ${LIB_OUTPUT_PATH}/Release )
set_target_properties( ${PROJNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${LIB_OUTPUT_PATH} )
set_target_properties( ${PROJNAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

message ( STATUS "Output:   ${LIB_OUTPUT_PATH}" )
message ( STATUS "Installed Bin: ${INSTALL_BIN_PATH}" )
message ( STATUS "Installed Inc: ${INSTALL_INC_PATH}" )

#####################################################################################
# Linkage
#
_LINK ( PROJECT ${PROJNAME} OPT ${LIBRARIES_OPTIMIZED} DEBUG ${LIBRARIES_DEBUG} PLATFORM ${PLATFORM_LIBRARIES} )

################################################################
# Windows specific
#
if ( WIN32 )    
	# instruct CMake to automatically build INSTALL project in Visual Studio 
	set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)
	# visual studio source groups
	source_group(Include FILES 
		${INCLUDE_FILES}  
	)
	source_group(Source FILES 
		${SOURCE_FILES}  
	)
endif()

################################################################
# Install Binaries
#
file (COPY "${CMAKE_CURRENT_SOURCE_DIR}/include/GL" DESTINATION ${INSTALL_INC_PATH} )
file (COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/glew.c" DESTINATION ${INSTALL_INC_PATH} )
install ( FILES ${INCLUDE_FILES} DESTINATION ${INSTALL_INC_PATH} )

install ( FILES ${INSTALL_LIST} DESTINATION ${INSTALL_BIN_PATH} )
install ( FILES $<TARGET_PDB_FILE:${PROJNAME}> DESTINATION ${INSTALL_BIN_PATH} OPTIONAL )
install ( FILES $<TARGET_LINKER_FILE:${PROJNAME}> DESTINATION ${INSTALL_BIN_PATH} )
_INSTALL ( FILES ${PACKAGE_DLLS} DESTINATION ${INSTALL_BIN_PATH} )
install ( TARGETS ${PROJNAME} DESTINATION ${INSTALL_BIN_PATH} )

#################################
# Done
message ( STATUS "\nLIBMIN INSTALL PATH: ${CMAKE_INSTALL_PREFIX}" )
message ( STATUS "LIBMIN will be installed to this path post-build (win32) or during make install (linux)")


