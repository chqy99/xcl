#pragma once

#include <pybind11/pybind11.h>
#include <torch/extension.h>
#include <torch/torch.h>

#include <climits>
#include <iostream>
#include <vector>

namespace py = pybind11;

namespace xcl {

/*
- enum说明：接口返回状态码
- enum值说明：
    运行成功，
    参数错误，
    无法支持情况，
    内存分配失败，
    运行时错误
*/
enum XclStatus {
  XCL_STATUS_SUCCESS = 0,
  XCL_STATUS_BAD_PARAM = 1,
  XCL_STATUS_NOT_SUPPORTED = 2,
  XCL_STATUS_ALLOC_FAILED = 3,
  XCL_STATUS_EXECUTION_FAILED = 4,
};

}  // namespace xcl
