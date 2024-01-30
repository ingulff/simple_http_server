cmake_minimum_required(VERSION 3.20)

project(tests
	VERSION 0.0.1
	DESCRIPTION "Tests project"
	LANGUAGES C CXX
)

#-----------------------------------------------------------------------------------------------------------------------------------------
# General options
#-----------------------------------------------------------------------------------------------------------------------------------------

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRING "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(EXECUTABLE_TARGET_NAME ${PROJECT_NAME})
add_executable(${EXECUTABLE_TARGET_NAME})

set(DEFAULT_SOURCE_DIR "${CMAKE_SPURCE_DIR}/resource")

# other option

#-----------------------------------------------------------------------------------------------------------------------------------------
# Dependencies
#-----------------------------------------------------------------------------------------------------------------------------------------

# other option

#------------------------------------------------------------------------------------------------------------------------------------------
# Sources
#------------------------------------------------------------------------------------------------------------------------------------------

set(${EXECUTABLE_TARGET_NAME}_source
	${CMAKE_SOURCE_DIR}/source/http_session.cpp
	${CMAKE_SOURCE_DIR}/source/error.cpp
	${CMAKE_SOURCE_DIR}/tests/main.cpp
	${CMAKE_SOURCE_DIR}/tests/http_client.cpp
)

# other source

#------------------------------------------------------------------------------------------------------------------------------------------
# Target
#------------------------------------------------------------------------------------------------------------------------------------------

target_sources(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		${${EXECUTABLE_TARGET_NAME}_source}
)

target_compile_definitions(${EXECUTABLE_TARGET_NAME}
	PUBLIC
        _WIN32_WINNT=0x0501
        OPENSSL_NO_SSL2
)

set(COMPILE_OPTIONS)
if(MSVC)
	list(APPEND 
		COMPILE_OPTIONS 
		/std:c++17
		/Zc:__cplusplus
	)
else()
	list(APPEND
		COMPILE_OPTIONS
		-std=c++17
	)
endif()

target_compile_options(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		${COMPILE_OPTIONS}
)

# target_link_options()

target_include_directories(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		${CMAKE_SOURCE_DIR}/source
		${CMAKE_SOURCE_DIR}/tests
		${CMAKE_SOURCE_DIR}/tests/3rdparty
		${BOOST_INCLUDE}
		${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		OpenSSL::SSL
		OpenSSL::Crypto
)


set_target_properties(${EXXCUTABLE_TARGET_NAME}
	PROPERTIES
		SOVERSION ${PROJECT_VERSION_MAJOR}
		VERSION ${PROJECT_VERSION}
)

