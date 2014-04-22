
# Source
# Name your Project
PROJECT_NAME = lamp_control

# Sourcefiles
SOURCE = main.cpp


# Optimization
# O0 - no optimization
# O1 and 02 - optimize for speed
# O3 - faster but not always correct
# Os - optimize for programmsize (default)
OPT=O0


INCLUDES = -I"../"


CPP = g++
CPPFLAGS += -std=c++11
CPPFLAGS += -Wall -Wunused -Wextra -Wpedantic -$(OPT) -fsigned-char -g
CPPFLAGS += $(INCLUDES)
CPPFLAGS += -Wfatal-errors
LDFLAGS = -lm -lserial

all: program

.PHONY: clean
clean:
	rm $(PROJECT_NAME)


# Compile the program.
program : $(SOURCE)
	$(CPP) $(CPPFLAGS) -o $(PROJECT_NAME) $(SOURCE) $(LDFLAGS)

