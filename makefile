CC = g++

all: main

main: main.cpp
	$(CC) main.cpp -o bin/main.exe -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network -Wall -Wextra