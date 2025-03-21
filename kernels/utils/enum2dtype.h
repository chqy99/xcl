#pragma once

#include <torch/extension.h>
#include <torch/torch.h>

#include <variant>

template <typename T>
struct TypeToEnum {
  static constexpr auto value = torch::kUInt8;
};

#define TYPE_MAP_ENUM(type, enum)                  \
  template <>                                      \
  struct TypeToEnum<type> {                        \
    static constexpr auto value = enum;            \
  }

TYPE_MAP_ENUM(float, torch::kFloat32);
// TYPE_MAP_ENUM(half, torch::kFloat16);
// TYPE_MAP_ENUM(bfloat16, torch::kBFloat16);
TYPE_MAP_ENUM(double, torch::kFloat64);
TYPE_MAP_ENUM(uint8_t, torch::kUInt8);
TYPE_MAP_ENUM(int8_t, torch::kInt8);
TYPE_MAP_ENUM(uint16_t, torch::kUInt16);
TYPE_MAP_ENUM(int16_t, torch::kInt16);
TYPE_MAP_ENUM(uint32_t, torch::kUInt32);
TYPE_MAP_ENUM(int32_t, torch::kInt32);
TYPE_MAP_ENUM(uint64_t, torch::kUInt64);
TYPE_MAP_ENUM(int64_t, torch::kInt64);
TYPE_MAP_ENUM(bool, torch::kBool);

template <typename... Types>
struct VariantHelper {
  using Type = std::variant<Types...>;

  static Type create(caffe2::TypeMeta enum_dtype) {
    Type var;
    bool not_support = true;
    ((enum_dtype == TypeToEnum<Types>::value ? var = Types{}, not_support = false : true), ...);

    if (not_support) {
      throw std::invalid_argument("unsupported type");
    }
    return var;
  }
};