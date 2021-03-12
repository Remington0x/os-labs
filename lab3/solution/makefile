all: prog matr_gen

prog: a.cpp
	g++ -pthread -o a.out a.cpp

matr_gen: matr_gen.cpp
	g++ -o mg matr_gen.cpp

clean:
	rm -rf *.o *.out
