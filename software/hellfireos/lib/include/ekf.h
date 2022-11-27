#ifndef __EKF_H
#define __EKF_H

#include <matrix.h>

struct ekf_out ekf(fixed_t X[7], fixed_t acc[3], fixed_t gyro[3], fixed_t mag[3], struct matrix Q, struct matrix R, struct matrix P_in);

#endif
