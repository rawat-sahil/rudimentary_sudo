sudo: dependency.o main.o
	g++   main.o dependency.o -lcrypt  -o sudo
	chmod u+s sudo
dependency.o: dependency.h dependency.cpp
	g++ -g -c dependency.cpp
main.o: main.cpp dependency.h
	g++ -g -c main.cpp

clean:
	rm *o