
# GENERICS
ROOT_DIR := $(ROOT)

FLTK_CONFIG = $(ROOT_DIR)/bin/fltk-config

CC = $(shell $(FLTK_CONFIG) --cxx)

SYSCFLAGS = $(shell $(FLTK_CONFIG) --cxxflags) -Wall -O3 -pedantic -W -g -Wignored-qualifiers -std=c++11
SYSLDFLAGS = -ffunction-sections -fdata-sections -dead_strip -std=c++11

FLTK_LINK = $(shell $(FLTK_CONFIG) --ldstaticflags)
FLTK_POST = $(FLTK_CONFIG) --post

PLATFORMS = "osx, linux, mingw32, mingw64"

HEADERS_DIR = $(ROOT_DIR)/include
LIBS_DIR = $(ROOT_DIR)/lib

# LUA CONFIG
LUA_LDFLAGS = -llua
LUA_CFLAGS =

################################################################################
## BUILD PROCESS
################################################################################

_CFLAGS = $(SYSCFLAGS) $(MYCFLAGS) -I$(HEADERS_DIR) $(LUA_CFLAGS)
_LDFLAGS = $(SYSLDFLAGS) $(MYLDFLAGS) -L$(LIBS_DIR) -I$(HEADERS_DIR) $(LUA_LDFLAGS)

MAIN_SRC := $(wildcard src/*.cpp)
MAIN_OBJ := $(patsubst src/%.cpp,tmp/main_%.o,$(MAIN_SRC))

MISC_SRC := $(wildcard src/misc/*.cpp)
MISC_OBJ := $(patsubst src/misc/%.cpp,tmp/misc_%.o,$(MISC_SRC))

# OBJ_SRC := $(wildcard src/obj/*.cpp)
# OBJ_OBJ := $(patsubst src/obj/%.cpp,tmp/obj_%.o,$(OBJ_SRC))

# LUA_SRC := $(wildcard src/lua/*.cpp)
# LUA_OBJ := $(patsubst src/lua/%.cpp,tmp/lua_%.o,$(LUA_SRC))

all: variant

variant: $(MAIN_OBJ) $(MISC_OBJ) #$(OBJ_OBJ) $(LUA_OBJ)
	$(CC) $(_LDFLAGS) -o $@ $^ $(FLTK_LINK)
	strip variant
	$(FLTK_POST) variant

tmp/main_%.o: src/%.cpp
	$(CC) $(_CFLAGS) -c -o $@ $<

# tmp/core_%.o: src/core/%.cpp
# 	$(CC) $(CFLAGS) -c -o $@ $<

# tmp/editor_%.o: src/editor/%.cpp
# 	$(CC) $(CFLAGS) -c -o $@ $<

tmp/misc_%.o: src/misc/%.cpp
	$(CC) $(_CFLAGS) -c -o $@ $<

clean:
	rm -rf variant docs/* tmp/* variant.app

test: $(MISC_OBJ)
	$(CC) test.cpp $(MY_LDFLAGS) -o $@ $^

