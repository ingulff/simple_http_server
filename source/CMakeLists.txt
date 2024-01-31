cmake_minimum_required(VERSION 3.20)

project(http_server
	VERSION 0.0.1
	DESCRIPTION "Simple http server"
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

set(DEFAULT_HOST_IP "127.0.0.1")
set(DEFAULT_HOST_PORT 1234)
set(DEFAULT_THREADS_NUMBER 4)

set(HOST_IP "${DEFAULT_HOST_IP}")
if(DEFINED TT_EXTERNAL_HOST_IP)
	set(HOST_IP "${TT_EXTERNAL_HOST_IP}")
endif()

set(HOST_PORT ${DEFAULT_HOST_PORT})
if(DEFINED TT_HOST_PORT)
	set(HOST_PORT ${TT_HOST_PORT})
endif()

set(THREADS_NUMBER ${DEFAULT_THREADS_COUNT})
if(DEFINED TT_THREADS_NUMBER)
	set(THREADS_NUMBER ${TT_THREADS_NUMBER})
endif()


# other options

#-----------------------------------------------------------------------------------------------------------------------------------------
# Dependencies
#-----------------------------------------------------------------------------------------------------------------------------------------

# other dependecies


#------------------------------------------------------------------------------------------------------------------------------------------
# Sources
#------------------------------------------------------------------------------------------------------------------------------------------

set(${EXECUTABLE_TARGET_NAME}_source
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/error/error.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/settings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/listener.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/http_server.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/http_session.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/utils/files_prepare.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/utils/filesystem_helpers.cpp


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
	PRIVATE
		HOST_IP="${HOST_IP}"
		HOST_PORT=${HOST_PORT}
		THREADS_NUMBER=${THREADS_NUMBER}
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
		${CMAKE_CURRENT_SOURCE_DIR}
		${BOOST_INCLUDE}
		${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(${EXECUTABLE_TARGET_NAME}
	PRIVATE
		OpenSSL::SSL
		OpenSSL::Crypto
)


set_target_properties(${EXECUTABLE_TARGET_NAME}
	PROPERTIES
		SOVERSION ${PROJECT_VERSION_MAJOR}
		VERSION ${PROJECT_VERSION}
)

