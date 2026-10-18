// add_client.cpp —— 客户端
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using AddTwoInts = example_interfaces::srv::AddTwoInts;
using namespace std::chrono_literals;

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("add_client");
  auto client = node->create_client<AddTwoInts>("add_two_ints");

  // ① 等服务端上线，最多等 5 次 * 1 秒
  int tries = 0;
  while (!client->wait_for_service(1s)) {
    if (++tries > 5) {
      RCLCPP_ERROR(node->get_logger(), "等不到服务端，退出");
      rclcpp::shutdown();
      return 1;
    }
    RCLCPP_INFO(node->get_logger(), "等待服务端…(%d)", tries);
  }

  // ② 造请求
  auto req = std::make_shared<AddTwoInts::Request>();
  req->a = 3;
  req->b = 5;

  // ③ 发请求并等结果
  auto future = client->async_send_request(req);
  if (rclcpp::spin_until_future_complete(node, future) ==
      rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(node->get_logger(), "结果: 3 + 5 = %ld", future.get()->sum);
  } else {
    RCLCPP_ERROR(node->get_logger(), "服务调用失败");
  }

  rclcpp::shutdown();
  return 0;
}
