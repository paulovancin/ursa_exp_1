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

	unsigned int u1;
	unsigned int u2;
	unsigned int u3;
	unsigned int u4;

} pid_data_t;



fixed_t float_to_fix(float val)
{
	return fix_val(val);
}

float fix_to_float(fixed_t val)
{
	float ip, fp;

	ip = (float)(val >> FIX_IBITS);
	fp = ((float)(val & FIX_FMASK)) / ((float)(1 << FIX_FBITS));

	return ip + fp;
}

int idSeq = 0;
int8_t msg[500];
float antes, depois;

//Address of the mpsoc platform at the network
#define MPSOC_ADDR "127.0.0.1"
#define MPSOC_PORT 9999

//Address of current rosnode at the network.
//Must be set as localhost (loopback). Port can safely+
//be changed as long as
#define ROSNODE_ADDR "127.0.0.1"
#define ROSNODE_PORT 8888

// #define FREQ_WRITE 12 // NO MU
#define FREQ_WRITE 25 // WITH MU

//ros references
ros::Publisher  orca_mpsoc_to_ros;
hector_uav_msgs::MotorPWM msg_ros_pwm;


void* recv_from_mpsoc(void* v){

	uint16_t source_cpu, source_port, data_size, channel;
	pid_data_t data;
	//begin = ros::Time::now().toSec();
	clock_t t;

	hf_recv_setup(ROSNODE_ADDR, ROSNODE_PORT);

	while(1){
		// antes = ros::Time::now().toSec();
		int res = hf_recv(&source_cpu, &source_port, (int8_t*)&msg, &data_size, &channel);
		// depois = ros::Time::now().toSec();
		// ROS_INFO("Elapsed Time: %f", depois - antes);

		if (channel == 100){

			data = *(pid_data_t*)msg;

			msg_ros_pwm.header.seq = idSeq++;
			msg_ros_pwm.header.stamp = ros::Time::now();
			msg_ros_pwm.header.frame_id ="world";
			msg_ros_pwm.pwm = {fix_int(data.u1), fix_int(data.u2), fix_int(data.u3), fix_int(data.u4)};
		}
	}
}

int main(int argc,char **argv){

	ros::init(argc, argv, "to_hector");
	ros::NodeHandle n;
	hf_send_setup(MPSOC_ADDR, MPSOC_PORT);
	orca_mpsoc_to_ros = n.advertise<hector_uav_msgs::MotorPWM>("/motor_pwm", FREQ_WRITE);
	pthread_t thread;
	pthread_create(&thread, NULL, recv_from_mpsoc, NULL);

	ros::Rate rate(FREQ_WRITE);

	while(ros::ok()){

		ros::spinOnce();
		orca_mpsoc_to_ros.publish(msg_ros_pwm);
		rate.sleep();
	}
	return 0;
}
