#pragma once

#include <pybind11/pybind11.h>
#include <torch/extension.h>
#include <torch/torch.h>

#include <iostream>
#include <vector>

template <typename T>
using vector = std::vector<T>;

using uint8 = unsigned char;

/*
- enum说明：接口返回状态码
- enum值说明：
    运行成功，
    参数错误，
    无法支持情况，
    内存分配失败，
    运行时错误
*/
enum XclStatus {
  XCL_STATUS_SUCCESS = 0,
  XCL_STATUS_BAD_PARAM = 1,
  XCL_STATUS_NOT_SUPPORTED = 2,
  XCL_STATUS_ALLOC_FAILED = 3,
  XCL_STATUS_EXECUTION_FAILED = 4,
};

template <typename T>
struct TPoint {
  T px;
  T py;

  TPoint() : px(static_cast<T>(0)), py(static_cast<T>(0)) {}
  TPoint(T x, T y) : px(x), py(y) {}
  TPoint(const TPoint &other) : px(other.px), py(other.py) {}
  TPoint &operator=(const TPoint &other) {
    if (this != &other) {
      px = other.px;
      py = other.py;
    }
    return *this;
  }
};
using Point = TPoint<int>;

/*
- enum说明：移动可选择的方向
- enum值说明：
    四连通，
    八连通，
    全连通
*/
enum ConnectedMode {
  FOUR_CONNECT,
  EIGHT_CONNECT,
  FULL_CONNECT
};

/*
- 函数说明：在input内逐[block_h, block_w]统计color_list中各个颜色的出现次数。
- 参数说明：
    input：输入图像，LAYOUT为NHWC
    color_list：需要统计的颜色信息，shape 为[L，C]
    output: 输出图像，shape为[L, Ho, Wo, Count]
    block_h: H维度的采样率
    block_w: W维度的采样率
*/
XclStatus xclCountInBlock(const at::Tensor &input, const at::Tensor &color_list,
                          at::Tensor output, const int &block_h,
                          const int &block_w);

/*
- 函数说明：输入序列图像和历史路径，输出到目的地的最短规划路径
- 参数说明：
    segms: 输入识别后的序列图像，LAYOUT为NHWC，DTYPE为uint6
    hispath: 长度为N的历史路径，第0个路径只含有一个起点，后续的路径与前（N-1）个输入序列相对应
    outpath: 到目的地的最短规划路径，目的地可能有多个
    valid_value: 可通行区域的标签值
    des_value: 目的地的标签值（目标值）
    connected_mode: 移动可选择的方向
*/
XclStatus xclPathPlan(const at::Tensor &segms, const vector<vector<Point>> &hispath,
  vector<vector<Point>> outpath, const vector<uint8> &valid_value,
  const vector<uint8> &des_value, ConnectedMode connected_mode);
