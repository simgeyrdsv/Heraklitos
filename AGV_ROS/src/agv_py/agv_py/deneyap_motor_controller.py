import rclpy
from rclpy.node import Node
from agv_interfaces.srv import Command
import serial
from rclpy.executors import SingleThreadedExecutor

class DeneyapMotorController(Node):

    def __init__(self):
        super().__init__('deneyap_motor_controller')
        try:
            self.serial_port = serial.Serial('/dev/motor_deneyap', 9600, timeout=1)
            self.get_logger().info("Deneyap motor controller node has been started")
        except serial.SerialException as e:
            self.get_logger().error(f'Failed to open serial port: {e}')
            raise e

        # Create a single service for handling all commands
        self.create_service(Command, 'send_command', self.handle_command)
        self.command_client = self.create_client(Command, 'command')

        # Add a timer to periodically check for incoming serial messages
        self.timer = self.create_timer(0.1, self.check_serial)

    def send_command_request(self, command):
        if not self.command_client.wait_for_service(timeout_sec=1.0):
            self.get_logger().error(f'Service {self.command_client.srv_name} not available')
            return

        request = Command.Request()
        request.command = command
        future = self.command_client.call_async(request)
        future.add_done_callback(
            lambda future: self.get_logger().info(f'Service {self.command_client.srv_name} call completed')
        )

    def handle_command(self, request, response):
        command = request.command

        try:
            self.serial_port.write(f'{command}\n'.encode())
            # self.get_logger().info(f'Sent command: {command}')

            # Wait for the specific response directly in this method
            while True:
                try:
                    if self.serial_port.in_waiting > 0:
                        received_data = self.serial_port.readline().decode('utf-8').strip()
                        # self.get_logger().info(f'Received: {received_data}')    

                        if received_data == f'{command} message processed':
                            response.success = True
                            self.get_logger().info(f'Response for command {command} received successfully')
                            return response
                except UnicodeDecodeError as e:
                    self.get_logger().error(f'Failed to decode received data: {e}')
                    response.success = False
                    return response
                except serial.SerialException as e:
                    self.get_logger().error(f'Serial port error: {e}')
                    response.success = False
                    return response
        except serial.SerialTimeoutException as e:
            self.get_logger().error(f'Failed to write to serial port: {e}')
            response.success = False
        except Exception as e:
            self.get_logger().error(f'Exception occurred: {e}')
            response.success = False

        return response

    def check_serial(self):
        if self.serial_port.in_waiting:
            try:
                received_data = self.serial_port.readline().decode('utf-8').strip()
                if received_data:
                    if(received_data == "there_is_a_return"):                        
                        self.send_command_request(received_data)
                    elif(received_data == "high_weight"):                        
                        self.send_command_request(received_data)
                    elif(received_data == "weight_ok"):                        
                        self.send_command_request(received_data)
                    else:
                        self.get_logger().info(f'Received and published: {received_data}')
            except UnicodeDecodeError as e:
                self.get_logger().error(f'Failed to decode received data: {e}')
            except serial.SerialException as e:
                self.get_logger().error(f'Serial port error: {e}')


def main(args=None):
    rclpy.init(args=args)
    node = DeneyapMotorController()
    executor = SingleThreadedExecutor()
    executor.add_node(node)
    executor.spin()
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()