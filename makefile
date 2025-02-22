# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

all: compile link 

compile: 
	@g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c src/Settlement.cpp -o bin/Settlement.o
	@g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c src/Facility.cpp -o bin/Facility.o
	@g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c src/SelectionPolicy.cpp -o bin/SelectionPolicy.o 
	@g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c src/Auxiliary.cpp -o bin/Auxiliary.o 
	@g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c src/Plan.cpp -o bin/Plan.o 
	@g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c src/Action.cpp -o bin/Action.o 
	@g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c src/Simulation.cpp -o bin/Simulation.o 
	@g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c src/main.cpp -o bin/main.o 

link: 
	@g++ -o bin/simulation bin/main.o bin/Action.o bin/Auxiliary.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Settlement.o bin/Simulation.o
