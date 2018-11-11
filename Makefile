CC=gcc
CFLAGS=-lWarn -pedantic

Sensor: Sensor.o libmyifttt.a
	$(CC) Sensor.o -L. -lmyifttt -lcurl -o Sensor

libmyifttt.a:	ifttt.o
	ar -rcs libmyifttt.a ifttt.o

ifttt.o: 	ifttt.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

Sensor.o:	Sensor.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

all:	Sensor

clean:
	rm Sensor *.o
