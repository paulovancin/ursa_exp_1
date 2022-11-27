/*
 * Implementation file for drone-pid task.
 * Copyright (C) 2018-2019 Anderson Domingues, <ti.andersondomingues@gmail.com>
 * This file is part of project URSA (http://https://github.com/andersondomingues/ursa).
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */



#include "pid-app.h"
#include <msg_format.h>
#include <quaternions.h>
#include <pid.h>
#include "orca-hardware-counters.h"

#define M0CS *M0_COUNTER_STORE
#define M0CL *M0_COUNTER_LOAD
#define M1CS *M1_COUNTER_STORE
#define M1CL *M1_COUNTER_LOAD
#define M2CS *M2_COUNTER_STORE
#define M2CL *M2_COUNTER_LOAD
#define CP1A *CPU_COUNTER_ARITH
#define CP1L *CPU_COUNTER_LOGICAL
#define CP1M *CPU_COUNTER_MUL
#define CP2S *CPU_COUNTER_SHIFT
#define CP2B *CPU_COUNTER_BRANCHES
#define CP3J *CPU_COUNTER_JUMPS
#define CP3L *CPU_COUNTER_LOADSTORE
#define CP4C *CPU_COUNTER_CYCLES_TOTAL
#define CP4S *CPU_COUNTER_CYCLES_STALL
#define ROUA *ROUTER_COUNTER_ACTIVE

void pid_main(void){

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


    int8_t buf[500];
    uint16_t cpu, port, size;
    int16_t val;
    fixed_t q[4], ea[3], height, u[4], pid_in[6], Ts, posX, posY;
    fixed_t ex, ey, ez;
    ekf_data_t data;
    pid_data_t data_send;

    struct pid_states pidS;

    uint32_t antes = 0;
    uint32_t depois = 0;


    if(hf_comm_create(hf_selfid(), 5000, 0))
        panic(0xff);

    while(1){

        int32_t i = hf_recvprobe();

        //check whether a packet arrived
        if(i >= 0){

            val = hf_recv(&cpu, &port, buf, &size, i);
            antes = *CPU_COUNTER_HOSTTIME;

            if(val){
                printf("hf_recv(): error %d\n", val);
              }

            else{

                data = *(ekf_data_t*)buf;

                q[0] = data.ros_qw;
                q[1] = data.ros_qx;
                q[2] = data.ros_qy;
                q[3] = data.ros_qz;
                height = data.pos_z;
                posX = data.pos_x;
                posY = data.pos_y;
                ex = data.euler_x;
                ey = data.euler_y;
                ez = data.euler_z;

                q2ea(ea, q);

                pid_in[0] = ex;
                pid_in[1] = ey;
                pid_in[2] = ez;
                pid_in[3] = height;
                pid_in[4] = posX;
                pid_in[5] = posY;


                pid(u, pid_in, &pidS);



                data_send.u1 = u[0];
                data_send.u2 = u[1];
                data_send.u3 = u[2];
                data_send.u4 = u[3];


                hf_send(0, 5000, (int8_t*)&data_send, sizeof(data_send), 100);
                depois = *CPU_COUNTER_HOSTTIME;
                printf("latency:%d\n", depois - antes);
                printf("\n");

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
    }

}
