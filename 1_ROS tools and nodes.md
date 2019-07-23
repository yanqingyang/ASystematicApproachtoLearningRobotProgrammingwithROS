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

  在minimal_nodes下的src中,创建文件minimal_publisher.cpp,并添加以下代码.详细代码及详解参见代码文件.


  命令roscd std_msgs, 可快速找到消息类型std_msgs的定义.

  命令rosmsg show ...,可显示任意消息类型的详情.

  while(ros::ok()):进入无限循环,直到检测到ROS系统被终止才会自行结束.而ROS系统的终止由函数ros::ok()实现.该方法可便于仅仅停止ROS系统(如通过kill roscore)即可关闭一系列节点.

### 1.2.3 编译ROS节点

  运行catkin_make可编译ROS节点.该命令必须在特定路径下执行,即ROS工作空间,如~/ros_ws.其将会编译该工作空间的所有功能包.

  构建一个catkin功能包后,其可执行文件位于/devel/lib下.

  编译前,必须通知catkin_make存在新的源代码,为此应编辑CMakeLists.txt,该文件在使用catkin_create_pkg创建功能包时自动生成.

  只需保证已声明功能包依赖项,通知编译器来编译新源代码,将编译的代码与需要的库链接.

  catkin_package_pkg已经填充以下片段:

  find_package(catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
  )

  include_directories(
    ${catkin_INCLUDE_DIRS}
  )

  需添加以下行:

  add_executable(minimal_publisher src/minimal_publisher.cpp)

  target_link_libraries(minimal_publisher ${catkin_LIBRARIES})

  以上两行通知该编译器新的源代码以及链接的库.

  add_executable中第一个参数是所创建的可执行文件的名称.其第二个参数是相对于该功能包路径去哪里找到该源文件.源文件的典型位置是一个功能包的src目录.

  编辑完CMakeLists.txt文件后,即可编译代码.在命令终端,切换到ROS工作空间目录下,使用catkin_make命令即可.

  若编译成功,可在devel/lib/minimal_nodes中看到一个新的可执行文件minimal_publisher.该名称是在CMakeLists.txt的add_executable()中选择的输出文件名称.

### 1.2.4 运行ROS节点

  运行任意节点前必须有且仅有一个roscore正在运行.
