##############################################################################
# examples of invoking this Makefile:
# building configurations: Debug (default), Release and Spy
# make
# make CONF=rel
# make CONF=spy
#
# cleaning configurations: Debug (default), Release, and Spy
# make clean
# make CONF=rel clean
# make CONF=spy clean
#
include ../../../config.mk
#-----------------------------------------------------------------------------
# project name
#
PROJECT     := mislib

#-----------------------------------------------------------------------------
# project directories
#

# location of the QP/C framework
MISLIB:= .


# list of all source directories used by this project
VPATH = \
	$(MISLIB)/libatomic \
	$(MISLIB)/libbase64 \
	$(MISLIB)/libcmd \
	$(MISLIB)/libconfig++ \
	$(MISLIB)/libconfig \
	$(MISLIB)/libconfig/ini \
	$(MISLIB)/libdebug \
	$(MISLIB)/libdict \
	$(MISLIB)/libdlmod \
	$(MISLIB)/libdock \
	$(MISLIB)/libfile \
	$(MISLIB)/libfilewatcher \
	$(MISLIB)/libgevent \
	$(MISLIB)/libhal \
	$(MISLIB)/libhash \
	$(MISLIB)/libipc \
	$(MISLIB)/libjpeg-ex \
	$(MISLIB)/liblock \
	$(MISLIB)/libmacro \
	$(MISLIB)/libmem \
	$(MISLIB)/libmp4parser \
	$(MISLIB)/libosal \
	$(MISLIB)/libp2p \
	$(MISLIB)/libptcp \
	$(MISLIB)/libqueue \
	$(MISLIB)/librbtree \
	$(MISLIB)/libringbuffer \
	$(MISLIB)/librpc \
	$(MISLIB)/librtsp \
	$(MISLIB)/libskt \
	$(MISLIB)/libsort \
	$(MISLIB)/libstrex \
	$(MISLIB)/libstun \
	$(MISLIB)/libthread \
	$(MISLIB)/libtime \
	$(MISLIB)/libuvc \
	$(MISLIB)/libvector \
	$(MISLIB)/libworkq

# list of all include directories needed by this project
INCLUDES  = \
	$(MISLIB)/libatomic \
	$(MISLIB)/libbase64 \
	$(MISLIB)/libcmd \
	$(MISLIB)/libconfig++ \
	$(MISLIB)/libconfig \
	$(MISLIB)/libconfig/ini \
	$(MISLIB)/libdebug \
	$(MISLIB)/libdict \
	$(MISLIB)/libdlmod \
	$(MISLIB)/libdock \
	$(MISLIB)/libfile \
	$(MISLIB)/libfilewatcher \
	$(MISLIB)/libgevent \
	$(MISLIB)/libhal \
	$(MISLIB)/libhash \
	$(MISLIB)/libipc \
	$(MISLIB)/libjpeg-ex \
	$(MISLIB)/liblock \
	$(MISLIB)/libmacro \
	$(MISLIB)/libmem \
	$(MISLIB)/libmp4parser \
	$(MISLIB)/libosal \
	$(MISLIB)/libp2p \
	$(MISLIB)/libptcp \
	$(MISLIB)/libqueue \
	$(MISLIB)/librbtree \
	$(MISLIB)/libringbuffer \
	$(MISLIB)/librpc \
	$(MISLIB)/librtsp \
	$(MISLIB)/libskt \
	$(MISLIB)/libsort \
	$(MISLIB)/libstrex \
	$(MISLIB)/libstun \
	$(MISLIB)/libthread \
	$(MISLIB)/libtime \
	$(MISLIB)/libuvc \
	$(MISLIB)/libvector \
	$(MISLIB)/libworkq
#-----------------------------------------------------------------------------
# files
#

# C source files
C_SRCS := \
	libatomic.c\
	libbase64.c\
	libcmd.c\
	libconfig.c\
	dictionary.c\
	ini_config.c\
	iniexample.c\
	iniparser.c\
	libdict.c\
	libdlmod.c\
	libdock.c\
	fio.c\
	io.c\
	libfile.c\
	libfilewatcher.c\
	epoll.c\
	libgevent.c\
	poll.c\
	select.c\
	libhal.c\
	libhash.c\
	libipc.c\
	msgq_posix.c\
	msgq_sysv.c\
	netlink.c\
	netlink_driver.c\
	shm.c\
	unix_socket.c\
	liblock.c\
	liblog.c\
	libmacro.c\
	libmp4parser.c\
	mp4parser_inner.c\
	patch.c\
	libosal.c\
	libp2p.c\
	libptcp.c\
	libqueue.c\
	librbtree.c\
	libringbuffer.c\
	librpc.c\
	librtsp_server.c\
	media_source.c\
	media_source_h264.c\
	media_source_live.c\
	request_handle.c\
	rtp.c\
	rtp_h264.c\
	rtsp_parser.c\
	sdp.c\
	transport_session.c\
	uri_parse.c\
	libskt.c\
	bubble_sort.c\
	heap_sort.c\
	libsort.c\
	libstrex.c\
	libstun.c\
	libthread.c\
	libtime.c\
	libuvc.c\
	libvector.c\
	libworkq.c

# C++ source files
CPP_SRCS :=

# defines
DEFINES  :=

#-----------------------------------------------------------------------------
# build options for various configurations
#

LIBFLAGS := rs

ifeq (rel, $(CONF))  # Release configuration .................................

BIN_DIR := rel

CFLAGS = -c -ffunction-sections -fdata-sections \
	-Os -Wall -W $(INCLUDES) $(DEFINES) -pthread -DNDEBUG

else  # default Debug configuration ..........................................

BIN_DIR := dbg

CFLAGS = -c -g -ffunction-sections -fdata-sections \
	-O -Wall -W $(INCLUDES) $(DEFINES) -pthread

endif  # .....................................................................


C_OBJS       := $(patsubst %.c,   %.o, $(C_SRCS))
CPP_OBJS     := $(patsubst %.cpp, %.o, $(CPP_SRCS))

TARGET_LIB   := $(BIN_DIR)/lib$(PROJECT).a
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o, %.d, $(C_OBJS_EXT))
CPP_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(CPP_OBJS))
CPP_DEPS_EXT := $(patsubst %.o, %.d, $(CPP_OBJS_EXT))

# create $(BIN_DIR) if it does not exist
ifeq ("$(wildcard $(BIN_DIR))","")
$(shell $(MKDIR) $(BIN_DIR))
endif

#-----------------------------------------------------------------------------
# rules
#

all: $(TARGET_LIB)
	-$(RM) $(BIN_DIR)/*.o $(BIN_DIR)/*.d

$(TARGET_LIB) : $(ASM_OBJS_EXT) $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	$(LIB) $(LIBFLAGS) $@ $^

$(BIN_DIR)/%.d : %.c
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.d : %.cpp
	$(CPP) -MM -MT $(@:.d=.o) $(CPPFLAGS) $< > $@

$(BIN_DIR)/%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.cpp
	$(CPP) $(CPPFLAGS) $< -o $@

# include dependency files only if our goal depends on their existence
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),show)
-include $(C_DEPS_EXT) $(CPP_DEPS_EXT)
endif
endif

#-----------------------------------------------------------------------------
# the clean target
#
.PHONY : clean
clean:
	-$(RM) $(BIN_DIR)/*.o 	$(BIN_DIR)/*.d $(TARGET_LIB)

#-----------------------------------------------------------------------------
# the show target for debugging
#
show:
	@echo PROJECT = $(PROJECT)
	@echo CONF = $(CONF)
	@echo TARGET_LIB = $(TARGET_LIB)
	@echo C_SRCS = $(C_SRCS)
	@echo CPP_SRCS = $(CPP_SRCS)
	@echo C_OBJS_EXT = $(C_OBJS_EXT)
	@echo C_DEPS_EXT = $(C_DEPS_EXT)
	@echo CPP_OBJS_EXT = $(CPP_OBJS_EXT)
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)
