# 3 Simulation in ROS

  本章介绍ROS中的仿真,以一个二维移动机器人仿真器开始,扩展到Gazebo(一个强大的动力学仿真器),并介绍用于建模机器人的URDF.

## 3.1 简单的二维机器人仿真器

  ROS仿真器例程:Simple Two-Dimensional Robot simulator--STDR,先安装该功能包:

  sudo apt-get install ros-$ROS_DISTRO-stdr-simulator

  使用以下命令启动STDR:

  roslaunch stdr_launchers server_with_map_and_gui_plus_robot.launch

  以该仿真器,可开发代码来解释传感器信号,避免碰撞并辨识机器人在空间的位置.控制该机器人的主题是/robot0/cmd_vel,以速度模式控制该机器人.

  查看该主题:

  rostopic info /robot0/cmd_vel

  Type: geometry_msgs/Twist

  Publishers: None

  Subscribers:

  * /robot_manager (http://albert-ubuntu16:41299/)

  * /stdr_gui_node_albert_ubuntu16_27265_877689102326918544 (http://albert-ubuntu16:35985/)

  查看该主题消息类型:

  rosmsg show geometry_msgs/Twist

  geometry_msgs/Vector3 linear

    float64 x

    float64 y

    float64 z

  geometry_msgs/Vector3 angular

    float64 x

    float64 y

    float64 z

  该Twist消息包含两个向量:速度向量(x,y,z分量), 角速度向量(x,y,z分量).对于该二维移动机器人,其只能前向运动和绕其中心旋转,对应于线速度的x分量和角速度的z分量.

  可在终端手工发布命令:

  rostopic pub -r 2 /robot0/cmd_vel geometry_msgs/Twist '{linear: {x: 0.5, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.0}}'

  上述命令使得该机器人以0.5m/s的速度向前移动,直到遇到障碍物.上述Twist命令以2Hz的频率发布,即-r 2.但是即使该rostopic pub命令被停止后,该机器人仍以最后接收到的Twist命令尝试向前运动.若要使得机器人停止,应发布速度为零的Twist命令.

  在ASystematicApproachtoLearningRobotProgrammingwithROS下新建文件夹Part_2.

  在Part_2下新建功能包stdr_control:

  cs_create_pkg stdr_control roscpp geometry_msgs/Twist

  在该功能包下src内新建文件stdr_open_loop_commander.cpp,详情参见代码.

  该机器人被命令:前进3m,逆时针转向90°,再前进3m.

  可使用rqt_plot查看相关速度曲线.

## 3.2 动力学仿真建模
