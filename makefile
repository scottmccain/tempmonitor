CC=g++
LDFLAGS=$(MYSQL_LFLAGS)
CFLAGS=$(MYSQL_CFLAGS) -c -Wall -I./lib/
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=monitor

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
<<<<<<< HEAD
        $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
        $(CC) $(CFLAGS) $< -o $@
=======
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

>>>>>>> 10467a5d2744d48e62cb0131f4a33dde1ab5a4fb

