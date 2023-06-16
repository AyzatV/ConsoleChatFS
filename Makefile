# область специфических параметров
EXEC = ConsoleChatFS
# конец области

.RECIPEPREFIX = >
.SUFFIXES:
.SUFFIXES: .cpp .o
CC = g++
CFLAGS = -c -Wall
SOURCES := $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS)
> $(CC) $(OBJECTS) -o $@

.cpp.o:
> $(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
> rm -rf $(OBJECTS)
