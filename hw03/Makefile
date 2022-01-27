# For building CS 211 Homework 3
# You don’t need to edit this file, and you probably shouldn’t.

# The Make variable SIZE defined here specifies the value of
# MAX_CANDIDATES inside your C source. You don't need to edit this file
# to change this value. Instead, pass it as an additional command-line
# argument to Make, like so:
#
#     % make test SIZE=16
#
SIZE       ?= 16

# Programs we can build:
EXES        = count test_vc

# C compiler to use:
CC         ?= cc
# Extra options for catching bad stuff:
SANFLAGS   += -fsanitize=address,undefined
# Flags for compiling individual files:
CFLAGS     += -g -O0 -std=c11 -DMAX_CANDIDATES=$(SIZE)
CFLAGS     += -pedantic-errors @.Wflags $(SANFLAGS)
# Flags for linking the final program:
LDFLAGS    += -l211 $(SANFLAGS)

# Suffixes for filenames
-Z          = -$(SIZE)
.O          = -$(SIZE).o

# Builds both programs and runs the tests.
test: $(EXES)
	./test_vc

# Builds both programs but doesn’t run anything.
all: $(EXES)

# Creates a link from the main project directory to a binary in .bin/
# that has MAX_CANDIDATES set to SIZE:
%: .bin/%$(-Z)
	ln -sf $< $@

# The next two rules actually build the two binaries:

.bin/test_vc$(-Z): test/test_vc$(.O) src/libvc$(.O)
	@mkdir -p .bin
	$(CC) -o $@ $^ $(LDFLAGS)

.bin/count$(-Z): src/count$(.O) src/libvc$(.O)
	@mkdir -p .bin
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule for building object files with SIZE in the name:
%$(.O): %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

clean:
	$(RM) $(EXES) */*.o
	$(RM) -R .bin

# Header dependencies:
src/count$(.O) src/libvc$(.O) test/test_vc$(.O): src/libvc.h

# Targets that are not actually files we can build:
.PHONY: all test clean
