CXX = g++
INCLUDE = -Iinclude
CXXFLAGS = $(INCLUDE) -lncurses -Wall -Wextra -Werror
SRC = src/main.cpp src/tic_tac_toe.cpp src/initialize.cpp
TARGET = build/TicTacToeGame

all:
	$(CXX) -o $(TARGET) $(SRC) $(CXXFLAGS)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)

rebuild: clean all

tic: rebuild run