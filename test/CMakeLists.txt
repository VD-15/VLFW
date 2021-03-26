add_executable(VLFWTestDriver
	test.cpp)

target_include_directories(VLFWTestDriver
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(VLFWTestDriver
	PUBLIC
		VLFW
)
