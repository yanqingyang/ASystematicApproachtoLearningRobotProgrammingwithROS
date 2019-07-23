## 1.1 一些ROS概念

  节点间的通讯涉及messages,topics,roscore,publishers and subscribers等概念.

  发布者要发布一个topic, 必须实例化一个类ros::Publisher对象.有了该对象后,用户代码可调用其功能函数advertise,并指定消息类型,声明topic name.自此,用户代码可使用该发布者对象的子函数publish来发送消息到给定的topic,该publish函数将其参数作为消息发布.

  节点都是通过roscore进行通讯的,所以启动任何ROS节点前必须先运行roscore.打开命令终端,键入roscore即可运行roscore.

  订阅者也通过roscore与其他节点通讯,使用类ros::Subscriber对象.该类子函数subscribe需要所订阅topic的名称作为参数.所以编程者必须知道所关注的topic是存在的且知道其名称.此外,该订阅者函数需要一个callback函数的名称.该函数提供到ROS中间件所需的hook.

## 1.2 编写ROS节点

  详细介绍最小发布者节点和最小订阅者节点.

  介绍ROS功能包概念,如何通过package.xml和CMakeLists.txt文件编译和链接代码.

  介绍一个ROS工具和命令,如rosrun, rostopic, rosnode, rqt_graph.

  该部分代码位于:Part_1/minimal_nodes.

  如何创建ROS工作空间并配置环境可参见:https://www.cnblogs.com/yanqingyang/p/9822425.html.

### 1.2.1 创建ROS功能包

  在ROS工作空间的src下创建功能包minimal_nodes:

  catkin_create_pkg minimal_nodes roscpp std_msgs

  在minimal_nodes路径下,已经存在文件package.xml, CMakeLists.txt和文件夹src,include.

  在使用cakin_create_pkg时有两个依赖项,roscpp依赖项表明使用C++编译器来创建ROS节点,需要C++编译接口,如类ros::Publisher 和 ros::Subscriber.std_msgs依赖项表明将需要使用一些ROS已定义的数据类型,如std_msgs::Float64.

  package.xml文件: 构建系统识别一个ROS功能包是通过查看其是否有一个package.xml文件.兼容的package.xml文件具有特定的结构,说明该功能包名称并列出其依赖项.minimal_nodes的package.xml是自动生成的,其内容参见代码块.

  该package.xml文件是使用XML格式的ASCII文本.

  其中行<name>minimal_nodes</name>对应于新的功能包名称.注意该名称必须与你的功能名称相对应.

  为使用其他功能包,如roscpp和std_msgs,必须在package.xml文件中将其添加进来,即编辑package.xml: 添加build_depend,build_export_depend和exec_depend行,参考对应行写法.

  子路径src是用于存储自编写的C++代码的.有了新节点后,需要编辑CMakeList.txt文件通知编译器我们有新节点需要编译.

### 1.2.2 编写一个最小ROS发布者

  
