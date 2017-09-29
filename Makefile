CC=clang
CFLAGS=-Weverything -Wall -Wextra -pedantic --std=gnu99 -ggdb3 -O0

all: cfmap map.bin
	@echo "Run 'make run' now to start the game!"

cfmap: cfmap.o
	$(CC) cfmap.o -o cfmap

cfmap.o: cfmap.c
	$(CC) $(CFLAGS) -c cfmap.c -o cfmap.o

map: map.S
	$(CC) -nostdlib -ggdb3 map.S -o map

map.hex: map
	xxd -g 0 \
		-seek `readelf -S map | sed -n '/\.text/ { s/^.* //; s.0*.0x.; p }'` \
		-l \
		`readelf -S ./map | sed -n '/.text/ { n; s/^[ 0]*\([^ ]*\).*/0x\1/; p }'`\
		map | cut -d' ' -f2 > map.hex

map.bin: map.hex
	xxd -r -p map.hex map.bin

run: all
	./cfmap ./map.bin

clean:
	rm -f *.o cfmap map map.bin map.hex
