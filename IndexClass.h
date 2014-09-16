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

		bool LessThanRRN(const IndexClass& Compare);
		bool GreatherThanRRN(const IndexClass& Compare);

		bool LessThanEqRRN(const IndexClass& Compare);
		bool GreatherThanEqRRN(const IndexClass& Compare);		

		bool EqualRRN(const IndexClass& Compare);

		bool operator<(const IndexClass&);
		bool operator<=(const IndexClass&);
		bool operator>(const IndexClass&);
		bool operator>=(const IndexClass&);
		bool operator==(const IndexClass&);
};



#endif