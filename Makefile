#############################################################################
# Makefile for building: alt.app/Contents/MacOS/alt
# Generated by qmake (2.01a) (Qt 4.7.4) on: Sat Nov 19 13:12:59 2011
# Project:  alt.pro
# Template: app
# Command: /Users/teemumerikoski/QtSDK/Desktop/Qt/474/gcc/bin/qmake -o Makefile alt.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB
CFLAGS        = -pipe -g -gdwarf-2 -arch x86_64 -Xarch_x86_64 -mmacosx-version-min=10.5 -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -g -gdwarf-2 -arch x86_64 -Xarch_x86_64 -mmacosx-version-min=10.5 -Wall -W $(DEFINES)
INCPATH       = -I../../QtSDK/Desktop/Qt/474/gcc/mkspecs/default -I. -I../../QtSDK/Desktop/Qt/474/gcc/lib/QtCore.framework/Versions/4/Headers -I../../QtSDK/Desktop/Qt/474/gcc/include/QtCore -I../../QtSDK/Desktop/Qt/474/gcc/lib/QtGui.framework/Versions/4/Headers -I../../QtSDK/Desktop/Qt/474/gcc/include/QtGui -I../../QtSDK/Desktop/Qt/474/gcc/lib/QtOpenGL.framework/Versions/4/Headers -I../../QtSDK/Desktop/Qt/474/gcc/include/QtOpenGL -I../../QtSDK/Desktop/Qt/474/gcc/include -I../shared -I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers -I/System/Library/Frameworks/AGL.framework/Headers -I. -F/Users/teemumerikoski/QtSDK/Desktop/Qt/474/gcc/lib
LINK          = g++
LFLAGS        = -headerpad_max_install_names -arch x86_64 -Xarch_x86_64 -mmacosx-version-min=10.5
LIBS          = $(SUBLIBS) -F/Users/teemumerikoski/QtSDK/Desktop/Qt/474/gcc/lib -L/Users/teemumerikoski/QtSDK/Desktop/Qt/474/gcc/lib -framework QtOpenGL -framework QtGui -framework QtCore -framework OpenGL -framework AGL 
AR            = ar cq
RANLIB        = ranlib -s
QMAKE         = /Users/teemumerikoski/QtSDK/Desktop/Qt/474/gcc/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
STRIP         = 
INSTALL_FILE  = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = $(COPY_FILE)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
export MACOSX_DEPLOYMENT_TARGET = 10.4

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = src/main.cpp \
		src/window.cpp \
		src/altcontext.cpp \
		src/altformattersyntax.cpp moc_window.cpp \
		moc_altcontext.cpp
OBJECTS       = main.o \
		window.o \
		altcontext.o \
		altformattersyntax.o \
		moc_window.o \
		moc_altcontext.o
DIST          = ../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/unix.conf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/mac.conf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/mac-g++.conf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/qconfig.pri \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/modules/qt_webkit_version.pri \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt_functions.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt_config.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/exclusive_builds.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/default_pre.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/default_pre.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/dwarf2.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/debug.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/default_post.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/default_post.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/x86_64.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/objective_c.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/warn_on.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/unix/opengl.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/unix/thread.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/moc.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/rez.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/sdk.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/resources.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/uic.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/yacc.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/lex.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/include_source_dir.prf \
		alt.pro
QMAKE_TARGET  = alt
DESTDIR       = 
TARGET        = alt.app/Contents/MacOS/alt

####### Custom Compiler Variables
QMAKE_COMP_QMAKE_OBJECTIVE_CFLAGS = -pipe \
		-g \
		-gdwarf-2 \
		-arch \
		x86_64 \
		-Xarch_x86_64 \
		-mmacosx-version-min=10.5 \
		-Wall \
		-W


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile alt.app/Contents/PkgInfo alt.app/Contents/Resources/empty.lproj alt.app/Contents/Info.plist $(TARGET)

$(TARGET):  $(OBJECTS)  
	@$(CHK_DIR_EXISTS) alt.app/Contents/MacOS/ || $(MKDIR) alt.app/Contents/MacOS/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: alt.pro  ../../QtSDK/Desktop/Qt/474/gcc/mkspecs/default/qmake.conf ../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/unix.conf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/mac.conf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/mac-g++.conf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/qconfig.pri \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/modules/qt_webkit_version.pri \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt_functions.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt_config.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/exclusive_builds.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/default_pre.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/default_pre.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/dwarf2.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/debug.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/default_post.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/default_post.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/x86_64.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/objective_c.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/warn_on.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/unix/opengl.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/unix/thread.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/moc.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/rez.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/sdk.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/resources.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/uic.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/yacc.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/lex.prf \
		../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/include_source_dir.prf
	$(QMAKE) -o Makefile alt.pro
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/unix.conf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/mac.conf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/mac-g++.conf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/qconfig.pri:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/modules/qt_webkit_version.pri:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt_functions.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt_config.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/exclusive_builds.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/default_pre.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/default_pre.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/dwarf2.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/debug.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/default_post.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/default_post.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/x86_64.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/objective_c.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/warn_on.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/unix/opengl.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/unix/thread.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/moc.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/rez.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/mac/sdk.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/resources.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/uic.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/yacc.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/lex.prf:
../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/include_source_dir.prf:
qmake:  FORCE
	@$(QMAKE) -o Makefile alt.pro

alt.app/Contents/PkgInfo: 
	@$(CHK_DIR_EXISTS) alt.app/Contents || $(MKDIR) alt.app/Contents 
	@$(DEL_FILE) alt.app/Contents/PkgInfo
	@echo "APPL????" >alt.app/Contents/PkgInfo
alt.app/Contents/Resources/empty.lproj: 
	@$(CHK_DIR_EXISTS) alt.app/Contents/Resources || $(MKDIR) alt.app/Contents/Resources 
	@touch alt.app/Contents/Resources/empty.lproj
	
alt.app/Contents/Info.plist: 
	@$(CHK_DIR_EXISTS) alt.app/Contents || $(MKDIR) alt.app/Contents 
	@$(DEL_FILE) alt.app/Contents/Info.plist
	@sed -e "s,@ICON@,,g" -e "s,@EXECUTABLE@,alt,g" -e "s,@TYPEINFO@,????,g" ../../QtSDK/Desktop/Qt/474/gcc/mkspecs/default/Info.plist.app >alt.app/Contents/Info.plist
dist: 
	@$(CHK_DIR_EXISTS) .tmp/alt1.0.0 || $(MKDIR) .tmp/alt1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/alt1.0.0/ && $(COPY_FILE) --parents src/window.h src/altcontext.h src/altformattersyntax.h .tmp/alt1.0.0/ && $(COPY_FILE) --parents src/main.cpp src/window.cpp src/altcontext.cpp src/altformattersyntax.cpp .tmp/alt1.0.0/ && (cd `dirname .tmp/alt1.0.0` && $(TAR) alt1.0.0.tar alt1.0.0 && $(COMPRESS) alt1.0.0.tar) && $(MOVE) `dirname .tmp/alt1.0.0`/alt1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/alt1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) -r alt.app
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all: moc_window.cpp moc_altcontext.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_window.cpp moc_altcontext.cpp
moc_window.cpp: src/window.h
	/Users/teemumerikoski/QtSDK/Desktop/Qt/474/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ src/window.h -o moc_window.cpp

moc_altcontext.cpp: src/altcontext.h
	/Users/teemumerikoski/QtSDK/Desktop/Qt/474/gcc/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ src/altcontext.h -o moc_altcontext.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

main.o: src/main.cpp src/window.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o src/main.cpp

window.o: src/window.cpp src/window.h \
		src/altcontext.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o window.o src/window.cpp

altcontext.o: src/altcontext.cpp src/altcontext.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o altcontext.o src/altcontext.cpp

altformattersyntax.o: src/altformattersyntax.cpp src/altformattersyntax.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o altformattersyntax.o src/altformattersyntax.cpp

moc_window.o: moc_window.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_window.o moc_window.cpp

moc_altcontext.o: moc_altcontext.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_altcontext.o moc_altcontext.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

