# torchOpCustom

这是一个基于 PyTorch 和 NumPy 的自定义算子库，使用 pybind11 绑定为 Python 模块。

## 安装依赖

```bash
pip install -r requirements.txt
```

## 代码规范
enum, 类, 结构体按大驼峰式命名；
函数按小驼峰式命名；
变量按小写加下划线的方式命名；
宏, enum值按全大写加下划线的方式命名；
c++ 格式刷使用 'clang-format -i --style="{BasedOnStyle: Google, IndentWidth: 2}" file_path'；
python 格式刷使用 'black file_path'；

## 问题
该项目无法支持MSVC编译器中文编译

## 环境变量设置
```shell
# windows
$env:PYTHONPATH=".\build\lib\Release;$env:PYTHONPATH"
# linux
export PYTHONPATH="./build/lib:$PYTHONPATH"
```
