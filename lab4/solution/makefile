all: lab4 lab4-child

lab4: lab4.c
	gcc -o lab4 lab4.c -pthread -lrt

lab4-child: lab4-child.c
	gcc -o lab4-child lab4-child.c -pthread -lrt

clean:
	rm -rf *.o lab4 lab4-child
