CC = gcc
CFLAGS = -Wall -Wextra -O2 -I./dependencies
LDFLAGS = -lSDL2 -lSDL2_ttf -lm

OBJS = main.o create_instance.o pollEvent.o raycaster.o minimap.o gameplay.o

raycaster: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) raycaster
