CC 			= g++
C_FLAGS 	= -c -O3 -msse4.2 
I_FLAG		= -I ./blend
DEBUG_FLAGS = -fsanitize=address,leak,undefined -Wall -g
SFML_FLAGS 	= -lsfml-graphics -lsfml-window -lsfml-system


all: alpha

alpha: alpha.o draw.o main.o
	$(CC) alpha.o draw.o main.o -o alpha $(DEBUG_FLAGS) $(SFML_FLAGS)

clear:
	rm -rf *.o


alpha.o: ./blend/alpha.cpp
	$(CC) $(C_FLAGS) ./blend/alpha.cpp $(I_FLAG)
draw.o: ./blend/draw.cpp
	$(CC) $(C_FLAGS) ./blend/draw.cpp  $(I_FLAG)
main.o: ./blend/main.cpp
	$(CC) $(C_FLAGS) ./blend/main.cpp  $(I_FLAG)