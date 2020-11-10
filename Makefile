P=8192
OBJECTS=
CFLAGS=`pkg-config --cflags ncurses` -g -Wall -O3 
LDLIBS=-lm `pkg-config --libs ncurses`

all: 
	mkdir ./build
	cc src/new_positions.c src/main.c -o build/$(P) $(CFLAGS) $(LDLIBS) 

clean:
	rm -r ./build

mem:
	cc src/new_positions.c src/main.c -o build/$(P) $(CFLAGS) $(LDLIBS) 
	valgrind --leak-check=yes ./build/$(P)

test-positions:
	@$(MAKE) -C tests positions