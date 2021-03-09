CC=gcc
CC_FLAGS=-g
AR=ar

SRC_FILES=unix_api.c
OBJ_FILES=$(patsubst %.c,%.o,$(SRC_FILES))
STATIC_LIB=libunix_lib.a
SHARED_LIB=libunix_lib.so

all:$(SRC_FILES)
	$(CC) -c $^ -o $(OBJ_FILES)
	$(AR) crv $(STATIC_LIB) $(OBJ_FILES)

	$(CC) -c -fPIC $^ -o $(OBJ_FILES)
	$(CC) -shared -o $(SHARED_LIB) $(OBJ_FILES)

.PHONY:clean
clean:
	rm $(OBJ_FILES)	$(STATIC_LIB) $(SHARED_LIB)
