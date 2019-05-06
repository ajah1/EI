.PHONY= clean
CC=g++
OPTIONS= -g -std=gnu++0x -Wall
DEBUG= #-D DEBUG
LIBDIR=lib
INCLUDEDIR=include
SOURCES := $(wildcard $(LIBDIR)/*.cpp)
_OBJ := $(subst .cpp,.o,$(SOURCES))
EJECUTABLE := practica3
MAIN := src/main.cpp

all: $(EJECUTABLE)

$(EJECUTABLE): $(MAIN) $(_OBJ)
	$(CC) $(OPTIONS) $(DEBUG) -I$(INCLUDEDIR) $(MAIN) $(_OBJ) -o $(EJECUTABLE)

$(LIBDIR)/%.o : $(LIBDIR)/%.cpp $(INCLUDEDIR)/%.h
	$(CC) $(OPTIONS) $(DEBUG) -c -I$(INCLUDEDIR) -o $@ $<

run: $(EJECUTABLE)
	./$^

corrector: $(EJECUTABLE)
	/bin/bash ./corrigeAlumno.sh

memory: memory.cpp
	$(CC) -o memory  $^

valgrind: $(EJECUTABLE)
	valgrind --tool=memcheck --leak-check=full $(EJECUTABLE)

clean:
	rm -f $(_OBJ) 
	rm -f $(EJECUTABLE)
	rm -f memory
	rm -f *.tmp
	rm -f *.res
	rm -f ./corpus/*.tk
info:
	$(info $(CC))
	$(info $(OPTIONS))
	$(info $(DEBUG))
	$(info $(LIBDIR))
	$(info $(INCLUDEDIR))
	$(info $(SOURCES))
	$(info $(_OBJ))
	$(info $(EJECUTABLE))
