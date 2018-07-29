
USE_BOARD:=1
#-----------------------------------------------------------------------------
# GNU toolset
#
#compile
ifeq ($(USE_BOARD), 1)
CROSS_COMPILE=arm-hisiv100nptl-linux-
else
CROSS_COMPILE=
endif
CC    := $(CROSS_COMPILE)gcc
LIB   := $(CROSS_COMPILE)ar
CXX=$(CROSS_COMPILE)g++ -w
LINK=$(CROSS_COMPILE)g++ -w
##############################################################################
# Typically, you should not need to change anything below this line

MKDIR := mkdir -p
RM    := rm -f

