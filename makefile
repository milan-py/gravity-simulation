CC = g++

output: main.o SpaceObject.o imgui_draw.o imgui_widgets.o imgui.o imgui_tables.o imgui-SFML.o imgui_demo.o
	g++ main.o SpaceObject.o imgui_draw.o imgui_widgets.o imgui.o imgui_tables.o imgui-SFML.o imgui_demo.o -o bin/main.exe -std=c++17 -Wall -Wextra -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-network -lopengl32 -static
	

main.o: main.cpp
	g++ -c main.cpp -std=c++17 -I include/
SpaceObject.o: SpaceObject.cpp SpaceObject.hpp
	g++ -c SpaceObject.cpp -std=c++17 -I include/

imgui_draw.o: lib\imgui_draw.cpp
	g++ lib\imgui_draw.cpp -c -I include/ 
imgui_widgets.o: lib\imgui_widgets.cpp
	g++ lib\imgui_widgets.cpp -c -I include/ 
imgui.o: lib\imgui.cpp
	g++ lib\imgui.cpp -c -I include/
imgui_tables.o: lib\imgui_tables.cpp
	g++ lib\imgui_tables.cpp -c -I include/ 
imgui-SFML.o: lib\imgui-SFML.cpp
	g++ lib\imgui-SFML.cpp -c -I include/ 
imgui_demo.o: lib\imgui_demo.cpp
	g++ lib\imgui_demo.cpp -c -I include/ 

