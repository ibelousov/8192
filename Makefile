P=8192
OBJECTS=
CFLAGS=`pkg-config --cflags ncurses ao` -lpthread -g -Wall -O3 
LDLIBS=-lm `pkg-config --libs ncurses ao`

all: 
	mkdir ./build
	cc src/sound.c src/new_positions.c src/main.c -o build/$(P) $(CFLAGS) $(LDLIBS) 

clean:
	rm -r ./build

mem:
	cc src/sound.c src/new_positions.c src/main.c -o build/$(P) $(CFLAGS) $(LDLIBS) 
	valgrind --leak-check=yes ./build/$(P)

test-positions:
	@$(MAKE) -C tests positions