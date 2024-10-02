# 生成可执行文件名
BINNAME = npc
# Verilog 顶层模块名
TOPNAME = top

# Verilog 源代码和 C++ 源代码
VSRC_DIR = ./vsrc
CSRC_DIR = ./csrc
VSRCS = $(shell find $(VSRC_DIR) -name "*.v")
CSRCS = $(shell find $(CSRC_DIR) -name "*.cpp")

# 构建目录及目标文件
BUILD_DIR = ./build
BIN = $(BUILD_DIR)/$(BINNAME)
$(shell mkdir -p $(BUILD_DIR))

# Varilator 相关
VERILATOR = verilator
VCC_DIR = $(CSRC_DIR)/DUT
VOBJ_DIR = $(BUILD_DIR)/v_obj

# C++ 相关
CXXFLAGS = -std=c++17 -Wall

INC_PATH += $(CSRC_DIR)
# INC_PATH += $(VOBJ_DIR)
INC_PATH += $(VERILATOR_ROOT)/include
INC_PATH += $(VERILATOR_ROOT)/include/vltstd
INCFLAGS = $(addprefix -I, $(INC_PATH))

COBJ_DIR = $(BUILD_DIR)/c_obj
COBJS = $(patsubst $(CSRC_DIR)/%.cpp,$(COBJ_DIR)/%.o,$(CSRCS))

# 链接相关
LD = $(CXX)
LDFLAGS +=
ARCHIVES += $(VOBJ_DIR)/V$(TOPNAME)__ALL.a

# Include other makefiles
FILELIST_MK = $(shell find -L ./csrc -name "filelist.mk")
include $(FILELIST_MK)
include build.mk
include ../Makefile
