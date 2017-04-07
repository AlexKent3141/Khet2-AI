APP=khet
CXXFLAGS=-Wall -std=c++11

build_all: main.o Utils.o Board.o MoveGenerator.o Evaluator.o
	$(CXX) main.o Utils.o Board.o MoveGenerator.o Evaluator.o -o $(APP) 

clean:
	rm -f *.o Tests/*.o $(APP)
