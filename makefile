APP=khet
CXXFLAGS=-Wall -std=c++11

build_all: main.o Utils.o Board.o
	$(CXX) main.o Utils.o Board.o -o $(APP) 

clean:
	rm -f *.o $(APP)
