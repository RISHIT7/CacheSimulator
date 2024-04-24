.DEFAULT_GOAL := all

CXX := gcc
CCBUILDFLAGS := -o

CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g
LDFLAGS = -lstdc++

SRC_PATH := src
INC_PATH := include

TARGET_NAME := cacheSim

ifeq ($(OS), Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif

SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.cpp)))
INC := $(foreach x, $(INC_PATH), $(wildcard $(addprefix $(x)/*,.hpp)))

TARGET := $(TARGET_NAME)

.PHONY: all
all: $(TARGET)

$(TARGET): $(SRC) $(INC)
	@$(CXX) $(CXXFLAGS) $(CCBUILDFLAGS) $@ $(SRC) $(LDFLAGS)

clean:
	@rm -f $(TARGET)