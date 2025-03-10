#include "ops_collection.h"

namespace py = pybind11;

PYBIND11_MODULE(xcl, m) {
  m.def("count_in_block", &xclCountInBlock,
        "Count the occurrences of each color in color_list within each [block_h, block_w] block in input.");
}
