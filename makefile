all:
	g++ main.cpp campo.cpp IndexClass.cpp  && ./a.out
clean:
	rm *.dat *.out