CC=gcc
CXX=g++

build: cli.o
	$(CXX) -o tridy main.cpp obj/cli.o
	rm obj/*.o

cli.o:
	$(CXX) -o obj/cli.o -c cli/cli.cpp