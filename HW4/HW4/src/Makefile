# CXX := /usr/local/gcc-9.3.0/bin/g++
CXX := g++
CXXFLAGS := -std=c++11 -O3 -march=native -pipe
LIBS := -lm
EXEC := ../bin/hw4
SRC_DIRS := .
SRCS := main.cpp $(wildcard $(SRC_DIRS:=/*.cpp))
OBJS := $(SRCS:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LIBS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(EXEC) $(OBJS)

judge:
	./judge.sh
