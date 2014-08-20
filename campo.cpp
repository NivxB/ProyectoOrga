#include "campo.h"

campo::campo(string n, int ti, int ta, int k){
	name = n;
	type = ti;
	size = ta;
	isKey = k;
}

campo::campo(){

}

campo::~campo(){

}

void campo::setName(string s){
	name = s;
}

void campo::setType(int i){
	type = i;
}

void campo::setSize(int i){
	size = i;
}

void campo::setKey(int i){//wtf
	isKey = i;
}

string campo::getName(){
	return name;
}

int campo::getType(){
	return type;
}

int campo::getSize(){
	return size;
}

int campo::getIsKey(){
	return isKey;
}