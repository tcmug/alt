
# GENERICS
ROOT_DIR := $(ROOT)

CC = $(shell $(ROOT_DIR)/bin/fltk-config --cxx)

SYSCFLAGS = $(shell $(ROOT_DIR)/bin/fltk-config --cxxflags) -Wall -O3 -pedantic -W -g -Wignored-qualifiers
SYSLDFLAGS = $(shell $(ROOT_DIR)/bin/fltk-config --ldstaticflags) -ffunction-sections -fdata-sections -dead_strip

PLATFORMS = "osx, linux, mingw32, mingw64"

HEADERS_DIR = $(ROOT_DIR)/include
LIBS_DIR = $(ROOT_DIR)/lib

# LUA CONFIG
LUA_LDFLAGS = -llua
LUA_CFLAGS =

# SDL CONFIG
FLTK_LDFLAGS =
FLTK_CFLAGS =

################################################################################
## GENERIC TARGET
################################################################################

generic:
	$(MAKE) -j 4 variant CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"

################################################################################
## OSX TARGET
################################################################################

# -F DOES NOT MAKE INTO NEXT CALL
OSX_COMMON          = -F /Library/Frameworks -mmacosx-version-min=10.9 -arch x86_64
OSX_CFLAGS          = $(OSX_COMMON)
OSX_LDFLAGS         = $(OSX_COMMON)

osx:
	$(MAKE) -j 4 variant CFLAGS="$(CFLAGS) $(OSX_CFLAGS)" LDFLAGS="$(LDFLAGS) $(OSX_LDFLAGS)"

################################################################################
## LINUX TARGET
################################################################################

linux:
	$(MAKE) -j 4 variant CFLAGS="$(CFLAGS) -m64" LDFLAGS="$(LDFLAGS) -m64"

################################################################################
## MINGW32 TARGET
################################################################################

mingw32:
	$(MAKE) variant CC="mingw32-gcc.exe" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"

################################################################################
## MINGW64 TARGET
################################################################################

mingw64:
	$(MAKE) variant CC="x86_64-w64-mingw64-gcc.exe" CFLAGS="" LDFLAGS="$(LDFLAGS)"

################################################################################
## BUILD PROCESS
################################################################################

CFLAGS = $(SYSCFLAGS) $(MYCFLAGS) -I$(HEADERS_DIR) $(LUA_CFLAGS) $(FLTK_CFLAGS)
LDFLAGS = $(SYSLDFLAGS) $(MYLDFLAGS) -L$(LIBS_DIR) -I$(HEADERS_DIR) $(LUA_LDFLAGS) $(FLTK_LDFLAGS)

MAIN_SRC := $(wildcard src/*.cpp)
MAIN_OBJ := $(patsubst src/%.cpp,tmp/main_%.o,$(MAIN_SRC))

# GFX_SRC := $(wildcard src/gfx/*.cpp)
# GFX_OBJ := $(patsubst src/gfx/%.cpp,tmp/gfx_%.o,$(GFX_SRC))

# OBJ_SRC := $(wildcard src/obj/*.cpp)
# OBJ_OBJ := $(patsubst src/obj/%.cpp,tmp/obj_%.o,$(OBJ_SRC))

# LUA_SRC := $(wildcard src/lua/*.cpp)
# LUA_OBJ := $(patsubst src/lua/%.cpp,tmp/lua_%.o,$(LUA_SRC))

all:
	@echo "Define platform: $(PLATFORMS)"

variant: $(MAIN_OBJ) #$(GFX_OBJ) $(OBJ_OBJ) $(LUA_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

tmp/main_%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# tmp/core_%.o: src/core/%.cpp
# 	$(CC) $(MY_CFLAGS) -c -o $@ $<

# tmp/editor_%.o: src/editor/%.cpp
# 	$(CC) $(MY_CFLAGS) -c -o $@ $<

# tmp/misc_%.o: src/misc/%.cpp
# 	$(CC) $(MY_CFLAGS) -c -o $@ $<

clean:
	rm -f variant docs/* tmp/*

test: $(MISC_OBJ)
	$(CC) test.cpp $(MY_LDFLAGS) -o $@ $^

