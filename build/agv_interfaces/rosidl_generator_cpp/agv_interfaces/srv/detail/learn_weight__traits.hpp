// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from agv_interfaces:srv/LearnWeight.idl
// generated code does not contain a copyright notice

#ifndef AGV_INTERFACES__SRV__DETAIL__LEARN_WEIGHT__TRAITS_HPP_
#define AGV_INTERFACES__SRV__DETAIL__LEARN_WEIGHT__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "agv_interfaces/srv/detail/learn_weight__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace agv_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const LearnWeight_Request & msg,
  std::ostream & out)
{
  (void)msg;
  out << "null";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const LearnWeight_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  (void)msg;
  (void)indentation;
  out << "null\n";
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const LearnWeight_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace agv_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use agv_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const agv_interfaces::srv::LearnWeight_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  agv_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use agv_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const agv_interfaces::srv::LearnWeight_Request & msg)
{
  return agv_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<agv_interfaces::srv::LearnWeight_Request>()
{
  return "agv_interfaces::srv::LearnWeight_Request";
}

template<>
inline const char * name<agv_interfaces::srv::LearnWeight_Request>()
{
  return "agv_interfaces/srv/LearnWeight_Request";
}

template<>
struct has_fixed_size<agv_interfaces::srv::LearnWeight_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<agv_interfaces::srv::LearnWeight_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<agv_interfaces::srv::LearnWeight_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace agv_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const LearnWeight_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: weight
  {
    out << "weight: ";
    rosidl_generator_traits::value_to_yaml(msg.weight, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const LearnWeight_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: weight
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "weight: ";
    rosidl_generator_traits::value_to_yaml(msg.weight, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const LearnWeight_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace agv_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use agv_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const agv_interfaces::srv::LearnWeight_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  agv_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use agv_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const agv_interfaces::srv::LearnWeight_Response & msg)
{
  return agv_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<agv_interfaces::srv::LearnWeight_Response>()
{
  return "agv_interfaces::srv::LearnWeight_Response";
}

template<>
inline const char * name<agv_interfaces::srv::LearnWeight_Response>()
{
  return "agv_interfaces/srv/LearnWeight_Response";
}

template<>
struct has_fixed_size<agv_interfaces::srv::LearnWeight_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<agv_interfaces::srv::LearnWeight_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<agv_interfaces::srv::LearnWeight_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<agv_interfaces::srv::LearnWeight>()
{
  return "agv_interfaces::srv::LearnWeight";
}

template<>
inline const char * name<agv_interfaces::srv::LearnWeight>()
{
  return "agv_interfaces/srv/LearnWeight";
}

template<>
struct has_fixed_size<agv_interfaces::srv::LearnWeight>
  : std::integral_constant<
    bool,
    has_fixed_size<agv_interfaces::srv::LearnWeight_Request>::value &&
    has_fixed_size<agv_interfaces::srv::LearnWeight_Response>::value
  >
{
};

template<>
struct has_bounded_size<agv_interfaces::srv::LearnWeight>
  : std::integral_constant<
    bool,
    has_bounded_size<agv_interfaces::srv::LearnWeight_Request>::value &&
    has_bounded_size<agv_interfaces::srv::LearnWeight_Response>::value
  >
{
};

template<>
struct is_service<agv_interfaces::srv::LearnWeight>
  : std::true_type
{
};

template<>
struct is_service_request<agv_interfaces::srv::LearnWeight_Request>
  : std::true_type
{
};

template<>
struct is_service_response<agv_interfaces::srv::LearnWeight_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // AGV_INTERFACES__SRV__DETAIL__LEARN_WEIGHT__TRAITS_HPP_
