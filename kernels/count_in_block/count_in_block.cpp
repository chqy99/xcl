#include "count_in_block.h"

#include "kernels/utils/logger.hpp"

namespace xcl {

XclStatus xclCountInBlockCheck(const at::Tensor &image,
                               const at::Tensor &colors,
                               const uint &block_height,
                               const uint &block_width) {
  // check dtype
  caffe2::TypeMeta image_type = image.dtype();
  auto colors_type = colors.dtype();
  TORCH_CHECK(image_type == colors_type,
              "The dtype of image must equal to The dtype of colors.");
  // check device
  TORCH_CHECK(image.is_cuda(), "image must be on CUDA device.");
  TORCH_CHECK(colors.is_cuda(), "colors must be on CUDA device.");
  // check single param
  TORCH_CHECK(block_height > 0, "block_height must great than 0.");
  TORCH_CHECK(block_width > 0, "block_width must great than 0.");
  // check single shape
  c10::IntArrayRef image_shape = image.sizes();
  int64_t image_dim = image_shape.size();
  auto colors_shape = colors.sizes();
  int64_t colors_dim = colors_shape.size();
  TORCH_CHECK(image_dim == 3);
  TORCH_CHECK(colors_dim == 2);
  // check correlation
  TORCH_CHECK(image_shape[image_dim - 1] == colors_shape[colors_dim - 1]);
  // check num
  int64_t image_num = image.numel();
  int64_t colors_num = colors.numel();
  TORCH_CHECK(image_num < INT_MAX);
  TORCH_CHECK(colors_num < INT_MAX);
  return XCL_STATUS_SUCCESS;
}

at::Tensor xclCountInBlock(const at::Tensor &image, const at::Tensor &colors,
                           const uint &block_height, const uint &block_width) {
  auto ret = xclCountInBlockCheck(image, colors, block_height, block_width);
  if (ret != XCL_STATUS_SUCCESS) {
    log("ERROR", "xclCountInBlock check param failed.");
    throw(ret);
  }

  long height = image.size(0);
  long width = image.size(1);
  long num_colors = colors.size(0);
  void *data_image = image.data_ptr();
  void *data_colors = colors.data_ptr();

  long output_height = height / block_height;
  long output_width = width / block_width;
  long num_blocks = output_height * output_width;

  at::Tensor result_tensor =
      torch::zeros({num_blocks, num_colors},
                   torch::dtype(torch::kInt32).device(torch::kCUDA));
  int *result_ptr = result_tensor.data_ptr<int>();

  ret = launchCountInBlock(image.dtype(), data_image, data_colors, result_ptr,
                           height, width, output_height, output_width,
                           block_height, block_width, num_colors);
  if (ret != XCL_STATUS_SUCCESS) {
    log("ERROR", "xclCountInBlock kernel encountered an error during runtime.");
    throw(ret);
  }

  return result_tensor.view({num_colors, output_height, output_width});
}

void register_count_in_block(py::module &m) {
  m.def("count_in_block", &xclCountInBlock,
        "Count the occurrences of each color in color_list within each "
        "[block_height, block_width] block in image.");
}

}  // namespace xcl
