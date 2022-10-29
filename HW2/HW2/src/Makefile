CXX := g++
CXXFLAGS := -std=c++11 -O3 -march=native -pipe
LIBS := -lm
INCLUDES = -I /usr/local/include/boost/
EXEC := ../bin/hw2
SRC_DIRS := .
SRCS := main.cpp $(wildcard $(SRC_DIRS:=/*.cpp))
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LIBS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(EXEC) $(OBJS) $(DEPS)