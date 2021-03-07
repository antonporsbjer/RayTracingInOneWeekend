CFLAGS = -std=c++17 -O2

RayTrace: main.cpp
	g++ $(CFLAGS) -o RayTrace.out main.cpp

.PHONY: render clean

render: RayTrace
	./RayTrace.out > image.ppm

clean:
	rm -f *.out *.ppm