CXX := g++
CXXFLAGS := -std=c++11 -O3 -march=native -pipe
LIBS := -lm
EXEC := ../bin/hw3
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

draw: $(EXEC)
	./$(EXEC) ../testcases/$(t).hardblocks ../testcases/$(t).nets  ../testcases/$(t).pl ../output/$(t).floorplan $(dsr)
	python3 ../scripts/draw.py -hb ../testcases/$(t).hardblocks -fp ../output/$(t).floorplan -dsr $(dsr)
