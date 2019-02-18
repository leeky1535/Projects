
#
EXE = flowfree
OBJS = main.o

COMPILER = g++
COMPILER_OPTS = -std=c++11 
LINKER = g++
#LINKER_OPTS = -lpng

all : $(EXE)

$(EXE) : $(OBJS)
	$(LINKER) $(OBJS) -o $(EXE)

main.o : main.cpp
	$(COMPILER) $(COMPILER_OPTS) main.cpp

clean :
	-rm -f *.o $(EXE)
