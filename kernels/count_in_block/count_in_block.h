#pragma once

#include "kernels/ops_common.h"

namespace xcl {

/*
- 函数说明：在image内逐[block_height, block_width]统计colors中各个颜色的出现次数。
- 参数说明：
    image：输入图像，LAYOUT为 HWC
    colors：需要统计的颜色信息，shape 为[L，C]
    block_height: H维度的采样率
    block_width: W维度的采样率
- 返回值:
    output: 输出图像，shape为 [L, Ho, Wo]
*/
at::Tensor xclCountInBlock(const at::Tensor &image, const at::Tensor &colors,
                           const uint &block_height, const uint &block_width);

XclStatus launchCountInBlock(caffe2::TypeMeta data_type, void *data_image,
                             void *data_colors, int *result_ptr, long height,
                             long width, long output_height, long output_width,
                             long block_height, long block_width,
                             long num_colors);

void register_count_in_block(py::module &m);

}  // namespace xcl
