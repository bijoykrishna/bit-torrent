CC=gcc
CFLAGS=-c -Wall
LDFLAGS= -lpthread 
SOURCES=Client.c  Config.c  Server.c  Utility.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=fileowner peer

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf *.o fileowner
	rm -rf *.o peer
