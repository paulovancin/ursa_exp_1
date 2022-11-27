#include <pid.h>
#include <quaternions.h>



void pid(fixed_t *u, fixed_t in[6], struct pid_states* pidS)
{

  fixed_t u_roll, u_pitch, u_yaw, u_height, u_aux[4];
  fixed_t err[4], derv1[4], dderv1[4];
  fixed_t kd, ki, kp, kd_alt, ki_alt, kp_alt, kdd, kdd_alt, nom_Thrust;
  fixed_t kp_yaw, ki_yaw, kd_yaw, kdd_yaw;
  fixed_t kpPOSx, kiPOSx, kdPOSx, kpPOSy, kiPOSy, kdPOSy, errX, errY, dervX, dervY, uR, uP;
  fixed_t sat_high = fix_val(255);
  fixed_t sat_low = fix_val(0);
  fixed_t roll, pitch, yaw;
  fixed_t rad_sat = fix_val(0.015);
  fixed_t u_sat = fix_val(0.5);

  // ------------------ POSITION CONTROL-----------------------------


  // Best So Far
  kpPOSx = fix_val(0.1);
  kiPOSx = fix_val(0.00001);
  kdPOSx = fix_val(2.5);

  kpPOSy = fix_val(0.1);
  kiPOSy = fix_val(0.00001);
  kdPOSy = fix_val(2.5);


  errX = fix_val(0) - in[4];
  errY = fix_val(0) - in[5];

  pidS->err_sumX += errX;
  pidS->err_sumY += errY;

  dervX = errX - pidS->last_errX;
  dervY = errY - pidS->last_errY;

  pidS->last_errX = errX;
  pidS->last_errY = errY;


  uR = fix_mul(fix_mul(fix_val(-1), kdPOSy), dervY) - fix_mul(kpPOSy, errY) - fix_mul(kiPOSy, pidS->err_sumY);
  uP = fix_mul(fix_mul(fix_val(1), kdPOSx), dervX) + fix_mul(kpPOSx, errX) + fix_mul(kiPOSx, pidS->err_sumX);

  if (uR > rad_sat){
    uR = rad_sat;
  }
  if (uR < fix_mul(fix_val(-1), rad_sat)){
    uR = fix_mul(fix_val(-1), rad_sat);
  }
  if (uP > rad_sat){
    uP = rad_sat;
  }
  if (uP < fix_mul(fix_val(-1), rad_sat)){
    uP = fix_mul(fix_val(-1), rad_sat);
  }

  // ------------------ ATTITUDE AND HEIGHT CONTROL-----------------------------

  kp = fix_val(10);
  ki = fix_val(0);
  kd = fix_val(10);
  kdd = fix_val(100);

  kp_yaw = fix_val(1);
  ki_yaw = fix_val(0);
  kd_yaw = fix_val(1);
  kdd_yaw = fix_val(0);

  kp_alt = fix_val(10);
  ki_alt = fix_val(0.005);
  kd_alt = fix_val(1000);
  kdd_alt = fix_val(10);

  nom_Thrust = fix_val(135);

  // Errors
  err[0] = uR - in[0];
  err[1] = uP - in[1];
  err[2] = fix_val(0) - in[2];
  err[3] = fix_val(1) - in[3];


  // Errors Sum
  pidS->err_sum[0] += (err[0] - pidS->anti[0]);
  pidS->err_sum[1] += (err[1] - pidS->anti[1]);
  pidS->err_sum[2] += (err[2] - pidS->anti[2]);
  pidS->err_sum[3] += (err[3] - pidS->anti[3]);


  // Derivatives First Order
  derv1[0] = err[0] - pidS->last_err[0];
  derv1[1] = err[1] - pidS->last_err[1];
  derv1[2] = err[2] - pidS->last_err[2];
  derv1[3] = err[3] - pidS->last_err[3];

  pidS->last_err[0] = err[0];
  pidS->last_err[1] = err[1];
  pidS->last_err[2] = err[2];
  pidS->last_err[3] = err[3];


  // Derivatives Second Order
  dderv1[0] = pidS->last_err[0] - pidS->dlast_err[0];
  dderv1[1] = pidS->last_err[1] - pidS->dlast_err[1];
  dderv1[2] = pidS->last_err[2] - pidS->dlast_err[2];
  dderv1[3] = pidS->last_err[3] - pidS->dlast_err[3];

  pidS->dlast_err[0] = pidS->last_err[0];
  pidS->dlast_err[1] = pidS->last_err[1];
  pidS->dlast_err[2] = pidS->last_err[2];
  pidS->dlast_err[3] = pidS->last_err[3];


  // Control Law for u_roll
  u_roll = fix_mul(kdd, dderv1[0]) + fix_mul(kd, derv1[0]) + fix_mul(kp, err[0]) + fix_mul(ki, pidS->err_sum[0]);
  // Control Law for u_pitch
  u_pitch =  fix_mul(kdd, dderv1[1]) + fix_mul(kd, derv1[1]) + fix_mul(kp, err[1]) + fix_mul(ki, pidS->err_sum[1]);
  // Control Law for u_yaw
  u_yaw = fix_mul(kdd_yaw, dderv1[2]) + fix_mul(kd_yaw, derv1[2]) + fix_mul(kp_yaw, err[2]) + fix_mul(ki_yaw, pidS->err_sum[2]);
  //Control Law for u_height
  u_height = fix_mul(kdd_alt, dderv1[3]) + fix_mul(kd_alt, derv1[3]) + fix_mul(kp_alt, err[3]) + fix_mul(ki_alt, pidS->err_sum[3]) + nom_Thrust;


  if (u_roll > u_sat){
    u_roll = u_sat;
  }
  if (u_roll < fix_mul(fix_val(-1), u_sat)){
    u_roll = fix_mul(fix_val(-1), u_sat);
  }
  if (u_pitch > u_sat){
    u_pitch = u_sat;
  }
  if (u_pitch < fix_mul(fix_val(-1), u_sat)){
    u_pitch = fix_mul(fix_val(-1), u_sat);
  }

// Working
u_aux[0] = fix_mul(fix_val(-1),u_pitch) + u_yaw + u_height;
u_aux[1] = fix_mul(fix_val(-1),u_roll) - u_yaw  + u_height;
u_aux[2] = u_pitch + u_yaw + u_height;
u_aux[3] = u_roll - u_yaw + u_height;

// u_aux[0] = fix_val(0);
// u_aux[1] = fix_val(0);
// u_aux[2] = fix_val(0);
// u_aux[3] = fix_val(0);

// Anti-Windup Actuator

// Control Law for Motor 1

if (u_aux[0] < sat_low){
  u[0] = sat_low;
  pidS->anti[0] = sat_low - u[0];
}

if (u_aux[0] > sat_high){
  u[0] = sat_high;
  pidS->anti[0] = sat_high - u[0];
}

if (sat_low <= u_aux[0] && u_aux[0] <= sat_high){
  u[0] = u_aux[0];
  pidS->anti[0] = fix_val(0);
}


// Control Law for Motor 2

if (u_aux[1] < sat_low){
  u[1] = sat_low;
  pidS->anti[1] = sat_low - u[1];
}

if (u_aux[1] > sat_high){
  u[1] = sat_high;
  pidS->anti[1] = sat_high - u[1];
}

if (sat_low <= u_aux[1] && u_aux[1] <= sat_high){
  u[1] = u_aux[1];
  pidS->anti[1] = fix_val(0);
}


// Control Law for Motor 3

if (u_aux[2] < sat_low){
  u[2] = sat_low;
  pidS->anti[2] = sat_low - u[2];
}

if (u_aux[2] > sat_high){
  u[2] = sat_high;
  pidS->anti[2] = sat_high - u[2];
}

if (sat_low <= u_aux[2] && u_aux[2] <= sat_high){
  u[2] = u_aux[2];
  pidS->anti[2] = fix_val(0);
}


// Control Law for Motor 4

if (u_aux[3] < sat_low){
  u[3] = sat_low;
  pidS->anti[3] = sat_low - u[3];
}

if (u_aux[3] > sat_high){
  u[3] = sat_high;
  pidS->anti[3] = sat_high - u[3];
}

if (sat_low <= u_aux[3] && u_aux[3] <= sat_high){
  u[3] = u_aux[3];
  pidS->anti[3] = fix_val(0);
}



}
