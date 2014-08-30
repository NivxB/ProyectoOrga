#ifndef INDEXCLASS_H
#define INDEXCLASS_H


class IndexClass{
	int RRN;
	int Key;

	public:
		IndexClass(int,int);
		IndexClass();
		int getRRN();
		int getKey();
		void setRRN(int);
		void setKey(int);
		bool operator<(const IndexClass&);
		bool operator<=(const IndexClass&);
		bool operator>(const IndexClass&);
		bool operator>=(const IndexClass&);
		
};



#endif