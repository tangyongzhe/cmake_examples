cmake_minimum_required(VERSION 3.15)
project(transmit
        LANGUAGES CXX
        VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Debug")

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORTS_CXX17)
#if(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
#else()
#    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
#endif()

set(ROOT_DIR ${CMAKE_HOME_DIRECTORY})
if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Debug")
else()
endif()

SET(build_type ${CMAKE_BUILD_TYPE})
message("build_type:${build_type}")

SET(CURRENT_SYSTME_NAME ${CMAKE_SYSTEM_NAME})
SET(CURRENT_SYSTME_NAME1 ${CMAKE_SYSTEM_NAME})
SET(CURRENT_SYSTME_NAME2 ${CMAKE_SYSTEM_NAME})

set(zlib_lib_name )
set(protobuf_lib_name )
set(jsoncpp_lib_name )
set(openssl_crypto_name )
set(openssl_ssl_name )
set(sqlite3_lib_name )
set(yaml-cpp_lib_name )

# 设置输出目录
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	message(STATUS "operation system is ${CMAKE_SYSTEM_NAME}")
	
	string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)

    if(CMAKE_BUILD_TYPE MATCHES "Release")
		MESSAGE("build release !!!!!!!!!!!!!!!!!!!!!!! ")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -ggdb")
	else()
		MESSAGE("build debug !!!!!!!!!!!!!!!!!!!!!!! ")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -ggdb ")
		#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer -ggdb")
	endif()

	SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_PATH}/bin/linux/${build_type})
	IF (BUILD_SHARED_LIBS)
		SET(LIBRARY_OUTPUT_PATH ${PROJECT_PATH}/lib/linux/${build_type})
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_PATH}/bin/linux/${build_type})
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_PATH}/lib/linux/${build_type})
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_PATH}/lib/linux/${build_type})
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_PATH}/bin/linux/${build_type})
	ELSE(BUILD_SHARED_LIBS)
		SET(LIBRARY_OUTPUT_PATH ${PROJECT_PATH}/lib/linux/${build_type})
	ENDIF(BUILD_SHARED_LIBS)

    set(zlib_lib_name libz.a)
    set(protobuf_lib_name libprotobuf.a)
    set(jsoncpp_lib_name jsoncpp_static.a)
    set(openssl_crypto_name libcrypto_static.a)
    set(openssl_ssl_name libssl_static.a)
    set(sqlite3_lib_name sqlite3.a)
    set(yaml-cpp_lib_name yaml-cpp.a)

ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE("operation system is ${CMAKE_SYSTEM_NAME}")
    
    SET(CURRENT_SYSTME_NAME1 "win64")
    SET(CURRENT_SYSTME_NAME2 "win")

    if(CMAKE_BUILD_TYPE MATCHES "Release")
		MESSAGE("build release !!!!!!!!!!!!!!!!!!!!!!! ")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od")
        set(protobuf_lib_name libprotobuf.lib)
        set(zlib_lib_name zlibstatic.lib)
        set(yaml-cpp_lib_name yaml-cpp.lib)
	else()
		MESSAGE("build debug !!!!!!!!!!!!!!!!!!!!!!! ")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od")
		#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer -ggdb")
        set(protobuf_lib_name libprotobufd.lib)
        set(zlib_lib_name zlibstaticd.lib)
        set(yaml-cpp_lib_name yaml-cppd.lib)
	endif()

    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_PATH}/bin/Windows)
	IF (BUILD_SHARED_LIBS)
		SET(LIBRARY_OUTPUT_PATH ${PROJECT_PATH}/lib/Windows)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_PATH}/bin/Windows)
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_PATH}/lib/Windows)
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_PATH}/lib/Windows)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_PATH}/bin/Windows)
	ELSE(BUILD_SHARED_LIBS)
		SET(LIBRARY_OUTPUT_PATH ${PROJECT_PATH}/lib/Windows)
	ENDIF(BUILD_SHARED_LIBS)

    set(jsoncpp_lib_name jsoncpp_static.lib)
    set(openssl_crypto_name libcrypto_static.lib)
    set(openssl_ssl_name libssl_static.lib)
    set(sqlite3_lib_name sqlite3.lib)
    
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
	MESSAGE("operation system is mac")
ELSE()
	MESSAGE("operation system unkonw ")
ENDIF()

# 2. 设置第三方库搜索路径
set(DEPEND_ROOT ${CMAKE_HOME_DIRECTORY}/../../Dependencies CACHE PATH "Third-party libraries root directory")
get_filename_component(DEPEND_ROOT "${DEPEND_ROOT}" ABSOLUTE)

set(PROTOBUF_PROTOC_EXECUTABLE ${DEPEND_ROOT}/protobuf/protobuf-3.20.3/bin/${CURRENT_SYSTME_NAME}/protoc.exe)
set(GRPC_CPP_PLUGIN_EXECUTABLE ${DEPEND_ROOT}/grpc/v1.51.3/bin/${CURRENT_SYSTME_NAME}/grpc_cpp_plugin.exe)

set(PROTO_BAT ${CMAKE_HOME_DIRECTORY}/generated_protoc.bat)

#Protobuf
find_path(Protobuf_INCLUDE_DIR NAMES google/protobuf/message.h
    PATHS ${CMAKE_PREFIX_PATH} 
    "${DEPEND_ROOT}/protobuf/protobuf-3.20.3/src" 
    PATH_SUFFIXES Protobuf)

find_library(Protobuf_LIBRARY
    NAMES ${protobuf_lib_name}
    PATHS ${CMAKE_PREFIX_PATH} 
        "${DEPEND_ROOT}/protobuf/protobuf-3.20.3/lib/${CURRENT_SYSTME_NAME}/${build_type}" 
    )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Protobuf
REQUIRED_VARS Protobuf_LIBRARY Protobuf_INCLUDE_DIR
)
cmake_policy(SET CMP0074 NEW)
set(Protobuf_ROOT "${DEPEND_ROOT}/protobuf/protobuf-3.20.3") 
set(PROTOBUF_PROTOC_EXECUTABLE "${DEPEND_ROOT}/protobuf/protobuf-3.20.3/bin/${CURRENT_SYSTME_NAME}/protoc.exe")
find_package(Protobuf REQUIRED)
# 检查是否找到了 Protobuf
if(Protobuf_FOUND)
    message(STATUS "Protobuf found: ${Protobuf_VERSION}")
    message(STATUS "Protoc executable: ${PROTOBUF_PROTOC_EXECUTABLE}")
else()
    message(FATAL_ERROR "Protobuf not found")
endif()

#yaml-cpp
find_path(yaml-cpp_INCLUDE_DIR NAMES yaml-cpp/yaml.h
    PATHS ${CMAKE_PREFIX_PATH} 
    "${DEPEND_ROOT}/yaml-cpp/0.8.0/include" 
    PATH_SUFFIXES yaml-cpp)

find_library(yaml-cpp_LIBRARY
    NAMES ${yaml-cpp_lib_name}
    PATHS ${CMAKE_PREFIX_PATH} 
        "${DEPEND_ROOT}/yaml-cpp/0.8.0/lib/${CURRENT_SYSTME_NAME}/${build_type}" 
    )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(yaml-cpp
REQUIRED_VARS yaml-cpp_LIBRARY yaml-cpp_INCLUDE_DIR
)

# set output dir 
include(GNUInstallDirs)
# set build output dir
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
#set install output dir
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
set(INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR} CACHE PATH "Installation directory for libraries")
set(INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR} CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKEDIR share/cmake/${PROJECT_NAME} CACHE PATH "Installation directory for CMake files")

set(FACTORY_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/factory)
set(CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(ASIO_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/3rd/asio/include)
set(SPDLOG_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/3rd/spdlog/include)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(msg)

# create build tree cmake
export(TARGETS client server plugins
       NAMESPACE "${PROJECT_NAME}::"
       FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigBuild.cmake
       )

# create install tree cmake
install(EXPORT ${PROJECT_NAME}Targets
        NAMESPACE "${PROJECT_NAME}::"
        DESTINATION ${INSTALL_CMAKEDIR}
        COMPONENT dev)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${INSTALL_CMAKEDIR}
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${INSTALL_CMAKEDIR}
)

install(DIRECTORY 
            include 3rd
        DESTINATION .
)
