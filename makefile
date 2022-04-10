OBJECTS = raytrace.o vec.o ray.o

CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
LFLAGS = $(shell pkg-config --libs gtk+-3.0) -lm

all: raytrace

raytrace: $(OBJECTS)
	gcc -o raytrace $(OBJECTS) $(LFLAGS) 

raytrace.o: raytrace.c vec.h
	gcc $(CFLAGS) -c raytrace.c

ray.o: ray.c ray.h vec.h
	gcc $(CFLAGS) -c ray.c

vec.o: vec.c vec.h
	gcc $(CFLAGS) -c vec.c

clean:
	rm -f $(OBJECTS)
