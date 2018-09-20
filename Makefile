CXX=g++

build: cli.o
	$(CXX) -o tridy main.cpp obj/cli.o
	rm obj/*.o

cli.o:
	$(CXX) -o obj/cli.o -c cli/cli.cpp

# DON'T USE THIS PART IN THE PACKAGE.SH
install: build
	sudo tridy local .
