# Makefile for UD CISC361 user-level thread library
# by Matthew Zelinsky

# Example usage
# --------------------
# to compile and run a specific test file 'X.c'
# run 'make X' to compile
# and './X' to run
#
# to compile all tests for phase X
# run 'make pX'
#
# to compile and run all tests for phase X
# run 'make test_pX'
#
# to compile all tests for all phases
# run 'make all'
#
# to compile and run all tests for all phases
# run 'make test_all'
# --------------------

CC = gcc
CFLAGS = -g -Wall
COMP = $(CC) $(CFLAGS) -c $<
LINK = $(CC) -o $@ $^
RUN = for X in $^; do echo "\n$$X"; ./$$X; done

# Source files for thread library
LIBSRCS = t_lib.c
# Object files
LIBOBJS = $(LIBSRCS:.c=.o)

# Source files for testing phases 1 - 4
# To add a test file for a certain phase, just add it to the end
# of the corresponding variable; no other changes need to be made
P1 = test00.c test01.c test01x.c
P2 = test01a.c test02.c test04.c test07.c
P3 = test03.c 3test.c philosophers.c
P4 = test05.c test06.c test08.c

TSTSRCS = $(P1) $(P2) $(P3) $(P4)

#Object files
TSTOBJS = $(TSTSRCS:.c=.o)

# Executables
P1_EXECS = $(P1:.c=)
P2_EXECS = $(P2:.c=)
P3_EXECS = $(P3:.c=)
P4_EXECS = $(P4:.c=)
EXECS = $(P1_EXECS) $(P2_EXECS) $(P3_EXECS) $(P4_EXECS)

# Creates static thread library (using ar)
t_lib.a: $(LIBOBJS) Makefile
	ar rcs $@ $(LIBOBJS)

t_lib.o: t_lib.c t_lib.h Makefile
	$(COMP)

# Rules to make and run testing executables
all: t_lib.a $(EXECS)

p1: $(P1_EXECS)

p2: $(P2_EXECS)

p3: $(P3_EXECS)

p4: $(P4_EXECS)

test_all: test_p1 test_p2 test_p3 test_p4

test_p1: $(P1_EXECS)
	$(RUN)

test_p2: $(P2_EXECS)
	$(RUN)

test_p3: $(P3_EXECS)
	$(RUN)

test_p4: $(P4_EXECS)
	$(RUN)

$(TSTOBJS): %.o: %.c ud_thread.h
	$(COMP)

$(EXECS): %: %.o t_lib.a
	$(LINK)

clean:
	rm -f t_lib.a $(EXECS) $(LIBOBJS) $(TSTOBJS) *~
