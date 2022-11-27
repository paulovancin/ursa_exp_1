import math
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.font_manager
from tf.transformations import euler_from_quaternion, quaternion_from_euler
from matplotlib.lines import Line2D


plt.rc('font',**{'family':'sans-serif','serif':['Helvetica']})
plt.rc('text', usetex=True)

ekf_data = open("001.cpu_uart.log", "r")

lines_ekf = ekf_data.readlines()

qw_r = []
qx_r = []
qy_r = []
qz_r = []

###################################################################

qw_e = []
qx_e = []
qy_e = []
qz_e = []

###################################################################

h = []





l_ek = []

###################################################################


for line in lines_ekf:
    if 'real_qw:' in line:
        sline = line.replace('real_qw:','').split()
        for item in sline:
           qw_r.append(float(item))


for line in lines_ekf:
    if 'real_qx:' in line:
        sline = line.replace('real_qx:','').split()
        for item in sline:
           qx_r.append(float(item))


for line in lines_ekf:
    if 'real_qy:' in line:
        sline = line.replace('real_qy:','').split()
        for item in sline:
           qy_r.append(float(item))


for line in lines_ekf:
    if 'real_qz:' in line:
        sline = line.replace('real_qz:','').split()
        for item in sline:
           qz_r.append(float(item))

###################################################################

for line in lines_ekf:
    if 'ekf_qw:' in line:
        sline = line.replace('ekf_qw:','').split()
        for item in sline:
           qw_e.append(float(item))


for line in lines_ekf:
    if 'ekf_qx:' in line:
        sline = line.replace('ekf_qx:','').split()
        for item in sline:
           qx_e.append(float(item))


for line in lines_ekf:
    if 'ekf_qy:' in line:
        sline = line.replace('ekf_qy:','').split()
        for item in sline:
           qy_e.append(float(item))


for line in lines_ekf:
    if 'ekf_qz:' in line:
        sline = line.replace('ekf_qz:','').split()
        for item in sline:
           qz_e.append(float(item))

###################################################################


for line in lines_ekf:
    if 'height:' in line:
        sline = line.replace('height:','').split()
        for item in sline:
           h.append(float(item))


height_setpoint = [1] * len(h)

for line in lines_ekf:
    if 'latency:' in line:
        sline = line.replace('latency:','').split()
        for item in sline:
           l_ek.append(float(item))


###################################################################

kf_data = open("002.cpu_uart.log", "r")

lines_kf = kf_data.readlines()

###################################################################

x_r = []
y_r = []

###################################################################

x_k = []
y_k = []

###################################################################
l_k = []
###################################################################


for line in lines_kf:
    if 'posX:' in line:
        sline = line.replace('posX:','').split()
        for item in sline:
           x_r.append(float(item))


for line in lines_kf:
    if 'posY:' in line:
        sline = line.replace('posY:','').split()
        for item in sline:
           y_r.append(float(item))



###################################################################


###################################################################


for line in lines_kf:
    if 'kf_x:' in line:
        sline = line.replace('kf_x:','').split()
        for item in sline:
           x_k.append(float(item))


for line in lines_kf:
    if 'kf_y:' in line:
        sline = line.replace('kf_y:','').split()
        for item in sline:
           y_k.append(float(item))



###################################################################







###################################################################

for line in lines_kf:
    if 'latency:' in line:
        sline = line.replace('latency:','').split()
        for item in sline:
           l_k.append(float(item))



###################################################################



kf_data.close()


pid_data = open("003.cpu_uart.log", "r")

lines_pid = pid_data.readlines()

##################################################################

l_p = []

##################################################################


for line in lines_pid:
    if 'latency:' in line:
        sline = line.replace('latency:','').split()
        for item in sline:
           l_p.append(float(item))



###################################################################


pid_data.close()

# KF Errors:

errX_kf = [x_k[i] - x_r[i] for i in range(min(len(x_k),len(x_r)))]
errY_kf = [y_k[i] - y_r[i] for i in range(min(len(y_k),len(y_r)))]

totalX_kf = 0
totalY_kf = 0


for i in range(0, len(errX_kf)):
    totalX_kf = totalX_kf + abs(errX_kf[i])

for i in range(0, len(errY_kf)):
    totalY_kf = totalY_kf + abs(errY_kf[i])


meanX_kf = totalX_kf/len(errX_kf)
meanY_kf = totalY_kf/len(errY_kf)

print("Mean KF Error:")
print([meanX_kf, meanY_kf])


# XY Errors:

x_setpoint = []
for i in range(0, len(x_r)):
    x_setpoint.append(0)

y_setpoint = []
for i in range(0, len(y_r)):
    y_setpoint.append(0)


errX_pid = [x_setpoint[i] - x_r[i] for i in range(min(len(x_setpoint),len(x_r)))]
errY_pid = [y_setpoint[i] - y_r[i] for i in range(min(len(y_setpoint),len(y_r)))]

totalX_pid = 0
totalY_pid = 0


for i in range(0, len(errX_kf)):
    totalX_pid = totalX_pid + abs(errX_pid[i])

for i in range(0, len(errY_kf)):
    totalY_pid = totalY_pid + abs(errY_pid[i])


meanX_pid = totalX_pid/len(errX_pid)
meanY_pid = totalY_pid/len(errY_pid)

print("Mean PID XY Error:")
print([meanX_pid, meanY_pid])



# EKF Errors:

errQW_ekf = [qw_e[i] - qw_r[i] for i in range(min(len(qw_e),len(qw_r)))]
errQX_ekf = [qx_e[i] - qx_r[i] for i in range(min(len(qx_e),len(qx_r)))]
errQY_ekf = [qy_e[i] - qy_r[i] for i in range(min(len(qy_e),len(qy_r)))]
errQZ_ekf = [qz_e[i] - qz_r[i] for i in range(min(len(qz_e),len(qz_r)))]

totalQW_ekf = 0
totalQX_ekf = 0
totalQY_ekf = 0
totalQZ_ekf = 0


for i in range(0, len(errQW_ekf)):
    totalQW_ekf = totalQW_ekf + abs(errQW_ekf[i])

for i in range(0, len(errQX_ekf)):
    totalQX_ekf = totalQX_ekf + abs(errQX_ekf[i])

for i in range(0, len(errQY_ekf)):
    totalQY_ekf = totalQY_ekf + abs(errQY_ekf[i])

for i in range(0, len(errQZ_ekf)):
    totalQZ_ekf = totalQZ_ekf + abs(errQZ_ekf[i])

meanQW_ekf = totalQW_ekf/len(errQW_ekf)
meanQX_ekf = totalQX_ekf/len(errQX_ekf)
meanQY_ekf = totalQY_ekf/len(errQY_ekf)
meanQZ_ekf = totalQZ_ekf/len(errQZ_ekf)


print("Mean EKF Error:")
print([meanQW_ekf, meanQX_ekf, meanQY_ekf, meanQZ_ekf])


# Attitude Errors:
qw_setpoint = []
for i in range(0, len(qw_e)):
    qw_setpoint.append(1)

qx_setpoint = []
for i in range(0, len(qx_e)):
    qx_setpoint.append(0)

qy_setpoint = []
for i in range(0, len(qy_e)):
    qy_setpoint.append(0)

qz_setpoint = []
for i in range(0, len(qz_e)):
    qz_setpoint.append(0)


errQW_pid = [qw_r[i] - qw_setpoint[i] for i in range(min(len(qw_r),len(qw_setpoint)))]
errQX_pid = [qx_r[i] - qx_setpoint[i] for i in range(min(len(qx_r),len(qx_setpoint)))]
errQY_pid = [qy_r[i] - qy_setpoint[i] for i in range(min(len(qy_r),len(qy_setpoint)))]
errQZ_pid = [qz_r[i] - qz_setpoint[i] for i in range(min(len(qz_r),len(qz_setpoint)))]

totalQW_pid = 0
totalQX_pid = 0
totalQY_pid = 0
totalQZ_pid = 0


for i in range(0, len(errQW_pid)):
    totalQW_pid = totalQW_pid + abs(errQW_pid[i])

for i in range(0, len(errQX_pid)):
    totalQX_pid = totalQX_pid + abs(errQX_pid[i])

for i in range(0, len(errQY_pid)):
    totalQY_pid = totalQY_pid + abs(errQY_pid[i])

for i in range(0, len(errQZ_pid)):
    totalQZ_pid = totalQZ_pid + abs(errQZ_pid[i])

meanQW_pid = totalQW_pid/len(errQW_pid)
meanQX_pid = totalQX_pid/len(errQX_pid)
meanQY_pid = totalQY_pid/len(errQY_pid)
meanQZ_pid = totalQZ_pid/len(errQZ_pid)


# print("Mean PID Error:")
# print([meanQW_pid, meanQX_pid, meanQY_pid, meanQZ_pid])


# fig = plt.figure(figsize=(10, 5))
# line, = plt.plot(qw_r, label = 'QW Real', linewidth = 2)
# line2, = plt.plot(qx_r, label = 'QX Real', linewidth = 2)
# line3, = plt.plot(qy_r, label = 'QY Real', linewidth = 2)
# line4, = plt.plot(qz_r, label = 'QZ Real', linewidth = 2)
# axes = plt.gca()
# plt.ylim([-0.05, 1.25])
# plt.ylabel('Quaternion',fontsize=16)
# plt.xlabel('Number of Samples',fontsize=16)
# plt.tick_params(labelsize=14)
# plt.grid(color='gray', linestyle='dotted', linewidth=1)
# plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1), fancybox=True, shadow=True, ncol=2, fontsize='xx-large')
# plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/real_quat.png',bbox_inches='tight')
#
# fig = plt.figure(figsize=(10, 5))
# line, = plt.plot(qw_e, label = 'QW EKF', linewidth = 2)
# line2, = plt.plot(qx_e, label = 'QX EKF', linewidth = 2)
# line3, = plt.plot(qy_e, label = 'QY EKF', linewidth = 2)
# line4, = plt.plot(qz_e, label = 'QZ EKF', linewidth = 2)
# axes = plt.gca()
# plt.ylim([-0.05, 1.25])
# plt.ylabel('Quaternion',fontsize=16)
# plt.xlabel('Number of Samples',fontsize=16)
# plt.tick_params(labelsize=14)
# plt.grid(color='gray', linestyle='dotted', linewidth=1)
# plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1), fancybox=True, shadow=True, ncol=2, fontsize='xx-large')
# plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/ekf_quat.png',bbox_inches='tight')


fig = plt.figure(figsize=(10, 5))
line, = plt.plot(qw_e, label = 'QW EKF', linewidth = 2)
line2, = plt.plot(qw_r, label = 'QW Real', linewidth = 2)
axes = plt.gca()
# plt.ylim([0.995, 1.005])
plt.ylabel('Quaternion',fontsize=16)
plt.xlabel('Number of Samples',fontsize=16)
plt.tick_params(labelsize=14)
plt.grid(color='gray', linestyle='dotted', linewidth=1)
plt.legend(loc='lower right', bbox_to_anchor=(1, 0), fancybox=True, shadow=True, ncol=1, fontsize='xx-large')
plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/qw_quat.png',bbox_inches='tight')


fig = plt.figure(figsize=(10, 5))
line, = plt.plot(qx_e, label = 'QX EKF', linewidth = 2)
line2, = plt.plot(qx_r, label = 'QX Real', linewidth = 2)
axes = plt.gca()
plt.ylim([-0.05, 0.05])
plt.ylabel('Quaternion',fontsize=16)
plt.xlabel('Number of Samples',fontsize=16)
plt.tick_params(labelsize=14)
plt.grid(color='gray', linestyle='dotted', linewidth=1)
plt.legend(loc='lower right', bbox_to_anchor=(1, 0), fancybox=True, shadow=True, ncol=1, fontsize='xx-large')
plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/qx_quat.png',bbox_inches='tight')


fig = plt.figure(figsize=(10, 5))
line, = plt.plot(qy_e, label = 'QY EKF', linewidth = 2)
line2, = plt.plot(qy_r, label = 'QY Real', linewidth = 2)
axes = plt.gca()
plt.ylim([-0.05, 0.05])
plt.ylabel('Quaternion',fontsize=16)
plt.xlabel('Number of Samples',fontsize=16)
plt.tick_params(labelsize=14)
plt.grid(color='gray', linestyle='dotted', linewidth=1)
plt.legend(loc='lower right', bbox_to_anchor=(1, 0), fancybox=True, shadow=True, ncol=1, fontsize='xx-large')
plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/qy_quat.png',bbox_inches='tight')


fig = plt.figure(figsize=(10, 5))
line, = plt.plot(qz_e, label = 'QZ EKF', linewidth = 2)
line2, = plt.plot(qz_r, label = 'QZ Real', linewidth = 2)
axes = plt.gca()
plt.ylim([-0.05, 0.25])
plt.ylabel('Quaternion',fontsize=16)
plt.xlabel('Number of Samples',fontsize=16)
plt.tick_params(labelsize=14)
plt.grid(color='gray', linestyle='dotted', linewidth=1)
plt.legend(loc='upper right', bbox_to_anchor=(1, 1), fancybox=True, shadow=True, ncol=1, fontsize='xx-large')
plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/qz_quat.png',bbox_inches='tight')


fig = plt.figure(figsize=(10, 5))
l1, = plt.plot(h, label = 'HEIGHT', linewidth = 2)
l2, = plt.plot(height_setpoint, label = 'SETPOINT', color = 'r', linestyle = 'dashed', linewidth = 2)
axes = plt.gca()
plt.ylabel(r"Height - ($m$)",fontsize=16)
plt.xlabel('Number of Samples',fontsize=16)
plt.tick_params(labelsize=14)
plt.grid(color='gray', linestyle='dotted', linewidth=1)
plt.legend(loc='lower right', bbox_to_anchor=(1, 0), fancybox=True, shadow=True, ncol=1, fontsize='xx-large')
plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/h.png',bbox_inches='tight')


fig = plt.figure(figsize=(10, 5))
line, = plt.plot(x_r, label = 'X Real', linewidth = 2)
# line2, = plt.plot(x_k, label = 'X KF', linewidth = 2)
# line3, = plt.plot(x_g, label = 'X GPS', linewidth = 2)
line4, = plt.plot(x_setpoint, label = 'X SETPOINT', linewidth = 2)
axes = plt.gca()
plt.ylim([-1, 0.75])
plt.ylabel('Position',fontsize=16)
plt.xlabel('Number of Samples',fontsize=16)
plt.tick_params(labelsize=14)
plt.grid(color='gray', linestyle='dotted', linewidth=1)
plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1), fancybox=True, shadow=True, ncol=2, fontsize='xx-large')
plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/x_pos.png',bbox_inches='tight')

fig = plt.figure(figsize=(10, 5))
line, = plt.plot(y_r, label = 'Y Real', linewidth = 2)
# line2, = plt.plot(y_k, label = 'Y KF', linewidth = 2)
# line3, = plt.plot(y_g, label = 'Y GPS', linewidth = 2)
line4, = plt.plot(y_setpoint, label = 'Y SETPOINT', linewidth = 2)
axes = plt.gca()
plt.ylim([-0.4, 0.3])
plt.ylabel('Position',fontsize=16)
plt.xlabel('Number of Samples',fontsize=16)
plt.tick_params(labelsize=14)
plt.grid(color='gray', linestyle='dotted', linewidth=1)
plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1), fancybox=True, shadow=True, ncol=2, fontsize='xx-large')
plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/y_pos.png',bbox_inches='tight')


fig = plt.figure(figsize=(10, 5))
line, = plt.plot(l_ek, label = 'Latency EKF (Core 1)', linewidth = 2)
line2, = plt.plot(l_k, label = 'Latency KF (Core 3)', linewidth = 2)
line3, = plt.plot(l_p, label = 'Latency PID (Core 2)', linewidth = 2)
axes = plt.gca()
# plt.ylim([0, 30])
plt.ylabel('ms',fontsize=16)
plt.xlabel('Number of Samples',fontsize=16)
plt.tick_params(labelsize=14)
plt.grid(color='gray', linestyle='dotted', linewidth=1)
plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1), fancybox=True, shadow=True, ncol=2, fontsize='xx-large')
plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/latency.png',bbox_inches='tight')



# legend_elements = [Line2D([0], [0], color='b', lw=2, label='XY POSITION'),
#                    Line2D([0], [0], marker='o', color='w', label='SETPOINT',
#                           markerfacecolor='r', markersize=15)]
#
# fig = plt.figure(figsize=(10, 4))
# l1, = plt.plot(x_r, y_r, label = 'XY POSITION', linewidth = 2)
# circle = plt.Circle(( 0.0 , 0.0 ), 0.05, color='r' )
# axes = plt.gca()
# axes.add_artist(circle)
# plt.ylim([-0.5, 0.5])
# plt.xlim([-1, 1])
# plt.ylabel(r"Y Position - ($m$)",fontsize=16)
# plt.xlabel(r"X Position - ($m$)",fontsize=16)
# plt.tick_params(labelsize=14)
# plt.grid(color='gray', linestyle='dotted', linewidth=1)
# plt.legend(handles=legend_elements, loc='lower right', bbox_to_anchor=(1, 0), fancybox=True, shadow=True, ncol=1, fontsize='xx-large')
# # plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/xy_pos.png',bbox_inches='tight')


# fig = plt.figure(figsize=(10, 5))
# line, = plt.plot(qw_setpoint, label = 'QW SETPOINT', linewidth = 2)
# line2, = plt.plot(qw_r, label = 'QW Real', linewidth = 2)
# axes = plt.gca()
# plt.ylim([0.995, 1.005])
# plt.ylabel('Quaternion',fontsize=16)
# plt.xlabel('Number of Samples',fontsize=16)
# plt.tick_params(labelsize=14)
# plt.grid(color='gray', linestyle='dotted', linewidth=1)
# plt.legend(loc='lower right', bbox_to_anchor=(1, 0), fancybox=True, shadow=True, ncol=1, fontsize='xx-large')
# plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/qw_quat_set.png',bbox_inches='tight')
#
#
# fig = plt.figure(figsize=(10, 5))
# line, = plt.plot(qx_setpoint, label = 'QX SETPOINT', linewidth = 2)
# line2, = plt.plot(qx_r, label = 'QX Real', linewidth = 2)
# axes = plt.gca()
# plt.ylim([-0.05, 0.05])
# plt.ylabel('Quaternion',fontsize=16)
# plt.xlabel('Number of Samples',fontsize=16)
# plt.tick_params(labelsize=14)
# plt.grid(color='gray', linestyle='dotted', linewidth=1)
# plt.legend(loc='lower right', bbox_to_anchor=(1, 0), fancybox=True, shadow=True, ncol=1, fontsize='xx-large')
# plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/qx_quat_set.png',bbox_inches='tight')
#
#
# fig = plt.figure(figsize=(10, 5))
# line, = plt.plot(qy_setpoint, label = 'QY SETPOINT', linewidth = 2)
# line2, = plt.plot(qy_r, label = 'QY Real', linewidth = 2)
# axes = plt.gca()
# plt.ylim([-0.05, 0.05])
# plt.ylabel('Quaternion',fontsize=16)
# plt.xlabel('Number of Samples',fontsize=16)
# plt.tick_params(labelsize=14)
# plt.grid(color='gray', linestyle='dotted', linewidth=1)
# plt.legend(loc='lower right', bbox_to_anchor=(1, 0), fancybox=True, shadow=True, ncol=1, fontsize='xx-large')
# plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/qy_quat_set.png',bbox_inches='tight')
#
#
# fig = plt.figure(figsize=(10, 5))
# line, = plt.plot(qz_setpoint, label = 'QZ SETPOINT', linewidth = 2)
# line2, = plt.plot(qz_r, label = 'QZ Real', linewidth = 2)
# axes = plt.gca()
# plt.ylim([-0.05, 0.25])
# plt.ylabel('Quaternion',fontsize=16)
# plt.xlabel('Number of Samples',fontsize=16)
# plt.tick_params(labelsize=14)
# plt.grid(color='gray', linestyle='dotted', linewidth=1)
# plt.legend(loc='upper right', bbox_to_anchor=(1, 1), fancybox=True, shadow=True, ncol=1, fontsize='xx-large')
# plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/qz_quat_set.png',bbox_inches='tight')

# fig = plt.figure(figsize=(10, 5))
# line, = plt.plot(errQW_ekf, label = 'QW EKF ERROR', linewidth = 2)
# line2, = plt.plot(errQW_pid, label = 'QW PID ERROR', linewidth = 2)
# axes = plt.gca()
# plt.ylim([-0.05, 0.25])
# plt.ylabel('Quaternion',fontsize=16)
# plt.xlabel('Number of Samples',fontsize=16)
# plt.tick_params(labelsize=14)
# plt.grid(color='gray', linestyle='dotted', linewidth=1)
# plt.legend(loc='upper right', bbox_to_anchor=(1, 1), fancybox=True, shadow=True, ncol=1, fontsize='xx-large')
# plt.savefig('/home/paulo/Documents/Thesis/Chap.6 - Experiments/Exp. 2/descentralized/qz_quat_set.png',bbox_inches='tight')


plt.show()
