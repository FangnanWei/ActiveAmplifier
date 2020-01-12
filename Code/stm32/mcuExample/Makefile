# Makefile

MODULE=mcuExample
# 1 for g++ link , 0 for gcc link
IS_CPP_PRJ=0
include ../makerules/Rules.make
include $(MakeRules_PATH)/CommonHeader.mk
INCLUDE+=-I../include -I../../DI_Public/include
LIB_LINK_PATH += -L../Build/mcuIf/lib/$(ARCH)
LIBS=-lmcuIf -lpthread -lrt

all: depend lib so exe

include $(MakeRules_PATH)/CommonFooter.mk