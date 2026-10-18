// add_server.cpp —— 服务端
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using AddTwoInts = example_interfaces::srv::AddTwoInts;

class AddServer : public rclcpp::Node
{
public:
  AddServer() : Node("add_server")
  {
    service_ = this->create_service<AddTwoInts>(
      "add_two_ints",
      std::bind(&AddServer::handle, this,
                std::placeholders::_1,    // Request
                std::placeholders::_2));  // Response

    RCLCPP_INFO(this->get_logger(), "服务 add_two_ints 已就绪");
  }

private:
  void handle(const std::shared_ptr<AddTwoInts::Request> req,
              std::shared_ptr<AddTwoInts::Response> res)
  {
    res->sum = req->a + req->b;
    RCLCPP_INFO(this->get_logger(), "收到请求: %ld + %ld = %ld",
                req->a, req->b, res->sum);
  }

  rclcpp::Service<AddTwoInts>::SharedPtr service_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AddServer>());
  rclcpp::shutdown();
  return 0;
}
