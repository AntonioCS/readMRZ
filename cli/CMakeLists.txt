project(cli DESCRIPTION "Cli tool to use lib")


file(GLOB_RECURSE CLI_FILES_SRC CONFIGURE_DEPENDS
	src/*.c* src/*.h
)

add_executable(readMRZcli ${CLI_FILES_SRC})

target_compile_options(readMRZcli
 PRIVATE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
		-Werror -Wall -Wextra
	>
	$<$<CXX_COMPILER_ID:MSVC>:
		/W4
	>
)

target_include_directories(readMRZcli
	PUBLIC
		${READMRZ_INCLUDES_DIR}
)

target_link_libraries(readMRZcli
	PRIVATE
		readMRZ
)

add_custom_command(
	TARGET readMRZcli POST_BUILD

	COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${READMRZ_DIR}/tessdata/"
        "${PROJECT_BINARY_DIR}/tessdata/"
)
