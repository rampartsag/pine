####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DLINUX_X11
CFLAGS        = -pipe -O2 -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -std=c++11 -O2 -Wall -W $(DEFINES)
INCPATH       = -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS) -lX11 -lEGL -lGLESv2 -lz 
AR            = ar cqs
RANLIB        = 
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = engine.cpp \
		gl_window.cpp \
		gl_common.cpp \
		main.cpp \
		shaders.cpp \
		gui/gui.cpp \
		gui/menu.cpp \
		shapes/shape4d.cpp \
		math/matrix.cpp \
		math/vector.cpp \
		shapes/shapes.cpp 
OBJECTS       = engine.o \
		gl_window.o \
		gl_common.o \
		main.o \
		shaders.o \
		gui.o \
		menu.o \
		shape4d.o \
		matrix.o \
		vector.o \
		shapes.o
DESTDIR       = 
TARGET        = pine

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

all: $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

engine.o: engine.cpp engine.h \
		gui/gui.h \
		gui/menu.h \
		shapes/shapes.h \
		shapes/shape4d.h \
		math/vector.h \
		math/matrix.h \
		shaders.h \
		gl_common.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o engine.o engine.cpp

gl_window.o: gl_window.cpp gl_window.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gl_window.o gl_window.cpp

gl_common.o: gl_common.cpp gl_common.h \
		engine.h \
		gui/gui.h \
		gui/menu.h \
		shapes/shapes.h \
		shapes/shape4d.h \
		math/vector.h \
		math/matrix.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gl_common.o gl_common.cpp

main.o: main.cpp engine.h \
		gui/gui.h \
		gui/menu.h \
		shapes/shapes.h \
		shapes/shape4d.h \
		math/vector.h \
		math/matrix.h \
		gl_window.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

shaders.o: shaders.cpp shaders.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o shaders.o shaders.cpp

gui.o: gui/gui.cpp gui/gui.h \
		gui/menu.h \
		gl_common.h \
		engine.h \
		shapes/shapes.h \
		shapes/shape4d.h \
		math/vector.h \
		math/matrix.h \
		shaders.h \
		image.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o gui.o gui/gui.cpp

menu.o: gui/menu.cpp gui/menu.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o menu.o gui/menu.cpp

shape4d.o: shapes/shape4d.cpp shapes/shape4d.h \
		math/vector.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o shape4d.o shapes/shape4d.cpp

matrix.o: math/matrix.cpp math/matrix.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o matrix.o math/matrix.cpp

vector.o: math/vector.cpp math/vector.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o vector.o math/vector.cpp

shapes.o: shapes/shapes.cpp shapes/shapes.h \
		shapes/shape4d.h \
		math/vector.h \
		geometry.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o shapes.o shapes/shapes.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

