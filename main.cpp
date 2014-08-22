#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iomanip>
#include "campo.h"

using namespace std;//ayy lmao

string workingFile();
void createStructure();
void imprimirEstructura(vector<campo>);
void AddRegistro();
void DeleteRegistro();
void ListarRegistros();

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
		cout<<"-------------------------\n1. Crear nueva estructura\n2. Ver registros en estructura\n3. Modificar\n4. Registros\n5. Salir"<<endl;
		cin>>sel;//validar sel

		switch(sel){
			case 1:{
				createStructure();
				break;
			}
			case 2:{
				ListarRegistros();
				break;
			}
			case 4:{
				bool RegistroSelect = true;
				while (RegistroSelect){
					int SelectionRegistro;
					cout<<"-------------------------\n1. Agregar\n2. Eliminar\n3. ?????\n4. Retornar"<<endl;
					cin>>SelectionRegistro;
					switch(SelectionRegistro){
						case 1:{
							AddRegistro();
							break;
						}
						case 2:{
							DeleteRegistro();
							break;
						}
						case 3:{
							cout<<"profit"<<endl;
							break;
						}			
						case 4:{
							RegistroSelect = false;
							break;

						}
					}
				}
				break;
			}
			case 3:{
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
	int LongitudRegistro = 0;
	bool thereIsKey=false;
	cout<<"Ingrese el nombre de la estructura:"<<endl;
	cin>>fileName;//validar que no exista otro con el mismo nombre(?)
	cout<<"¿Cuántos campos?:"<<endl;
	cin>>fieldNumber;
	LongitudRegistro += fieldNumber+1;
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
		LongitudRegistro+=fieldS;
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
		estruc<<"0-1$"<<campos.size()<<"$"<<LongitudRegistro<<"$\n";
		for(int i=0;i<campos.size();i++){
			estruc<<campos.at(i).getName()<<'|'<<campos.at(i).getType()<<'|'<<campos.at(i).getSize()<<'|'<<campos.at(i).getIsKey()<<"$";
		}
		estruc<<"\n";
		estruc.close();
  	}
 	else 
		cout << "No se pudo crear el archivo"<<endl;

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

void AddRegistro(){
	string file = workingFile();
	const char * c = file.c_str();
	if(strcmp(c,"NULL")==0){
		cout<<"oyy vey schlomo mcShekels"<<endl;
	}else{
		//agregar registros
		ifstream regis(c);
		vector<string> Header1;
		vector<vector<string> > Campos;
		vector<string> buffer;

		//entradas posibles
		int enterInt;
		double enterDouble;
		//char enterChar;
		string enterString;//tamaño
		////////////////////

		if(regis.is_open()){
			stringstream StringOutFile;

			string NewAvail = "0-1";
			fstream fs;
			FILE* OutAvail;
			string line;
			getline(regis, line);
			Header1 = split(line,'$');
			const char * CAvailList = Header1.at(0).c_str();
			bool Avail = false;
			if (strcmp(CAvailList,"0-1"))
				Avail = true;
			const char * CNumeroCampos = Header1.at(1).c_str();
			int LongitudRegistro = atoi(Header1.at(2).c_str());
			
			int NumeroCampos = atoi (CNumeroCampos);
			int AvailNum = atoi(CAvailList);

			//cout <<"AvailNum "<<AvailNum<<endl;
			
			string LineCampos;
			getline(regis,LineCampos);
			int HeaderSize = regis.tellg();

			vector<string> tempCampos = split(LineCampos,'$');
			for (int i = 0; i<tempCampos.size();i++){
				buffer = split(tempCampos.at(i),'|');
				Campos.push_back(buffer);
			}
			
			regis.close();
			
			if (!Avail){
				fs.open(c, fstream::out|fstream::app);
			}else{
				//char NewAvail[3];
				OutAvail = fopen(c,"r+");
				//fseek(OutAvail,(AvailNum*LongitudRegistro)+HeaderSize,SEEK_SET);
				//fgetc(OutAvail);
				//for(int i = 0 ; i < 3 ; i++)
				//	NewAvail[i] = fgetc(OutAvail);
				//fseek(OutAvail,0,SEEK_SET);
				//fputs(NewAvail,OutAvail);
			}

			for(int i=0;i<Campos.size();i++){//to do: validar no meter valores repetidos si es llave
				const char * c3 = Campos.at(i).at(1).c_str();//tipo
				int tipo = atoi(c3);
				cout<<"Ingrese el valor de "<<Campos.at(i).at(0);
				if(tipo==1){
					cout<<" (Caracter";
				}else if(tipo==2){
					cout<<" (Entero";
				}else if(tipo==3){
					cout<<" (Real";
				}else{
					cout<<"Estructura corrupta"<<endl;
					break;
				}

				cout<<", tamaño "<<Campos.at(i).at(2)<<")"<<endl;//en lugar de int en tipo debe decir "caracter", "entero", etc
						
					const char * csize = Campos.at(i).at(2).c_str();
					int size = atoi(csize);
					if(tipo==1){//char
						while(true){
						cin>>enterString;
						if(enterString.size()>size){
							cout<<"Ha ingresado un caracter de mayor tamaño. Por favor ingrese uno de tamaño "<<size<<" o menor."<<endl;
						}else{
							enterString.resize(size, ' ');
							break;
						}
						}
						if (!Avail){
							fs<<enterString;
							
							fs<<"|";
						}else{
							StringOutFile<<enterString<<"|";
							//fseek()
						}
					}else if(tipo==2){//int
						while(true){
							cin>>enterInt;
							if(enterInt>=(pow(10, size))){
								cout<<"Ha ingresado un entero de mayor tamaño. Por favor ingrese un entero de tamaño "<<size<<" o menor."<<endl;
							}else if(enterInt<1){
								cout<<"Ha ingresado un entero negativo. Por gavor solo ingrese valores positivos"<<endl;
							}else{
								break;
							}
						}
						if (!Avail){
							fs<< setw(size) << setfill('0') << enterInt;
						//fs<<enterInt;
							fs<<"|";
						}else{
							StringOutFile<<setw(size) << setfill('0') << enterInt <<"|";
						}
					}else if(tipo==3){//real//VALIDAR TODO ESTO
						cin>>enterDouble;
						fs<<enterDouble;
						if(i!=(Campos.size()-1)){
							fs<<"|";
						}
					}else{
						cout<<"Estructura corrupta"<<endl;
						break;
					}
			}
			if (!Avail){
				fs<<"\n";

				fs.close();
			}else{
				char NewAvail[3];
				fseek(OutAvail,(AvailNum*LongitudRegistro)+HeaderSize,SEEK_SET);
				fgetc(OutAvail);
				for(int i = 0 ; i < 3 ; i++)
					NewAvail[i] = fgetc(OutAvail);
				fseek(OutAvail,0,SEEK_SET);
				fputs(NewAvail,OutAvail);

				fclose(OutAvail);
				OutAvail = fopen(c,"r+");
				fseek(OutAvail,(AvailNum*LongitudRegistro)+HeaderSize,SEEK_SET);
				//cout << StringOutFile.str() << endl;
				fputs(StringOutFile.str().c_str(),OutAvail);
				fclose(OutAvail);
			}
		}else{
			cout<<"Error abriendo la estructura"<<endl;
		}
	}
}

void DeleteRegistro(){
	string file = workingFile();
	const char * c = file.c_str();
	if(strcmp(c,"NULL")==0){
		cout<<"oyy vey schlomo mcShekels"<<endl;
	}else{
	
		ifstream regis(c);
		vector<string> Header1;
		int HeaderSize = 0;
		string AvailList = "0-1";
		int LongitudRegistro = 0;
		
		if(regis.is_open()){
			string line;
			getline(regis, line);
			vector<string> tempHeader = split(line,'$');
			AvailList = tempHeader[0];

			LongitudRegistro = atoi(tempHeader[2].c_str());
			getline(regis,line);

			vector<string> tempEstructura = split(line,'$');
			for (int i = 0; i<tempEstructura.size();i++){
				vector<string> buffer = split(tempEstructura.at(i),'|');
			}

			HeaderSize = regis.tellg();
		}

		string ReadLine;
		int i = 0;
		vector<vector<string> > Registros;
		while(getline(regis,ReadLine)){
			char numstr[21]; // enough to hold all numbers up to 64-bits
			snprintf(numstr, sizeof(numstr), "%d", i);
			
			ReadLine += numstr;
			
			vector<string> buffer = split(ReadLine,'|');
			const char * css = buffer.at(0).c_str();
			if (ReadLine[0] != '*'){
				Registros.push_back(buffer);
				cout<<i<<")\t";
				for (int j = 0;j<buffer.size()-1;j++)
					cout<<buffer[j]<<"\t";
				cout<<"\n";
			}
			i++;
		}
		regis.close();
		

		//ofstream RegisO;
		//RegisO.open(c,ofstream::app);
		int Selection;
		cout<<"Ingrese registro a eliminar"<<endl;
		cin>>Selection;//validar
		
		string SNewAvail = Registros.at(Selection).at(Registros.at(Selection).size()-1);
		Selection = atoi(SNewAvail.c_str());
		string NewAvail;
		if (SNewAvail.size() < 10)
			NewAvail = "00"+SNewAvail;
		else if (SNewAvail.size() < 100)
			NewAvail = "0";
		else
			NewAvail = SNewAvail;
		
			
		//LONGITUD REGISTRO (SUMA DE LONGITUD INDIVIDUAL + CAMPOS(por los |))
		
		
		//RegisO.seekp((Selection*LongitudRegistro)+HeaderSize);
		string Deleted = "*"+AvailList;
		//RegisO.write(Deleted.c_str(),Deleted.size());
		FILE* OutF = fopen(c,"r+");
		fseek(OutF,(Selection*LongitudRegistro)+HeaderSize,SEEK_SET);
		fputs(Deleted.c_str(),OutF);

		fseek(OutF,0,SEEK_SET);
		fputs(NewAvail.c_str(),OutF);
		fclose(OutF);
		
	}
}

void ListarRegistros(){
	string file = workingFile();
	const char * c = file.c_str();
	if(strcmp(c,"NULL")==0){
		cout<<"oyy vey schlomo mcShekels"<<endl;
	}else{
		//agregar registros
		ifstream regis(c);
		vector<string> Header1;
		if(regis.is_open()){
			string line;
			getline(regis, line);
			/*
			Header1 = split(line,'$');
			const char * CAvailList = Header1.at(0).c_str();
			const char * CNumeroCampos = Header1.at(1).c_str();
			
			int AvailList = atoi (CAvailList);
			int NumeroCampos = atoi (CNumeroCampos);
			
			string LineCampos;
			*/
			getline(regis,line);
			
			vector<string> tempCampos = split(line,'$');
			cout<<"--\t";
			for (int i = 0; i<tempCampos.size();i++){
				vector<string> buffer = split(tempCampos.at(i),'|');
				cout<<buffer[0]<<"\t";
			}
			cout<<endl;
			
		}

		//Para la impresion bonita?
		string ReadLine;
		int i = 0;
		while(getline(regis,ReadLine)){
			vector<string> buffer = split(ReadLine,'|');
			const char * css = buffer.at(0).c_str();
			if (ReadLine[0] != '*'){
				cout<<i<<")\t";
				for (int j = 0;j<buffer.size();j++)
					cout<<buffer[j]<<"\t";
				cout<<"\n";
			}
			i++;
		}
	}
}
//ugly hacks everyfuckingwhere