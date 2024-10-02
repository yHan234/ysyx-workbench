default: all

$(VOBJ_DIR)/V$(TOPNAME)__ALL.a: v2cc
	$(VERILATOR) --cc --build --top-module $(TOPNAME) --Mdir $(VOBJ_DIR) $(VSRCS)

$(COBJ_DIR)/%.o: $(CSRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

$(BIN): v2cc $(COBJS) $(ARCHIVES)
	$(LD) -o $@ $(COBJS) $(LDFLAGS) $(addprefix -L,$(ARCHIVES)) $(LIBS)

veri:
	$(CXXFLAGS) += $(INCFLAGS)
	$(VERILATOR) -MMD --build -cc  \
				-O3 --x-assign fast --x-initial fast --noassert \
		--top-module $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CXXFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN))

all: $(BIN)

# 通过 .v 生成 C++ 代码
v2cc: $(VSRC)
	$(VERILATOR) --cc --top-module $(TOPNAME) --Mdir $(VCC_DIR) $(VSRCS)

sim: $(BIN)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	$(BIN) $(IMG)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: default all v2cc sim clean
