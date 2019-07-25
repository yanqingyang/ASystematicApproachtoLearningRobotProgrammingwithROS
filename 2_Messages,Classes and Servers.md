# 2 Messages, Classes and Servers

  前一章使用标准消息类型和publisher-subscriber机制实现异步通讯.本章介绍点到点通讯:client-Services机制.client-Services交互会address concerns of publish and subscribe in terms of knowledge of the source of communications and guaranteed receipt of messages.client-Services机制的限制在于传输是阻滞的,client节点会被挂起,直到service节点响应.还有第三种通讯机制:action clients 和 action servers.

## 2.1 定制消息

  ROS消息类型是基于14种基类(内嵌类型)实现的.

  功能包std_msgs定义了32种消息类型,大部分是单独的内置域类型.Header.msg具有三个域值,分别对应一个内嵌类型.

  可使用rosmsg show 交互地查看定义的消息,所跟参数是功能包名和消息名:

  rosmsg show std_msgs/Header

  如果ROS发布版中已经存在一个消息类型,应使用该消息.否则需要自定义消息.

### 2.1.1 定义定制消息

  在ROS工作空间下的src/ASystematicApproachtoLearningRobotProgrammingwithROS/Part_1下新建功能包example_ros_msg:

  cs_create_pkg example_ros_msg roscpp std_msgs

  在功能包example_ros_msg下创建新文件夹msg,并在其中创建文件ExampleMessage.msg,包含以下内容:

  Header header

  int32 demo_int

  float64 demo_double

  该消息类型具有三个域值.

  为通知编译器,需要生成新的消息头文件,必须编辑package.xml文件,添加以下行:

  <build_depend>message_generation</build_depend>

  <run_depend>message_generation</run_depend>

  使用catkin_simple时,添加库/消息等时不需要修改CMakeLists.txt文件.

  定义完消息类型后,可生成对应的头文件,以用于C++中的file inclusion.使用catkin_make编译代码会产生头文件,该头文件位于:~/learnros_ws/devel/include/example_ros_msg/并在其中创建文件ExampleMessage.h.

  要使用该新消息类型的节点源文件应依赖于功能包example_ros_msg(在对应的package.xml)并以下列行样式包括新头文件:

  # include <example_ros_msg/ExampleMessage.h>

  在example_ros_msg下的src下创建新文件example_ros_message_publisher.cpp,详细代码和详解参见代码.

  ros::Time::now()查询当前时间并以与header兼容的格式返回一个值(由second和nanoseconds的两个域值组成).注意绝对时间基本上无意义的,时间差值可作为有效的时间增量.

  修改CMakeLists.txt文件并编译该文件.可运行该节点并查看相应主题.

  roscore

  rosrun example_ros_msg example_ros_message_publisher

  rostopic list

  rostopic echo /example_topic

### 2.1.2 定义变长度消息

  ROS基类消息可用于定义任意长度的消息.

  在Part_1下创建功能包custom_msgs:

  cs_create_pkg custom_msgs roscpp std_msgs message_generation

  在该功能包下新建文件夹msg,在其中新建文件VecOfDoubles.msg:

  float64[] dbl_vec

  修改CMakeLists.txt文件,添加以下行:

  <build_depend>message_generation</build_depend>

  <run_depend>message_time</run_depend>

  编译该ROS工作空间.可查看该消息类型:

  rosmsg show custom_msgs/VecOfDoubles

  当该消息被订阅者接收后,该类型会被解释为double型的C++ vector.

  在功能包example_ros_msg的src下新建文件vector_publisheer.cpp,详细代码和详解参见代码.

  为了使得功能包example_ros_msg能够使用该新消息类型,必须在其package.xml中通知新消息的依赖项:

  <build_depend>custom_msgs</build_depend>

  <run_depend>custom_msgs</run_depend>

  修改CMakeLists.txt文件,通知其编译新节点:

  cs_add_executable(vector_publisher src/vector_publisher.cpp)

  编译并运行该节点:

  catkin_make

  rosrun example_ros_msg vector_publisher

  rostopic echo /vec_topic

  在example_ros_msg下的src下创建新文件vector_subscriber.cpp:详细代码和详解参见代码.

  修改CMakeLists.txt文件并编译该工作空间.

  依次运行上述两个节点:

  roscore

  rosrun example_ros_msg vector_publisher

  rosrun example_ros_msg vector_subscriber

  可看到发布者和订阅者成功使用新的变长度消息类型实现通讯.

  在使用变长度消息时,必须注意:

  1.保证变长的vectors不能过大.否则会消耗所有的CPU内存和通讯带宽;

  2.与普通数组类似,如果你试图to access memory,这是不允许的,会出错.例如,如果你接收的消息复制值vec_of_doubles是三个元素的长度,你试图获得vec_of_doubles[3]的值,将导致run-time error,不会有编译错误.

  上述介绍了定义消息类型.定义用于ROS Services和ROS action servers的附加消息类型也是类似的过程.

## 2.2 ROS Services
