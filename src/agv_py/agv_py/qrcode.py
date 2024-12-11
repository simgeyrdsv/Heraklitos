import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import serial

class GM65Node(Node):
    def __init__(self):
        super().__init__('gm65_node')
        self.publisher_ = self.create_publisher(String, 'qr_code', 10)
        
        # Serial port setup
        try:
            self.ser = serial.Serial('/dev/qr_code', 9600, timeout=1)  # Configure serial port and baud rate
        except serial.SerialException as e:
            self.get_logger().error(f'Error opening serial port: {str(e)}')
            self.destroy_node()
            return
        self.get_logger().info("QrCode scanner initilaze ... ")
        # Timer setup
        self.timer = self.create_timer(0.3, self.read_barcode)  # 500 ms loop

        self.previous_barcode_data = ""

    def read_barcode(self):# İlk deneme için
        try:
            if self.ser.in_waiting > 0:
                barcode_data = self.ser.readline().decode('utf-8').strip()
                if barcode_data and barcode_data is not self.previous_barcode_data:
                    self.previous_barcode_data = barcode_data 
                    msg = String()
                    msg.data = barcode_data
                    self.publisher_.publish(msg)
                    self.get_logger().info(f'Barcode data: {barcode_data}')
        except serial.SerialException as e:
            self.get_logger().error(f'Error reading from serial port: {str(e)}')

    def destroy_node(self):
        # Close the serial connection when the node is destroyed
        if self.ser.is_open:
            self.ser.close()
        super().destroy_node()

def main(args=None):
    rclpy.init(args=args)
    node = GM65Node()
    if node:
        try:
            rclpy.spin(node)
        except KeyboardInterrupt:
            pass
        finally:
            node.destroy_node()
            rclpy.shutdown()

if __name__ == '__main__':
    main()
