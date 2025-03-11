cmake_minimum_required(VERSION 3.10.2)
project(xcl)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/test")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)

################################################################################
# XCL_SPECIFIC_OP
set(XCL_SPECIFIC_OP ${XCL_SPECIFIC_OP})

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/kernel_depends.cmake)
# populate_op is provided by kernel_depends.cmake
if (XCL_SPECIFIC_OP)
  populate_op(XCL_SPECIFIC_OP SPECIFIC_OP ${XCL_SPECIFIC_OP})
  message(STATUS "XCL_SPECIFIC_OP (populated): ${XCL_SPECIFIC_OP}")
endif()

if (NOT XCL_SPECIFIC_OP)
  message(STATUS "Build all kernels")
  file(GLOB all_kernels "${CMAKE_CURRENT_LIST_DIR}/kernels/*")
  foreach (o ${all_kernels})
    if (IS_DIRECTORY ${o})
      get_filename_component(kernelname ${o} NAME)
      set(build_kernel ${kernelname} ${build_kernel})
    endif()
  endforeach ()
else()
  set(build_kernel ${XCL_SPECIFIC_OP})
endif()

list(SORT build_kernel)
message(STATUS "build_kernel:[${build_kernel}]")

file(GLOB all_kernels "${CMAKE_CURRENT_LIST_DIR}/kernels/*")
foreach(kernel ${build_kernel} )
  set(kernel_parent_dir '')
  foreach (o ${all_kernels})
    if (IS_DIRECTORY ${o})
      get_filename_component(kernelname ${o} NAME)
      if(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/kernels/${kernel} OR
         IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/kernels/${kernelname}/${kernel})
        set(kernel_parent_dir ${kernelname})
      endif()
    endif()
  endforeach ()
  if (IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/kernels/${kernel}")
    file(GLOB_RECURSE src_files ${src_files} "${CMAKE_CURRENT_SOURCE_DIR}/kernels/${kernel}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/kernels/${kernel}/*.mlu")
  elseif(IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/kernels/${kernel_parent_dir}/${kernel}")
    file(GLOB_RECURSE src_files ${src_files} "${CMAKE_CURRENT_SOURCE_DIR}/kernels/${kernel_parent_dir}/${kernel}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/kernels/${kernel_parent_dir}/${kernel}/*.mlu")
  else()
    message(WARNING "kernel/${kernel} is not a directory, ${kernel} is an alias")
    continue()
  endif()
endforeach()

# 编译源文件，指定编译为模块
# message(STATUS "src_files:[${src_files}]")
add_library(xcl MODULE ${src_files} pybind_main.cpp)
################################################################################

# 查找 pybind11
find_package(pybind11 REQUIRED)

# 查找 Torch (PyTorch)，需要设置 TORCH_DIR
# 通过环境变量或 CMake 提供路径
find_package(Torch REQUIRED)

find_library(TORCH_PYTHON_LIBRARY torch_python PATH "${TORCH_INSTALL_PREFIX}/lib")

# 启用 CUDA
enable_language(CUDA)

# 设定 CUDA 目标链接语言为 CUDA
set_target_properties(xcl PROPERTIES LINKER_LANGUAGE CUDA)

# 链接 pybind11, torch 模块
target_link_libraries(xcl PRIVATE pybind11::module ${TORCH_LIBRARIES} ${TORCH_PYTHON_LIBRARY})

# 设定 include 路径
target_include_directories(xcl PRIVATE ${TORCH_INCLUDE_DIRS})

# 设置输出路径为共享库 (.pyd 或 .so)
if(WIN32)
    set_target_properties(xcl PROPERTIES
        PREFIX ""
        SUFFIX ".pyd"
    )
else()
    set_target_properties(xcl PROPERTIES
        PREFIX ""
        SUFFIX ".so"
    )
endif()
