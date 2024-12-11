#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import socket
import numpy as np
from nav_msgs.msg import OccupancyGrid
import threading

class MapToTcpClient(Node):
    def __init__(self):
        super().__init__('map_to_tcp_client')
        
        # IP adresi ve port parametrelerini tanımlayın
        self.declare_parameter('ip_address', '0.0.0.0')
        self.declare_parameter('port', 65432)

        self.ip_address = self.get_parameter('ip_address').get_parameter_value().string_value
        self.port = self.get_parameter('port').get_parameter_value().integer_value

        # TCP sunucu soketini oluştur ve başlat
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind((self.ip_address, self.port))
        self.sock.listen(1)
        self.get_logger().info(f'Server listening on {self.ip_address}:{self.port}')

        # Client bağlantısı için değişkenler
        self.client_conn, self.client_addr = None, None
        self.conn_lock = threading.Lock()

        # Bağlantı kabul eden bir thread başlat
        self.accept_thread = threading.Thread(target=self.accept_connection)
        self.accept_thread.start()
        
        # /map konusuna abone olun
        self.subscription = self.create_subscription(
            OccupancyGrid,  # Humble ile uyumlu nav_msgs/OccupancyGrid mesaj türü
            '/map',
            self.map_callback,
            10
        )

    def accept_connection(self):
        """Yeni bir istemci bağlantısını kabul et."""
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


    def destroy_node(self):
        """Close the socket when the node is closed."""
        if self.client_conn:
            self.client_conn.close()
        super().destroy_node()

def main(args=None):
    rclpy.init(args=args)
    map_to_tcp_client = MapToTcpClient()

    try:
        rclpy.spin(map_to_tcp_client)
    except KeyboardInterrupt:
        pass
    finally:
        map_to_tcp_client.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
