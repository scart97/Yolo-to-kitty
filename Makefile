# Assim compila
#g++ -o test main.cpp `pkg-config --libs opencv` darknet/darknet.so -std=c++11 -Wall

CC = g++

CXXFLAGS = -Wall -std=c++11
LFLAGS = darknet/darknet.so `pkg-config --libs opencv`
IFLAGS = 

SOURCES = main.cpp
OBJ = $(SOURCES: .cpp=.o)


#rules

all: yolo_to_kitty

yolo_to_kitty: $(OBJ)
	@$(CC) -o $@ $^ $(CXXFLAGS) $(LFLAGS)

%.o: %.cpp
	@$(CC) -o $@ -c $< $(CXXFLAGS) $(LFLAGS)