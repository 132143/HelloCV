// listener.cpp —— 话题订阅者 + 显式 QoS
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Listener : public rclcpp::Node
{
public:
  Listener() : Node("listener")
  {
    // 显式构造 QoS：深度 10、可靠传输
    rclcpp::QoS qos(10);
    qos.reliable();

    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "chatter", qos,
      std::bind(&Listener::on_msg, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "listener 已启动，等待消息…");
  }

private:
  void on_msg(const std_msgs::msg::String::SharedPtr msg)
  {
    RCLCPP_INFO(this->get_logger(), "收到: '%s'", msg->data.c_str());
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Listener>());
  rclcpp::shutdown();
  return 0;
}
