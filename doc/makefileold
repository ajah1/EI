SOURCES := $(wildcard lib/*.cpp)
OBJECTS := $(subst .cpp,.o,$(SOURCES))
EJECUTABLE := practica1
INCLUDE := -I /include
CC := g++

$(EJECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ main.cpp

%.o: %.cpp
	$(CC) -o $@ -c $^ $(INCLUDE)

clean: $(OBJECTS)
	rm $^
	rm $(EJECUTABLE)
	rm memory
	rm *.res

run: $(EJECUTABLE)
	./$^

corrector:
	sh ./corrigeAlumno.sh

memoryc:
	$(CC) -o memory memory.cpp
	./memory $(EJECUTABLE)

info:
	$(info $(SOURCES))
	$(info $(OBJECTS))
	$(info $(EJECUTABLE))
	$(info $(INCLUDE))
	$(info $(CC))
