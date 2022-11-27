#include "ekf-app.h"

#include <msg_format.h>
#include <hellfire.h>
#include <noc.h>
#include <fixed.h>
#include <ekf.h>

#include "orca-hardware-counters.h"

#define M0CS *M0_COUNTER_STORE
#define M0CL *M0_COUNTER_LOAD
#define M1CS *M1_COUNTER_STORE
#define M1CL *M1_COUNTER_LOAD
#define M2CS *M2_COUNTER_STORE
#define M2CL *M2_COUNTER_LOAD
#define CP1A *CPU_COUNTER_ARITH
#define CP1M *CPU_COUNTER_MUL
#define CP1L *CPU_COUNTER_LOGICAL
#define CP2S *CPU_COUNTER_SHIFT
#define CP2B *CPU_COUNTER_BRANCHES
#define CP3J *CPU_COUNTER_JUMPS
#define CP3L *CPU_COUNTER_LOADSTORE
#define CP4C *CPU_COUNTER_CYCLES_TOTAL
#define CP4S *CPU_COUNTER_CYCLES_STALL
#define ROUA *ROUTER_COUNTER_ACTIVE

void ekf_main(void){


  int8_t buf[100];
	uint16_t cpu, port, size;
	int16_t val;
  int is_set = 0;

  fixed_t ACC[3], GYR[3], MAG[3], Xin[4], Xout[4];
  fixed_t height, posX, posY, real_qw, real_qx, real_qy, real_qz;
  fixed_t eux, euy, euz;

  struct matrix Q, R, Pin;
  struct ekf_out ST;

  uint32_t antes = 0;
  uint32_t depois = 0;

  ekf_data_t data;
  ekf_data_t data_send;

// -------------------Setting Covariance Matrices-------------------------------


  fixed_t valQ[16] = {fix_val(0.01),fix_val(0),fix_val(0),fix_val(0),
                      fix_val(0),fix_val(1),fix_val(0),fix_val(0),
                      fix_val(0),fix_val(0),fix_val(1),fix_val(0),
                      fix_val(0),fix_val(0),fix_val(0),fix_val(0.01)};

  Q = set_values(4, 4, valQ);

  fixed_t valR[16] =  {fix_val(0.1),fix_val(0),fix_val(0),fix_val(0),
                       fix_val(0),fix_val(10),fix_val(0),fix_val(0),
                       fix_val(0),fix_val(0),fix_val(10),fix_val(0),
                       fix_val(0),fix_val(0),fix_val(0),fix_val(10)};

  R = set_values(4, 4, valR);
  Pin = setEye(4);


// -----------------------------------------------------------------------------
	//start new communication on port 5000
  if (hf_comm_create(hf_selfid(), 5000, 0))
		panic(0xff);

  printf("\n");

	while(1){
		//check whether a new packet has arrived
		int32_t i = hf_recvprobe();

		if(i >= 0){

			//recv packet from the buffer
			val = hf_recv(&cpu, &port, buf, &size, i);
      antes = *CPU_COUNTER_HOSTTIME;

			if (val){
				printf("hf_recv(): error %d\n", val);
			}

      else{

        data = *(ekf_data_t*)buf; //get data from buffer

        if (is_set == 0){

          ACC[0] = data.acc_x;
    			ACC[1] = data.acc_y;
    			ACC[2] = data.acc_z;
          GYR[0] = data.ang_x;
          GYR[1] = data.ang_y;
          GYR[2] = data.ang_z;
    			MAG[0] = data.mag_x;
          MAG[1] = data.mag_y;
          MAG[2] = data.mag_z;
          height = data.pos_z;
          posX = data.pos_x;
          posY = data.pos_y;
          real_qw = data.ros_qw;
          real_qx = data.ros_qx;
          real_qy = data.ros_qy;
          real_qz = data.ros_qz;
          eux = data.euler_x;
          euy = data.euler_y;
          euz = data.euler_z;


          Xin[0] = fix_val(1);
          Xin[1] = fix_val(0);
          Xin[2] = fix_val(0);
          Xin[3] = fix_val(0);

          ST = ekf(Xin, ACC, GYR, MAG, Q, R, Pin);

          for(int j = 0; j < 4; j++){
            Xout[j] = get_value(ST.states, j, 0);
          }

          data_send.ros_qw = real_qw;
          data_send.ros_qx = real_qx;
          data_send.ros_qy = real_qy;
          data_send.ros_qz = real_qz;
          data_send.pos_x = posX;
          data_send.pos_y = posY;
          data_send.pos_z = height;
          data_send.euler_x = eux;
          data_send.euler_y = euy;
          data_send.euler_z = euz;



          hf_send(2, 5000, (int8_t*)&data_send, sizeof(ekf_data_t), 100);

          depois = *CPU_COUNTER_HOSTTIME;
          is_set = 1;
          printf("latency = %d ms\n", depois - antes);
          printf("\n");
        }

        else{

          ACC[0] = data.acc_x;
          ACC[1] = data.acc_y;
          ACC[2] = data.acc_z;
          GYR[0] = data.ang_x;
          GYR[1] = data.ang_y;
          GYR[2] = data.ang_z;
          MAG[0] = data.mag_x;
          MAG[1] = data.mag_y;
          MAG[2] = data.mag_z;
          height = data.pos_z;
          posX = data.pos_x;
          posY = data.pos_y;
          real_qw = data.ros_qw;
          real_qx = data.ros_qx;
          real_qy = data.ros_qy;
          real_qz = data.ros_qz;
          eux = data.euler_x;
          euy = data.euler_y;
          euz = data.euler_z;

          // printf("real_qw:");
          // fix_print(real_qw);
          // printf("real_qx:");
          // fix_print(real_qx);
          // printf("real_qy:");
          // fix_print(real_qy);
          // printf("real_qz:");
          // fix_print(real_qz);
          // printf("height:");
          // fix_print(height);
          //

          for(int j = 0; j < 4; j++){
            Xin[j] = get_value(ST.states, j, 0);
          }

          Pin = copy_matrix(ST.MatP);

          ST = ekf(Xin, ACC, GYR, MAG, Q, R, Pin);

          for(int j = 0; j < 4; j++){
            Xout[j] = get_value(ST.states, j, 0);
          }

          data_send.ros_qw = real_qw;
          data_send.ros_qx = real_qx;
          data_send.ros_qy = real_qy;
          data_send.ros_qz = real_qz;
          data_send.pos_x = posX;
          data_send.pos_y = posY;
          data_send.pos_z = height;
          data_send.euler_x = eux;
          data_send.euler_y = euy;
          data_send.euler_z = euz;


          hf_send(2, 5000, (int8_t*)&data_send, sizeof(ekf_data_t), 100);
          depois = *CPU_COUNTER_HOSTTIME;
          printf("latency = %d ms\n", depois - antes);
          printf("\n");
        }
      }
	  }

    // printf("MEM0: writes=%u, reads=%u\n", M0CS, M0CL);
    // printf("MEM1: writes=%u, reads=%u\n", M1CS, M1CL);
    // printf("MEM2: writes=%u, reads=%u\n", M2CS, M2CL);
    // printf("---\n");
    //
    // printf("CPU1: arith=%u, logical=%u, mul=%u\n", CP1A, CP1L, CP1M);
    // printf("CPU2: shift=%u, branches=%u\n", CP2S, CP2B);
    // printf("CPU3: jumps=%u, loadstore=%u\n", CP3J, CP3L);
    // printf("CPU4: cycles=%u, stalls=%u\n", CP4C, CP4S);
    // printf("---\n");
    //
    // printf("ROUTER: active=%u\n", ROUA);
    // printf("---\n");
    //
    // M0CS = 0;
    // M0CL = 0;
    // M1CS = 0;
    // M1CL = 0;
    // M2CS = 0;
    // M2CL = 0;
    // CP1A = 0;
    // CP1L = 0;
    // CP1M = 0;
    // CP2S = 0;
    // CP2B = 0;
    // CP3J = 0;
    // CP3L = 0;
    // CP4C = 0;
    // CP4S = 0;
    // ROUA = 0;
    // CP1M = 0;


  }


}
