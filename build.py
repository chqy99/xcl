import os
import subprocess
import sys
import argparse

# 解析命令行参数
parser = argparse.ArgumentParser(description="Build script with optional filter")
parser.add_argument("--filter", help="Set XCL_SPECIFIC_OP environment variable")
args = parser.parse_args()

# 检查并创建构建目录
build_dir = "build"
if not os.path.exists(build_dir):
    os.makedirs(build_dir)
    print(f"Build directory '{build_dir}' created")

# 设置 XCL_SPECIFIC_OP 环境变量（如果提供了 --filter 参数）
if args.filter:
    os.environ["XCL_SPECIFIC_OP"] = args.filter
    print(f"XCL_SPECIFIC_OP environment variable set to: {args.filter}")

# 获取 pybind11 的 CMake 路径
try:
    pybind11_dir = subprocess.check_output(
        [sys.executable, "-c", "import pybind11; print(pybind11.get_cmake_dir())"]
    ).decode().strip()
except subprocess.CalledProcessError:
    print("Error: pybind11 not found!")
    sys.exit(1)

print(f"Found pybind11 CMake directory: {pybind11_dir}")

# 获取 LibTorch 的 CMake 路径
try:
    torch_dir = subprocess.check_output(
        [sys.executable, "-c", "import torch; print(torch.utils.cmake_prefix_path)"]
    ).decode().strip()
except subprocess.CalledProcessError:
    print("Error: LibTorch (PyTorch) not found!")
    sys.exit(1)

print(f"Found Torch CMake directory: {torch_dir}")

# 进入构建目录
os.chdir(build_dir)

# 运行 CMake 配置
cmake_command = [
    "cmake",
    f"-Dpybind11_DIR={pybind11_dir}",
    f"-DCMAKE_PREFIX_PATH={torch_dir}",
    ".."
]
print("Running cmake...")
subprocess.run(cmake_command, check=True)

# 编译项目
build_tool = "make"  # 你可以根据平台选择不同的构建工具
if os.name == "nt":  # Windows
    build_tool = "cmake --build . --config Release"

print("Compiling project...")
subprocess.run(build_tool, shell=True, check=True)

print("Build complete!")

# 取消 XCL_SPECIFIC_OP 环境变量（如果之前设置过）
if args.filter:
    del os.environ["XCL_SPECIFIC_OP"]
    print("XCL_SPECIFIC_OP environment variable unset")
