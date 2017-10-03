CC = g++

CXXFLAGS = -Wall -std=c++11 -Weffc++ -Wextra -pedantic
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

clean:
	@rm yolo_to_kitty