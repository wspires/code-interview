##################### makefile ################################################
#
#    Author:     Wade Spires
#    Program:    ws_tools
#    Date:       2005/02/25
#    Description:
# 	  Makes libws_tools.
#    Cleans directory of object files and executables.
#
###############################################################################

###############################################################################
# Declare variables
###############################################################################

NAME = ws_tools

C  = gcc
CC = g++
AR = ar

FLAGS =
#FLAGS += -g
FLAGS += -O2
#FLAGS += -O3
FLAGS += -Wall

INCLUDES = 
#INCLUDES += -I<include_directory>

LD_FLAGS =
#LD_FLAGS +=

LINK_DIRS = 
#LINK_DIRS += -L<link_directory>

LIBS =
LIBS += -lm
#LIBS = -lpthread
#LIBS += -l<library>

LINK = $(LINK_DIRS) $(LIBS) $(LD_FLAGS)

HEADERS =

SOURCES = 

OBJECTS =

RM = /bin/rm -f

TESTS = '1-arrays-and-strings'
TESTS += 'graph'
#TESTS += 'test_quotes'
#TESTS += 'test_dir'
#TESTS += 'test_random'
#TESTS += 'test_name'

###############################################################################
# Rules for compiling
###############################################################################

# compile each source file into object code
.c.o:
		$(C)  -c $(FLAGS) $< $(INCLUDES)

.cc.o:
		$(CC) -c $(FLAGS) $< $(INCLUDES)

.SUFFIXES: .cpp .o
.cpp.o:
		$(CC) -c $(FLAGS) $< $(INCLUDES)

# link all object modules into executable
$(NAME): $(OBJECTS)
	@for test in $(TESTS); do \
		make -C $$test; \
	done;

# link all object modules into executable
all: $(OBJECTS)
	@for test in $(TESTS); do \
		make -C $$test; \
	done;

###############################################################################
# Rules for other stuff
###############################################################################

# create static library
lib: $(OBJECTS)
	$(AR) rcs lib$(NAME).a $(OBJECTS)

# create source files' dependency list of header files
depend:
	makedepend -- $(FLAGS) -- $(SOURCES) $(INCLUDES) -s'# DO NOT DELETE THIS LINE -- `makedepend` depends on it.'

# remove object files
clean:
	$(RM) ${OBJECTS}
	$(RM) lib${NAME}.a
	@for test in $(TESTS); do \
		make -C $$test clean; \
	done;

# DO NOT DELETE THIS LINE -- `makedepend` depends on it.

