build:
	g++ src/*.cpp -o presto
02:
	g++ -O2 src/*.cpp types.h -o presto
clean:
	rm presto
