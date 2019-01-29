SOURCES := $(wildcard lib/*.cpp)
OBJECTS := $(subst .cpp,.o,$(SOURCES))
EJECUTABLE := practica1

$(EJECUTABLE): $(OBJECTS)
	g++ -o $@ $^

%.o: %.cpp
	g++ -o $@ -c $^ -I /include

clean: $(OBJECTS)
	rm $^

run: $(EJECUTABLE)
	./$^

corrector:
	./corrector/corrigeAlumno.sh

info:
	$(info $(SOURCES))
	$(info $(OBJECTS))
