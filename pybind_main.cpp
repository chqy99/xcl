#include "ops_collection.h"

namespace py = pybind11;

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