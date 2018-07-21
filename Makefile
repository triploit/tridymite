CC=gcc
CXX=g++

build: cli.o
	$(CXX) -o tridy main.cpp cli.o
	rm *.o

cli.o:
	$(CXX) -c cli.cpp