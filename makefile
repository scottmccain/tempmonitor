CC=g++
LDFLAGS=$(MYSQL_LFLAGS)
CFLAGS=$(MYSQL_CFLAGS) -c -Wall -I./lib/
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=monitor

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


