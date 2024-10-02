CXXFLAGS += -fPIE
LDFLAGS += $(shell llvm-config --libs) -lxml2 -lz -lcurses