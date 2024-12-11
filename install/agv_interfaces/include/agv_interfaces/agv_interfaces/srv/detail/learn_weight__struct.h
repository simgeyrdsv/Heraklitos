// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from agv_interfaces:srv/LearnWeight.idl
// generated code does not contain a copyright notice

#ifndef AGV_INTERFACES__SRV__DETAIL__LEARN_WEIGHT__STRUCT_H_
#define AGV_INTERFACES__SRV__DETAIL__LEARN_WEIGHT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/LearnWeight in the package agv_interfaces.
typedef struct agv_interfaces__srv__LearnWeight_Request
{
  uint8_t structure_needs_at_least_one_member;
} agv_interfaces__srv__LearnWeight_Request;

// Struct for a sequence of agv_interfaces__srv__LearnWeight_Request.
typedef struct agv_interfaces__srv__LearnWeight_Request__Sequence
{
  agv_interfaces__srv__LearnWeight_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} agv_interfaces__srv__LearnWeight_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/LearnWeight in the package agv_interfaces.
typedef struct agv_interfaces__srv__LearnWeight_Response
{
  bool success;
  float weight;
} agv_interfaces__srv__LearnWeight_Response;

// Struct for a sequence of agv_interfaces__srv__LearnWeight_Response.
typedef struct agv_interfaces__srv__LearnWeight_Response__Sequence
{
  agv_interfaces__srv__LearnWeight_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} agv_interfaces__srv__LearnWeight_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // AGV_INTERFACES__SRV__DETAIL__LEARN_WEIGHT__STRUCT_H_
