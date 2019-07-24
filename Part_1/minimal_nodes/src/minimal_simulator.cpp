//模拟F=ma,计算速度值
#include<ros/ros.h>
#include<std_msgs/Float64.h>
std_msgs::Float64 g_velocity;
std_msgs::Float64 g_force;

void myCallback(const std_msgs::Float64& message_holder) {
    // checks for messages on topic "force_cmd"
    ROS_INFO("received force value is: %f", message_holder.data);
    g_force.data = message_holder.data;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "minimal_simulator");
    ros::NodeHandle nh; // node handle
    //create a Subscriber object and have it subscribe to the topic "force_cmd"
    ros::Subscriber my_subscriber_object = nh.subscribe("force_cmd", 1, myCallback);
    //simulate accelerations and publish the resulting velocity;
    ros::Publisher my_publisher_object = nh.advertise<std_msgs::Float64>("velocity", 1);
    double mass = 1.0;
    double dt = 0.01; //10ms integration time step
    double sample_rate = 1.0 / dt; // compute the corresponding update frequency
    ros::Rate naptime(sample_rate);
    g_velocity.data = 0.0;
    g_force.data = 0.0; // initialize force to 0; will get updated by callback
    while (ros::ok()) {
        g_velocity.data = g_velocity.data + (g_force.data / mass) * dt; // Euler integration of acceleration
        my_publisher_object.publish(g_velocity); // publish the system state (trivial--1-D)
        ROS_INFO("velocity = %f", g_velocity.data);
        ros::spinOnce(); //allow data update from callback
        naptime.sleep();
    }
    return 0;
}
