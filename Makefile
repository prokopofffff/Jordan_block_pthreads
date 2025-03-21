OPTS = -std=c++11 -O3 -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=2 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-property-attribute-mismatch -pthread
all: clear a.out
a.out: main.o funcs.o jordan.o matrix.o
	g++ $(OPTS) $^ -o a.out
main.o: main.cpp funcs.h jordan.h matrix.h
	g++ -c $(OPTS) $<
funcs.o: funcs.cpp funcs.h
	g++ -c $(OPTS) $<
jordan.o: jordan.cpp jordan.h
	g++ -c $(OPTS) $<
matrix.o: matrix.cpp matrix.h
	g++ -c $(OPTS) $<
clear:
	rm -f *.o
clean:
	rm -f *.out *.o *.bak
