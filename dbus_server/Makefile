TARGET1_NAME=dbus_server
TARGET1 = $(TARGET_BIN_DIR)/$(TARGET1_NAME)

CFLAGS  +=-I./ 
CFLAGS  +=-L./lib
CFLAGS=  -O2 $(APP_FLAGS) -Werror
#LDFLAGS  += -Wl -ldbus-1 -lpthread
LDFLAGS  +=  -lpthread -ldbus-1 -lxml2 
INCLUDES = -I./include -I./include/dbus/ -I./include/libxml
#INCLUDES = -I./include -I./include/dbus/ 

SRCDIR = src
TARGET_DIR=target
TARGET_BIN_DIR=target_bin

OBJ_PATH = objs
PREFIX_BIN = $(TARGET_DIR)

C_SRCDIR = $(SRCDIR)
C_SOURCES = $(foreach d,$(C_SRCDIR),$(wildcard $(d)/*.c) )
C_OBJS = $(patsubst %.c, $(OBJ_PATH)/%.o, $(C_SOURCES))

default: init  compile
$(C_OBJS):$(OBJ_PATH)/%.o:%.c
	$(CC) -c $(CFLAGS) $(INCLUDES)  $(LDFLAGS)  $< -o $@

$(OBJ_PATH)/$(TARGET_DIR)/$(TARGET1_NAME).o: $(TARGET_DIR)/$(TARGET1_NAME).c $(C_OBJS) 
	$(CC) -c $(CFLAGS) $(INCLUDES) $(LDFLAGS)  $< -o $@   

init:
	$(foreach d,$(SRCDIR), mkdir -p $(OBJ_PATH)/$(d);)
	$(foreach d,$(TARGET_DIR), mkdir -p $(OBJ_PATH)/$(d);)
	
compile:$(C_OBJS) $(OBJ_PATH)/$(TARGET_DIR)/$(TARGET1_NAME).o
	$(CC) $(INCLUDES) $(CFLAGS) $(LDFLAGS)  $^ -o $(TARGET1)  $(LDFLAGS) 

clean:
	rm -rf $(OBJ_PATH)
	rm -rf $(TARGET_BIN_DIR)/*
	
