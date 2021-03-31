project(unit_tests DESCRIPTION "Tests for readmrz")


file(GLOB_RECURSE TEST_FILES_SRC CONFIGURE_DEPENDS
	src/*.c* src/*.h
)

add_executable(unit_tests ${TEST_FILES_SRC})

target_compile_options(unit_tests
 PRIVATE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
		-Werror -Wall -Wextra
	>
	$<$<CXX_COMPILER_ID:MSVC>:
		/W4
	>
)

target_include_directories(unit_tests
	PUBLIC
		${READMRZ_INCLUDES_DIR}
)

find_package(Catch2 CONFIG REQUIRED)

target_link_libraries(unit_tests
	PRIVATE
		${READMRZ_LIBNAME}
		Catch2::Catch2
)

file(GLOB TEST_FILES_IMAGES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/test_data/*
)

#copy command does not accept wildcards  - https://cmake.org/cmake/help/latest/manual/cmake.1.html#run-a-command-line-tool
add_custom_command(
	TARGET unit_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/test_data/"
        "${PROJECT_BINARY_DIR}/test_data/"

	COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${READMRZ_DIR}/tessdata/"
        "${PROJECT_BINARY_DIR}/tessdata/"
)

include(Catch)
