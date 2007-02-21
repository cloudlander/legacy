.PHONY: clean strip copy

# Set the default target. When you make with no arguments,
# this will be the target built.
COMPILER = lc
PRODUCTS = $(COMPILER) 

default: $(PRODUCTS)

# Set up the list of source and object files
SRCS = errors.cpp util.cpp symtable.cpp quad.cpp vm.cpp main.cpp

# OBJS can deal with either .cc or .c files listed in SRCS
OBJS = y.tab.o lex.yy.o $(patsubst %.cpp, %.o, $(filter %.cpp,$(SRCS))) $(patsubst %.c, %.o, $(filter %.c, $(SRCS))) 

JUNK =  *.o *.core core

# Define the tools we are going to use
CC= g++
LD = g++
LEX = flex
YACC = bison

# Set up the necessary flags for the tools

# We want debugging and most warnings, but lex/yacc generate some
# static symbols we don't use, so turn off unused warnings to avoid clutter
# Also STL has some signed/unsigned comparisons we want to suppress
CFLAGS = -g -Wall -Wno-unused -Wno-sign-compare

# The -d flag tells lex to set up for debugging. Can turn on/off by
# setting value of global yy_flex_debug inside the scanner itself
LEXFLAGS = -d

# The -d flag tells yacc to generate header with token types
# The -v flag writes out a verbose description of the states and conflicts
# The -t flag turns on debugging capability
# The -y flag means imitate yacc's output file naming conventions
YACCFLAGS = -dvty

# Link with standard c library, math library, and lex library
LIBS = -lc -lm -lfl

# Rules for various parts of the target

.yy.o: $*.yy.c
	$(CC) $(CFLAGS) -c -o $@ $*.cpp

lex.yy.c: lex.l  parser.y y.tab.h 
	$(LEX) $(LEXFLAGS) lex.l

y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c -o y.tab.o y.tab.c

y.tab.h y.tab.c: parser.y
	$(YACC) $(YACCFLAGS) parser.y
.cpp.o: $*.cpp
	$(CC) $(CFLAGS) -c -o $@ $*.cpp

# rules to build compiler

$(COMPILER) :  $(OBJS) 
	$(LD) -o $@ $(OBJS) $(LIBS)

# This target is to build small for testing (no debugging info), removes
# all intermediate products, too
strip : $(PRODUCTS)
	strip $(PRODUCTS)
	rm -rf $(JUNK)


# make depend will set up the header file dependencies for the 
# assignment.  You should make depend whenever you add a new header
# file to the project or move the project between machines
#
depend:
	makedepend -- $(CFLAGS) $(SRCS) --

clean:
	rm -f $(JUNK) y.output *~ .*~
	find -name $(PRODUCTS) -exec rm {} -f \;
# DO NOT DELETE
