
include(FetchContent)

# 1.在使用FetchContent之前，先调用find_package(ZLIB)，这样FetchContent就不会再次调用find_package(ZLIB)。
# 2.或者，我们可以通过设置CMAKE_DISABLE_FIND_PACKAGE_ZLIB=ON来禁止FetchContent调用find_package(ZLIB)，然后我们自己调用find_package(ZLIB)。
#但是，注意用户可能希望使用系统安装的zlib，也可能是从第三方目录构建zlib。根据之前的对话，用户正在尝试使用third_party/zlib，并且设置了自定义输出目录。
#我们来看看之前的CMakeLists.txt中关于zlib的部分。用户使用了FetchContent_MakeAvailable(ZLIB)，这会导致FetchContent尝试调用find_package(ZLIB)，
#而FindZLIB.cmake模块被设计为通过find_package(ZLIB)调用。因此，我们可以修改CMakeLists.txt，避免使用FetchContent，而是直接使用add_subdirectory，
#或者通过设置CMAKE_DISABLE_FIND_PACKAGE_ZLIB=ON来禁止FetchContent调用find_package。但是，根据错误信息，警告来自FetchContent.cmake，说明
#FetchContent正在尝试调用find_package(ZLIB)。我们可以通过设置FETCHCONTENT_TRY_FIND_PACKAGE_MODE来改变FetchContent的行为。

set(CMAKE_DISABLE_FIND_PACKAGE_ZLIB ON)

set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
  zlib
  GIT_REPOSITORY https://github.com/madler/zlib.git
  GIT_TAG        v1.3.1
  #GIT_SHALLOW       TRUE       # 只克隆最新提交，加快下载速度
  GIT_PROGRESS      TRUE       # 显示下载进度
  SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/zlib
  BINARY_DIR        ${CMAKE_CURRENT_BINARY_DIR}/zlib
  #SUBBUILD_DIR      ${CMAKE_CURRENT_BINARY_DIR}/zlib-subbuild
  SUBBUILD_DIR      ${CMAKE_CURRENT_BINARY_DIR}/zlib

  #CMAKE_CACHE_ARGS -DABSL_PROPAGATE_CXX_STD:BOOL=ON
  FIND_PACKAGE_ARGS
)

# 方法2: 使用存档文件（替代方案）
# FetchContent_Declare(
#     zlib
#     URL      https://zlib.net/zlib-1.3.1.tar.gz
#     URL_HASH MD5=9a93b2b7dfdac77ceba5a558a580e746
# )

# 可选：如果希望静态链接，设置BUILD_SHARED_LIBS
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ZLIB_EXAMPLE_BUILD_TESTS "Build tests" ON)
option(ZLIB_EXAMPLE_BUILD_EXAMPLES "Build examples" ON)
option(ZLIB_EXAMPLE_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ZLIB_EXAMPLE_ENABLE_USAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ZLIB_EXAMPLE_USE_SYSTEM_ZLIB "Use system zlib instead of downloading" OFF)

# 设置zlib选项
set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(ZLIB_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(ZLIB_COMPAT OFF CACHE BOOL "" FORCE)

# 设置zlib的自定义输出目录
set(ZLIB_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/zlib" CACHE PATH "Base output directory for zlib")
    
# 分别设置zlib各种输出目录
set(ZLIB_LIBRARY_OUTPUT_DIRECTORY "${ZLIB_OUTPUT_DIR}/lib/${CURRENT_SYSTME_NAME}/${build_type}" CACHE PATH "Library output directory for zlib")
set(ZLIB_ARCHIVE_OUTPUT_DIRECTORY "${ZLIB_OUTPUT_DIR}/lib/${CURRENT_SYSTME_NAME}/${build_type}" CACHE PATH "Archive output directory for zlib")
set(ZLIB_RUNTIME_OUTPUT_DIRECTORY "${ZLIB_OUTPUT_DIR}/bin/${CURRENT_SYSTME_NAME}/${build_type}" CACHE PATH "Runtime output directory for zlib")

message(STATUS "Zlib custom output directories:")
message(STATUS "  Base: ${ZLIB_OUTPUT_DIR}")
message(STATUS "  Library: ${ZLIB_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "  Archive: ${ZLIB_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "  Runtime: ${ZLIB_RUNTIME_OUTPUT_DIRECTORY}")

# 保存当前的输出目录设置
set(ORIG_CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(ORIG_CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(ORIG_CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

# 临时设置zlib的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ZLIB_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ZLIB_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ZLIB_ARCHIVE_OUTPUT_DIRECTORY})

FetchContent_MakeAvailable(ZLIB)
set(FETCHCONTENT_QUIET ON)

 # 恢复原始目录设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ORIG_CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ORIG_CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ORIG_CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

# 检查是否成功获取
if(NOT zlib_POPULATED)
    message(FATAL_ERROR "Failed to populate zlib")
else()
    message(STATUS "Success to populate zlib")
endif()

 set(ZLIB_TARGET zlibstatic)
 
# #set(protobuf_DIR ${protobuf_DIR} PARENT_SCOPE)
# #set(grpc_cpp_plugin_location ${grpc_cpp_plugin_location} PARENT_SCOPE)

organize_targets_in("third_party")

#if (MSVC)
#  set(protobuf_DIR "${grpc_BINARY_DIR}/third_party/protobuf/cmake" PARENT_SCOPE)
#else ()
#  set(protobuf_DIR "${grpc_BINARY_DIR}/third_party/protobuf/lib/cmake/protobuf" PARENT_SCOPE)
#endif()

#set(grpc_cpp_plugin_location $<TARGET_FILE:grpc_cpp_plugin> PARENT_SCOPE)

organize_targets_in("zlib")
