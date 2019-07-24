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

  运行任意节点前必须有且仅有一个roscore正在运行.在命令终端输入:

  roscore

  然后在新的命令终端启动该新发布节点:

  rosrun minimal_nodes minimal_publisher

  命令rosrun的参数是功能包名称和可执行文件名称. rosrun package_name executable_name.

  一般情况下,源代码名称,可执行文件名称和节点名称最好一样.

### 1.2.5 检测正在运行的最小发布节点

  查看主题,可使用命令rostopic.在命令终端输入rostopic,可得到:

  rostopic is a command-line tool for printing information about ROS Topics.

  Commands:

    rostopic bw	display bandwidth used by topic

    rostopic delay	display delay of topic from timestamp in header

    rostopic echo	print messages to screen

    rostopic find	find topics by type

    rostopic hz	display publishing rate of topic  

    rostopic info	print information about active topic

    rostopic list	list active topics

    rostopic pub	publish data to topic

    rostopic type	print topic or field type

  Type rostopic <command> -h for more detailed usage, e.g. 'rostopic echo -h'

  rostopic list :显示ROS系统中的active topics.

  rostopic hz topicname :显示主题topicname的发布频率.

  rostopic bw topicname :显示主题topicname消耗了多少可用通讯带宽.

  rostopic info topicname :显示主题topicname的消息类型,发布节点和订阅节点.

  rostopic echo topicname :打印发布在主题topicname的所有信息.

  命令rostopic可显示目前ROS系统的很多状态信息.

  命令rosnode可显示ROS系统中active nodes.

  rosnode is a command-line tool for printing information about ROS Nodes.

    Commands:

    rosnode ping	test connectivity to node

    rosnode list	list active nodes

    rosnode info	print information about node

    rosnode machine	list nodes running on a particular machine or list machines

    rosnode kill	kill a running node

    rosnode cleanup	purge registration information of unreachable nodes

  Type rosnode <command> -h for more detailed usage, e.g. 'rosnode ping -h'

  节点rosout是一个通用进程节点,用于将文本显示到终端,由roscore默认启动.
### 1.2.6 安排节点时序

  上述minimal_publisher节点会过度使用CPU性能和通讯带宽.实际上,机器人系统的节点所需的更新频率一般不超过30kHz.对于大多实时的底层节点而言,其合理的更新频率是1kHz.以下例程使用ROS timer将发布者的更新频率降为1Hz.

  新建修改版源文件sleepy_minimal_publisher.cpp,详细代码及详解参见代码文件.

  ros::Rate naptime(1.0); //实例化类ros::Rate,并设置该sleep timer为1Hz的重复频率

  naptime.sleep(); //在while loop中使用ROS类Rate的成员函数sleep(),该函数会suspend该节点,直到达到the balance of the desired period.

  修改CMakeLists.txt文件并使用catkin_make完成编译.可运行该节点并查看主题/topic1的特性.

  rostopic hz /topic1

  rostopic bw /topic1

  由上述两个指令可看到主题/topic1现在的更新频率是1Hz,且所消耗的通讯带宽明显降低.

### 1.2.7 编写最小ROS订阅者

  在minimal_nodes的src下创建文件minimal_subcriber.cpp,详细代码和详解参见代码.

  void myCallback(const std_msgs::Float64& message_holder)

  该函数有一个引用指针参数,指向对象类std_msgs::Float64,这是相对应主题的消息类型.

  该callback函数的重要性在于:无论何时有新数据被发布到相关联的主题,都会唤醒该回调函数.一旦新数据被发布到相关联的主题,该回调函数运行,新发布的数据出现在该回调函数的参数中,此处即为message_holder.该message holder的类型必须与相关联主题的消息类型一致.

  在该回调函数中,可对接收的数据进行处理.此处仅将数据显示出来.

  ROS_INFO("received value is: %f",message_holder.data);

  此处显示使用ROS_INFO(),而不是cout()或printf().ROS_INFO()使用消息发布, 避免了由于显示驱动而降低时序重要的代码.此外,使用ROS_INFO()使得数据可用于记录或监控.ROS_INFO()的参数等同于C语言的printf().

  ROS_INFO()的可替代函数是ROS_INFO_STREAM(),即为:

  ROS_INFO_STREAM("received value is: "<<message_holder.data<<std::endl);

  ROS_INFO_STREAM()函数可获得相同的显示,但使用cout()的语法.

  一旦该callback函数执行完,其将go back to sleep,准备好被该主题的新消息的再次到达而再次唤醒.

  ros::Subscriber my_subscriber_object= n.subscribe("topic1",1,myCallback);

  实例化类ros::Subscriber,该类的子函数subscribe()有三个参数:主题名称,队列尺寸,回调函数名称.

  ros::spin();

  该行引入一个关键ROS概念.无论何时相关联主题上出现新消息,都会激活其回调函数.但是main()必须等待一定时间,使得回调函数来响应.可通过spin()命令来实现.spin使得主程序to suspend,但是保持该回调函数alive.如果主程序运行到结尾,该回调函数将不再是poised to react to new messages.该spin()函数保持main() alive,而不消耗太多CPU时间.

### 1.2.8 编译并运行最小订阅者

  修改CMakeLists.txt文件并重新编译该ROS工作空间.

  在三个命令终端依次运行:

  roscore

  rosrun minimal_nodes sleepy_minimal_publisher

  rosrun minimal_nodes minimal_subscriber

  可看到每1s显示更新一次.

  使用命令rosnode list可看到有三个节点:/minimal_publisher2, /minimal_subscriber, /rosout.

  使用命令rqt_graph可看到该正在运行的ROS系统的图形显示:节点,主题及相互间连接关系.

### 1.2.9 最小订阅者和发布者节点总结

  硬实时高速伺服环可能需要非ROS的专用控制器,用户需要设计硬实时控制器并编写一个ROS兼容的订阅接口.

## 1.3 ROS tools: CATKIN_SIMPLE, ROSLAUNCH, RQT_CONSOLE, AND ROSBAG

  本节介绍的ROS工具是catkin_simple, roslaunch, rqt_console和rosbag.

### 1.3.1 使用catkin_simple简化CMakeLists.txt

  如前所述,创建新功能包所生成的CMakeLists.txt文件是相当长的.功能包catkin_simple可有助于简化该CMakeLists.txt文件,该功能包位于https://github.com/catkin/catkin_simple.

  在该ROS工作空间的src文件夹下新建文件夹learnros_external_packages,然后将功能包catkin_simple克隆到该文件夹中:

  git clone https://github.com/catkin/catkin_simple.git

  然后将一个Python脚本文件cs_create_pkg.py复制到文件夹learnros_external_packages中.然后切换到该文件所在位置,使用 chmod a+x cs_create_pkg.py 命令改变该脚本文件的权限.

  为运行该脚本,可定义一个别名指向该脚本,以此作为命令使用.在命令终端:

  alias cs_create_pkg="~/learnros_ws/src/ASystematicApproachtoLearningRobotProgrammingwithROS/learnros_external_packages/cs_create_pkg.py"

  然后在同一命令终端,切换到~/learnros_ws/src/ASystematicApproachtoLearningRobotProgrammingwithROS/Part_1下,创建新功能包minimal_nodes2:

  cs_create_pkg minimal_nodes2 roscpp std_msgs

  目前该命令cs_create_pkg仅可在该命令终端被识别.为实现通用,应在你的.bashrc文件中包括该命令的别名定义.打开文件~/.bashrc,添加行:

  alias cs_create_pkg="~/learnros_ws/src/ASystematicApproachtoLearningRobotProgrammingwithROS/learnros_external_packages/cs_create_pkg.py"

  然后保存该文件.自此新的命令终端均可识别该cs_create_pkg命令.

  使用新命令cs_create_pkg,创建的新功能包minimal_nodes2包含所期待的结构:子文件夹src和include, 文件package.xml和CMakeLists.txt, 一个新文件README.md.该README.md文件应描述该功能包的目的及如何使用该功能包运行例程.该README.md使用markdown格式.

  其中的package.xml类似于使用catkin_create_pkg创建的相同文件,区别在于添加了以下依赖项:

  <buildtool_depend>catkin_simple</buildtool_depend>

  而CMakeLists.txt文件被合理简化.

  行catkin_simple()调用action来自动执行该CMakeLists.txt的大部分复杂编辑.

  为了编译理想源代码而修改该CMakeLists.txt文件时,仅需要修改以下行:

  cs_add_executable(example src/example.cpp)

  将minimal_publisher.cpp复制到该新功能包的src下.修改其CMakeLists.txt文件的以下行即可编译该节点:

  cs_add_executable(minimal_publisher3 src/minimal_publisher.cpp)

  上行表明编译该minimal_publisher.cpp文件,且其可执行文件是minimal_publisher3.使用catkin_simple时,不需要指定与库的链接.当需要链接很多库,创建很多库,创建很多定制消息时,catkin_simple将会很有用.

### 1.3.2 自动启动多个节点
