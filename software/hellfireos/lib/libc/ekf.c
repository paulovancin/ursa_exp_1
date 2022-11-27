#include <ekf.h>
#include <quaternions.h>

struct ekf_out ekf(fixed_t X[7], fixed_t acc[3], fixed_t gyro[3], fixed_t mag[3], struct matrix Q, struct matrix R, struct matrix P_in)
{
  struct matrix w, wt, sw, zero, omega, half_omega, ts_omega, X2;
  struct matrix m_wt, m_sw, eye4, F, Ft, X_mat, P1, P2, P, PR, K1, K;
  struct matrix q_mat, corr_mat, K_corr, eyeK;
  struct ekf_out OUT;

  fixed_t Ts = fix_val(0.05);
  fixed_t quat_measured[4];

// --------- Transform Sensor Data Into Quaternions ----------

  imu2q(quat_measured, acc, mag);
  q_mat = set_values(4,1, quat_measured);

// -----------------------------------------------------------

  w = set_values(3, 1, gyro);
  wt = transposed(w);
  sw = crossProduct3(gyro[0], gyro[1], gyro[2]);
  zero = zeros(1,1);

  m_wt = multE(wt, fix_val(-1));
  m_sw = multE(sw, fix_val(-1));

  int conf_omega[4] = {2,2,4,4};
  omega = customMat(4, conf_omega, zero, m_wt, w, m_sw);
  half_omega = multE(omega, fix_val(0.5));

  eye4 = setEye(4);
  ts_omega = multE(omega, Ts);
  F = sum(eye4, ts_omega);


// ------------------ Prediction --------------------------------

  X_mat = set_values(4, 1, X);
  X2 = multiplication(F, X_mat);
  Ft = transposed(F);
  P1 = multiplication(F, P_in);
  P2 = multiplication(P1, Ft);
  P = sum(P2, Q);

// ------------------ Kalman Gain --------------------------------

  PR = sum(P, R);
  K1 = four_by_four_invert(PR);
  K = multiplication(P, K1);

// ------------------ Correction --------------------------------

  corr_mat = subtraction(q_mat, X2);
  K_corr = multiplication(K, corr_mat);
  X_mat = sum(X2, K_corr);
  eyeK = subtraction(eye4, K);
  P = multiplication(eyeK, P);

  OUT.states = copy_matrix(X_mat);
  OUT.MatP = copy_matrix(P);

  return OUT;

}

// ---------------------------------------------------------------------------
