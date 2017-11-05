
# GENERICS
ROOT_DIR := $(ROOT)

CC = $(shell $(ROOT_DIR)/bin/fltk-config --cxx)

SYSCFLAGS = $(shell $(ROOT_DIR)/bin/fltk-config --cxxflags) -Wall -O3 -pedantic -W -g -Wignored-qualifiers -std=c++11
SYSLDFLAGS = -ffunction-sections -fdata-sections -dead_strip -std=c++11

LINKFLTK = $(shell $(ROOT_DIR)/bin/fltk-config --ldstaticflags)
POSTFLTK = $(ROOT_DIR)/bin/fltk-config --post

PLATFORMS = "osx, linux, mingw32, mingw64"

HEADERS_DIR = $(ROOT_DIR)/include
LIBS_DIR = $(ROOT_DIR)/lib

# LUA CONFIG
LUA_LDFLAGS = -llua
LUA_CFLAGS =

################################################################################
## OSX TARGET
################################################################################

# -F DOES NOT MAKE INTO NEXT CALL
OSX_COMMON          = -F /Library/Frameworks -mmacosx-version-min=10.9 -arch x86_64
OSX_CFLAGS          = $(OSX_COMMON)
OSX_LDFLAGS         = $(OSX_COMMON)

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
	$(CC) $(_LDFLAGS) -o $@ $^ $(LINKFLTK)
	strip variant
	$(POSTFLTK) variant

tmp/main_%.o: src/%.cpp
	$(CC) $(_CFLAGS) -c -o $@ $<

# tmp/core_%.o: src/core/%.cpp
# 	$(CC) $(CFLAGS) -c -o $@ $<

# tmp/editor_%.o: src/editor/%.cpp
# 	$(CC) $(CFLAGS) -c -o $@ $<

tmp/misc_%.o: src/misc/%.cpp
	$(CC) $(_CFLAGS) -c -o $@ $<

clean:
	rm -f variant docs/* tmp/*

test: $(MISC_OBJ)
	$(CC) test.cpp $(MY_LDFLAGS) -o $@ $^

