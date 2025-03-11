#include "ops_collection.h"

XclStatus xclPathPlan(const at::Tensor &segms,
                      const vector<vector<Point>> &hispath,
                      vector<vector<Point>> outpath,
                      const vector<uint8> &valid_value,
                      const vector<uint8> &des_value,
                      ConnectedMode connected_mode) {
  return XCL_STATUS_SUCCESS;
}

void register_path_plan(py::module &m) { m.def("path_plan", &xclPathPlan, ""); }
