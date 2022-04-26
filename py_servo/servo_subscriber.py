import rclpy 
import pigpio 
from rclpy.node import Node

from std_msgs.msg import Int16

SERVO_PIN = 22 
pi = pigpio.pi()

class ServoSubscriber (Node):
	def __init__(self):
		super(). __init__('servo_subscriber')
		self.subscription = self.create_subscription(
			Int16, 
			'servo_topic', 
			self.listener_callback,
			10)
		self. Subscription

	def listener_callback(self, msg):
		self.get_logger().info('Subscribed, "%d"' % msg.data) 
		p_width = msg.data 
		pi.set_servo_pulsewidth(SERVO_PIN, p_width)

def main(args=None): 
	try:
		rclpy.init(args=args)
		servo_subscriber = ServoSubscriber()
		rclpy.spin(servo_subscriber)
	except KeyboardInterrupt:
		pass 
	finally:
		servo_subscriber.destroy_node() 
	rclpy.shutdown()
	
If __name__ == '__main__': 
    main()

