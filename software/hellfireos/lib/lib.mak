libc:
	$(CC) $(CFLAGS) \
		$(SRC_DIR)/lib/libc/libc.c \
		$(SRC_DIR)/lib/libc/math.c \
		$(SRC_DIR)/lib/libc/ekf.c \
		$(SRC_DIR)/lib/libc/pid.c \
		$(SRC_DIR)/lib/libc/matrix.c \
		$(SRC_DIR)/lib/libc/quaternions.c \
		$(SRC_DIR)/lib/libc/fixed.c \
		$(SRC_DIR)/lib/misc/crc.c
