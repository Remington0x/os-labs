all: prog matr_gen

prog: a.c
	gcc -pthread -o a.out a.c

matr_gen: matr_gen.cpp
	g++ -o mg matr_gen.cpp

clean:
	rm -rf *.o *.out
