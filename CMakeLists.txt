cmake_minimum_required(VERSION 3.10)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In-source builds are not allowed, please conduct builds from within a separate build directory")
endif()

project(VLFW VERSION 0.1.0)

add_library(VLFW
    ${CMAKE_CURRENT_SOURCE_DIR}/src/VLFW.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window.cpp
)

set_target_properties(
	VLFW PROPERTIES
		SOVERSION ${VLFW_VERSION_MAJOR}
		VERSION ${VLFW_VERSION}
		CXX_STANDARD 14
		CXX_STANDARD_REQUIRED FALSE
)

target_include_directories(VLFW PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

#set(VLFW_REQUIRED_VLK_CORE_VERSION 0.0.0)
set(VLFW_REQUIRED_VLK_COMMON_VERSION 0.0.0)
set(VLFW_REQUIRED_GLFW_VERSION 3.3.0)

#if(NOT TARGET ValkyrieEngineCore)
#	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/ValkyrieEngineCore)
#elseif(ValkyrieEngineCore_VERSION VERSION_LESS VLFW_REQUIRED_VLK_CORE_VERSION)
#	message(FATAL_ERROR "Version mismatch: ValkyrieEngineCore. Found: ${ValkyrieEngineCore_VERSION}, must be at least: ${VLFW_REQUIRED_VLK_CORE_VERSION}")
#endif()

if(NOT TARGET ValkyrieEngineCommon)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/ValkyrieEngineCommon)
elseif(ValkyrieEngineCommon_VERSION VERSION_LESS VLFW_REQUIRED_VLK_COMMON_VERSION)
	message(FATAL_ERROR "Version mismatch: ValkyrieEngineCommon. Found: ${ValkyrieEngineCommon_VERSION}, must be at least: ${VLFW_REQUIRED_VLK_COMMON_VERSION}")
endif()

if(NOT TARGET glfw)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/GLFW)
elseif(glfw_VERSION VERSION_LESS VLFW_REQUIRED_GLFW_VERSION)
	message(FATAL_ERROR "Version mismatch: GLFW. Found: ${glfw_VERSION}, must be at least: ${VLFW_REQUIRED_GLFW_VERSION}")
endif()

target_link_libraries(VLFW
    PUBLIC
#		ValkyrieEngineCore
		ValkyrieEngineCommon
    PRIVATE
		glfw
)

if (${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
	if (BUILD_TESTING)
		enable_testing()
		#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/Catch2)
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
	endif()
endif()
