#ifndef __QUATERNIONS_H
#define __QUATERNIONS_H

#include <fixed.h>
#include <matrix.h>


void imu2q(fixed_t *quat, fixed_t acc[3], fixed_t mag[3]);
void q2ea(fixed_t *ea, fixed_t quat[4]);


#endif
