#include "count_in_block.h"

#include "kernels/utils/logger.hpp"

namespace xcl {

XclStatus xclCountInBlockCheck(const at::Tensor &input,
                               const at::Tensor &colors, const uint &block_h,
                               const uint &block_w) {
  // check dtype
  caffe2::TypeMeta input_type = input.dtype();
  auto colors_type = colors.dtype();
  TORCH_CHECK(input_type == colors_type,
              "The dtype of input must equal to The dtype of colors.");
  // check device
  TORCH_CHECK(input.is_cuda(), "input must be on CUDA device.");
  TORCH_CHECK(colors.is_cuda(), "colors must be on CUDA device.");
  // check single param
  TORCH_CHECK(block_h > 0, "block_h must great than 0.");
  TORCH_CHECK(block_w > 0, "block_w must great than 0.");
  // check single shape
  c10::IntArrayRef input_shape = input.sizes();
  int64_t input_dim = input_shape.size();
  auto colors_shape = colors.sizes();
  int64_t colors_dim = colors_shape.size();
  TORCH_CHECK(input_dim == 3 || input_dim == 4);
  TORCH_CHECK(colors_dim == 2);
  // check correlation

  // check num
  int64_t input_num = input.numel();
  int64_t colors_num = colors.numel();
  TORCH_CHECK(input_num < INT_MAX);
  TORCH_CHECK(colors_num < INT_MAX);
  return XCL_STATUS_SUCCESS;
}

at::Tensor xclCountInBlock(const at::Tensor &input, const at::Tensor &colors,
                           const uint &block_h, const uint &block_w) {
  auto ret = xclCountInBlockCheck(input, colors, block_h, block_w);
  if (ret != XCL_STATUS_SUCCESS) {
    log("ERROR", "xclCountInBlock check param failed.");
    throw(ret);
  }
  return input;
}

void register_count_in_block(py::module &m) {
  m.def("count_in_block", &xclCountInBlock,
        "Count the occurrences of each color in color_list within each "
        "[block_h, block_w] block in input.");
}

}  // namespace xcl
