#include "ros/ros.h"

#include "/home/paulo/hectorQuad_mp/devel/include/hector_uav_msgs/MotorPWM.h"

using namespace hector_uav_msgs;

#include <iostream>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

//ros references
ros::Publisher  pwm_out;
ros::Subscriber pwm_in;

clock_t t;


void callback(const hector_uav_msgs::MotorPWM& msg)
{

		pwm_out.publish(msg);
		printf ("It took me %d clicks (%f seconds).\n",clock()-t,((float)(clock()-t))/CLOCKS_PER_SEC);
		t = clock();


}


//Main routine: setup publishers and subscribers. Data incoming from the
//mpsoc will be published to the "orca_mpsoc_to_udp" topic, and all messages
//received on "orca_udp_to_mpsoc" will be pushed to mpsoc's udp adapter
int main(int argc,char **argv){

	ros::init(argc, argv, "example_orca_udp");
	ros::NodeHandle n;


	 pwm_in = n.subscribe("/motor_pwm_in", 10, callback);

	  		//publish to the "orca_mpsoc_to_ros" topic. All messages
		//in this topic come from the MPSoC
		pwm_out = n.advertise<hector_uav_msgs::MotorPWM>("/motor_pwm", 10);

	//start receiving thread
	//ros::Rate r(10);
	ros::spin();

	return 0;
}
