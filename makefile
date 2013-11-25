CC=g++
LDFLAGS=$(MYSQL_LFLAGS) -lspidev
CFLAGS=$(MYSQL_CFLAGS) -c -Wall -I./lib/
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=monitor

all: $(SOURCES) $(EXECUTABLE)

install: $(EXECUTABLE)
	mv -f $(EXECUTABLE) /opt/bin/$(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm $(EXECUTABLE)
