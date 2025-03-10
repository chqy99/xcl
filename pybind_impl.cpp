#include "ops_collection.h"

PYBIND11_MODULE(xcl, m) {
  m.def("count_in_block", &xclCountInBlock,
        "在input内逐[block_h, block_w]统计color_list中各个颜色的出现次数。");
}