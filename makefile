CC = g++

output: main.o SpaceObject.o 
	g++ main.o SpaceObject.o -o bin/main.exe -Wall -Wextra -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network
	del *.o
	

main.o: main.cpp
	g++ -c main.cpp

SpaceObject.o: SpaceObject.cpp SpaceObject.hpp
	g++ -c SpaceObject.cpp
	