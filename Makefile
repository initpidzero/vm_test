CC=gcc
CFLAGS=-I.
#DEPS = vm.h
OBJ = vm.o 

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

vm.out: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

