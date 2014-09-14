all:
	g++ main.cpp campo.cpp IndexClass.cpp Tree.cpp TreeNode.cpp  && ./a.out
clean:
	rm *.dat *.out