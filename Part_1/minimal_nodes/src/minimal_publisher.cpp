#include <ros/ros.h> //核心ROS库的头文件,任何以C++编写的ROS源代码的第一行
#include <std_msgs/Float64.h>//描述对象消息类型std_msgs::Float64的头文件

int main(int argc, char **argv) {//每个节点只能有一个mian()
    ros::init(argc, argv, "minimal_publisher"); //每个ROS节点必须项,节点名是"minimal_publisher"
    ros::NodeHandle n; //句柄用于建立节点间的网络通讯,句柄名称n是任意的
    ros::Publisher my_publisher_object = n.advertise<std_msgs::Float64>("topic1", 1);
    // 实例化发布者类,发布主题名是"topic1",其消息类型是std_msgs::Float64
    // the "1" argument says to use a buffer size of 1; could make larger, if expect network backups

    std_msgs::Float64 input_float; //创建变量input_float,类型是"Float64",
    // std_msgs::Float64定义在: /opt/ros/kinetic/share/std_msgs,可使用命令roscd std_msgs快速找到其详细定义.
    // std_msgs::Float64仅包含单个域值data,即为float64类型的值.

    input_float.data = 0.0;//初始化该值

    //进入无限循环,直到检测到ROS系统被终止才会自行结束.ROS系统的终止使用函数ros::ok()来实现.
    while (ros::ok())
    {
        // this loop has no sleep timer, and thus it will consume excessive CPU time
        // expect one core to be 100% dedicated (wastefully) to this small task
        input_float.data = input_float.data + 0.001; //increment by 0.001 each iteration
        my_publisher_object.publish(input_float); //发布该值到the topic "topic1"
    }
}
