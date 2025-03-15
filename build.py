import os
import subprocess
import sys
import argparse

# 定义颜色代码
RED = "\033[91m" # 错误
GREEN = "\033[92m" # 成功
YELLOW = "\033[93m" # 警告
BLUE = "\033[94m" # 提示
RESET = "\033[0m"  # 重置颜色

# 解析命令行参数
parser = argparse.ArgumentParser(description="Build script with optional filter")
parser.add_argument("--filter", help="Set XCL_SPECIFIC_OP environment variable")
args = parser.parse_args()

# 检查并创建构建目录
build_dir = "build"
if not os.path.exists(build_dir):
    os.makedirs(build_dir)
    print(f"{GREEN}Build directory '{build_dir}' created{RESET}")

# 设置 XCL_SPECIFIC_OP 环境变量（如果提供了 --filter 参数）
if args.filter:
    xcl_specific_op = args.filter
else:
    xcl_specific_op = ""

# 获取 pybind11 的 CMake 路径
try:
    pybind11_dir = subprocess.check_output(
        [sys.executable, "-c", "import pybind11; print(pybind11.get_cmake_dir())"]
    ).decode().strip()
except subprocess.CalledProcessError:
    print(f"{RED}Error: pybind11 not found!{RESET}")
    sys.exit(1)

print(f"{GREEN}Found pybind11 CMake directory: {pybind11_dir}{RESET}")

# 获取 LibTorch 的 CMake 路径
try:
    torch_dir = subprocess.check_output(
        [sys.executable, "-c", "import torch; print(torch.utils.cmake_prefix_path)"]
    ).decode().strip()
except subprocess.CalledProcessError:
    print(f"{RED}Error: LibTorch (PyTorch) not found!{RESET}")
    sys.exit(1)

print(f"{GREEN}Found Torch CMake directory: {torch_dir}{GREEN}")

# 进入构建目录
os.chdir(build_dir)

# 运行 CMake 配置
cmake_command = [
    "cmake",
    "../",
    f"-Dpybind11_DIR={pybind11_dir}",
    f"-DCMAKE_PREFIX_PATH={torch_dir}",
    f"-DXCL_SPECIFIC_OP={xcl_specific_op}"
]
print(f"{BLUE}Running cmake...{RESET}")
subprocess.run(cmake_command, check=True)

# 编译项目
build_tool = "make"  # 你可以根据平台选择不同的构建工具
if os.name == "nt":  # Windows
    build_tool = "cmake --build . --config Release"

print(f"{BLUE}Compiling project...{RESET}")
subprocess.run(build_tool, shell=True, check=True)

print("Build complete!")
