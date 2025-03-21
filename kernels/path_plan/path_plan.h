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
using Distance = Point;

class Path2d {
 public:
  std::vector<Point> points;
  std::vector<Distance> get_move_diffs();
  Distance get_whole_move_diff();
};

/*
- enum说明：移动可选择的方向
- enum值说明：
    四连通，
    八连通，
    全连通
*/
enum ConnectedMode { FOUR_CONNECT, EIGHT_CONNECT, FULL_CONNECT };
/*
路径规划方式
*/
enum PathPlanMethod { BROADEST, FURTHEST };

class XclPathPlan2d {
 public:
  std::vector<Path2d> get_optional_paths(at::Tensor segm, const Point start,
                                         ConnectedMode connected_mode);
  void feedback_selected_path(Path2d path, Distance actual_diff);

 private:
  at::Tensor segms;  // segms: 输入识别后的序列图像，LAYOUT为NHWC，DTYPE为uint8_t
  int sample_ratio;                  // 重采样比例
  at::Tensor whole_scene;            // 序列图像重建全景
  std::vector<uint8_t> valid_value;    // 记录有效区域的值
  std::vector<uint8_t> des_value;      // 记录目标区域的值
  PathPlanMethod method = BROADEST;  // 路径查询方法
  bool check_repeat = true;          // 是否避免重复
  bool fix_view_angle = true;  // 是否固定视角，目前只支持固定视角情况
  bool check_segm_diff = false;      // 是否需要真实的场景偏移情况
  std::vector<Path2d> hispath;       // 记录历史路径
  std::vector<Distance> segm_diffs;  // 记录场景移动变化
};

void register_path_plan(py::module &m);

}  // namespace xcl
