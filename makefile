CC = g++

output: main.o SpaceObject.o 
	g++ main.o SpaceObject.o -o bin/main.exe -std=c++17 -Wall -Wextra -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network
	del *.o
	

main.o: main.cpp
	g++ -c main.cpp -std=c++17

SpaceObject.o: SpaceObject.cpp SpaceObject.hpp
	g++ -c SpaceObject.cpp -std=c++17
	