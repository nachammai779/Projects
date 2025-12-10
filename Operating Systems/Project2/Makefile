CC      = gcc
CFLAGS  = -c
OBJECTS = logParse.o
RM      = rm -rf

all:    logParse
prog:   $(OBJECTS)
        $(CC) $(OBJECTS) -o logParse

%.o:    %.c
        $(CC) $(CFLAGS) $<

clean:
        $(RM) *.o
