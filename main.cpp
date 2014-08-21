#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "campo.h"

using namespace std;//ayy lmao

string workingFile();
void createStructure();
void imprimirEstructura(vector<campo>);

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

int main(int argc, char* argv[]){
	bool cont = true;

	while(cont){
	int sel;
	cout<<"-------------------------\n1. Crear nueva estructura\n2. Ver registros en estructura\n3. Agregar registros\n4. Modificar\n5. Salir"<<endl;
	cin>>sel;//validar sel

	switch(sel){
		case 1:{
		createStructure();
		break;
		}
		case 2:{
		cout<<"que hueva"<<endl;
		break;
		}
		case 3:{
		string file = workingFile();
		const char * c = file.c_str();
		if(strcmp(c,"NULL")==0){
			cout<<"oyy vey schlomo mcShekels"<<endl;
		}else{
			//agregar registros
			ifstream regis(c);
			vector<vector<string> > mat;
			vector<string> buffer;

			//entradas posibles
			int enterInt;
			double enterDouble;
			char enterChar;
			string enterString;//tamaño
			////////////////////

			if(regis.is_open()){
				string line;
				getline(regis, line);
				const char * c2 = line.c_str();
				int nF = atoi (c2);//number of fields

				for(int i=0;i<nF;i++){
					getline(regis, line);
					buffer = split(line, '|');
					mat.push_back(buffer);
					buffer.clear();
				}
				regis.close();

				fstream fs;
				fs.open(c, fstream::out|fstream::app);

				for(int i=0;i<mat.size();i++){//to do: validar no meter valores repetidos si es llave
					cout<<"Ingrese el valor de "<<mat.at(i).at(0)<<" (Tipo "<<mat.at(i).at(1)<<", tamaño "<<mat.at(i).at(2)<<")"<<endl;//en lugar de int en tipo debe decir "caracter", "entero", etc
						const char * c3 = mat.at(i).at(1).c_str();//tipo
						int tipo = atoi(c3);
						//const char * csize = mat.at(i).at(2).c_str();
						//int size = atoi(csize);
						cout<<"-----------------------"<<c3<<"------------------------"<<endl;
						if(tipo==1){//char
							/*if(size==1){
								cin>>enterChar;
								getchar();
								fs<<enterChar;
							}/*else if (size>1){								
								cin>>enterString;
								enterString.resize(size, '0');
								cout<<"-----------------------"<<enterString<<"------------------------"<<endl;
								fs<<enterString;
							}*/
							//lo de arriba no funciona
							cin>>enterChar;//test, al parecer siempre entra aqui a pesar de que quiero int o double.... ffs EDIT: parece que lo arreglé, will look tomorrow
							if(i!=(mat.size()-1)){
								fs<<"|";
							}
						}else if(tipo==2){//int
							cin>>enterInt;
							fs<<enterInt;
							if(i!=(mat.size()-1)){
								fs<<"|";
							}
						}else if(tipo==3){//real
							cin>>enterDouble;
							fs<<enterDouble;
							if(i!=(mat.size()-1)){
								fs<<"|";
							}
						}else{
							cout<<"Estructura corrupta"<<endl;
						}
				}
				fs<<"\n";

				fs.close();
			}else{
				cout<<"Error abriendo la estructura"<<endl;
			}
		}
		break;
		}
		case 4:{
		cout<<"lalalala esto es bono"<<endl;
		break;
		}
		case 5:{
		cont = false;
		break;
		}
		default:
		break;
	}
}
	return 0;
}

string workingFile(){
	int nLines = 0;
	ifstream estruc("estructuras.dat");
	string line = "NULL";
	if(estruc.is_open()){
		while(getline(estruc,line)){
			nLines++;
    		cout<<nLines<<'\t'<<line<<'\n';
    	}    
	}else{
		cout<<"Error al abrir el archivo estructuras.dat"<<endl;
	}
	estruc.close();

	if(nLines>0){//hay alguna manera de hacer esto sin tener que abrir el archivo dos veces, lo se en mi corazon
		cout<<"Ingrese el numero del archivo sobre el que desea trabajar: "<<endl;
		int sel;
		cin>>sel;//validar sel
		int i=0;

		ifstream estruc2("estructuras.dat");//ni idea de por que falla si uso el mismo estruc de arriba
		if(estruc2.is_open()){
			while(i<nLines){
				getline(estruc2, line);
				i++;
			}
    	estruc2.close();
		}else{
			cout<<"Error al abrir el archivo estructuras.dat"<<endl;
		}
	}else{
		cout<<"No hay estructuras :( Cree una primero."<<endl;
	}
	return line;
}

void createStructure(){
	string fileName;//nombre archivo
	int fieldNumber;//numero de campos
	bool thereIsKey=false;
	cout<<"Ingrese el nombre de la estructura:"<<endl;
	cin>>fileName;//validar que no exista otro con el mismo nombre(?)
	cout<<"¿Cuántos campos?:"<<endl;
	cin>>fieldNumber;

	vector<campo> campos;

	for(int i=0;i<fieldNumber;i++){
		string fieldName;
		int fieldT, fieldS, isKey=0;
		char k;
		cout<<"Ingrese el nombre del campo "<<(i+1)<<endl;//validar que no hayan otros con el mismo nombre
		cin>>fieldName;
		cout<<"Ingrese el tipo:\n1: Caracter\n2: Entero\n3: Real"<<endl;//to do: real, validar
		cin>>fieldT;
		cout<<"Ingrese el tamaño:"<<endl;//validar
		cin>>fieldS;
		if(!thereIsKey){
		cout<<"¿Es llave primaria? (s/n)"<<endl;
		cin>>k;
		if(k=='s'){
			isKey = 1;
			thereIsKey=true;
		}else{
			isKey = 0;
		}
		}
		campos.push_back(campo(fieldName,fieldT, fieldS, isKey));
	}
	imprimirEstructura(campos);
	//confirmar si esta bien (s/n) n= volver al for, thereIsKey = false, vaciar vector

	fileName+=".dat";

	const char * c = fileName.c_str();

	ofstream estruc(c);

	if(estruc.is_open()){
    estruc<<campos.size()<<'\n';
    for(int i=0;i<campos.size();i++){
    	estruc<<campos.at(i).getName()<<'|'<<campos.at(i).getType()<<'|'<<campos.at(i).getSize()<<'|'<<campos.at(i).getIsKey()<<'\n';
    }
    estruc.close();
  	}
 	else cout << "No se pudo crear el archivo"<<endl;

 	fstream fs;
  	fs.open ("estructuras.dat", fstream::out|fstream::app);
  	fs << c;
  	fs << "\n";
  	fs.close();
}

void imprimirEstructura(vector<campo> v){
	cout<<"Campo\tNombre\tTipo\tLongitud\tLlave"<<endl;
	for(int i=0;i<v.size();i++){
		cout<<(i+1)<<'\t'<<v.at(i).getName()<<'\t';
		
		if(v.at(i).getType()==1){
			cout<<"Char";
		}else if(v.at(i).getType()==2){
			cout<<"Entero";
		}else if(v.at(i).getType()==3){
			cout<<"Real";
		}
		cout<<"\t"<<v.at(i).getSize()<<"\t\t";

		if(v.at(i).getIsKey()==1){
			cout<<'*';
		}
		cout<<endl;
	}
}

//ugly hacks everyfuckingwhere