APP=khet
CXXFLAGS=-Wall -std=c++11
LDFLAGS=-lpthread

src=$(wildcard *.cpp)
obj=$(src:.cpp=.o)

build_all: $(obj)
	$(CXX) $^ -o $(APP) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(APP)
