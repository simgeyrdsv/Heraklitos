import rclpy
from rclpy.node import Node
from agv_interfaces.srv import Command
from std_msgs.msg import Bool, Float32, String
import socket
import numpy as np
from nav_msgs.msg import OccupancyGrid
import threading
import select

class WebInterfaceServerNode(Node):
    def __init__(self):
        super().__init__('web_interface_server_node')
        self.declare_parameter('ip_address', '0.0.0.0')
        self.declare_parameter('port', 65432)

        self.ip_address = self.get_parameter('ip_address').get_parameter_value().string_value
        self.port = self.get_parameter('port').get_parameter_value().integer_value

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind((self.ip_address, self.port))
        self.sock.listen(1)
        self.get_logger().info(f'Server listening on {self.ip_address}:{self.port}')

        self.client_conn, self.client_addr = None, None
        self.conn_lock = threading.Lock()

        self.accept_thread = threading.Thread(target=self.accept_connection)
        self.accept_thread.start()

        # Subscribers
        self.close_button_state = True
        self.start_button_state = False
        self.left_distance = True
        self.right_distance = True
        self.first_temperature = 0.0
        self.second_temperature = 0.0
        self.voltage = 0.0
        self.current = 0.0
        self.qr_code = ""

        self.create_subscription(Bool, 'close_button_state', self.close_button_state_callback, 10)
        self.create_subscription(Bool, 'start_button_state', self.start_button_state_callback, 10)
        self.create_subscription(Bool, 'left_distance', self.left_distance_callback, 10)
        self.create_subscription(Bool, 'right_distance', self.right_distance_callback, 10)
        self.create_subscription(Float32, 'first_temperature', self.first_temperature_callback, 10)
        self.create_subscription(Float32, 'second_temperature', self.second_temperature_callback, 10)
        self.create_subscription(Float32, 'voltage', self.voltage_callback, 10)
        self.create_subscription(Float32, 'current', self.current_callback, 10)
        self.create_subscription(String, 'qr_code', self.qr_code_callback, 10)

        self.create_timer(1.0, self.send_sensor_data)
        self.create_timer(0.1, self.check_for_commands)

        self.subscription = self.create_subscription(
            OccupancyGrid,
            '/map',
            self.map_callback,
            10
        )

        # Create a single Command client
        self.command_client = self.create_client(Command, 'command')

    def accept_connection(self):
        while rclpy.ok():
            try:
                client_conn, client_addr = self.sock.accept()
                with self.conn_lock:
                    if self.client_conn:
                        self.client_conn.close()
                    self.client_conn = client_conn
                    self.client_addr = client_addr
                self.get_logger().info(f'Connection from {client_addr}')
            except Exception as e:
                self.get_logger().error(f'Error accepting connection: {e}')

    def close_button_state_callback(self, msg):
        self.close_button_state = msg.data

    def start_button_state_callback(self, msg):
        self.start_button_state = msg.data

    def left_distance_callback(self, msg):
        self.left_distance = msg.data

    def right_distance_callback(self, msg):
        self.right_distance = msg.data

    def first_temperature_callback(self, msg):
        self.first_temperature = msg.data

    def second_temperature_callback(self, msg):
        self.second_temperature = msg.data

    def voltage_callback(self, msg):
        self.voltage = msg.data

    def current_callback(self, msg):
        self.current = msg.data

    def qr_code_callback(self, msg):
        self.qr_code = msg.data

    def send_sensor_data(self):
        with self.conn_lock:
            if self.client_conn:
                try:
                    data = (
                        f'close_button_state:{self.close_button_state},'
                        f'start_button_state:{self.start_button_state},'
                        f'left_distance:{self.left_distance},'
                        f'right_distance:{self.right_distance},'
                        f'first_temperature:{self.first_temperature:.2f},'
                        f'second_temperature:{self.second_temperature:.2f},'
                        f'voltage:{self.voltage:.2f},'
                        f'current:{self.current:.2f},'
                        f'qr_code:{self.qr_code}'
                    )
                    self.client_conn.sendall(data.encode('utf-8'))
                    self.get_logger().info(f'Sent data: {data}')
                except (BrokenPipeError, ConnectionResetError) as e:
                    self.get_logger().error(f'Client disconnected: {e}')
                    self.client_conn.close()
                    self.client_conn = None
            else:
                self.get_logger().info('No client connection, waiting for client...')

    def check_for_commands(self):
        with self.conn_lock:
            if self.client_conn:
                try:
                    ready, _, _ = select.select([self.client_conn], [], [], 0)
                    if ready:
                        message = self.client_conn.recv(1024).decode('utf-8')
                        if message:
                            self.get_logger().info(f'Received command: {message}')
                            self.handle_command(message.strip())
                except (BrokenPipeError, ConnectionResetError) as e:
                    self.get_logger().error(f'Client disconnected: {e}')
                    self.client_conn.close()
                    self.client_conn = None

    def listen_for_commands(self):
        self.get_logger().error('Gelen veri dinleniyor')
        with self.conn_lock:
            if self.client_conn:
                try:
                    message = self.client_conn.recv(1024).decode('utf-8')
                    if message:
                        self.get_logger().info(f'Received command: {message}')
                        self.handle_command(message.strip())
                except (BrokenPipeError, ConnectionResetError) as e:
                    self.get_logger().error(f'Client disconnected: {e}')
                    self.client_conn.close()  # Bağlantıyı kapat
                    self.client_conn = None  # Bağlantıyı temizle
            else:
                self.get_logger().error('No client connection')

    def handle_command(self, command):
        valid_commands = {
            'forward', 'backward', 'turn_left', 'turn_right', 'start_button', 
            'close_button', 'stop', 'emergency_stop_on', 'emergency_stop_off'
        }
        if command in valid_commands:
            # self.get_logger().info(f'Received command: {command}')
            self.send_command_request(command)

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

    def map_callback(self, msg):
        """Processes and sends map data from the subscribed topic."""
        if self.client_conn:
            try:
                # Convert map data to PGM format
                pgm_data = self.convert_to_pgm(msg)
                
                # Send PGM data over TCP/IP
                self.client_conn.sendall(pgm_data)
                
                # Print PGM data to terminal (optional)
                self.get_logger().info(f"Sending PGM data of size: {len(pgm_data)} bytes")

            except Exception as e:
                self.get_logger().error(f"An error occurred while sending data: {e}")
                with self.conn_lock:
                    self.client_conn.close()
                    self.client_conn = None  # Clear socket to try reconnecting in case of connection failure

    def convert_to_pgm(self, msg):
        """Convert OccupancyGrid message to PGM format."""
        width = msg.info.width
        height = msg.info.height
        data = np.array(msg.data).reshape((height, width))

        # Let's make the unknown values ​​(e.g. -1) a midtone gray (127)
        data = np.where(data == -1, 127, data)

        # Convert values ​​in the range 0-100 to 0-255
        data = np.interp(data, (0, 100), (255, 0))

        # Convert data to uint8 type
        data = data.astype(np.uint8)

        # PGM title
        header = f"P5\n{width} {height}\n255\n".encode()

        # Combine PGM data
        pgm_data = header + data.tobytes()

        return pgm_data


def main(args=None):
    rclpy.init(args=args)
    node = WebInterfaceServerNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('Server shutting down...')
    finally:
        with node.conn_lock:
            if node.client_conn:
                node.client_conn.close()
        node.sock.close()
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main() 	