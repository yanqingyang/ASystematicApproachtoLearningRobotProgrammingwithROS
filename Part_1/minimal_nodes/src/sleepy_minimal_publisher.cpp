#include <ros/ros.h>
#include <std_msgs/Float64.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "minimal_publisher2");
    ros::NodeHandle n;
    ros::Publisher my_publisher_object = n.advertise<std_msgs::Float64>("topic1", 1);


    std_msgs::Float64 input_float;

    input_float.data = 0.0;//初始化该值

    ros::Rate naptime(1.0);//创建类ros::Rate实例对象,设置该sleep timer为1kHz的重复频率,参数单位是Hz.
    //进入无限循环,直到检测到ROS系统被终止才会自行结束.ROS系统的终止使用函数ros::ok()来实现.
    while (ros::ok())
    {
        // this loop has no sleep timer, and thus it will consume excessive CPU time
        // expect one core to be 100% dedicated (wastefully) to this small task
        input_float.data = input_float.data + 0.001; //increment by 0.001 each iteration
        my_publisher_object.publish(input_float); //发布该值到the topic "topic1"
        naptime.sleep();//使得该loop to sleep for the balance of the desied period,以实现特定循环频率
    }
}
