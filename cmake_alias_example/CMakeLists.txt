# 主项目CMakeLists.txt - 修复目标名不匹配问题
cmake_minimum_required(VERSION 3.14)
project(MyCMakeAliasLibProject LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(OPTION_ASAN "Build with address sanitizer")
option(OPTION_EXCEPTIONS "Build with exceptions enabled" true)
option(OPTION_RTTI "Build with runtime type info enabled" true)

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

ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE("operation system is ${CMAKE_SYSTEM_NAME}")
    
    SET(CURRENT_SYSTME_NAME1 "win64")
    SET(CURRENT_SYSTME_NAME2 "win")

    if(CMAKE_BUILD_TYPE MATCHES "Release")
		MESSAGE("build release !!!!!!!!!!!!!!!!!!!!!!! ")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od")
        set(protobuf_lib_name libprotobuf.lib)
        set(zlib_lib_name zlibstatic.lib)
	else()
		MESSAGE("build debug !!!!!!!!!!!!!!!!!!!!!!! ")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od")
		#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer -ggdb")
        set(protobuf_lib_name libprotobufd.lib)
        set(zlib_lib_name zlibstaticd.lib)
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

message(STATUS "BUILD_SHARED_LIBS:${BUILD_SHARED_LIBS}")
message(STATUS "EXECUTABLE_OUTPUT_PATH:${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "LIBRARY_OUTPUT_PATH: ${LIBRARY_OUTPUT_PATH}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# 1. 包含第三方静态库管理器
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(${CMAKE_HOME_DIRECTORY}/../../cmake/CommonFunction.cmake)
include(${CMAKE_HOME_DIRECTORY}/../../cmake/ThirdPartyStaticManager.cmake)
include(${CMAKE_HOME_DIRECTORY}/../../cmake/PathUtils.cmake)

# 2. 设置第三方库搜索路径
set(DEPEND_ROOT ${CMAKE_HOME_DIRECTORY}/../../../Dependencies CACHE PATH "Third-party libraries root directory")

# 3. 系统库别名设置
message(STATUS "=== 设置系统库别名 ===")

# 数学库
if(UNIX)
    add_library(m INTERFACE)
    target_link_libraries(m INTERFACE m)
    add_library(MyProject_ThirdParty_System_Math ALIAS m)
endif()

# 线程库
find_package(Threads REQUIRED)
if(TARGET Threads::Threads)
    add_library(MyProject_ThirdParty_System_Threads ALIAS Threads::Threads)
endif()

# 4. 导入预编译静态库
message(STATUS "=== 导入预编译静态库 ===")

# 示例1: JSON库
import_static_library(${jsoncpp_lib_name}
    ALIAS_NAME JsonCpp
    LIBRARY_PATH "${DEPEND_ROOT}/jsoncpp/1.9.6/lib/${CURRENT_SYSTME_NAME}/${build_type}"
    INCLUDE_PATH "${DEPEND_ROOT}/jsoncpp/1.9.6/include"
    NAMESPACE MyProject_ThirdParty
    VERSION "1.9.6"
    DEFINES "JSON_USE_NULLREF=0"
   # COMPILE_OPTIONS "-Wno-deprecated-declarations"
)

# 示例2: ZLIB压缩库
import_static_library(${zlib_lib_name}
    ALIAS_NAME ZLib
    LIBRARY_PATH "${DEPEND_ROOT}/zlib/v1.2.12/lib/${CURRENT_SYSTME_NAME}/${build_type}"
    INCLUDE_PATH "${DEPEND_ROOT}/zlib/v1.2.12"
    NAMESPACE MyProject_ThirdParty
    VERSION "1.2.12"
)

# 示例3: OpenSSL加密库
import_static_library(${openssl_crypto_name}
    ALIAS_NAME OpenSSL_Crypto
    LIBRARY_PATH "${DEPEND_ROOT}/openssl/OpenSSL_1_1_1s/lib/${CURRENT_SYSTME_NAME1}"
    INCLUDE_PATH "${DEPEND_ROOT}/openssl/OpenSSL_1_1_1s/include"
    NAMESPACE MyProject_ThirdParty
    VERSION "1.1.1"
)

import_static_library(${openssl_ssl_name}
    ALIAS_NAME OpenSSL_SSL
    LIBRARY_PATH "${DEPEND_ROOT}/openssl/OpenSSL_1_1_1s/lib/${CURRENT_SYSTME_NAME1}"
    INCLUDE_PATH "${DEPEND_ROOT}/openssl/OpenSSL_1_1_1s/include"
    NAMESPACE MyProject_ThirdParty
    VERSION "1.1.1"
    DEPENDENCIES MyProject_ThirdParty::OpenSSL_Crypto
)

# 示例4: SQLite3数据库
import_static_library(${sqlite3_lib_name}
    ALIAS_NAME SQLite3
    LIBRARY_PATH "${DEPEND_ROOT}/sqlite/version-3.51.1/lib/${CURRENT_SYSTME_NAME}"
    INCLUDE_PATH "${DEPEND_ROOT}/sqlite/version-3.51.1/include"
    NAMESPACE MyProject_ThirdParty
    VERSION "3.51.1"
    DEFINES "SQLITE_THREADSAFE=1"
)

# 5. 创建库组别名
message(STATUS "=== 创建库组别名 ===")

# 网络库组
create_static_library_group(Networking
    NAMESPACE MyProject_ThirdParty
    LIBRARIES
        MyProject_ThirdParty::OpenSSL_SSL
        MyProject_ThirdParty::OpenSSL_Crypto
    DEFINES "USE_NETWORKING=1"
)

# 数据处理组
create_static_library_group(DataProcessing
    NAMESPACE MyProject_ThirdParty
    LIBRARIES
        MyProject_ThirdParty::JsonCpp
        MyProject_ThirdParty::ZLib
    DEFINES "USE_DATA_PROCESSING=1"
)

# 递归扫描 GLOB_RECURSE
file(GLOB SOURCES_HeaderFiles ./*.h)
file(GLOB SOURCES_SourceFiles ./*.cpp)
file(GLOB_RECURSE SOURCES_Data_Files ${CMAKE_CURRENT_SOURCE_DIR}/data/*.h ${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp)
file(GLOB_RECURSE SOURCES_Network_Files ${CMAKE_CURRENT_SOURCE_DIR}/network/*.h ${CMAKE_CURRENT_SOURCE_DIR}/network/*.cpp)

assign_source_group(${SOURCES_Data_Files})
assign_source_group(${SOURCES_Network_Files})

SOURCE_GROUP("header_files" FILES ${SOURCES_HeaderFiles})
SOURCE_GROUP("source_files" FILES ${SOURCES_SourceFiles})
SOURCE_GROUP("data_files" FILES ${SOURCES_Data_Files})
SOURCE_GROUP("network_files" FILES ${SOURCES_Network_Files})

#SOURCE_GROUP("extern/rijndael" FILES ${RIJNDAEL_sources})

# 6. 创建可执行文件
add_executable(${PROJECT_NAME}
    ${SOURCES_HeaderFiles}
    ${SOURCES_SourceFiles}
    ${SOURCES_Data_Files}
    ${SOURCES_Network_Files}
    #data.cpp
)

# 等同于setup_target_compile_options
# if (CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
#     if (NOT ASYNC_LIB_EXCEPTIONS)
#         add_compile_options("-fno-exceptions")
#     endif ()
#     if (NOT ASYNC_LIB_RTTI)
#         add_compile_options("-fno-rtti")
#     endif ()
# endif ()
# 默认参数不填全
setup_target_compile_options(TARGET_NAME "${PROJECT_NAME}")
# 默认参数填全
#setup_target_compile_options(TARGET_NAME "${PROJECT_NAME}" IWYU_FLAG ASAN true EXCEPTIONS true RTTI true)


# 11. 设置C++标准
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# 10. 编译定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_BUILD
    $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN>
    $<$<PLATFORM_ID:Linux>:_GNU_SOURCE -Wno-deprecated-declarations>
)

# 7. 条件定义，用于源代码中的条件编译
if(TARGET MyProject_ThirdParty::JsonCpp)
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_JSON=1)
endif()

if(TARGET MyProject_ThirdParty::ZLib)
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_ZLIB=1)
endif()

if(TARGET MyProject_ThirdParty::OpenSSL_SSL)
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_OPENSSL=1)
endif()

if(TARGET MyProject_ThirdParty::SQLite3)
    target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_SQLITE3=1)
endif()


# 10. 包含目录
get_target_property(jsoncpp_includes MyProject_ThirdParty::JsonCpp INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "get jsoncpp include path:${jsoncpp_includes}")
convert_to_relative(
    INPUT_PATH "${jsoncpp_includes}"
    RELATIVE_TO "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VAR jsconcpp_include_absolute_path
)
message(STATUS "get jsoncpp include absolute path:${jsconcpp_include_absolute_path}")

get_target_property(openssl_includes MyProject_ThirdParty::OpenSSL_SSL INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "get openssl include path:${openssl_includes}")
convert_to_relative(
    INPUT_PATH "${openssl_includes}"
    RELATIVE_TO "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VAR openssl_include_absolute_path
)
message(STATUS "get openssl include absolute path:${openssl_include_absolute_path}")

get_target_property(sqlite3_includes MyProject_ThirdParty::SQLite3 INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "get sqlite3 include path:${sqlite3_includes}")
convert_to_relative(
    INPUT_PATH "${sqlite3_includes}"
    RELATIVE_TO "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VAR slqit3_include_absolute_path
)
message(STATUS "get sqlite3 include absolute path:${slqit3_include_absolute_path}")

get_target_property(zlib_includes MyProject_ThirdParty::ZLib INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(zlib_library_path MyProject_ThirdParty::ZLib IMPORTED_LOCATION)
message(STATUS "get zlib include path:${zlib_includes}")
message(STATUS "get zlib library path:${zlib_library_path}")
convert_to_relative(
    INPUT_PATH "${zlib_includes}"
    RELATIVE_TO "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VAR zlib_include_absolute_path
)
convert_to_relative(
    INPUT_PATH "${zlib_library_path}"
    RELATIVE_TO "${CMAKE_CURRENT_SOURCE_DIR}"
    OUTPUT_VAR zlib_library_absolute_path
)
message(STATUS "get zlib include absolute path:${zlib_include_absolute_path}")
message(STATUS "get zlib library absolute path:${zlib_library_absolute_path}")

# 如果设置了 ZLIB_INCLUDE_DIR ZLIB_LIBRARY ZLIB_FOUND 造成FetchContent不会自动下载zlib库，相当于使用自定义设置的目录
# set(ZLIB_INCLUDE_DIR ${zlib_include_absolute_path})
# set(ZLIB_LIBRARY ${zlib_library_absolute_path})

# include(FindPackageHandleStandardArgs)
# find_package_handle_standard_args(zlib
# REQUIRED_VARS ZLIB_INCLUDE_DIR ZLIB_LIBRARY
# )
# set(ZLIB_FOUND true)

 target_include_directories(${PROJECT_NAME} PRIVATE 
    ${jsconcpp_include_absolute_path}
    ${openssl_include_absolute_path}
    ${slqit3_include_absolute_path}
    ${zlib_include_absolute_path}
     #"$<TARGET_PROPERTY:MyProject_ThirdParty_JsonCpp,INTERFACE_INCLUDE_DIRECTORIES>"
     #${jsoncpp_includes}
     #${openssl_includes}
     #${sqlite3_includes}
     #${zlib_includes}
 )

# 8. 链接静态库（使用修复后的别名）
target_link_libraries(${PROJECT_NAME} PRIVATE
    # 网络功能
    MyProject_ThirdParty_Group_Networking
    
    # 数据处理
    MyProject_ThirdParty_Group_DataProcessing
    
    # 数据库
    MyProject_ThirdParty::SQLite3
    
    # 系统库
    MyProject_ThirdParty_System_Threads
)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")

ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ws2_32.lib
    )
ENDIF()

# 9. 设置静态链接选项
if(UNIX AND NOT APPLE)
    # Linux静态链接
    target_link_options(${PROJECT_NAME} PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()

add_subdirectory("third_party")

organize_targets_in("${CMAKE_HOME_DIRECTORY}")

# 12. 打印配置摘要
print_static_library_summary()

# 13. 测试
enable_testing()
add_test(NAME test_my_app
    COMMAND $<TARGET_FILE:${PROJECT_NAME}> --test
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
