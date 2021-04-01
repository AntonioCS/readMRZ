cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(ReadMRZ CXX)            

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
else()
  if (CMAKE_CXX_STANDARD VERSION_LESS 20)
    message(FATAL_ERROR "CMAKE_CXX_STANDARD of 20 or later required.")
  endif()
endif()
if (NOT DEFINED CMAKE_CXX_EXTENSIONS)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if (MSVC)
	add_definitions(-D_UNICODE -DUNICODE -DWIN32_LEAN_AND_MEAN -DNOMINMAX -DWINVER=0x0A00 -D_WIN32_WINNT=0x0A00)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
	add_definitions(-D_ATL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
endif()

option(READMRZ_ENABLE_TESTS "Build the tests" ON)

set(READMRZ_DIR ${PROJECT_SOURCE_DIR})
set(READMRZ_INCLUDES_DIR ${PROJECT_SOURCE_DIR}/include/)
set(READMRZ_SRC_DIR ${PROJECT_SOURCE_DIR}/src/)

file(
	GLOB_RECURSE PROJ_SRC_FILES
	CONFIGURE_DEPENDS
	${READMRZ_SRC_DIR}/*.cpp ${READMRZ_SRC_DIR}/*.h* ${READMRZ_INCLUDES_DIR}/readMRZ/*.h*
)

set(READMRZ_LIBNAME readMRZ)

add_library(${READMRZ_LIBNAME} STATIC
	${PROJ_SRC_FILES}
)

#target_compile_options(${OUTPUT_FILE}
# PRIVATE
#	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
#		-Werror -Wall -Wextra
#	>
#	$<$<CXX_COMPILER_ID:MSVC>:
#		/W4
#	>
#)

target_include_directories(
    ${READMRZ_LIBNAME} PUBLIC
    $<BUILD_INTERFACE:${READMRZ_INCLUDES_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}>
)

#to generate debug lib with d append to it
set_target_properties(${READMRZ_LIBNAME} 
	PROPERTIES 
	DEBUG_POSTFIX _d
)

find_package(spdlog CONFIG REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Tesseract CONFIG REQUIRED)
#find_package(Leptonica CONFIG REQUIRED)
    
target_link_libraries(${READMRZ_LIBNAME} PRIVATE spdlog::spdlog spdlog::spdlog_header_only)
target_link_libraries(${READMRZ_LIBNAME} PUBLIC opencv_core opencv_imgcodecs opencv_imgproc opencv_highgui)
target_link_libraries(${READMRZ_LIBNAME} PRIVATE libtesseract)
#target_link_libraries(${OUTPUT_FILE} PRIVATE leptonica)

#if (READMRZ_ENABLE_TESTS)
	add_subdirectory(tests)
#endif()