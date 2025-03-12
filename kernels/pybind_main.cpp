#include "count_in_block/count_in_block.h"
#include "path_plan/path_plan.h"

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

  register_count_in_block(m);
  register_path_plan(m);
}

}  // namespace xcl
