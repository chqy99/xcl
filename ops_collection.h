#pragma once

#include <pybind11/pybind11.h>
#include <torch/torch.h>

namespace py = pybind11;

// ops interface return status
typedef enum {
  XCL_STATUS_SUCCESS          = 0,
  XCL_STATUS_ALLOC_FAILED     = 1,
  XCL_STATUS_BAD_PARAM        = 2,
  XCL_STATUS_NOT_SUPPORTED    = 3,
  XCL_STATUS_EXECUTION_FAILED = 4,
} xclStatus;

xclStatus xclPathPlan();
