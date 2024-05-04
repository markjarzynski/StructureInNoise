project(AnomalyDetection)

message(STATUS "Configuring main project: " ${CMAKE_PROJECT_NAME})

#set(CMAKE_FIND_DEBUG_MODE 1)

set(PROJECT_VERSION_MAJOR  1)
set(PROJECT_VERSION_MINOR  0)
set(PROJECT_VERSION_PATCH  0)

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    CACHE INTERNAL "The project version.")

###############################################################################

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

set(CMAKE_BUILD_TYPE Release)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lfftw3 -lfftw3f -g -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lfftw3 -lfftw3f  -O3 -w -ffast-math -fomit-frame-pointer -fstrength-reduce")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

###############################################################################

cmake_minimum_required(VERSION 2.8.2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules/")
#set(CMAKE_VERBOSE_MAKEFILE ON)

###############################################################################

#find_package( CBLAS REQUIRED )
#find_package( LAPACKE REQUIRED )
find_package (TIFF REQUIRED)
find_package (JPEG REQUIRED)
find_package (PNG REQUIRED)
#find_package (Boost REQUIRED)
include_directories (PUBLIC ${TIFF_INCLUDE_DIR} PUBLIC ${JPEG_INCLUDE_DIR} PUBLIC ${PNG_INCLUDE_DIRS} PUBLIC ${FFTW_INCLUDES})

###############################################################################

find_path (FFTW_INCLUDES fftw3.h)
find_library (FFTWF_LIBRARIES NAMES fftw3f)
link_libraries (${TIFF_LIBRARIES} ${JPEG_LIBRARIES} ${PNG_LIBRARIES} ${FFTWF_LIBRARIES})

###############################################################################

FIND_PACKAGE( OpenMP REQUIRED )
if(OPENMP_FOUND)
#	message("-- OpenMP found.")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

###############################################################################

add_subdirectory(src/model)
add_subdirectory(src)
