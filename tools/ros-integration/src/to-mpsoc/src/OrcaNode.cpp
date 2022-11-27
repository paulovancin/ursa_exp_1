#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/PoseStamped.h"
#include "/home/paulo/final_shit/ursa_exp_1/software/hellfireos/lib/include/fixed.h"
#include "std_msgs/Float32.h"

#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <geometry_msgs/PointStamped.h>
#include <sensor_msgs/Range.h>
#include "/home/paulo/hectorQuad_mp/devel/include/hector_uav_msgs/MotorPWM.h"

using namespace sensor_msgs;
using namespace message_filters;
using namespace geometry_msgs;
using namespace hector_uav_msgs;


#include "orca-lib.h"


#include <iostream>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

typedef struct {

	fixed_t acc_x;
	fixed_t acc_y;
	fixed_t acc_z;

	fixed_t ang_x;
	fixed_t ang_y;
	fixed_t ang_z;

	fixed_t mag_x;
	fixed_t mag_y;
	fixed_t mag_z;

	fixed_t pos_x;
	fixed_t pos_y;
	fixed_t pos_z;

	fixed_t ros_qw;
	fixed_t ros_qx;
	fixed_t ros_qy;
	fixed_t ros_qz;

	fixed_t euler_x;
	fixed_t euler_y;
	fixed_t euler_z;


} input_data_t;


input_data_t pkt;

fixed_t float_to_fix(float val)
{
	return fix_val(val);
}

// float fix_to_float(fixed_t val)
// {
// 	float ip, fp;
//
// 	ip = (float)(val >> FIX_IBITS);
// 	fp = ((float)(val & FIX_FMASK)) / ((float)(1 << FIX_FBITS));
//
// 	return ip + fp;
// }


//Address of the mpsoc platform at the network
#define MPSOC_ADDR "127.0.0.1"
#define MPSOC_PORT 9999

//Address of current rosnode at the network.
//Must be set as localhost (loopback). Port can safely
//be changed as long as
#define ROSNODE_ADDR "127.0.0.1"
#define ROSNODE_PORT 8888

// #define FREQ_READ 12 // NO MU
#define FREQ_READ 25 // WITH MU


//ros references
ros::Subscriber imu_to_mpsoc;
ros::Subscriber mag_to_mpsoc;
ros::Subscriber height_to_mpsoc;
ros::Subscriber euler_to_mpsoc;


int8_t msg[100];
void callback_imu(const sensor_msgs::Imu::ConstPtr& msg)
{

	pkt.acc_x = float_to_fix(msg->linear_acceleration.x);
	pkt.acc_y = float_to_fix(msg->linear_acceleration.y);
	pkt.acc_z = float_to_fix(msg->linear_acceleration.z);
	pkt.ang_x = float_to_fix(msg->angular_velocity.x);
	pkt.ang_y = float_to_fix(msg->angular_velocity.y);
	pkt.ang_z = float_to_fix(msg->angular_velocity.z);

}

void callback_mag(const geometry_msgs::Vector3Stamped::ConstPtr& msg)
{

	pkt.mag_x = float_to_fix(msg->vector.x);
	pkt.mag_y = float_to_fix(msg->vector.y);
	pkt.mag_z = float_to_fix(msg->vector.z);

}


void callback_h(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
	pkt.pos_x = float_to_fix(msg->pose.position.x);
	pkt.pos_y = float_to_fix(msg->pose.position.y);
	pkt.pos_z = float_to_fix(msg->pose.position.z);
	pkt.ros_qx = float_to_fix(msg->pose.orientation.x);
	pkt.ros_qy = float_to_fix(msg->pose.orientation.y);
	pkt.ros_qz = float_to_fix(msg->pose.orientation.z);
	pkt.ros_qw = float_to_fix(msg->pose.orientation.w);
}

void callback_euler(const geometry_msgs::Vector3Stamped::ConstPtr& msg)
{
	pkt.euler_x = float_to_fix(msg->vector.x);
	pkt.euler_y = float_to_fix(msg->vector.y);
	pkt.euler_z = float_to_fix(msg->vector.z);
}

int main(int argc,char **argv){

	ros::init(argc, argv, "to_mpsoc");
	ros::NodeHandle n;
	ROS_INFO("Running to-mpsoc Node");
	hf_send_setup(MPSOC_ADDR, MPSOC_PORT);

	imu_to_mpsoc = n.subscribe("/raw_imu", FREQ_READ, callback_imu);
	mag_to_mpsoc = n.subscribe("/magnetic", FREQ_READ, callback_mag);
	height_to_mpsoc = n.subscribe("/ground_truth_to_tf/pose", FREQ_READ, callback_h);
	euler_to_mpsoc = n.subscribe("/ground_truth_to_tf/euler", FREQ_READ, callback_euler);
	ros::Rate rate(FREQ_READ);

	while(ros::ok()){
		ros::spinOnce();
		hf_send(1, 5000, (int8_t*)(&pkt), sizeof(pkt), 100);
		rate.sleep();

	}
	return 0;
}
