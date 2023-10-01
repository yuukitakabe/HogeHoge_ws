/**********
 * 2020929
*/

/**********
 * spinどうするか
 * https://qiita.com/strv/items/dd8eef1447e17e4b076e
 * 
*/

#include "ros/ros.h"
#include "ros/package.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include "main_pubsub/main_pubsub.h"

#include <tf/transform_datatypes.h>

// 

class RobotTestNode
{
private:
  // pub
  ros::Publisher pub_odom_;
  ros::Publisher pub_encodeder_;
  ros::Publisher pub_sensor_;
  ros::Publisher pub_switch_;
  
  // sub
  ros::Subscriber sub_twist_;
  ros::Subscriber sub_motor_;
  ros::Subscriber sub_cylinder_;
  ros::Subscriber sub_serve_;

  // 
  geometry_msgs::Twist twist_;

  std_msgs::Float32MultiArray motor_;
  std_msgs::Float32MultiArray cylinder_;
  std_msgs::Float32MultiArray serve_;
  
  // nav_msgs::Odometry odom_;
  // void cbOdom(const nav_msgs::Odometry::ConstPtr& msg)
  // {
  //   // ROS_INFO("vel %f", msg->twist.twist.linear.x);
  //   // ROS_INFO("ang %f\n", msg->twist.twist.angular.z);
  //   odom_ = *msg;  // 追記 
  // }

  void twistcb(const geometry_msgs::Twist::ConstPtr& msg){
    twist_ = *msg;
  }

  void motorcb(const std_msgs::Float32MultiArray::ConstPtr& msg){
    motor_ = *msg;
  }

  void cylindercb(const std_msgs::Float32MultiArray::ConstPtr& msg){
    cylinder_ = *msg;
  }

  void servecb(const std_msgs::Float32MultiArray::ConstPtr& msg){
    serve_ = *msg;
  }

  void sensor_pub(nav_msgs::Odometry odom, std_msgs::Float32MultiArray encodeder, std_msgs::Float32MultiArray sensor, std_msgs::Float32MultiArray switch_){
    pub_odom_.publish(odom);
    pub_encodeder_.publish(encodeder);
    pub_sensor_.publish(sensor);
    pub_switch_.publish(switch_);
  }

  public:
  RobotTestNode()
  {
    ros::NodeHandle nh;
    // pub
    pub_odom_ = nh.advertise<geometry_msgs::Twist>("/odom", 1);
    pub_encodeder_ = nh.advertise<std_msgs::Float32MultiArray>("/encodeder", 1);
    pub_sensor_ = nh.advertise<std_msgs::Float32MultiArray>("/sensor", 1);
    pub_switch_ = nh.advertise<std_msgs::Float32MultiArray>("/switch", 1);

    // sub
    sub_twist_ = nh.subscribe("/cmd_vel", 1,&RobotTestNode::twistcb, this);
    sub_motor_ = nh.subscribe("/motor", 1,&RobotTestNode::motorcb, this);
    sub_cylinder_ = nh.subscribe("/cylinder", 1,&RobotTestNode::cylindercb, this);
    sub_serve_ = nh.subscribe("/serve", 1,&RobotTestNode::servecb, this);

    // ros::Subscriber sub = nh.subscribe("chatter", 5, chatterCallback);
  }
  void mainloop()
  {
    ROS_INFO("Hello ROS World!");

    ros::Rate rate(1.0);

    ros::spin();
  /*
    while (ros::ok())
    {
        ros::spinOnce();
        rate.sleep();
    }
  */
  }
};

int main(int argc, char* argv[])
{
  ros::init(argc, argv, "main");

  RobotTestNode robot_test;

  robot_test.mainloop();
}
