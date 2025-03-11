#include "ops_collection.h"
#include "kernels/utils/logger.hpp"

XclStatus xclCountInBlockCheck(const at::Tensor &input,
                               const at::Tensor &colors, at::Tensor output,
                               const uint &block_h, const uint &block_w) {
  // check dtype
  caffe2::TypeMeta input_type = input.dtype();
  auto colors_type = colors.dtype();
  auto output_type = output.dtype();
  TORCH_CHECK(input_type == colors_type,
              "The dtype of input must equal to The dtype of colors.");
  TORCH_CHECK(input_type == output_type,
              "The dtype of input must equal to The dtype of output.");
  // check device
  TORCH_CHECK(input.is_cuda(), "input must be on CUDA device.");
  TORCH_CHECK(colors.is_cuda(), "colors must be on CUDA device.");
  TORCH_CHECK(output.is_cuda(), "output must be on CUDA device.");
  // check single param
  TORCH_CHECK(block_h > 0, "block_h must great than 0.");
  TORCH_CHECK(block_w > 0, "block_w must great than 0.");
  // check single shape
  c10::IntArrayRef input_shape = input.sizes();
  int64_t input_dim = input_shape.size();
  auto colors_shape = colors.sizes();
  int64_t colors_dim = colors_shape.size();
  auto output_shape = output.sizes();
  int64_t output_dim = output_shape.size();
  TORCH_CHECK(input_dim == 3 || input_dim == 4);
  TORCH_CHECK(colors_dim == 2);
  TORCH_CHECK(output_dim == 4 || output_dim == 5);
  // check correlation
  TORCH_CHECK(input_dim + 1 == output_dim);
  // check num

  return XCL_STATUS_SUCCESS;
}

XclStatus xclCountInBlock(const at::Tensor &input, const at::Tensor &colors,
                          at::Tensor output, const uint &block_h,
                          const uint &block_w) {
  auto ret = xclCountInBlockCheck(input, colors, output, block_h, block_w);
  if (ret != XCL_STATUS_SUCCESS) {
    log("ERROR", "xclCountInBlock check param failed.");
    return ret;
  }
  return XCL_STATUS_SUCCESS;
}

void register_count_in_block(py::module& m) {
  m.def("count_in_block", &xclCountInBlock,
        "Count the occurrences of each color in color_list within each [block_h, block_w] block in input.");
}
