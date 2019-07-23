## 1.1 一些ROS概念

  节点间的通讯涉及messages,topics,roscore,publishers and subscribers等概念.

  发布者要发布一个topic, 必须实例化一个类ros::Publisher对象.有了该对象后,用户代码可调用其功能函数advertise,并指定消息类型,声明topic name.自此,用户代码可使用该发布者对象的子函数publish来发送消息到给定的topic,该publish函数将其参数作为消息发布.

  
