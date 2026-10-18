// tf_broadcaster.cpp —— 发布 world -> robot 的坐标变换
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2/LinearMath/Quaternion.h"
#include "geometry_msgs/msg/transform_stamped.hpp"

class TfBroadcaster : public rclcpp::Node
{
public:
  TfBroadcaster() : Node("tf_broadcaster")
  {
    broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(100),
      std::bind(&TfBroadcaster::on_timer, this));
    RCLCPP_INFO(this->get_logger(), "开始广播 world -> robot 变换");
  }

private:
  void on_timer()
  {
    geometry_msgs::msg::TransformStamped t;
    t.header.stamp = this->now();     // 必须用当前时间
    t.header.frame_id = "world";      // 父坐标系
    t.child_frame_id = "robot";       // 子坐标系

    t.transform.translation.x = 1.0;
    t.transform.translation.y = 0.5;
    t.transform.translation.z = 0.0;

    tf2::Quaternion q;
    q.setRPY(0.0, 0.0, 0.785);        // 绕 Z 轴转 45 度
    t.transform.rotation.x = q.x();
    t.transform.rotation.y = q.y();
    t.transform.rotation.z = q.z();
    t.transform.rotation.w = q.w();

    broadcaster_->sendTransform(t);
  }

  std::unique_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TfBroadcaster>());
  rclcpp::shutdown();
  return 0;
}
