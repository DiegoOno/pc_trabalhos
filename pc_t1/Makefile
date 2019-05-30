CC = gcc
C_PARALLEL_FLAGS = -lpthread -lm
C_SEQUENTIAL_FLAGS = -lm
OUT_DIR = bin
SRC_DIR = src

PARALLEL_PROG_NAME = k_means_parallel.c
SEQUENTIAL_PROG_NAME = k_means_sequential.c

DEST_PARALLEL_NAME = k_means_parallel
DEST_SEQUENTIAL_NAME = k_means_sequential

DEFAULT: BOTH

BOTH:
	if [ ! -d $(OUT_DIR) ] ; then mkdir $(OUT_DIR) ; fi
	$(CC) $(SRC_DIR)/$(PARALLEL_PROG_NAME) $(C_PARALLEL_FLAGS) -o $(OUT_DIR)/$(DEST_PARALLEL_NAME)
	$(CC) $(SRC_DIR)/$(SEQUENTIAL_PROG_NAME) $(C_SEQUENTIAL_FLAGS) -o $(OUT_DIR)/$(DEST_SEQUENTIAL_NAME)

sequential:
	if [ ! -d $(OUT_DIR) ] ; then mkdir $(OUT_DIR) ; fi
	$(CC) $(SRC_DIR)/$(SEQUENTIAL_PROG_NAME) $(C_SEQUENTIAL_FLAGS) -o $(OUT_DIR)/$(DEST_SEQUENTIAL_NAME)

parallel:
	if [ ! -d $(OUT_DIR) ] ; then mkdir $(OUT_DIR) ; fi
	$(CC) $(SRC_DIR)/$(PARALLEL_PROG_NAME) $(C_PARALLEL_FLAGS) -o $(OUT_DIR)/$(DEST_PARALLEL_NAME)

clean:
	rm -rf $(OUT_DIR)

