#ifndef _MSG_FORMAT_LIB_HF_H
#define _MSG_FORMAT_LIB_HF_H

#include <hellfire.h>
#include <fixed.h>

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

} ekf_data_t;


typedef struct {

	fixed_t u1;
	fixed_t u2;
	fixed_t u3;
	fixed_t u4;

} pid_data_t;




#endif
