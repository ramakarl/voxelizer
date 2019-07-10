cmake_minimum_required(VERSION 2.8)
set(PROJNAME Voxelizer)
Project(${PROJNAME})
Message(STATUS "-------------------------------")
Message(STATUS "Processing Project ${PROJNAME}:")

####################################################################################
# Bootstrap
#
set( BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
find_path ( HELPERS_PATH "Helpers.cmake" HINTS ${BASE_DIRECTORY}/../libraries/helpers ${BASE_DIRECTORY}/helpers ${CMAKE_HELPERS_PATH} )
if ( ${HELPERS_PATH} STREQUAL "HELPERS-NOTFOUND" )
    message ( FATAL_ERROR "\n Please set the CMAKE_HELPERS_PATH to for cmake helpers." )
endif()
include( ${HELPERS_PATH}/Helpers.cmake )  # Cross-Platform functions

#####################################################################################
# Sample requirements

set ( REQUIRE_JPG "0" )
set ( REQUIRE_PNG "0" )
set ( REQUIRE_TGA "1" )
set ( REQUIRE_VEC "1" )
set ( REQUIRE_CAM "1" )
set ( REQUIRE_NVGUI "1" )
set ( REQUIRE_GLEW "1" )
set ( REQUIRE_DIRECTORY "0" )
set ( REQUIRE_STRHELPER "0" )
set ( REQUIRE_MAIN "1" )

# add_definitions(-DBUILD_BMP)


####################################################################################
# Find Sample Utils
#
find_package(Utils)

#####################################################################################
# Require OpenGL
#
IF(WIN32)
  LIST(APPEND LIBRARIES_OPTIMIZED "opengl32.lib" )
  LIST(APPEND LIBRARIES_DEBUG "opengl32.lib" )
ENDIF()

#####################################################################################
# Source files for this project
#
file(GLOB SOURCE_FILES *.cpp *.c *.h )


########################################################################3
# Collect shaders for reference
file(GLOB GLSL_FILES ${ASSET_PATH}/*.glsl )

#####################################################################################
# Executable
#
unset ( ALL_SOURCE_FILES )
list( APPEND ALL_SOURCE_FILES ${SOURCE_FILES} )
list( APPEND ALL_SOURCE_FILES ${COMMON_SOURCE_FILES} )
list( APPEND ALL_SOURCE_FILES ${PACKAGE_SOURCE_FILES} )
list( APPEND ALL_SOURCE_FILES ${UTIL_SOURCE_FILES} )
list( APPEND ALL_SOURCE_FILES ${CUDA_FILES} )

if ( NOT DEFINED WIN32 )
  find_library(NVTOOLSEXT nvToolsExt HINTS ${CUDA_TOOLKIT_ROOT_DIR}/lib64)
  find_library(CUDART cudart HINTS ${CUDA_TOOLKIT_ROOT_DIR}/lib64)
  find_library(GVDBLIB gvdb HINTS ${GVDB_LIB_DIR} )
  set(libdeps GL GLEW X11 cuda ${GVDBLIB} ${NVTOOLSEXT} ${CUDART})
  LIST(APPEND LIBRARIES_OPTIMIZED ${libdeps})
  LIST(APPEND LIBRARIES_DEBUG ${libdeps})
ENDIF()

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")    
add_definitions(-DGVDB_IMPORTS -DGLEW_STATIC -DGLEW_NO_GLU)  
add_definitions(-DASSET_PATH="${ASSET_PATH}/")
add_executable (${PROJNAME} ${ALL_SOURCE_FILES} ${CUDA_FILES} ${PTX_SOURCES} ${GLSL_FILES} )
set_property ( TARGET ${PROJNAME} APPEND PROPERTY DEPENDS ${PTX_SOURCES} )


if ( MSVC  )
    set_target_properties( ${PROJNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} )
    set_target_properties( ${PROJNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_OUTPUT_PATH} )
    set_target_properties( ${PROJNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH} )    	
	source_group(CUDA FILES ${CUDA_FILES})
	source_group(PTX FILES  ${PTX_FILES})
	source_group(Helpers FILES ${UTIL_SOURCE_FILES})

	source_group(Core FILES ${SOURCE_FILES})
endif ()



#####################################################################################
# Library dependencies
#
set_property(GLOBAL PROPERTY DEBUG_CONFIGURATIONS Debug) 

foreach (loop_var IN ITEMS ${LIBRARIES_OPTIMIZED} )   
   target_link_libraries ( ${PROJNAME} optimized ${loop_var} )
endforeach()

foreach (loop_var IN ITEMS ${LIBRARIES_DEBUG} )
   target_link_libraries ( ${PROJNAME} debug ${loop_var} )
endforeach()

message ( STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}" )
message ( STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}" )
message ( STATUS "EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}" )


