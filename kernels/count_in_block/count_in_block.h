#pragma once

#include "kernels/ops_common.h"

namespace xcl {

/*
- 函数说明：在input内逐[block_h, block_w]统计colors中各个颜色的出现次数。
- 参数说明：
    input：输入图像，LAYOUT为 HWC or NHWC
    colors：需要统计的颜色信息，shape 为[L，C]
    block_h: H维度的采样率
    block_w: W维度的采样率
- 返回值:
    output: 输出图像，shape为 [L, Ho, Wo, Count] or [N, L, Ho, Wo, Count]
*/
at::Tensor xclCountInBlock(const at::Tensor &input, const at::Tensor &colors,
                           const uint &block_h, const uint &block_w);

void register_count_in_block(py::module &m);

}  // namespace xcl
