CFLAGS = -std=c++17 -O2

RayTrace: src/main.cpp
	g++ $(CFLAGS) -o builds/RayTrace.out src/main.cpp

.PHONY: render clean

render: RayTrace
	./builds/RayTrace.out > renders/image.ppm

clean:
	rm -f *.out *.ppm