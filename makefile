include config.mk

TARGET = phone

PRE_DEF = 

VPATH = \
	./src/app

QP_DIR := ./src/componets/qp/qpc
XML_DIR := ./src/componets/xmllib/mxml-2.11
ESYLG_DIR := ./src/componets/EasyLogger
EV_DIR := ./src/componets/libev-4.15
EVENT_DIR := ./src/componets/libevent-2.1.8-stable
LIB_COMMON_DIR := ./src/componets/libraries_common

# list of all include directories needed by this project
INCLUDES  = \
	-I. \
	-I$(QP_DIR)/include \
	-I$(QP_DIR)/src \
	-I$(QP_DIR)/ports/posix \
	-I./src/app \
	-I$(ESYLG_DIR)/easylogger/inc \
	-I$(XML_DIR) \
	-I$(EVENT_DIR) \
	-I$(EVENT_DIR)/include \
	-I$(EVENT_DIR)/include/event2 \
	-I$(LIB_COMMON_DIR)/libskt \
	-I$(LIB_COMMON_DIR)/liblist
	


#lib
#ARM_SHARED_LIB = -lm -ldl -lpthread -lsqlite3 -liconv 
ARM_SHARED_LIB = -lm -ldl -lpthread
#INC_PATH = -I../lib/sqlite	-I../lib/libiconv/include
INC_PATH = 
ifeq (${USE_BOARD}, 1)
	LIB_PATH = -L../lib/board
#	ARM_STATIC_LIB = ../lib/board/libiconv.a
	ARM_STATIC_LIB = 
else
	LIB_PATH = -L../lib/linux
#	ARM_STATIC_LIB = ../lib/linux/libiconv.a
	ARM_STATIC_LIB =
endif

LIBVAR = -lqp -lmxml -leasylogger -levent_core -levent_pthreads -lskt \
		-llist #levent_extra lenvent_pthreads levent -lev -lvector

ifeq (rel, $(CONF))

LIB_PATH += -L$(QP_DIR)/ports/posix/rel
LIB_PATH += -L$(XML_DIR)
LIB_PATH += -L$(ESYLG_DIR)/rel
# LIB_PATH += -L$(EV_DIR)/.libs
LIB_PATH += -L$(EVENT_DIR)/.libs
LIB_PATH += -L$(LIB_COMMON_DIR)/libskt
LIB_PATH += -L$(LIB_COMMON_DIR)/liblist
else

LIB_PATH += -L$(QP_DIR)/ports/posix/dbg
LIB_PATH += -L$(XML_DIR)
LIB_PATH += -L$(ESYLG_DIR)/dbg
# LIB_PATH += -L$(EV_DIR)/.libs
LIB_PATH += -L$(EVENT_DIR)/.libs
LIB_PATH += -L$(LIB_COMMON_DIR)/libskt
LIB_PATH += -L$(LIB_COMMON_DIR)/liblist
endif

APP_SRC := \
	main.c \
	bsp.c \
	blinky.c \
	tcp_server.c \
	tcp_client.c \
	event_tcp_server.c \
	udp_server.c \
	config.c \
	call_sock.c \
	qp_task.c
#	event_tcp_server.c

	# ev_task.c
	# ev_tcp_server.c 
ifeq (rel, $(CONF))  # Release configuration .................................

BIN_DIR := rel

CFLAGS = -c -ffunction-sections -fdata-sections \
	-Os -Wall -W -pthread -DNDEBUG

else  # default Debug configuration ..........................................

BIN_DIR := dbg

CFLAGS = -c -g -ffunction-sections -fdata-sections \
	-O -Wall -W -pthread

endif  # .....................................................................

CFLAGS += -D__LINUX $(INC_PATH) $(INCLUDES) $(PRE_DEF)
CXXFLAGS += -D__LINUX $(INC_PATH) $(INCLUDES) $(PRE_DEF)

C_SOURCES +=$(APP_SRC)

C_SOURCES += $(wildcard *.c)
C_OBJS = $(patsubst %.c,%.o,$(C_SOURCES))
CPP_SOURCES = $(wildcard *.cpp)
CPP_OBJS = $(patsubst %.cpp,%.o,$(CPP_SOURCES))
OBJ = $(C_OBJS) $(CPP_OBJS)

all: version $(TARGET) # lib
$(TARGET): $(OBJ)
	$(LINK) -o $(TARGET) $(OBJ) $(ARM_STATIC_LIB) $(LIBVAR) -lrt $(INC_PATH) $(LIB_PATH) $(ARM_SHARED_LIB) 
	$(CROSS_COMPILE)strip $(TARGET)

version:
	sh version.sh

# lib:
# ifeq (rel, $(CONF))
# 	(cd $(QP_DIR)/ports/posix; make CONF=rel)
# 	(cd $(ESYLG_DIR); make CONF=rel)
# else
# 	(cd $(QP_DIR)/ports/posix; make)
# 	(cd $(ESYLG_DIR); make)
# endif

clean:
#	(cd $(QP_DIR)/ports/posix; make clean)
#	(cd $(ESYLG_DIR); make clean)
	rm -rf $(OBJ) $(TARGET) version.h
	rm -rf *.ncb
	rm -rf *.suo
	rm -rf *.user
	rm -rf *.o

show:
	@echo C_SOURCES = $(C_SOURCES)
