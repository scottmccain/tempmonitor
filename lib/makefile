CC=g++
CFLAGS=-Wall -fPIC
SHAREDNAME=libspidev.so
LIBNAME=$(SHAREDNAME).1
LDFLAGS=-shared -Wl,-soname,$(LIBNAME)
SOURCES=spi-dev.cpp
OBJS = $(SOURCES:.cpp=.o)
VERSION=0
LIBRARY=$(LIBNAME).$(VERSION)

all: $(SOURCES) $(LIBRARY)

install: $(LIBRARY)
	mv $(LIBRARY) /usr/local/lib/$(LIBRARY)
	ln -sf /usr/local/lib/$(LIBRARY) /usr/local/lib/$(LIBNAME)
	ln -sf /usr/local/lib/$(LIBRARY) /usr/local/lib/$(SHAREDNAME)

$(LIBRARY): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

.ccp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm $(LIBRARY)
