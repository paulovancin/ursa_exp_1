# DRONE_PID_DIR = $(SRC_DIR)/../applications/drone-pid
#
# drone-pid.o:
# 	$(CC) $(CFLAGS) -c $(DRONE_PID_DIR)/drone-pid.c -o drone-pid.o
# Do not modify the lines below
APP_PID_NAME  := pid-app
APP_PID_DIR   := $(SRC_DIR)/../applications/$(APP_PID_NAME)
APP_PID_SRC   := /home/paulo/final_shit/ursa_exp_1/software/applications/pid-app
APP_PID_INC   := /home/paulo/final_shit/ursa_exp_1/software/applications/pid-app
APP_PID_LIB   := app-$(APP_PID_NAME).a

# Update these lines with your source code
APP_PID_OBJS := pid-app.o
#pack everithing in a single lib
$(APP_PID_LIB) : $(APP_PID_OBJS)
	ar rcs $(APP_PID_LIB) $(APP_PID_OBJS)

#compile each individual object file
%.o: $(APP_PID_SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -I$(APP_PID_INC)

#check whether .h are up to date
$(APP_PID_SRC)/%.c: $(APP_PID_SRC)/%.h
