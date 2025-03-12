#pragma once

#include "kernels/ops_common.h"

namespace xcl {

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
enum ConnectedMode { FOUR_CONNECT, EIGHT_CONNECT, FULL_CONNECT };

/*
- 函数说明：输入序列图像和历史路径，输出到目的地的最短规划路径
- 参数说明：
    segms: 输入识别后的序列图像，LAYOUT为NHWC，DTYPE为uint6
    hispath: 前N-1个为历史序列，第N个表示起点位置
    valid_value: 可通行区域的标签值
    des_value: 目的地的标签值（目标值）
    connected_mode: 移动可选择的方向
- 返回值:
    outpath: 到目的地的最短规划路径，目的地可能有多个
*/
vector<vector<Point>> xclPathPlan(const at::Tensor &segms,
                                  const vector<vector<Point>> &hispath,
                                  const vector<uint8> &valid_value,
                                  const vector<uint8> &des_value,
                                  ConnectedMode connected_mode);
void register_path_plan(py::module &m);

}  // namespace xcl
