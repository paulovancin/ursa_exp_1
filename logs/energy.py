import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.font_manager
from matplotlib.lines import Line2D


plt.rc('font',**{'family':'sans-serif','serif':['Helvetica']})
plt.rc('text', usetex=True)

# Power characterization results and energy estimation for each instruction class
# of the processor
#
# arithmetic = 2,653(mW) ========> 0,002653 (W)
arithmetic = 0.002653

# logical = 2,610(mW) ========> 0,002610 (W)
logical = 0.002610

# mult = 2,524(mW) ========> 0,002524 (W)
mult = 0.002524

# shift = 2,595(mW) ========> 0,002595 (W)
shift = 0.002595

# move = 2,591(mW) ========> 0,002591 (W)
move = 0.002591

# nop = 2,567(mW) ========> 0,002567 (W)
nop = 0.002567

# branches = 2,596(mW) ========> 0,002596 (W)
branches = 0.002596

# jumps = 2,576(mW) ========> 0,002576 (W)
jumps = 0.002576

# load-store = 2,605(mW) ========> 0,002605 (W)
load_store = 0.002605


# Memory Measures:

# P_leak_memory = 0,340 (mW) ========> 0,00340 (W)
P_leak_memory = 0.00340

# E_load = 67 (pJ) ========> 0.000000000067 (J)
E_load = 0.000000008102944

# E_store = 38 (pJ) ========> 0.000000000038 (J)
E_store = 0.000000008102944


# Router Power:

# P_idle_buffer = 364,64 (uW) ========> 0,00036464 (W)
P_idle_buffer = 0.00036464

# P_ide_comb = 575,64 (uW) ========> 0,00057564 (W)
P_idle_comb = 0.00057564

# P_active_buffer = 755,56(uW) ========> 0,00075556 (W)
P_active_buffer = 0.00075556

# P_active_comb = 2655,25 (uW) ========> 0,00265525 (W)
P_active_comb = 0.00265525

# P_leak_router = 223,08 (uW) ========> 0,00022308 (W)
P_leak_router = 0.00022308


# Number of cycles per instruction:
CPI_mult = 1
CPI_arithmetic = 1
CPI_logical = 1
CPI_shift = 1
CPI_move = 1
CPI_nop = 1
CPI_branches = 1
CPI_jumps = 1
CPI_load_store = 2

# T = 1666,6599999999999 (ns) ========> 0,00000166666 (s)
# T = 0.00000166666
T = 0.000000004
n_ports = 5

# Power Leakage from Processor:
# P_proc_leak = 0,452 (mW) ========> 0,000452 (W)
P_proc_leak = 0.000452

def E_proc_arit(n_inst_arit):
    E_dyn_arit = arithmetic * CPI_arithmetic * T
    result = n_inst_arit * E_dyn_arit
    return result

def E_proc_logic(n_inst_logic):
    E_dyn_logic = logical * CPI_logical * T
    result = n_inst_logic * E_dyn_logic
    return result

def E_proc_mult(n_inst_mult):
    E_dyn_mult = mult * CPI_mult * T
    result = n_inst_mult * E_dyn_mult
    return result

def E_proc_shift(n_inst_shift):
    E_dyn_shift = shift * CPI_shift * T
    result = n_inst_shift * E_dyn_shift
    return result

def E_proc_branch(n_inst_branch):
    E_dyn_branch = branches * CPI_branches * T
    result = n_inst_branch * E_dyn_branch
    return result

def E_proc_jump(n_inst_jump):
    E_dyn_jump = jumps * CPI_jumps * T
    result = n_inst_jump * E_dyn_jump
    return result

def E_proc_load_store(n_inst_load_store):
    E_dyn_load_store = load_store * CPI_load_store * T
    result = n_inst_load_store * E_dyn_load_store
    return result

def E_proc(E_proc_arit, E_proc_logic, E_proc_shift, E_proc_branch, E_proc_jump, E_proc_load_store, E_proc_mult):
    result = E_proc_arit + E_proc_logic + E_proc_shift + E_proc_branch + E_proc_jump + E_proc_load_store + E_proc_mult
    return result

def E_mem(n_inst_store, n_inst_load):
    result = n_inst_load * E_load + n_inst_store * E_store
    return result

def E_rou(cycles_t, cycles_active):
    E_idle_rou = ((n_ports * P_idle_buffer) + P_idle_comb) * ((cycles_t - cycles_active) * T)
    E_active_rou = (((n_ports - 1) * P_idle_buffer) + P_active_buffer + P_active_comb) * ((cycles_active) * T)
    result = E_idle_rou + E_active_rou
    return result

def E_leak(cycles_t):
    result = (P_proc_leak + P_leak_memory + P_leak_router) * cycles_t * T
    return result

def E_pe(E_proc, E_mem, E_rou, E_leak):
    result = E_proc + E_mem + E_rou + E_leak
    return result


def energy_estimation_instant(data_array):
    E_pr_ar = E_proc_arit(data_array[6])
    E_pr_log = E_proc_logic(data_array[7])
    E_pr_mul = E_proc_mult(data_array[8])
    E_pr_sh = E_proc_shift(data_array[9])
    E_pr_br = E_proc_branch(data_array[10])
    E_pr_jp = E_proc_jump(data_array[11])
    E_pr_ls = E_proc_load_store(data_array[12])
    E_pr = E_proc(E_pr_ar, E_pr_log, E_pr_sh, E_pr_br, E_pr_jp, E_pr_ls, E_pr_mul)
    E_m0 = E_mem(data_array[0], data_array[1])
    E_m1 = E_mem(data_array[2], data_array[3])
    E_m2 = E_mem(data_array[4], data_array[5])
    E_r = E_rou(data_array[14], data_array[13])
    E_l = E_leak(data_array[14])
    E_m = E_m0 + E_m1 + E_m2
    result = E_pe(E_pr, E_m, E_r, E_l)
    return result

def energy_estimation_proc(data_array):
    E_pr_ar = E_proc_arit(data_array[6])
    E_pr_log = E_proc_logic(data_array[7])
    E_pr_mul = E_proc_mult(data_array[8])
    E_pr_sh = E_proc_shift(data_array[9])
    E_pr_br = E_proc_branch(data_array[10])
    E_pr_jp = E_proc_jump(data_array[11])
    E_pr_ls = E_proc_load_store(data_array[12])
    E_pr = E_proc(E_pr_ar, E_pr_log, E_pr_sh, E_pr_br, E_pr_jp, E_pr_ls, E_pr_mul)
    return E_pr

def energy_estimation_mem(data_array):
    E_m0 = E_mem(data_array[0], data_array[1])
    E_m1 = E_mem(data_array[2], data_array[3])
    E_m2 = E_mem(data_array[4], data_array[5])
    E_m = E_m0 + E_m1 + E_m2
    return E_m

def energy_estimation_router(data_array):
    E_r = E_rou(data_array[14], data_array[13])
    return E_r

def get_proc_vector(data_array):
    vec = []
    vec_res = []
    for i in range(len(data_array)):
        vec = data_array[i][:]
        vec_res.append(energy_estimation_proc(vec)*1e9)
    return vec_res

def get_router_vector(data_array):
    vec = []
    vec_res = []
    for i in range(len(data_array)):
        vec = data_array[i][:]
        vec_res.append(energy_estimation_router(vec)*1e9)
    return vec_res

def get_mem_vector(data_array):
    vec = []
    vec_res = []
    for i in range(len(data_array)):
        vec = data_array[i][:]
        vec_res.append(energy_estimation_mem(vec)*1e9)
    return vec_res

def get_energy_vector(data_array):
    vec = []
    vec_res = []
    for i in range(len(data_array)):
        vec = data_array[i][:]
        vec_res.append(energy_estimation_instant(vec)*1e9)
    return vec_res



filename1 = "001.cpu_uart.log"
infile = open(filename1, 'r')
lines = infile.readlines()


mem0 = []
mem1 = []
mem2 = []
cpu1 = []
cpu2 = []
cpu3 = []
cpu4 = []
rout = []

for line in lines:
    if 'MEM0' in line:
        sline = line.replace('MEM0:','').replace('writes=','').replace(',','').replace('reads=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        mem0.append(x)
    elif 'MEM1' in line:
        sline = line.replace('MEM1:','').replace('writes=','').replace(',','').replace('reads=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        mem1.append(x)
    elif 'MEM2' in line:
        sline = line.replace('MEM2:','').replace('writes=','').replace(',','').replace('reads=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        mem2.append(x)
    elif 'CPU1' in line:
        sline = line.replace('CPU1:','').replace('arith=','').replace(',','').replace('logical=','').replace(',','').replace('mul=','').split()
        x = []
        for item in sline:
            print(item)
            x.append(float(item))
        cpu1.append(x)
    elif 'CPU2' in line:
        sline = line.replace('CPU2:','').replace('shift=','').replace(',','').replace('branches=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        cpu2.append(x)
    elif 'CPU3' in line:
        sline = line.replace('CPU3:','').replace('jumps=','').replace(',','').replace('loadstore=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        cpu3.append(x)
    elif 'CPU4' in line:
        sline = line.replace('CPU4:','').replace('cycles=','').replace(',','').replace('stalls=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        cpu4.append(x)
    elif 'ROUTER' in line:
        sline = line.replace('ROUTER:','').replace('active=','').replace(',','').split()
        x = []
        for item in sline:
            x.append(float(item))
        rout.append(x)



infile.close()



mem0_load = []
mem0_store = []
mem1_load = []
mem1_store = []
mem2_load = []
mem2_store = []
arit = []
logi = []
mul = []
shi = []
bran = []
jum = []
loadsto = []
cycl = []
stalls = []
acti = []
data = []


for i in range(len(mem0)):
    mem0_load.append(mem0[i][0])
    mem0_store.append(mem0[i][1])

for i in range(len(mem1)):
    mem1_load.append(mem1[i][0])
    mem1_store.append(mem1[i][1])

for i in range(len(mem2)):
    mem2_load.append(mem2[i][0])
    mem2_store.append(mem2[i][1])

for i in range(len(cpu1)):
    arit.append(cpu1[i][0])
    logi.append(cpu1[i][1])
    mul.append(cpu1[i][2])

for i in range(len(cpu2)):
    shi.append(cpu2[i][0])
    bran.append(cpu2[i][1])

for i in range(len(cpu3)):
    jum.append(cpu3[i][0])
    loadsto.append(cpu3[i][1])

for i in range(len(cpu4)):
    cycl.append(cpu4[i][0])
    stalls.append(cpu4[i][1])

for i in range(len(rout)):
    acti.append(rout[i][0])



for i in range(len(rout)):
    data.append([mem0_load[i], mem0_store[i], mem1_load[i], mem1_store[i], mem2_load[i], mem2_store[i], arit[i], logi[i], mul[i], shi[i], bran[i], jum[i], loadsto[i], acti[i], cycl[i], stalls[i]])


ekf = []
ekf = get_energy_vector(data)


filename2 = "002.cpu_uart.log"
infile = open(filename2, 'r')
lines = infile.readlines()


mem0 = []
mem1 = []
mem2 = []
cpu1 = []
cpu2 = []
cpu3 = []
cpu4 = []
rout = []

for line in lines:
    if 'MEM0' in line:
        sline = line.replace('MEM0:','').replace('writes=','').replace(',','').replace('reads=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        mem0.append(x)
    elif 'MEM1' in line:
        sline = line.replace('MEM1:','').replace('writes=','').replace(',','').replace('reads=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        mem1.append(x)
    elif 'MEM2' in line:
        sline = line.replace('MEM2:','').replace('writes=','').replace(',','').replace('reads=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        mem2.append(x)
    elif 'CPU1' in line:
        sline = line.replace('CPU1:','').replace('arith=','').replace(',','').replace('logical=','').replace(',','').replace('mul=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        cpu1.append(x)
    elif 'CPU2' in line:
        sline = line.replace('CPU2:','').replace('shift=','').replace(',','').replace('branches=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        cpu2.append(x)
    elif 'CPU3' in line:
        sline = line.replace('CPU3:','').replace('jumps=','').replace(',','').replace('loadstore=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        cpu3.append(x)
    elif 'CPU4' in line:
        sline = line.replace('CPU4:','').replace('cycles=','').replace(',','').replace('stalls=','').split()
        x = []
        for item in sline:
            x.append(float(item))
        cpu4.append(x)
    elif 'ROUTER' in line:
        sline = line.replace('ROUTER:','').replace('active=','').replace(',','').split()
        x = []
        for item in sline:
            x.append(float(item))
        rout.append(x)



infile.close()



mem0_load = []
mem0_store = []
mem1_load = []
mem1_store = []
mem2_load = []
mem2_store = []
arit = []
logi = []
mul = []
shi = []
bran = []
jum = []
loadsto = []
cycl = []
stalls = []
acti = []
data = []



for i in range(len(mem0)):
    mem0_load.append(mem0[i][0])
    mem0_store.append(mem0[i][1])

for i in range(len(mem1)):
    mem1_load.append(mem1[i][0])
    mem1_store.append(mem1[i][1])

for i in range(len(mem2)):
    mem2_load.append(mem2[i][0])
    mem2_store.append(mem2[i][1])

for i in range(len(cpu1)):
    arit.append(cpu1[i][0])
    logi.append(cpu1[i][1])
    mul.append(cpu1[i][2])

for i in range(len(cpu2)):
    shi.append(cpu2[i][0])
    bran.append(cpu2[i][1])

for i in range(len(cpu3)):
    jum.append(cpu3[i][0])
    loadsto.append(cpu3[i][1])

for i in range(len(cpu4)):
    cycl.append(cpu4[i][0])
    stalls.append(cpu4[i][1])

for i in range(len(rout)):
    acti.append(rout[i][0])


for i in range(len(rout)):
    data.append([mem0_load[i], mem0_store[i], mem1_load[i], mem1_store[i], mem2_load[i], mem2_store[i], arit[i], logi[i], mul[i], shi[i], bran[i], jum[i], loadsto[i], acti[i], cycl[i], stalls[i]])


pid = []
pid = get_energy_vector(data)
#
# ekf_mean = sum(ekf) / len(ekf)
# pid_mean = sum(pid) / len(pid)
#
#
#
# print("ekf_mean:", ekf_mean)
# print("pid_mean:", pid_mean)

# fig = plt.figure(figsize=(10, 5))
# line, = plt.plot(ekf, label = 'EKF (Core 1)', linewidth = 3)
# axes = plt.gca()
# plt.ylabel('Energy (nJ)',fontsize=16)
# plt.xlabel('Number of Samples',fontsize=16)
# plt.tick_params(labelsize=14)
# plt.grid(color='gray', linestyle='dotted', linewidth=1)
# plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.1), fancybox=True, shadow=True, ncol=2, fontsize='xx-large')






plt.show()
