// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from agv_interfaces:srv/LearnWeight.idl
// generated code does not contain a copyright notice

#ifndef AGV_INTERFACES__SRV__DETAIL__LEARN_WEIGHT__BUILDER_HPP_
#define AGV_INTERFACES__SRV__DETAIL__LEARN_WEIGHT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "agv_interfaces/srv/detail/learn_weight__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace agv_interfaces
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::agv_interfaces::srv::LearnWeight_Request>()
{
  return ::agv_interfaces::srv::LearnWeight_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace agv_interfaces


namespace agv_interfaces
{

namespace srv
{

namespace builder
{

class Init_LearnWeight_Response_weight
{
public:
  explicit Init_LearnWeight_Response_weight(::agv_interfaces::srv::LearnWeight_Response & msg)
  : msg_(msg)
  {}
  ::agv_interfaces::srv::LearnWeight_Response weight(::agv_interfaces::srv::LearnWeight_Response::_weight_type arg)
  {
    msg_.weight = std::move(arg);
    return std::move(msg_);
  }

private:
  ::agv_interfaces::srv::LearnWeight_Response msg_;
};

class Init_LearnWeight_Response_success
{
public:
  Init_LearnWeight_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LearnWeight_Response_weight success(::agv_interfaces::srv::LearnWeight_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_LearnWeight_Response_weight(msg_);
  }

private:
  ::agv_interfaces::srv::LearnWeight_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::agv_interfaces::srv::LearnWeight_Response>()
{
  return agv_interfaces::srv::builder::Init_LearnWeight_Response_success();
}

}  // namespace agv_interfaces

#endif  // AGV_INTERFACES__SRV__DETAIL__LEARN_WEIGHT__BUILDER_HPP_
