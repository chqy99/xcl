#include "ops_collection.h"

namespace xcl {

vector<vector<Point>> xclPathPlan(const at::Tensor &segms,
                      const vector<vector<Point>> &hispath,
                      const vector<uint8> &valid_value,
                      const vector<uint8> &des_value,
                      ConnectedMode connected_mode) {
  vector<vector<Point>> outpath;
  outpath.clear();
  outpath.push_back({Point(1, 1)});
  outpath.push_back({Point(2, 2)});
  return outpath;
}

void register_path_plan(py::module &m) {
  py::enum_<ConnectedMode>(m, "ConnectedMode")
    .value("FOUR_CONNECT", FOUR_CONNECT)
    .value("EIGHT_CONNECT", EIGHT_CONNECT)
    .value("FULL_CONNECT", FULL_CONNECT);
  m.def("path_plan", &xclPathPlan, "");
}

}
