#include "count_in_block.h"
#include <cuda_runtime.h>
#include "kernels/utils/enum2dtype.h"

namespace xcl {

// CUDA 内核函数
template <typename T>
__global__ void color_count_kernel(T *data_image, T *data_colors, int *result,
                                   long height, long width, long block_height,
                                   long block_width, long num_colors) {
  int block_x = blockIdx.x;
  int block_y = blockIdx.y;

  int start_x = block_x * block_width;
  int start_y = block_y * block_height;

  int end_x = min((int)(start_x + block_width), (int)width);
  int end_y = min((int)(start_y + block_height), (int)height);

  for (int y = start_y + threadIdx.y; y < end_y; y += blockDim.y) {
    for (int x = start_x + threadIdx.x; x < end_x; x += blockDim.x) {
      T color = data_image[y * width + x];
      for (long i = 0; i < num_colors; ++i) {
        if (color == data_colors[i]) {
          long index =
              i * gridDim.x * gridDim.y + (block_y * gridDim.x + block_x);
          atomicAdd(&result[index], 1);
        }
      }
    }
  }
}

XclStatus launchCountInBlock(caffe2::TypeMeta data_type, void *data_image,
                             void *data_colors, int *result_ptr, long height,
                             long width, long output_height, long output_width,
                             long block_height, long block_width,
                             long num_colors) {
  // 配置 CUDA 内核
  dim3 threads_per_block(16, 16);
  dim3 blocks_per_grid(output_width, output_height);

  auto var = VariantHelper<uint8_t, int, float>::create(data_type);

  std::visit(
      [&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        // 启动 CUDA 内核，结果直接写入 PyTorch 张量的内存
        color_count_kernel<<<blocks_per_grid, threads_per_block>>>(
            (T *)data_image, (T *)data_colors, result_ptr, height, width,
            block_height, block_width, num_colors);
      },
      var);

  // 同步和错误检查
  cudaDeviceSynchronize();
  cudaError_t err = cudaGetLastError();
  if (err != cudaSuccess) {
    std::cerr << "CUDA Error: " << cudaGetErrorString(err) << std::endl;
    return XCL_STATUS_EXECUTION_FAILED;
  }

  return XCL_STATUS_SUCCESS;
}

}  // namespace xcl
