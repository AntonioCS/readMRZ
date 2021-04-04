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

if(READMRZ_ENABLE_TESTS)
	list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

set(READMRZ_DIR ${PROJECT_SOURCE_DIR})
set(READMRZ_INCLUDES_DIR ${PROJECT_SOURCE_DIR}/include/)
set(READMRZ_SRC_DIR ${PROJECT_SOURCE_DIR}/src/)

file(
	GLOB_RECURSE PROJ_SRC_FILES
	CONFIGURE_DEPENDS
	${READMRZ_SRC_DIR}/*.cpp ${READMRZ_SRC_DIR}/*.h* ${READMRZ_INCLUDES_DIR}/readMRZ/*.h*
)


add_library(readMRZ STATIC
	${PROJ_SRC_FILES}
)

target_compile_options(readMRZ
 PRIVATE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
		-Werror -Wall -Wextra
	>
	$<$<CXX_COMPILER_ID:MSVC>:
		/W4
	>
)

target_include_directories(readMRZ PUBLIC
    $<BUILD_INTERFACE:${READMRZ_INCLUDES_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}>
)

#to generate debug lib with d append to it
set_target_properties(readMRZ
	PROPERTIES 
	DEBUG_POSTFIX _d
)

find_package(OpenCV REQUIRED)
find_package(Tesseract CONFIG REQUIRED)
target_link_libraries(readMRZ PRIVATE opencv_core opencv_imgcodecs opencv_imgproc)
target_link_libraries(readMRZ PRIVATE libtesseract)

if (READMRZ_ENABLE_TESTS)
	add_subdirectory(tests)
endif()

add_subdirectory(cli)
