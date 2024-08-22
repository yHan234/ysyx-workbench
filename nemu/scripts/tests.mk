TEST_SRC=$(wildcard $(NEMU_HOME)/tests/*_tests.c)
TESTS=$(patsubst %.c,build/%,$(TEST_SRC))

tests: LDLIBS += $(TARGET)