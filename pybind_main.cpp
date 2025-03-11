#include "ops_collection.h"

namespace xcl {

/*
独立编译时， PYBIND11_MODULE(xcl, m) 可以多次使用，
当前是作为整体编译。
*/
PYBIND11_MODULE(xcl, m) {
  py::enum_<XclStatus>(m, "XclStatus")
      .value("XCL_STATUS_SUCCESS", XCL_STATUS_SUCCESS)
      .value("XCL_STATUS_BAD_PARAM", XCL_STATUS_BAD_PARAM)
      .value("XCL_STATUS_NOT_SUPPORTED", XCL_STATUS_NOT_SUPPORTED)
      .value("XCL_STATUS_ALLOC_FAILED", XCL_STATUS_ALLOC_FAILED)
      .value("XCL_STATUS_EXECUTION_FAILED", XCL_STATUS_EXECUTION_FAILED);
  py::class_<Point>(m, "Point")
      .def(py::init<>())
      .def(py::init<int, int>())
      .def_readwrite("px", &Point::px)
      .def_readwrite("py", &Point::py)
      .def("__repr__", [](const Point &point) {
          return "Point(px=" + std::to_string(point.px) + ", py=" + std::to_string(point.py) + ")";
      });
  register_count_in_block(m);
  register_path_plan(m);
}

}
