#include "path_plan.h"

namespace xcl {

/*
- 函数说明：输入序列图像和历史路径，输出到目的地的最短规划路径
- 参数说明：
    segms: 输入识别后的序列图像，LAYOUT为HWC，DTYPE为uint6
    start: 寻路起点
    connected_mode: 移动可选择的方向
- 返回值:
    outpath: 到目的地的最短规划路径，目的地可能有多个
*/
vector<Path2d> XclPathPlan2d::get_optional_paths(const at::Tensor segm,
                                                 const Point start,
                                                 ConnectedMode connected_mode) {
}

void register_path_plan(py::module &m) {
  py::class_<Point>(m, "Point")
      .def(py::init<>())
      .def(py::init<int, int>())
      .def_readwrite("px", &Point::px)
      .def_readwrite("py", &Point::py)
      .def("__repr__", [](const Point &point) {
        return "Point(px=" + std::to_string(point.px) +
               ", py=" + std::to_string(point.py) + ")";
      });
  py::enum_<ConnectedMode>(m, "ConnectedMode")
      .value("FOUR_CONNECT", FOUR_CONNECT)
      .value("EIGHT_CONNECT", EIGHT_CONNECT)
      .value("FULL_CONNECT", FULL_CONNECT);
}

}  // namespace xcl
