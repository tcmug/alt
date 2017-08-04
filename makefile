
# GENERICS
CC = g++
COMMON_FLAGS = -Wno-c++11-extensions -O3 -mmacosx-version-min=10.7 -arch x86_64

ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
# LUA CONFIG
HEADERS_DIR = $(ROOT_DIR)/3rd/include
LIBS_DIR = $(ROOT_DIR)/3rd/lib
LUA_LDFLAGS = -llua
LUA_CFLAGS =

# WX CONFIG
WX_CONFIG ?= ./3rd/bin/wx-config
WX_PORT ?= $(shell $(WX_CONFIG) --query-toolkit)
WX_VERSION ?= $(shell $(WX_CONFIG) --query-version | sed -e 's/\([0-9]*\)\.\([0-9]*\)/\1\2/')
WX_VERSION_MAJOR = $(shell echo $(WX_VERSION) | cut -c1,1)
WX_VERSION_MINOR = $(shell echo $(WX_VERSION) | cut -c2,2)
WX_CONFIG_FLAGS = --toolkit=$(WX_PORT) --version=$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR)
WX_CFLAGS = $(shell $(WX_CONFIG) --cxxflags --static $(WX_CONFIG_FLAGS))
WX_LDFLAGS = $(shell $(WX_CONFIG) --libs --static $(WX_CONFIG_FLAGS))
WX_CFLAGS = `$(WX_CONFIG) --cxxflags --static $(WX_CONFIG_FLAGS)`
WX_LDFLAGS = `$(WX_CONFIG) --libs --static $(WX_CONFIG_FLAGS)`

# SPECIFICS
CFLAGS =  -I$(HEADERS_DIR) $(COMMON_FLAGS) -Wall $(WX_CFLAGS) $(LUA_CFLAGS)
LDFLAGS = -L$(LIBS_DIR) -ffunction-sections -fdata-sections -Wl $(COMMON_FLAGS) -dead_strip $(WX_LDFLAGS) $(LUA_LDFLAGS)

CORE_SRC := $(wildcard src/core/*.cpp)
CORE_OBJ := $(patsubst src/core/%.cpp,tmp/core_%.o,$(CORE_SRC))

EDITOR_SRC := $(wildcard src/editor/*.cpp)
EDITOR_OBJ := $(patsubst src/editor/%.cpp,tmp/editor_%.o,$(EDITOR_SRC))

MISC_SRC := $(wildcard src/misc/*.cpp)
MISC_OBJ := $(patsubst src/misc/%.cpp,tmp/misc_%.o,$(MISC_SRC))

MAIN_SRC := $(wildcard src/*.cpp)
MAIN_OBJ := $(patsubst src/%.cpp,tmp/main_%.o,$(MAIN_SRC))

all: mini

# Link

variant: $(MAIN_OBJ) $(CORE_OBJ) $(EDITOR_OBJ) $(MISC_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Core stuff
tmp/main_%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

tmp/core_%.o: src/core/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

tmp/editor_%.o: src/editor/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

tmp/misc_%.o: src/misc/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Core stuff

clean:
	rm -f variant docs/* tmp/*

mini: variant
	strip variant ; SetFile -t APPL Variant

dist: mini
	upx variant

test: $(MISC_OBJ)
	$(CC) test.cpp $(LDFLAGS) -o $@ $^

