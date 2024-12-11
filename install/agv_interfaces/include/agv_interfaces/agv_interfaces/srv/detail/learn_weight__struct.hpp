// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from agv_interfaces:srv/LearnWeight.idl
// generated code does not contain a copyright notice

#ifndef AGV_INTERFACES__SRV__DETAIL__LEARN_WEIGHT__STRUCT_HPP_
#define AGV_INTERFACES__SRV__DETAIL__LEARN_WEIGHT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__agv_interfaces__srv__LearnWeight_Request __attribute__((deprecated))
#else
# define DEPRECATED__agv_interfaces__srv__LearnWeight_Request __declspec(deprecated)
#endif

namespace agv_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct LearnWeight_Request_
{
  using Type = LearnWeight_Request_<ContainerAllocator>;

  explicit LearnWeight_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  explicit LearnWeight_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->structure_needs_at_least_one_member = 0;
    }
  }

  // field types and members
  using _structure_needs_at_least_one_member_type =
    uint8_t;
  _structure_needs_at_least_one_member_type structure_needs_at_least_one_member;


  // constant declarations

  // pointer types
  using RawPtr =
    agv_interfaces::srv::LearnWeight_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const agv_interfaces::srv::LearnWeight_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<agv_interfaces::srv::LearnWeight_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<agv_interfaces::srv::LearnWeight_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      agv_interfaces::srv::LearnWeight_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<agv_interfaces::srv::LearnWeight_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      agv_interfaces::srv::LearnWeight_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<agv_interfaces::srv::LearnWeight_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<agv_interfaces::srv::LearnWeight_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<agv_interfaces::srv::LearnWeight_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__agv_interfaces__srv__LearnWeight_Request
    std::shared_ptr<agv_interfaces::srv::LearnWeight_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__agv_interfaces__srv__LearnWeight_Request
    std::shared_ptr<agv_interfaces::srv::LearnWeight_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const LearnWeight_Request_ & other) const
  {
    if (this->structure_needs_at_least_one_member != other.structure_needs_at_least_one_member) {
      return false;
    }
    return true;
  }
  bool operator!=(const LearnWeight_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct LearnWeight_Request_

// alias to use template instance with default allocator
using LearnWeight_Request =
  agv_interfaces::srv::LearnWeight_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace agv_interfaces


#ifndef _WIN32
# define DEPRECATED__agv_interfaces__srv__LearnWeight_Response __attribute__((deprecated))
#else
# define DEPRECATED__agv_interfaces__srv__LearnWeight_Response __declspec(deprecated)
#endif

namespace agv_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct LearnWeight_Response_
{
  using Type = LearnWeight_Response_<ContainerAllocator>;

  explicit LearnWeight_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->weight = 0.0f;
    }
  }

  explicit LearnWeight_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->weight = 0.0f;
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;
  using _weight_type =
    float;
  _weight_type weight;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__weight(
    const float & _arg)
  {
    this->weight = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    agv_interfaces::srv::LearnWeight_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const agv_interfaces::srv::LearnWeight_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<agv_interfaces::srv::LearnWeight_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<agv_interfaces::srv::LearnWeight_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      agv_interfaces::srv::LearnWeight_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<agv_interfaces::srv::LearnWeight_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      agv_interfaces::srv::LearnWeight_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<agv_interfaces::srv::LearnWeight_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<agv_interfaces::srv::LearnWeight_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<agv_interfaces::srv::LearnWeight_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__agv_interfaces__srv__LearnWeight_Response
    std::shared_ptr<agv_interfaces::srv::LearnWeight_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__agv_interfaces__srv__LearnWeight_Response
    std::shared_ptr<agv_interfaces::srv::LearnWeight_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const LearnWeight_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    if (this->weight != other.weight) {
      return false;
    }
    return true;
  }
  bool operator!=(const LearnWeight_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct LearnWeight_Response_

// alias to use template instance with default allocator
using LearnWeight_Response =
  agv_interfaces::srv::LearnWeight_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace agv_interfaces

namespace agv_interfaces
{

namespace srv
{

struct LearnWeight
{
  using Request = agv_interfaces::srv::LearnWeight_Request;
  using Response = agv_interfaces::srv::LearnWeight_Response;
};

}  // namespace srv

}  // namespace agv_interfaces

#endif  // AGV_INTERFACES__SRV__DETAIL__LEARN_WEIGHT__STRUCT_HPP_
