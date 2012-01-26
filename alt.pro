
TEMPLATE = app
TARGET = 
DEPENDPATH += . src
VPATH += ../shared
INCLUDEPATH += ../shared
INCLUDEPATH += ../../other/lua-5.2.0/src
DEPENDPATH += ../../other/lua-5.2.0/src
#CONFIG-=app_bundle

# Input
HEADERS += src/window.h 
HEADERS += src/altcontext.h src/altformattersyntax.h src/altfilerow.h src/altformatterblockiterator.cpp
HEADERS += src/altlua.h

SOURCES += src/main.cpp src/window.cpp
SOURCES += src/altcontext.cpp src/altformattersyntax.cpp src/altfilerow.cpp src/altformatterblockiterator.cpp
SOURCES += src/altlua.c

LIBS += -L../../other/lua-5.2.0/src/
LIBS += -llua
LIBS += -dead_strip 

