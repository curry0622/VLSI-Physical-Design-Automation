all: hw2

CC = g++
CFLAGS = -std=c++11 -O3 -lm -march=native -pipe
INCLUDE = /usr/local/include/boost/

hw2:main.o FM.o BucketList.o Cell.o Net.o
	$(CC) -o hw2 main.o FM.o BucketList.o Cell.o Net.o
main.o:main.cpp FM.hpp Cell.hpp Net.hpp
	$(CC) -I $(INCLUDE) $(CFLAGS) -c main.cpp
FM.o:FM.cpp FM.hpp
	$(CC) -I $(INCLUDE) $(CFLAGS) -c FM.cpp
BucketList.o:BucketList.cpp BucketList.hpp
	$(CC) -I $(INCLUDE) $(CFLAGS) -c BucketList.cpp
Cell.o:Cell.cpp Cell.hpp
	$(CC) -I $(INCLUDE) $(CFLAGS) -c Cell.cpp
Net.o:Net.cpp Net.hpp
	$(CC) -I $(INCLUDE) $(CFLAGS) -c Net.cpp
clean:
	rm -f *.o *.out hw2 out
