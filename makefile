# Tritone: A bad Vector Calculator
# Name: Caleb Andreano (andreanoc@msoe.edu)
# Date: 10-17-2023
# Lab 7

CC=gcc                      # c compiler
CFLAGS=-c -Wall -ggdb            # compiler flags
LDFLAGS=                    # linker arguments
SOURCES=main.c tritone.c vec.c ast.c vectable.c  # source files
OBJECTS=$(patsubst %.c,build/%.o,$(SOURCES))
DEPS=$(patsubst %.o,%.d,$(OBJECTS))
EXECUTABLE=build/tritone

all: $(EXECUTABLE)

# pull in dependency info for *existing* .o files
-include $(DEPS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	# ./$@

build/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > build/$*.d

clean:
	rm -rf build/*.o build/*.d $(EXECUTABLE)
