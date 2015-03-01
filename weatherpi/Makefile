CC = g++
CFLAGS = -std=c++0x -I. -lwiringPi -lbcm2835
DEPS =
OBJ = execcmd.o strhelper.o bmp180.o dht22.o weather.o

%.o: %.cpp $(DEPS)
	$(CC) ${CFLAGS} -c -o $@ $<

weatherpi: $(OBJ)
	g++ -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
	rm -f weatherapi
