#ifndef __PID_H
#define __PID_H

#include <fixed.h>
#include <matrix.h>

struct pid_states{
  fixed_t anti[4];
  fixed_t err_sum[4];
  fixed_t last_err[4];
  fixed_t dlast_err[4];
  fixed_t err_sumX;
  fixed_t err_sumY;
  fixed_t last_errX;
  fixed_t last_errY;
} ;

void pid(fixed_t *out, fixed_t in[6], struct pid_states* pidS);

#endif
