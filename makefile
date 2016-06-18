
WX_CONFIG = /Users/tmer/dev/3rd/wxWidgets-3.1.0/build-release/wx-config
CC = g++
COMMON_FLAGS = -mmacosx-version-min=10.7 -arch x86_64

# Port of the wx library to build against [gtk1,gtk2,msw,x11,motif,osx_cocoa,osx_carbon,dfb]
WX_PORT ?= $(shell $(WX_CONFIG) --query-toolkit)
WX_VERSION ?= $(shell $(WX_CONFIG) --query-version | sed -e 's/\([0-9]*\)\.\([0-9]*\)/\1\2/')
WX_VERSION_MAJOR = $(shell echo $(WX_VERSION) | cut -c1,1)
WX_VERSION_MINOR = $(shell echo $(WX_VERSION) | cut -c2,2)
WX_CONFIG_FLAGS = --toolkit=$(WX_PORT) --version=$(WX_VERSION_MAJOR).$(WX_VERSION_MINOR)
WX_CFLAGS = $(shell $(WX_CONFIG) --cxxflags --static $(WX_CONFIG_FLAGS))
WX_LDFLAGS = $(shell $(WX_CONFIG) --libs --static $(WX_CONFIG_FLAGS))
WX_CFLAGS = `$(WX_CONFIG) --cxxflags --static $(WX_CONFIG_FLAGS)`
WX_LDFLAGS = `$(WX_CONFIG) --libs --static $(WX_CONFIG_FLAGS)`


CFLAGS = $(COMMON_FLAGS) -O2 -Wall $(WX_CFLAGS)
LDFLAGS = -ffunction-sections -fdata-sections -Wl $(COMMON_FLAGS) -dead_strip $(WX_LDFLAGS)

CORE_SRC := $(wildcard src/core/*.cpp)
CORE_OBJ := $(patsubst src/core/%.cpp,tmp/core_%.o,$(CORE_SRC))

MAIN_SRC := $(wildcard src/*.cpp)
MAIN_OBJ := $(patsubst src/%.cpp,tmp/main_%.o,$(MAIN_SRC))

all: alt mini

# Link

alt: $(MAIN_OBJ) $(CORE_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Core stuff
tmp/main_%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

tmp/core_%.o: src/core/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Core stuff
clean:
	rm -f alt docs/* tmp/*

mini:
	strip alt ; SetFile -t APPL alt
