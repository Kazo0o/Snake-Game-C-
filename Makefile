CXX = g++
CXXFLAGS= -c -Wall -std=c++17
SRC =src
TARGET=bin/snake-game-c
OUTPUT_DIR=bin
LIB =-Llib -lmingw32 -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32
output: main.o game.o
	$(CXX) main.o game.o -o $(TARGET)  $(LIB)
main.o: main.cpp	
	$(CXX) $(CXXFLAGS) main.cpp -Iinclude -DSFML_STATIC 

game.o: src/game.cpp include/game.h
	$(CXX) $(CXXFLAGS)  $(SRC)/game.cpp -Iinclude -DSFML_STATIC

run: $(TARGET)
	$(TARGET)
clean:
	rm *.o  bin/*.exe .*exe
