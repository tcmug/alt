
TEMPLATE = app
TARGET = 
DEPENDPATH += . src
VPATH += ../shared
INCLUDEPATH += ../shared

# Input
HEADERS += src/window.h src/altcontext.h src/altformattersyntax.h src/altfilerow.h src/altformatterblockiterator.cpp
SOURCES += src/main.cpp src/window.cpp src/altcontext.cpp src/altformattersyntax.cpp src/altfilerow.cpp src/altformatterblockiterator.cpp

# QT += opengl

#target.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogl
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS hellogl.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogl
#INSTALLS += target sources

#symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
