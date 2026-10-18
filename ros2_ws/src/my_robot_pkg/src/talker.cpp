// talker.cpp —— 话题发布者 + 参数示例
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Talker : public rclcpp::Node
{
public:
  Talker() : Node("talker")
  {
    // ---------- ① 声明参数（带默认值）----------
    this->declare_parameter<std::string>("prefix", "hello");
    this->declare_parameter<int>("rate_ms", 500);

    // ---------- ② 创建发布者 ----------
    // 第二个参数是 QoS：默认 可靠 + 深度 10
    publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);

    // ---------- ③ 创建定时器 ----------
    int period = static_cast<int>(this->get_parameter("rate_ms").as_int());
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(period),
      std::bind(&Talker::on_timer, this));

    RCLCPP_INFO(this->get_logger(), "talker 启动，发布间隔 %d ms", period);
  }

private:
  void on_timer()
  {
    auto msg = std_msgs::msg::String();
    // 每次都现读 prefix，所以 ros2 param set 之后下一条就能看到变化
    msg.data = this->get_parameter("prefix").as_string() +
               " #" + std::to_string(count_++);
    publisher_->publish(msg);
    RCLCPP_INFO(this->get_logger(), "发布: '%s'", msg.data.c_str());
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_ = 0;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Talker>());
  rclcpp::shutdown();
  return 0;
}
