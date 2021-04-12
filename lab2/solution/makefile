all: lab2.out lab2-child.out

stringops.o: stringops.c stringops.h
	gcc -c stringops.c

lab2.out: lab2.o stringops.o
	gcc stringops.o lab2.o -o lab2.out

lab2.o: stringops.o lab2.c
	gcc -c lab2.c

lab2-child.o: stringops.o lab2-child.c
	gcc -c lab2-child.c

lab2-child.out: stringops.o lab2-child.o
	gcc stringops.o lab2-child.o -o lab2-child.out

clean:
	rm -rf *.o *.out a b
