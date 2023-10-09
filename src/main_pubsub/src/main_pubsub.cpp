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
#include "std_msgs/Float32.h"
#include "std_msgs/Float32MultiArray.h"
#include "vector"
#include <boost/bind.hpp>
// #include "main_pubsub/main_pubsub.h"

/*
class encorder
{
  // ros::Publisher pub_encoder_;
  // ros::Publisher pub_gylo_;
  float encorder[4];
  float gylo[12];
};

class sensor
{
  // ros::Publisher pub_sensor_;
  // ros::Publisher pub_switch_;
  float sensor[8];
  float swit[8];
};
*/

class motor
{
  public:
  float motor[6];
};

class cylinder
{
  public:
  float cylinder[8];
};

class servo
{
  public:
  float servo[8];
};

class RobotTestNode
{
private:
  // pub
  ros::Publisher pub_encoder_;
  ros::Publisher pub_gylo_;
  ros::Publisher pub_sensor_;
  ros::Publisher pub_switch_;
  
  void sensor_pub(std_msgs::Float32MultiArray encoder,std_msgs::Float32MultiArray gylo, std_msgs::Float32MultiArray sensor, std_msgs::Float32MultiArray switch_){
    pub_encoder_.publish(encoder);
    pub_gylo_.publish(gylo);
    pub_sensor_.publish(sensor);
    pub_switch_.publish(switch_);
  }
  
  // sub cb
  motor Motor;
  void motorcb(const std_msgs::Float32::ConstPtr& msg, const int id){
    Motor.motor[id] = msg->data;
  }

  cylinder Cylinder;
  void cylindercb(const std_msgs::Float32::ConstPtr& msg, const int id){
    Cylinder.cylinder[id] = msg->data;
  }

  servo Servo;
  void servocb(const std_msgs::Float32::ConstPtr& msg, const int id){
    Servo.servo[id] = msg->data;
  }


  public:
  RobotTestNode()
  {
    ros::NodeHandle nh;
    // pub
    pub_encoder_ = nh.advertise<std_msgs::Float32MultiArray>("/encoder", 1);
    pub_sensor_ = nh.advertise<std_msgs::Float32MultiArray>("/sensor", 1);
    pub_switch_ = nh.advertise<std_msgs::Float32MultiArray>("/switch", 1);
     
    // sub
    std::vector<ros::Subscriber> motor;
    for(int i = 0; i > sizeof(Motor.motor); i++){
      std::string motor_ = "motor" + i;
      // ros::Subscriber sub_motor_ = nh.subscribe(motor_, 1,&motorcb, this);
      ros::Subscriber sub_motor_ = nh.subscribe<std_msgs::Float32>(motor_, 1, boost::bind(&motorcb, _1, i));
      motor.push_back(sub_motor_);
    }

    std::vector<ros::Subscriber> cylinder;
    for(int i = 0; i > sizeof(Cylinder.cylinder); i++){
      std::string cylinder_ = "cylinder" + i;
      // ros::Subscriber sub_cylinder_ = nh.subscribe(cylinder_, 1,&cylindercb, this);
      ros::Subscriber sub_cylinder_ = nh.subscribe<std_msgs::Float32>(cylinder_, 1, boost::bind(&cylindercb, _1, i));
      cylinder.push_back(sub_cylinder_);
    }

    std::vector<ros::Subscriber> servo;
    for(int i = 0; i > sizeof(Servo.servo); i++){
      std::string servo_ = "servo" + i;
      // ros::Subscriber sub_servo_ = nh.subscribe(servo_, 1,&servocb, this);
      ros::Subscriber sub_servo_ = nh.subscribe<std_msgs::Float32>(servo_, 1, boost::bind(&servocb, _1, i));
      servo.push_back(sub_servo_);
    }

    // sub_motor_ = nh.subscribe("/motor", 1,&RobotTestNode::motorcb, this);
    // sub_cylinder_ = nh.subscribe("/cylinder", 1,&RobotTestNode::cylindercb, this);
    // sub_servo_ = nh.subscribe("/servo", 1,&RobotTestNode::servocb, this);
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
