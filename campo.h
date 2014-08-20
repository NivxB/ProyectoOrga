#ifndef CAMPO_H
#define CAMPO_H
#include <string>

using namespace std;

class campo{
public:
	campo(string, int, int, int);
	campo();
	~campo();
	void setName(string);
	void setType(int);
	void setSize(int);
	void setKey(int);//wtf

	string getName();
	int getType();
	int getSize();
	int getIsKey();
private:
	string name;
	int type;
	int size;
	int isKey;//1=yes 0=no
};

#endif