from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
 ld = LaunchDescription()

 deneyap_sensor_controller_node = Node(
  package = "agv_py",
  executable = "deneyap_sensor_controller"
 )
 deneyap_motor_controller_node = Node(
  package = "agv_py",
  executable = "deneyap_motor_controller"
 )
 main_node = Node(
  package = "agv_cpp",
  executable = "main_node"
 )
 qrcode_node = Node(
  package = "agv_py",
  executable = "qrcode"
 )

#  web_interface = Node(
#   package = "agv_py",
#   executable = "web_interface"
#  )


 ld.add_action(main_node)
 ld.add_action(deneyap_sensor_controller_node)
 ld.add_action(deneyap_motor_controller_node)
 ld.add_action(qrcode_node)
#  ld.add_action(web_interface)

 return ld