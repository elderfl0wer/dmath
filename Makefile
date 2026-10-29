VERSION = 0.2.0
NAME = dmath
DESCRIPTION = "Math structures and operations on them in C++23."

CXX = clang++
CXXFLAGS = -std=c++23 -g -O2 -Wall -Wextra -Wpedantic

BUILD = build
TEST_SRC = tests/vec2_test.cpp
OUT = $(BUILD)/vec2_test

$(BUILD):
	mkdir -p $(BUILD)

tests: $(BUILD)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(OUT)

clean:
	rm -rf $(BUILD)

.PHONY: tests clean
