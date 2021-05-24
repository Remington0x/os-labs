FLAGS = -g -O2 -pedantic -pthread -std=c++17 -Wall -Werror -Wextra

all: control calculation

control: control.cpp topology.hpp mq.hpp
	g++ $(FLAGS) control.cpp -lzmq -o control

calculation: calculation.o
	g++ $(FLAGS) calculation.o -lzmq -o calculation

calculation_node.o: calculation.cpp mq.hpp
	g++ $(FLAGS) -c calculation.cpp -lzmq

clean:
	rm -rf *.o calculation control
