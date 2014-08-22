all:
	g++ main.cpp campo.cpp -o Final && ./Final
clean:
	rm *.dat ./Final