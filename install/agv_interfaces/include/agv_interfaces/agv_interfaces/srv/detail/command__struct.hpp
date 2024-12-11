// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from agv_interfaces:srv/Command.idl
// generated code does not contain a copyright notice

#ifndef AGV_INTERFACES__SRV__DETAIL__COMMAND__STRUCT_HPP_
#define AGV_INTERFACES__SRV__DETAIL__COMMAND__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__agv_interfaces__srv__Command_Request __attribute__((deprecated))
#else
# define DEPRECATED__agv_interfaces__srv__Command_Request __declspec(deprecated)
#endif

namespace agv_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Command_Request_
{
  using Type = Command_Request_<ContainerAllocator>;

  explicit Command_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->command = "";
    }
  }

  explicit Command_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : command(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->command = "";
    }
  }

  // field types and members
  using _command_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _command_type command;

  // setters for named parameter idiom
  Type & set__command(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->command = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    agv_interfaces::srv::Command_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const agv_interfaces::srv::Command_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<agv_interfaces::srv::Command_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<agv_interfaces::srv::Command_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      agv_interfaces::srv::Command_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<agv_interfaces::srv::Command_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      agv_interfaces::srv::Command_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<agv_interfaces::srv::Command_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<agv_interfaces::srv::Command_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<agv_interfaces::srv::Command_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__agv_interfaces__srv__Command_Request
    std::shared_ptr<agv_interfaces::srv::Command_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__agv_interfaces__srv__Command_Request
    std::shared_ptr<agv_interfaces::srv::Command_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Command_Request_ & other) const
  {
    if (this->command != other.command) {
      return false;
    }
    return true;
  }
  bool operator!=(const Command_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Command_Request_

// alias to use template instance with default allocator
using Command_Request =
  agv_interfaces::srv::Command_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace agv_interfaces


#ifndef _WIN32
# define DEPRECATED__agv_interfaces__srv__Command_Response __attribute__((deprecated))
#else
# define DEPRECATED__agv_interfaces__srv__Command_Response __declspec(deprecated)
#endif

namespace agv_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Command_Response_
{
  using Type = Command_Response_<ContainerAllocator>;

  explicit Command_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  explicit Command_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    agv_interfaces::srv::Command_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const agv_interfaces::srv::Command_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<agv_interfaces::srv::Command_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<agv_interfaces::srv::Command_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      agv_interfaces::srv::Command_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<agv_interfaces::srv::Command_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      agv_interfaces::srv::Command_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<agv_interfaces::srv::Command_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<agv_interfaces::srv::Command_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<agv_interfaces::srv::Command_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__agv_interfaces__srv__Command_Response
    std::shared_ptr<agv_interfaces::srv::Command_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__agv_interfaces__srv__Command_Response
    std::shared_ptr<agv_interfaces::srv::Command_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Command_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    return true;
  }
  bool operator!=(const Command_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Command_Response_

// alias to use template instance with default allocator
using Command_Response =
  agv_interfaces::srv::Command_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace agv_interfaces

namespace agv_interfaces
{

namespace srv
{

struct Command
{
  using Request = agv_interfaces::srv::Command_Request;
  using Response = agv_interfaces::srv::Command_Response;
};

}  // namespace srv

}  // namespace agv_interfaces

#endif  // AGV_INTERFACES__SRV__DETAIL__COMMAND__STRUCT_HPP_
