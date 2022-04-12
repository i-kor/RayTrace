OBJECTS = raytrace.o vec.o ray.o camera.o utility.o

CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
LFLAGS = $(shell pkg-config --libs gtk+-3.0) -lm

all: raytrace

raytrace: $(OBJECTS)
	gcc -o raytrace $(OBJECTS) $(LFLAGS) 

raytrace.o: raytrace.c vec.h
	gcc $(CFLAGS) -c raytrace.c

camera.o: camera.c camera.h vec.h
	gcc $(CFLAGS) -c camera.c

ray.o: ray.c ray.h vec.h
	gcc $(CFLAGS) -c ray.c

vec.o: vec.c vec.h
	gcc $(CFLAGS) -c vec.c

utility.o: utility.c utility.h
	gcc $(CFLAGS) -c utility.c

clean:
	rm -f $(OBJECTS)
