// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from agv_interfaces:srv/Command.idl
// generated code does not contain a copyright notice

#ifndef AGV_INTERFACES__SRV__DETAIL__COMMAND__STRUCT_H_
#define AGV_INTERFACES__SRV__DETAIL__COMMAND__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'command'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/Command in the package agv_interfaces.
typedef struct agv_interfaces__srv__Command_Request
{
  rosidl_runtime_c__String command;
} agv_interfaces__srv__Command_Request;

// Struct for a sequence of agv_interfaces__srv__Command_Request.
typedef struct agv_interfaces__srv__Command_Request__Sequence
{
  agv_interfaces__srv__Command_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} agv_interfaces__srv__Command_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/Command in the package agv_interfaces.
typedef struct agv_interfaces__srv__Command_Response
{
  bool success;
} agv_interfaces__srv__Command_Response;

// Struct for a sequence of agv_interfaces__srv__Command_Response.
typedef struct agv_interfaces__srv__Command_Response__Sequence
{
  agv_interfaces__srv__Command_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} agv_interfaces__srv__Command_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // AGV_INTERFACES__SRV__DETAIL__COMMAND__STRUCT_H_
