import rclpy
from rclpy.node import Node
import time
# ROS2节点主入口main函数
def main(args=None):
    # ROS2 Python接口初始化
    rclpy.init(args=args)
    # 创建ROS2节点对象并进行初始化
    node = Node("node_helloworld")
    
    while rclpy.ok():
        # ROS2日志输出
        node.get_logger().info("Hello World")
        # 休眠控制循环时间
        time.sleep(0.5)
    
    node.destroy_node()
    rclpy.shutdown()
