CC=gcc
CC_FLAGS=-g
AR=ar

SRC_FILES:=unix_api.c
HEAD_FILES:=unix_api.h

OBJ_FILES:=$(patsubst %.c,%.o,$(SRC_FILES))
STATIC_LIB:=libunix_lib.a
SHARED_LIB:=libunix_lib.so

INSTALL_INC_DIR=/usr/include
INSTALL_LIB_DIR=/usr/lib

# 构建最终目标
all:$(SRC_FILES)
	@# 生成静态库
	$(CC) -c $^ -o $(OBJ_FILES)
	$(AR) crv $(STATIC_LIB) $(OBJ_FILES)

	@# 生成动态库
	$(CC) -c -fPIC $^ -o $(OBJ_FILES)
	$(CC) -shared -o $(SHARED_LIB) $(OBJ_FILES)

#####################################################
.PHONY:clean install
clean:
	rm $(OBJ_FILES)	$(STATIC_LIB) $(SHARED_LIB)

install:
	cp $(HEAD_FILES) $(INSTALL_INC_DIR)
	cp $(STATIC_LIB) $(SHARED_LIB) $(INSTALL_LIB_DIR)
