cmake_minimum_required(VERSION 3.10.2)
project(CUDAExample)

# 查找 pybind11
find_package(pybind11 REQUIRED)

# 查找 Torch (PyTorch)，需要设置 TORCH_DIR
# 通过环境变量或 CMake 提供路径
find_package(Torch REQUIRED)

find_library(TORCH_PYTHON_LIBRARY torch_python PATH "${TORCH_INSTALL_PREFIX}/lib")

# 查找 CUDA
enable_language(CUDA)

# 设置 C++ 和 CUDA 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD 17)

# 编译源文件，指定编译为模块
add_library(example MODULE example.cu)

# 设定 CUDA 目标链接语言为 CUDA
set_target_properties(example PROPERTIES LINKER_LANGUAGE CUDA)

# 链接 pybind11 模块
target_link_libraries(example PRIVATE pybind11::module)

target_link_libraries(example PRIVATE ${TORCH_LIBRARIES} ${TORCH_PYTHON_LIBRARY})

# 设定 include 路径
target_include_directories(example PRIVATE ${TORCH_INCLUDE_DIRS})

# 设置输出路径为共享库 (.so)
set_target_properties(example PROPERTIES
    PREFIX ""
    SUFFIX ".so"
)