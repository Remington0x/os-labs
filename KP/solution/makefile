all: alpha bravo charlie

alpha: alpha.cpp
	g++ -lzmq alpha.cpp -o a.out

bravo: bravo.cpp
	g++ -lzmq bravo.cpp -o b.out

charlie: charlie.cpp
	g++ -lzmq charlie.cpp -o c.out

clean:
	rm -rf *.out
