$(COBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@
	$(call call_fixdep, $(@:.o=.d), $@)

$(BIN): v2cc $(COBJS)

default: all

# 通过 .v 生成 C++ 代码
v2cc: $(VSRCS)
	$(VERILATOR) --cc --top-module $(TOPNAME) --Mdir $(VOBJ_DIR) $(VSRCS)

# Verilator 生成静态库文件
vobj: v2cc
	make -C $(VOBJ_DIR) -f V$(TOPNAME).mk

all: $(BIN)

sim: $(BIN)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	$(BIN) $(IMG)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: default all v clean sim
