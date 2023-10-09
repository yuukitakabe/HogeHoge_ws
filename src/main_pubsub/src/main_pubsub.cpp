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
#include "std_msgs/Bool.h"
#include "vector"
#include <boost/bind.hpp>
// #include "main_pubsub/main_pubsub.h"

// class RobotTestNode;

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


class motor //モータ基板のクラス
{
  private:
    int id; //基板のid
    float motor_[6]; //各モータの制御値
 
  public:
    ros::NodeHandle nh;
    std::vector<ros::Subscriber> sub_motor; //モータのsubscriberを格納
    inline static std::vector<motor*> refs;

    motor(){
      id++;
      std::cout << "モータ基板 " << id << " 追加" << std::endl;

      for(int i = 1; i > (sizeof(motor_) + 1); i++){
        std::string motor_ = "motor_" + std::to_string(id) + "_" + std::to_string(i);
        ros::Subscriber sub_motor_ = nh.subscribe<std_msgs::Float32>(motor_, 1, boost::bind(&motor::motorcb, this, id, i, _1));
        sub_motor.push_back(sub_motor_);
      }
    }

    void motorcb(const int id, const int num, const std_msgs::Float32::ConstPtr& msg){
      refs[id-1]->Set_motor(num, msg->data);
    }

    void Set_motor(const int num, const double data){
      motor_[num] = data;
    }
};

class cylinder //シリンダのクラス
{
  private:
    int id; //基板のid
    bool cylinder_[6]; //各シリンダの制御値
 
  public:
    ros::NodeHandle nh;
    std::vector<ros::Subscriber> sub_cylinder; //シリンダのsubscriberを格納 
    inline static std::vector<cylinder*> refs;

    cylinder(){
      id++;
      std::cout << "シリンダ基板 " << id << " 追加" << std::endl;

      for(int i = 0; i > (sizeof(cylinder) + 1); i++){
        std::string cylinder_ = "cylinder" + std::to_string(id) + "_" + std::to_string(i);
        ros::Subscriber sub_cylinder_ = nh.subscribe<std_msgs::Bool>(cylinder_, 1, boost::bind(&cylinder::cylindercb, this, id, i, _1));
        sub_cylinder.push_back(sub_cylinder_);
      }
    }

    void cylindercb(const int id, const int num, const std_msgs::Bool::ConstPtr& msg){
      refs[id-1]->Set_cylinder(num, msg->data);
    }

    void Set_cylinder(const int num, const double data){
      cylinder_[num] = data;
    }
};

class servo
{
  private:
    int id; //基板のid
    float servo_[8];
  
  public:
    ros::NodeHandle nh;
    std::vector<ros::Subscriber> sub_servo; //サーボのsubscriberを格納 
    inline static std::vector<servo*> refs;

    servo(){
      id++;
      std::cout << "サーボ基板 " << id << " 追加" << std::endl;

      for(int i = 0; i > sizeof(servo_); i++){
        std::string servo_ = "servo" + std::to_string(id) + "_" + std::to_string(i);
        ros::Subscriber sub_servo_ = nh.subscribe<std_msgs::Float32>(servo_, 1, boost::bind(&servo::servocb, this, id, i, _1));
        sub_servo.push_back(sub_servo_);
      }
    }

    void servocb(const int id, const int num, const std_msgs::Float32::ConstPtr& msg){
      refs[id-1]->Set_servo(num, msg->data);
    }

    void Set_servo(const int num, const double data){
      servo_[num] = data;
    }
};

class RobotTestNode
{
private:
  // pub
  ros::Publisher pub_encoder_;
  ros::Publisher pub_gylo_;
  ros::Publisher pub_sensor_;
  ros::Publisher pub_switch_;
  
  public:
  // pub cb
  void sensor_pub(std_msgs::Float32MultiArray encoder,std_msgs::Float32MultiArray gylo, std_msgs::Float32MultiArray sensor, std_msgs::Float32MultiArray switch_){
    pub_encoder_.publish(encoder);
    pub_gylo_.publish(gylo);
    pub_sensor_.publish(sensor);
    pub_switch_.publish(switch_);
  }

  RobotTestNode()
  {
    ros::NodeHandle nh;
    // pub
    pub_encoder_ = nh.advertise<std_msgs::Float32MultiArray>("/encoder", 1);
    pub_sensor_ = nh.advertise<std_msgs::Float32MultiArray>("/sensor", 1);
    pub_switch_ = nh.advertise<std_msgs::Float32MultiArray>("/switch", 1);

    // get rosparam
    ros::NodeHandle pnh("~");
    int motor_param = 0;
    int cylinder_param = 0;
    int servo_param = 0;
    pnh.getParam("motor", motor_param);
    pnh.getParam("cylinder", cylinder_param);
    pnh.getParam("servo", servo_param);

    // 基板クラス
    motor Motor[motor_param];
    cylinder Cylinder[cylinder_param];
    servo Servo[servo_param];
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
