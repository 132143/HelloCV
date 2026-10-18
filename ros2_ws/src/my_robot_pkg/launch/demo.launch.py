# demo.launch.py —— 一次启动 talker + listener + add_server + tf_broadcaster
import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    pkg_share = get_package_share_directory('my_robot_pkg')
    params_file = os.path.join(pkg_share, 'config', 'params.yaml')

    talker = Node(
        package='my_robot_pkg',
        executable='talker',
        name='talker',
        output='screen',
        parameters=[params_file],
        remappings=[('chatter', 'my_topic')],
    )

    listener = Node(
        package='my_robot_pkg',
        executable='listener',
        name='listener',
        output='screen',
        remappings=[('chatter', 'my_topic')],
    )

    add_server = Node(
        package='my_robot_pkg',
        executable='add_server',
        name='add_server',
        output='screen',
    )

    tf_broadcaster = Node(
        package='my_robot_pkg',
        executable='tf_broadcaster',
        name='tf_broadcaster',
        output='screen',
    )

    return LaunchDescription([talker, listener, add_server, tf_broadcaster])
