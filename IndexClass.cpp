#include "IndexClass.h"

IndexClass::IndexClass(int K,int R){
	RRN = R;
	Key = K;
}

IndexClass::IndexClass(){
	RRN=0;
	Key=0;
}

int IndexClass::getRRN(){
	return RRN;
}

int IndexClass::getKey(){
	return Key;
}

void IndexClass::setRRN(int R){
	RRN = R;
}

void IndexClass::setKey(int K){
	Key = K;
}

bool IndexClass::operator<(const IndexClass& Compare){
	if (Key < Compare.Key)
		return true;
	return false;
}

bool IndexClass::operator>(const IndexClass& Compare){
	if (Key > Compare.Key)
		return true;
	return false;
}

bool IndexClass::operator<=(const IndexClass& Compare){
	if (Key <= Compare.Key)
		return true;
	return false;
}

bool IndexClass::operator>=(const IndexClass& Compare){
	if (Key >= Compare.Key)
		return true;
	return false;
}

bool IndexClass::operator==(const IndexClass& Compare){
	if (Key == Compare.Key)
		return true;
	return false;
}

bool IndexClass::LessThanRRN(const IndexClass& Compare){
	if (RRN < Compare.RRN)
		return true;
	return false;
}

bool IndexClass::GreatherThanRRN(const IndexClass& Compare){
	if (RRN > Compare.RRN)
		return true;
	return false;
}

bool IndexClass::LessThanEqRRN(const IndexClass& Compare){
	if (RRN <= Compare.RRN)
		return true;
	return false;
}

bool IndexClass::GreatherThanEqRRN(const IndexClass& Compare){
	if (RRN >= Compare.RRN)
		return true;
	return false;
}		

bool IndexClass::EqualRRN(const IndexClass& Compare){ 
	if (RRN == Compare.RRN)
		return true;
	return false;
}