import rclpy 
from rclpy.node import Node
from std_msgs.msg import Int16

class ServoPublisher (Node):
	def __init__(self):
		super(). __init__('servo_publisher') 
		selt.publisher_ = self.create_publisher(Int16, 'servo_topic',10) 
		timer-period = 3 
		selt.timer = self.create timer (timer_period, self.timer_catllback) 
		self.i = 500
	def timer_callback(self):
		msg = Int16() 
		msg.data = self.i 
		self.publisher_.publish(msg) 
		self.get_logger().info('Publishing, "%d" ' % msg.data)
		if self.i ==2500:
		self.i = 500 else:
		self.i += 100

def main(args=None):
	try:
		rclpy.init(args%3Dargs)
		servo publisher = ServoPublisher
		rclpy.spin(servo_publisher) 
	except KeyboardInterrupt:
		pass 
	finally:
		servo_publisher.destroy_node() 
		rclpy.shutdown()

if __name__ == '__main__':
    main()

