#include "path_plan.h"

namespace xcl {

std::vector<Distance> Path2d::get_move_diffs() {
  std::vector<Distance> res;
  if (points.size() >= 2) {
    Point last = points[0];
    for (int i = 1; i < points.size(); ++i) {
      Point cur = points[i];
      res.push_back(Distance(cur.px - last.px, cur.py - last.py));
      last = cur;
    }
  }
  return res;
}

Distance Path2d::get_whole_move_diff() {
  Distance res;
  size_t len = points.size();
  if (len >= 2) {
    res.px = points[len - 1].px - points[0].px;
    res.py = points[len - 1].py - points[0].py;
  }
  return res;
}

std::vector<Path2d> XclPathPlan2d::get_optional_paths(
    const at::Tensor segm, const Point start, ConnectedMode connected_mode) {
  std::vector<Path2d> res;
  return res;
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
