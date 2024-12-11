import rclpy
from rclpy.node import Node
from std_msgs.msg import Bool, Float32
import serial
from rclpy.executors import SingleThreadedExecutor
from agv_interfaces.srv import LearnWeight

class DeneyapSensorController(Node):
    def __init__(self):
        super().__init__('deneyap_sensor_controller')

        # Open and check serial connection
        try:
            self.serial_port = serial.Serial('/dev/sensor_deneyap', 9600, timeout=1)
            self.get_logger().info("deneyap_sensor_controller node has been started")
        except serial.SerialException as e:
            self.get_logger().error(f'Failed to establish deneyap_sensor_controller connection: {e}')
            rclpy.shutdown()
            return


        # Create publishers
        self.close_button_publisher = self.create_publisher(Bool, 'close_button_state', 10)
        self.start_button_publisher = self.create_publisher(Bool, 'start_button_state', 10)
        self.left_obstacle_publisher = self.create_publisher(Bool, 'left_obstacle', 10)
        self.right_obstacle_publisher = self.create_publisher(Bool, 'right_obstacle', 10)
        self.first_temperature_publisher = self.create_publisher(Float32, 'first_temperature', 10)
        self.second_temperature_publisher = self.create_publisher(Float32, 'second_temperature', 10)
        self.voltage_publisher = self.create_publisher(Float32, 'voltage', 10)
        self.current_publisher = self.create_publisher(Float32, 'current', 10)

        # Create service
        self.server_ = self.create_service(LearnWeight, "learn_weight", self.callback_learn_weight)
        self.weight_from_arduino = 0.0
        
        # Create timer
        self.create_timer(0.2, self.read_and_publish_data)

        self.serial_port.flush()    

    def callback_learn_weight(self, request, response):
        try:
            self.serial_port.write(b"measure_weight\n")
        except serial.SerialTimeoutException as e:
            self.get_logger().error(f'Failed to write to serial port: {e}')
            response.success = False
            return response

        while True:
            try:
                if self.serial_port.in_waiting > 0:
                    line = self.serial_port.readline().decode('utf-8').strip()
                    try:
                        self.weight_from_arduino = float(line)
                        response.weight = self.weight_from_arduino
                        response.success = True
                        self.get_logger().info(f'Weight received: {self.weight_from_arduino}')
                        return response
                    except ValueError:
                        self.get_logger().info(f'Received invalid weight data: {line}')
            except serial.SerialException as e:
                self.get_logger().error(f'Failed to read from serial port: {e}')
                response.success = False
                return response

    def read_and_publish_data(self):
        try:
            if self.serial_port.in_waiting > 0:
                line = self.serial_port.readline().decode('utf-8').strip()
                data = self.parse_data(line)
                if data is not None:
                    self.publish_data(data)
                else:
                    self.get_logger().warn(f"Invalid data format received: {line}")
        except serial.SerialException as e:
            self.get_logger().error(f'Failed to read from serial port: {e}')

    def parse_data(self, data_string):
        try:
            data_dict = {}
            for item in data_string.split(','):
                key, value = item.split(':')
                data_dict[key] = value
            return data_dict
        except ValueError:
            return None

    def publish_data(self, data):
        try:
            close_button_msg = Bool(data=bool(int(data['close_button_state'])))
            self.close_button_publisher.publish(close_button_msg)
            # self.get_logger().info(f"Published close_button_state: {close_button_msg.data}")

            start_button_msg = Bool(data=bool(int(data['start_button_state'])))
            self.start_button_publisher.publish(start_button_msg)
            # self.get_logger().info(f"Published start_button_state: {start_button_msg.data}")

            left_obstacle_msg = Bool(data=bool(int(data['left_distance'])))
            self.left_obstacle_publisher.publish(left_obstacle_msg)
            # self.get_logger().info(f"Published left_obstacle: {left_obstacle_msg.data}")

            right_obstacle_msg = Bool(data=bool(int(data['right_distance'])))
            self.right_obstacle_publisher.publish(right_obstacle_msg)
            # self.get_logger().info(f"Published right_obstacle: {right_obstacle_msg.data}")

            first_temperature_msg = Float32(data=float(data['first_temperature']))
            self.first_temperature_publisher.publish(first_temperature_msg)
            # self.get_logger().info(f"Published first_temperature: {first_temperature_msg.data}")

            second_temperature_msg = Float32(data=float(data['second_temperature']))
            second_temperature_msg.data = 27.4
            self.second_temperature_publisher.publish(second_temperature_msg)
            # self.get_logger().info(f"Published second_temperature: {second_temperature_msg.data}")

            voltage_msg = Float32(data=float(data['voltage']))
            self.voltage_publisher.publish(voltage_msg)
            # self.get_logger().info(f"Published voltage: {voltage_msg.data}")

            current_msg = Float32(data=float(data['current']))
            current_msg.data = 3.05
            self.current_publisher.publish(current_msg)
            # self.get_logger().info(f"Published current: {current_msg.data}")

            # self.get_logger().info("----------------------------------")
        except KeyError as e:
            self.get_logger().error(f"Missing key in data: {e}")
        except ValueError as e:
            self.get_logger().error(f"Invalid value in data: {e}")

def main(args=None):
    rclpy.init(args=args)
    node = DeneyapSensorController()
    executor = SingleThreadedExecutor()
    executor.add_node(node)
    executor.spin()
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()