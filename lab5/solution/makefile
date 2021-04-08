FLAGS = -g -O2 -pedantic -std=c11 -Wall -Werror

static1: dynamic1 static_main
	gcc $(FLAGS) static_main.o -L. -limp1 -lm -o solution

static2: dynamic2 static_main
	gcc $(FLAGS) static_main.o -L. -limp2 -lm -o solution

static_main: static_main.c
	gcc $(FLAGS) -c static_main.c

dynamic: dynamic_main dynamic1 dynamic2
	gcc $(FLAGS) dynamic_main.o -o solution -ldl

dynamic_main: dynamic_main.c
	gcc $(FLAGS) -c dynamic_main.c -ldl

dynamic1: declaration.h imp1.c
	gcc $(FLAGS) -shared -o libimp1.so -fPIC imp1.c

dynamic2: declaration.h imp2.c
	gcc $(FLAGS) -shared -o libimp2.so -fPIC imp2.c

clean:
	rm -rf *.o *.so solution
