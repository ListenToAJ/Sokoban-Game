CC = g++
CFLAGS = --std=c++17  -Werror -pedantic -g
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework
INCLUDEDIR = -I/opt/homebrew/Cellar/boost/1.85.0/include/ -I/opt/homebrew/Cellar/sfml/2.6.1/include/
LIBDIR = -L/opt/homebrew/Cellar/boost/1.85.0/lib/ -L/opt/homebrew/Cellar/sfml/2.6.1/lib/
DEPS = Sokoban.hpp TileObject.hpp
# Your compiled .o files
OBJECTS = Sokoban.o TileObject.o
# The name of your program
PROGRAM = Sokoban

.PHONY: all clean lint

all: $(PROGRAM) $(PROGRAM).a

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $< $(INCLUDEDIR)

$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBDIR) $(LIB) 

$(PROGRAM).a : $(OBJECTS)
	ar rcs $@ $^ 

clean:
	rm *.o $(PROGRAM) test $(PROGRAM).a 

lint:
	cpplint *.cpp *.hpp
