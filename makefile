.PHONY= clean
CC=g++
OPTIONS= -g -std=gnu++0x
DEBUG= #-D DEBUG
LIBDIR=lib
INCLUDEDIR=include
SOURCES := $(wildcard $(LIBDIR)/*.cpp)
_OBJ := $(subst .cpp,.o,$(SOURCES))
EJECUTABLE := practica1
MAIN := main.cpp

all: $(EJECUTABLE)

$(EJECUTABLE): $(MAIN) $(_OBJ)
	$(CC) $(OPTIONS) $(DEBUG) -I$(INCLUDEDIR) $(MAIN) $(_OBJ) -o $(EJECUTABLE)

$(LIBDIR)/%.o : $(LIBDIR)/%.cpp $(INCLUDEDIR)/%.h
	$(CC) $(OPTIONS) $(DEBUG) -c -I$(INCLUDEDIR) -o $@ $<

run: $(EJECUTABLE)
	./$^

corrector: $(EJECUTABLE)
	sh ./corrigeAlumno.sh

memory: memory.cpp
	$(CC) -o memory  $^

valgrind: $(EJECUTABLE)
	valgrind --tool=memcheck --leak-check=full ./practica1

clean:
	rm -f $(_OBJ) 
	rm -f $(EJECUTABLE)
	rm -f memory
	rm *.tmp
info:
	$(info $(CC))
	$(info $(OPTIONS))
	$(info $(DEBUG))
	$(info $(LIBDIR))l
	$(info $(INCLUDEDIR))
	$(info $(SOURCES))
	$(info $(_OBJ))
	$(info $(EJECUTABLE))
