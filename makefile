
# GENERICS
ROOT_DIR := $(ROOT)

FLTK_CONFIG = $(ROOT_DIR)/bin/fltk-config

CC = $(shell $(FLTK_CONFIG) --cxx)

# -stdlib=libc++ -mmacosx-version-min=10.11

SYSCFLAGS = $(shell $(FLTK_CONFIG) --cxxflags) -Wall -O3 -pedantic -W -Wignored-qualifiers -std=c++11
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

MAIN_SRC := $(wildcard src/*.cxx)
MAIN_OBJ := $(patsubst src/%.cxx,tmp/main_%.o,$(MAIN_SRC))

MISC_SRC := $(wildcard src/misc/*.cxx)
MISC_OBJ := $(patsubst src/misc/%.cxx,tmp/misc_%.o,$(MISC_SRC))

EDITOR_SRC := $(wildcard src/editor/*.cxx)
EDITOR_OBJ := $(patsubst src/editor/%.cxx,tmp/editor_%.o,$(EDITOR_SRC))

# LUA_SRC := $(wildcard src/lua/*.cxx)
# LUA_OBJ := $(patsubst src/lua/%.cxx,tmp/lua_%.o,$(LUA_SRC))

all: variant

variant: $(MAIN_OBJ) $(MISC_OBJ) $(EDITOR_OBJ)
	$(CC) $(_LDFLAGS) -o $@ $^ $(FLTK_LINK)
	strip variant
	$(FLTK_POST) variant

tmp/main_%.o: src/%.cxx
	$(CC) $(_CFLAGS) -c -o $@ $<

# tmp/core_%.o: src/core/%.cxx
# 	$(CC) $(_CFLAGS) -c -o $@ $<

tmp/editor_%.o: src/editor/%.cxx
	$(CC) $(_CFLAGS) -c -o $@ $<

tmp/misc_%.o: src/misc/%.cxx
	$(CC) $(_CFLAGS) -c -o $@ $<

clean:
	rm -rf variant docs/* tmp/* variant.app

test: $(MISC_OBJ)
	$(CC) test.cxx $(MY_LDFLAGS) -o $@ $^

