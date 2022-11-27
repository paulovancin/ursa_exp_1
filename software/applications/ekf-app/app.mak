# DRONE_EKF_DIR = $(SRC_DIR)/../applications/drone-ekf
#
# drone-ekf.o:
# 	$(CC) $(CFLAGS) -c $(DRONE_EKF_DIR)/drone-ekf.c -o drone-ekf.o
# 	$(info ************  TEST Begin ************)
# 	print_CFLAGS := $(CFLAGS)
#
# 	$(C_OBJS): %.o: %.c
# 		@$(if $(print_CFLAGS), echo "CFLAGS: $(print_CFLAGS)" $(eval print_CFLAGS:=))
# 		@ echo -n "Compiling " $*.c;
# 		@ $(CC) -c $(CFLAGS) $*.c -o $*.o
# 		@ echo " ...... Done
#
# 	$(info ************  TEST End ************)
# Do not modify the lines below
APP_EKF_NAME  := ekf-app
APP_EKF_DIR   := $(SRC_DIR)/../applications/$(APP_EKF_NAME)
APP_EKF_SRC   := /home/paulo/final_shit/ursa_exp_1/software/applications/ekf-app
APP_EKF_INC   := /home/paulo/final_shit/ursa_exp_1/software/applications/ekf-app
APP_EKF_LIB   := app-$(APP_EKF_NAME).a

# # Update these lines with your source code
APP_EKF_OBJS := ekf-app.o

#pack everithing in a single lib
$(APP_EKF_LIB) : $(APP_EKF_OBJS)
	ar rcs $(APP_EKF_LIB) $(APP_EKF_OBJS)

#compile each individual object file
%.o: $(APP_EKF_SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -I$(APP_EKF_INC)

#check whether .h are up to date
$(APP_EKF_SRC)/%.c: $(APP_EKF_INC)/%.h
