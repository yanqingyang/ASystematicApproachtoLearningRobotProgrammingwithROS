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

  在publish-subscribe通讯机制中,订阅者可能丢失信息.这类通讯适用于重复性消息.

  有时需要建立双向的,点到点的可靠通讯.the client sends a request to a service, and the service sends a response to the client.

  当一个client发送一个请求时,该client被挂起,直到返回一个回答.ROS节点应能忍受这样的延迟.如果该请求涉及大量的计算或延时,应使用the action server and action client 机制.

### 2.2.1 Service messages

  在Part_1下创建新功能包example_ros_service:

  cs_create_pkg example_ros_service roscpp std_msgs

  定义一个定制的service message,该消息分别描述一个请求和一个响应的数据类型和域名,这是由ROS模板实现的,即一个*.srv文件.该文件的内容自动生成C++头文件,以在C++文件中使用.

  在example_ros_service下新建文件夹srv,在其中新建文件ExampleServiceMsg.srv:

  string name
  ---
  bool on_the_list
  bool good_guy
  int32 age
  string nickname

  上述代码中,'---'以上部分表示请求结构,'---'以下部分表示响应结构.

  修改package.xml文件通知编译器需要生成新消息头文件:

  <build_depend>message_generation</build_depend>

  <run_depend>message_generation</run_depend>

  编译该ROS工作空间后,自动生成响应的C++头文件.该C++头文件位于:~/learnros_ws/devel/include/example_ros_service.不同于自定义消息类型,自定义服务消息类型经编译后会生成三个C++头文件:ExampleServiceMsg.h, ExampleServiceMsgRequest.h, ExampleServiceMsgResponse.h.后两个头文件均包含在ExampleServiceMsg.h中.

  为在一个节点中使用一个新的服务消息类型,应在其C++源代码中包含相关头文件:

  # include <example_ros_service/ExampleServiceMsg.h>

  分隔符<...>告诉编译器在期望位置处查找该文件,例如/devel/include.

  如果在其他功能包中使用该新服务消息类型,除了在其节点源文件中添加 #include <example_ros_service/ExampleServiceMsg.h>外,在其package.xml中有两点不同.1.不需要依赖项message_generation或message_runtime,这是因为该消息类型仅需生成一次.2.新功能包的package.xml文件必须添加依赖项example_ros_service,即添加行:<build_depend>example_ros_service</build_depend>.

### 2.2.2 ROS Services nodes

  在功能包example_ros_service中的src下新建文件example_ros_service.cpp,详细代码和详解参见代码.

  创建服务类ros::ServiceServer的实例.当该Service接收到一个请求时,将唤醒callback函数.

  编辑CMakeLists.txt,并编译该工作空间,运行该节点:

  rosrun example_ros_service example_ros_service

### 2.2.3 与ROS Services人工交互

  当节点example_ros_service正在运行时,可使用以下命令查看该Service是否可用:

  rosservice list

  上述指令会显示一个/lookup_by_name的服务.

  rosservice

  Commands:

    rosservice args	print service arguments

    rosservice call	call the service with the provided args

    rosservice find	find services by service type

    rosservice info	print information about service

    rosservice list	list active services

    rosservice type	print service type

    rosservice uri	print service ROSRPC uri

  Type rosservice <command> -h for more detailed usage, e.g. 'rosservice call -h'

  使用以下命令与上述服务进行人工交互:

  rosservice call lookup_by_name 'Ted'

### 2.2.4 ROS service client

  为与ROS service进行程序化交互,需要一个ROS client.

  在example_ros_service中的src下新建文件example_ros_client.cpp,详细代码和详情参见代码.

  使用ros::ServiceClient实例化客户端对象.

  编辑CMakeLists.txt文件并编译该工作空间.

### 2.2.5 运行service和client例程

  启动该服务:

  roscore

  rosrun example_ros_service example_ros_service

  该服务显示"Ready to look up names",然后suspends action,直到到来一个服务请求.在等待服务请求时,尽管该服务是active,但仅消耗可忽略的资源(CPU周期和带宽).

  再启动客户端:

  rosrun example_ros_service example_ros_client

  ROS 包含一些预定义的服务消息类型,存在于功能包std_srvs.

  只有快速请求和快速响应交互时才使用Services.对于需要较长时间才能准备号响应的交互,应使用action-servers和action-clients.为介绍action servers,需要先看看在节点中如何使用C++类.

## 2.3 在ROS中使用C++类

  使用C++类:

  1.在一个头文件中定义一个类:

    定义该类的所有成员函数的原型;

    定义私有和公有数据成员;

    定义该类构造函数的原型;

  2.编写一个单独的实现文件:

    包含上述头文件;

    包含所声明成员函数的代码;

    在构造函数中包含必要的初始化;

  新建功能包example_ros_class,并新建头文件example_ros_class.h,详情及详解见代码.

  新建源文件example_ros_class.cpp,详情和详解参见代码.

  上述例程演示如何在C++类中引入ROS publisher, subscriber,和Services.

## 2.4 在ROS中创建库模块

  本部分描述创建库的步骤.涉及创建库,使用库.

  创建新功能包creating_a_ros_library:

  cs_create_pkg creating_a_ros_library roscpp std_msgs std_srvs

  将功能包example_ros_class下的源文件example_ros_class.cpp复制到功能包creating_a_ros_library下的src下,移除main()部分并修改所包含的头文件:

  #include <creating_a_ros_library/example_ros_class.h>.

  其他详情参见代码.

  在功能包creating_a_ros_library下include内新建文件夹creating_a_ros_library,并将功能包example_ros_class中的example_ros_class.h复制过去.

  修改功能包creating_a_ros_library中的CMakeLists.txt:

  cs_add_library(example_ros_library src/example_ros_class.cpp)

  上行告知catkin_make将创建一个新库.该新库名为example_ros_library,该库的源文件位于src/example_ros_class.cpp.

  此时可使用catkin_make进行编译该库.编译完成后,在文件夹devle/lib下出现新文件libexample_ros_library.so.该库的基名example_ros_library.so由在cs_add_library中的名称指定,而构建系统添加前缀"lib".但是链接到该库并不需要知道该库的名称.只需要在package.xml中标注该功能包依赖项并在相关头文件中包含对应头文件即可, #include <creating_a_ros_library/example_ros_class.h>.

  在功能包creating_a_ros_library下src内新建文件example_ros_class_test_main.cpp,以验证新库是否可用,详情参见代码.

  修改CMakeLists.txt文件:

  cs_add_executable(ros_library_test_main src/example_ros_class_test_main.cpp)

  target_link_libraries(ros_library_test_main example_ros_library)

  上行链接器命令中,第一个参数是可执行文件名,第二个参数新库名.

  编译并运行,可得到与example_ros_class一样的结果.上述演示如何使用新库.

  创建新功能包using_a_ros_library:

  cs_create_pkg using_a_ros_library roscpp std_msgs std_srvs creating_a_ros_library

  将example_ros_class_test_main.cpp复制到功能包using_a_ros_library下的src下.修改CMakeLists.txt文件:

  cs_add_executable(ros_library_external_test_main,src/example_ros_class_test_main.cpp)

  此时使用catkin_simple进行编译构建,因此不需要添加链接项.但如果节点使用同一功能包创建的新库,仍需要添加依赖项.

  编译运行,即可获得应有结果.

## 2.5 action servers and action clients简介

  ROS中第三种重要通讯框架是action server-action client 模式.该模式类似于service-client通讯,也是点对点通讯.而service-client模式的限制在于client会阻断直到service响应.构建action servers和action clients会有很多选择和变化,详情参见http://wiki.ros.org/actionlib.

  可研究下SMACH,参见http://wiki.ros.org/smach

### 2.5.1 创建一个action server功能包

  创建一个功能包example_action_server:

  cs_create_pkg example_action_server roscpp actionlib

  在该功能包下新建文件夹action,用于定义通讯消息类型.为使得catkin_make可预处理该action message,编辑package.xml:

  <build_depend>message_generation</build_depend>

  <run_depend>message_runtime</run_depend>

  以上可自动生成多个头文件,以定义用于action client-action server通讯的消息类型.

### 2.5.2 自定义action-server消息

  类似于创建消息和service 消息的过程.

  在子文件夹action中,创建新文件demo.action.action message有三个域值:goal, result 和 feedback.

  #goal definition

  int32 input

  ---

  #result definition

  int32 output

  int32 goal_stamp

  ---

  #feedback

  int32 fdbk

  action message必须以上述格式编写,且三短线和次序都是关键的.定义于这些域内的部分可包含任何已有的消息类型,只要对应的消息功能包位于package.xml文件中且对应的头文件被包含在源文件中.

  一旦编译该新功能包,构建系统会生成多个新的*.h头文件.这些头文件位于devel/include下与功能包同名的子文件夹中,即example_action_server.在该文件夹中,被创建了7个*.h文件,每个头文件名以demo起始.其中六个头文件被包含在头文件demoAction.h中.通过在action 节点代码中包含不同的头文件,可使用不同的消息类型,如demoGoal, demoResult.

  在src下新建文件example_action_server.cpp,详情参见代码.

  #include <actionlib/server/simple_action_server.h>

  上行引入与功能包simple_action_server相关的头文件.

  class ExampleActionServer {}

  上行定义类ExampleActionServer,该类包含一个类SimpleActionServer的实例as_,类SimpleActionServer定义在库actionlib中.

  当该server 返回goal 消息给其client时,该client接收的值被赋值给result_.output.

  类ExampleActionServer的callback函数是重要的.回调函数的名称是任意的,此处为executeCB().

  该例程不演示如何给client发送反馈消息.反馈消息的目的在于当正在生成goal过程中给client提供状态反馈.反馈消息不是必须的,但很有用.

### 2.5.3 设计一个action client

  在src下新建文件example_action_client.cpp,详情参见代码.

  action client的成员函数waitForServer()用于尝试链接到特定server.如果链接成功,返回true.

### 2.5.4 运行例程节点

  编辑CMakeLists.txt文件并编译.

  roscore

  rosrun example_action_server example_action_server

  rosrun example_action_server example_action_client

  当上述节点运行时,在example_action下有五个新主题:cancel, feedback, goal, result, status.

  ROS 的action servers的一个特性是将client与server进行重连或新连接似乎是不可靠的.

  在src下新建文件example_action_server_w_fdbk.cpp,详情参见代码.

  关于action server有待深入学习研究.

## 2.6 参数服务器简介
