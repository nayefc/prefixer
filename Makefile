PROGS = prefixer test
OBJECTS = prefixer.o Stack/stack.o
TESTOBJ = test.o
CFLAGS = -Werror -Wmissing-prototypes
LIBS = -lm
CC = gcc

all : $(PROGS)

# prefixer
prefixer : $(OBJECTS)
	$(CC) -o prefixer $(OBJECTS) $(LIBS)
prefixer.o : prefixer.c Stack/stack.h
	$(CC) -c prefixer.c -o $@ $(CFLAGS)
stack.o : Stack./stack.c Stack/stack.h
	$(CC) -c Stack/stack.c -o $@ $(CFLAGS)

# prefixer test
test : $(TESTOBJ)
	$(CC) -o test $(TESTOBJ) $(LIBS)
test.o : test.c prefixer.o
	$(CC) -c test.c -o $@ $(CFLAGS)

.INTERMEDIATE: $(OBJECTS) $(TESTOBJ)

.PHONY: clean
clean : 
	-rm prefixer test file.txt
