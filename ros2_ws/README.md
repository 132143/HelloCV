# ros2_ws —— 第三次任务（ROS2）

> 本目录属于**第三次任务 · ROS2**，与第二次任务的 `UnitConverter/`（CMake）、
> `skeleton/`（LLM 网关）互不相干。

## 内容

```text
ros2_ws/
└── src/
    └── my_robot_pkg/          # 唯一的 ROS2 功能包
        ├── CMakeLists.txt
        ├── package.xml
        ├── config/params.yaml
        ├── launch/demo.launch.py
        └── src/
            ├── talker.cpp          # 话题发布者 + 参数
            ├── listener.cpp        # 话题订阅者 + 显式 QoS
            ├── add_server.cpp      # 服务端
            ├── add_client.cpp      # 客户端
            └── tf_broadcaster.cpp  # TF 坐标变换广播
```

## 不提交的目录

`build/`、`install/`、`log/` 是 `colcon build` 自动生成的，已在仓库根 `.gitignore` 中排除。

## 快速开始

```bash
cd ~/ros2_ws
colcon build --symlink-install --packages-select my_robot_pkg
source install/setup.bash
ros2 launch my_robot_pkg demo.launch.py
```

## 各节点说明

| 节点 | 做什么 | 涉及知识点 |
|---|---|---|
| `talker` | 每 `rate_ms` 毫秒往 `chatter` 发一条字符串，内容带 `prefix` 参数 | `rclcpp::Publisher`、定时器、参数声明与动态读取 |
| `listener` | 订阅 `chatter`，打印收到的内容 | `rclcpp::Subscription`、`rclcpp::QoS` |
| `add_server` | 提供 `add_two_ints` 服务，返回两数之和 | `rclcpp::Service`、Client-Server 模型 |
| `add_client` | 调用 `add_two_ints`，发送 3 + 5 | `rclcpp::Client`、`wait_for_service`、`spin_until_future_complete` |
| `tf_broadcaster` | 每 100 ms 广播 `world → robot` 变换（平移 1.0/0.5/0，绕 Z 转 45°） | `tf2_ros::TransformBroadcaster`、四元数 |

## launch 里的三件事

`launch/demo.launch.py` 同时演示了原文要求的三项能力：

1. **一次启动多个节点**：talker、listener、add_server、tf_broadcaster 四个；
2. **从文件加载参数**：`parameters=[params_file]` 指向 `config/params.yaml`
   （所以 `ros2 param get /talker prefix` 返回 `hi-from-param`，`rate_ms` 变成 1000）；
3. **话题重映射**：`remappings=[('chatter', 'my_topic')]`
   （所以 `ros2 topic list` 里看到的是 `/my_topic`，不是 `/chatter`）。

> ⚠️ remap **必须两边同时改到同一个名字**，只改一边会导致收不到消息且不报错。

## 实测结果（供参考）

- 编译：`Summary: 1 package finished [35.0s]`，5 个可执行文件全部生成
- 话题频率：`ros2 topic hz /chatter` ≈ **2.015 Hz**（对应 `rate_ms=500`）
- 服务：`ros2 service call` 返回 `sum=8`；`add_client` 输出 `3 + 5 = 8`
- TF：`tf2_echo world robot` → 平移 `[1.000, 0.500, 0.000]`、RPY yaw `44.977°`
- TF 树图：`view_frames` 生成 `frames_<时间戳>.pdf`（Humble 的文件名带时间戳，
  不是 `frames.pdf`），图里 `world → robot`，平均 10.198 Hz
- launch：4 个节点齐启，`/my_topic` 出现，`param get /talker prefix` 返回 `hi-from-param`
