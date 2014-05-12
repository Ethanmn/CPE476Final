SRCDIR=src
CPP_EXT=cpp
# The C++ source files (but not the headers)
SRCS=$(shell find $(SRCDIR) -type f -name '*.$(CPP_EXT)')

# The directory to store the compiled .o files.
OBJDIR=obj

# The .o files. One per cpp file. Put them in the obj directory.
OBJECTS=$(patsubst %.$(CPP_EXT),$(OBJDIR)/%.o,$(SRCS))

# The packages loaded by pkg-config.
PKGS=glew assimp

# The flags you want to use when compiling individual objects (.o files)
# -g=generate information for debugging (used with debugging tools such as gdb
#  or valgrind)
# -Wall=Give me all warnings!
# -Wextra=Give me extra warnings!
# -std=c++11=Enforce C++11 standard compliance. (You could also use C++11 if you
#  want to be more up-to-date).
# -MMD=Create a .d file to store the rule for the header dependencies of each object.
CFLAGS=-g -Wall -Wextra -std=c++11 -MMD -I$(SRCDIR) `sdl2-config --cflags` `pkg-config --cflags $(PKGS)`

# LDLIBS (Load Libraries)
# External libraries you are using that need to be linked.
# ``=run a shell command (command substitution)
# sdl-config=a command that generates the load libs/cflags necessary depending
# on the platform (OS/Linux/Win)
LDLIBS=`sdl2-config --libs` `pkg-config --libs $(PKGS)` 

# LDFLAGS (Load/linker flags)
ifeq ($(shell uname -s),Darwin)
	LDFLAGS=-framework OpenGL
else
	LDFLAGS=
endif

# The C++ compiler you are using.
CC=clang++

# The directory to put the executable.
BINDIR=gen

# What you run to play the game!
EXECUTABLE=deer

# If files named all/clean/run exist in the directory, then
# Make will not run these commands (because they will be "Up-to-Date")
# .PHONY is a special way of telling Make to run these no matter what.
.PHONY: all clean run docs

all: $(EXECUTABLE)

# Pastes in all of the Make rules inside of the generated .d files.
# Rules are of the format (file.o: file.cpp header1.h header2.h)
# Each .d file gets generated whenever its .o file is compiled.
# Special thanks to /u/yurik for pointing this out to me!
-include $(OBJECTS:.o=.d)

# Links the objects into a "cavestory" executable.
# Also links the libs in $(LDLIBS) (SDL1.2 in our case)
# Puts the Executable in the bindir.
# re-link when an object or dependency has changed.
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) -o $(BINDIR)/$(EXECUTABLE) $(OBJECTS) $(LDLIBS)

# The rule for how to compile an .o (object) file from a .cpp (C++ source) file
# "-c" creates a .o file
# "-o" says to create an object file (.o)
# "$@" in this case is the name of the object file (.o). MUST FOLLOW -o
# "$<" in this case is the .cpp file
# Make the directory if it doesn't exist
$(OBJDIR)/%.o: %.$(CPP_EXT)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

docs:
	doxygen Doxyfile

# Deletes all .o/.d files and the executable. This helps when you want to
# force recompilation.
clean:
	rm -rf obj gen

# Only needs to be run once. Really simple for cleaning after a checkout
git-hooks:
	cd ./.git/hooks/ &&\
	echo "exec make clean" > post-checkout &&\
	chmod 700 post-checkout

# Just a nice way of running the game. (Since you have to be in the bindir for
# the content to load properly)
run:
	cd $(BINDIR) && ./$(EXECUTABLE)
