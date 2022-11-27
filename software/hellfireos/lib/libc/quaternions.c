#include <quaternions.h>


void imu2q(fixed_t *quat, fixed_t acc[3], fixed_t mag[3])
{

		fixed_t roll_acc, pitch_acc, yaw_acc;
		fixed_t auxyaw1, auxyaw2;

		if ((fix_sqrt(fix_square(acc[0])+fix_square(acc[2]))) == fix_val(0)){
			roll_acc = fix_val(0);
		}
		else{
			roll_acc = fix_atan(fix_div(acc[1], fix_sqrt(fix_square(acc[0]) + fix_square(acc[2]))));
		}

		if ((fix_sqrt(fix_square(acc[1])+fix_square(acc[2]))) == fix_val(0)){
			pitch_acc = fix_val(0);
		}
		else{
			pitch_acc = fix_atan(fix_div(fix_mul(fix_val(-1),acc[0]), fix_sqrt(fix_square(acc[1]) + fix_square(acc[2]))));
		}


		if ((fix_mul(mag[0],fix_cos(pitch_acc)) + fix_mul(mag[1],fix_mul(fix_sin(pitch_acc),fix_sin(roll_acc))) + fix_mul(mag[2],fix_mul(fix_sin(pitch_acc),fix_cos(roll_acc)))) == fix_val(0)){
			yaw_acc = fix_val(0);
		}
		else{
			auxyaw1 = fix_mul(fix_mul(fix_val(-1),mag[1]), fix_cos(roll_acc)) + fix_mul(mag[2], fix_sin(roll_acc));
			auxyaw2 = fix_mul(mag[0],fix_cos(pitch_acc)) + fix_mul(mag[1],fix_mul(fix_sin(pitch_acc),fix_sin(roll_acc))) + fix_mul(mag[2],fix_mul(fix_sin(pitch_acc),fix_cos(roll_acc)));
			yaw_acc = fix_atan(fix_div(auxyaw1,auxyaw2));
		}


		quat[0] = fix_mul((fix_cos(fix_div(pitch_acc,fix_val(2)))),(fix_mul(fix_cos(fix_div(roll_acc,fix_val(2))),fix_cos(fix_div(yaw_acc,fix_val(2)))))) + fix_mul(fix_sin(fix_div(pitch_acc,fix_val(2))),fix_mul(fix_sin(fix_div(roll_acc,fix_val(2))),fix_sin(fix_div(yaw_acc,fix_val(2)))));
		quat[1] = fix_mul((fix_sin(fix_div(pitch_acc,fix_val(2)))),fix_mul(fix_cos(fix_div(roll_acc,fix_val(2))),fix_cos(fix_div(yaw_acc,fix_val(2))))) + fix_mul(fix_cos(fix_div(pitch_acc,fix_val(2))),fix_mul(fix_sin(fix_div(roll_acc,fix_val(2))),fix_sin(fix_div(yaw_acc,fix_val(2)))));
		quat[2] = fix_mul((fix_cos(fix_div(pitch_acc,fix_val(2)))),fix_mul(fix_sin(fix_div(roll_acc,fix_val(2))),fix_cos(fix_div(yaw_acc,fix_val(2))))) + fix_mul(fix_sin(fix_div(pitch_acc,fix_val(2))),fix_mul(fix_cos(fix_div(roll_acc,fix_val(2))),fix_sin(fix_div(yaw_acc,fix_val(2)))));
		quat[3] = fix_mul((fix_cos(fix_div(pitch_acc,fix_val(2)))),fix_mul(fix_cos(fix_div(roll_acc,fix_val(2))),fix_sin(fix_div(yaw_acc,fix_val(2))))) + fix_mul(fix_sin(fix_div(pitch_acc,fix_val(2))),fix_mul(fix_sin(fix_div(roll_acc,fix_val(2))),fix_cos(fix_div(yaw_acc,fix_val(2)))));

}


void q2ea(fixed_t *ea, fixed_t quat[4])
{
	fixed_t qx, qy, qz, qw, eaz1, eaz2, eay1, eay2, eax1, eax2, eax3;

	qw = quat[0];
	qx = quat[1];
	qy = quat[2];
	qz = quat[3];

	eax1 = fix_mul(fix_val(2),(fix_mul(qw,qx) + fix_mul(qy,qz)));
	eax2 = fix_val(1) - fix_mul(fix_val(2),(fix_square(qx)+fix_square(qy)));
	eay1 = fix_mul(fix_val(2),(fix_mul(qw,qy)-fix_mul(qz,qx)));
	eaz1 = fix_mul(fix_val(2),(fix_mul(qw,qz) + fix_mul(qx,qy)));
	eaz2 = fix_val(1) - fix_mul(fix_val(2),(fix_square(qy)+fix_square(qz)));

	ea[0] = fix_atan2(eax1, eax2);
	ea[1] = fix_asin(eay1);
	ea[2] = fix_atan2(eaz1, eaz2);


}
