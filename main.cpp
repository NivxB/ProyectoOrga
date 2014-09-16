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
	#include <algorithm>
	#include "campo.h"
	#include "IndexClass.h"
	#include "Tree.h"

	using namespace std;//ayy lmao

	string workingFile();
	void createStructure();
	void imprimirEstructura(vector<campo>);
	void AddRegistro();
	void DeleteRegistro();
	void ListarRegistros();
	void buscarRegistro();
	void BuscarRegistroIndice();
	void GenerarDatos();
	void Reindexar();

	vector<IndexClass> Index;
	Tree BTree(32);

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

	/**
	 * C++ version 0.4 std::string style "itoa":
	 * Contributions from Stuart Lowe, Ray-Yuan Sheu,
	 * Rodrigo de Salvo Braz, Luc Gallant, John Maloney
	 * and Brian Hunt
	 */
	std::string itoa(int value, int base) {
		
		std::string buf;
		
		// check that the base if valid
		if (base < 2 || base > 16) return buf;

		enum { kMaxDigits = 35 };
		buf.reserve( kMaxDigits ); // Pre-allocate enough space.
		
		int quotient = value;
		
		// Translating number to string with base:
		do {
			buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
			quotient /= base;
		} while ( quotient );
		
		// Append the negative sign
		if ( value < 0) buf += '-';
		
		reverse( buf.begin(), buf.end() );
		return buf;
	}

void quickSort(IndexClass* arr, int left, int right) {
      int i = left, j = right;
      IndexClass tmp;
      IndexClass pivot = arr[(left + right) / 2];
 
      /* partition */
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      };
 
      /* recursion */
      if (left < j)
            quickSort(arr, left, j);
      if (i < right)
            quickSort(arr, i, right);
}
	int main(int argc, char* argv[]){
		bool cont = true;

		while(cont){
			int sel;
			cout<<"-------------------------\n1. Crear nueva estructura\n2. Ver registros en estructura\n3. Modificar\n4. Registros\n5. Reindexar\n6. Generar Datos\n7. Salir"<<endl;
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
						cout<<"-------------------------\n1. Agregar\n2. Eliminar\n3. Buscar \n4. Retornar"<<endl;
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
								int selBuscar;
								cout<<"-------------------------\n1. Indice\n2. Sin Indice\n"<<endl;
								cin >> selBuscar;
								switch(selBuscar){
									case 1:{
										BuscarRegistroIndice();
										break;
									}
									case 2:{
										buscarRegistro();
										break;
									}
								}
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
					Reindexar();
					break;
				}
				case 6:{
					cout << "Generando 10,000 registros..." << endl;
					GenerarDatos();
					cout << "Finalizado" << endl;
					break;
				}
				case 7:{
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


		string IndexFile = "Index"+line;
		ifstream IndexW(IndexFile.c_str());
		Index.clear();

		if (IndexW.is_open()){
			string Lines;
			while(getline(IndexW,Lines)){
				vector<string> SplitLine = split(Lines,'\t');
				Index.push_back(IndexClass(atoi(SplitLine[0].c_str()),atoi(SplitLine[1].c_str())));
			}
		}

		BTree.clear();

		string BTreeFile = "BTree"+line;
		ifstream BTreeF(BTreeFile.c_str());

		// Suponiendo que la primera linea siempre sea la root

		if (BTreeF.is_open()){
			string Line;
			while (getline(BTreeF,Line)){
				/*
				Estructura ?
				#Keys \t Key1|Key2|Key3 \t Pagina1|Pagina2|Pagina3
				*/
				//vector<string> buffer = split(Line,'\t');
				//vector<string> Keys = split(line,'|');
				//cout<<"NK "<< Keys[1] << endl;
				//vector<string> Pages = split(buffer[2],'|');

				//for (int i = 0 ; i<Keys.size() ; i++)
					BTree.insertar(atoi(Line.c_str()));
			}

			//BTree.recorrerInorden();
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
		string IndexFile = "Index"+fileName;
		string ArbolFile = "BTree"+fileName;

		const char * c = fileName.c_str();

		ofstream estruc(c);

		ofstream IndexEstruc(IndexFile.c_str());
		IndexEstruc.close();

		ofstream AFile(ArbolFile.c_str());
		AFile.close();

		if(estruc.is_open()){
			estruc<<"000-1$"<<campos.size()<<"$"<<LongitudRegistro<<"$\n";
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
		IndexClass NewIndexL(0,0);
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

				string NewAvail = "000-1";
				fstream fs;
				FILE* OutAvail;
				string line;
				getline(regis, line);
				Header1 = split(line,'$');
				const char * CAvailList = Header1.at(0).c_str();
				bool Avail = false;
				if (strcmp(CAvailList,"000-1"))
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
					OutAvail = fopen(c,"r+");
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
									cout << Campos[i][3] << endl;
									if(!strcmp(Campos[i][3].c_str(),"1")){
										NewIndexL.setKey(enterInt);
										BTree.insertar(enterInt);
									}
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

					int Pos = fs.tellg();
					int NewRRN = ((Pos - HeaderSize)/LongitudRegistro)-1;
					NewIndexL.setRRN(NewRRN);

					//cout <<"Set RRN "<<NewRRN<<endl;

					fs.close();
				}else{
					NewIndexL.setRRN(AvailNum);

					char NewAvail[5];
					fseek(OutAvail,(AvailNum*LongitudRegistro)+HeaderSize,SEEK_SET);
					fgetc(OutAvail);
					for(int i = 0 ; i < 5 ; i++)
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
				Index.push_back(NewIndexL);
				quickSort(&Index[0],0,Index.size()-1);


				//Reescribiendo Indice
				string IndexW = "Index"+file;
				ofstream NewIndex(IndexW.c_str());
				for (int i = 0;i<Index.size();i++)
					NewIndex<<Index[i].getKey()<<"\t"<<Index[i].getRRN()<<"\n";

				//Reescribiendo Arbol
				string ArbolW = "BTree"+file;
				ofstream NewArbol(ArbolW.c_str());
				BTree.write(NewArbol);
				NewArbol.close();

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
			string AvailList = "000-1";
			int LongitudRegistro = 0;
			
			vector<IndexClass> ActualIndex;

			if(regis.is_open()){
				cout<< "ini " << regis.tellg()<<endl;
			
				string line;
				getline(regis, line);
				cout<< "1 " << regis.tellg()<<endl;
				vector<string> tempHeader = split(line,'$');
				AvailList = tempHeader[0];

				LongitudRegistro = atoi(tempHeader[2].c_str());
				getline(regis,line);
				cout<< "2 " << regis.tellg()<<endl;

				vector<string> tempEstructura = split(line,'$');
				for (int i = 0; i<tempEstructura.size();i++){
					vector<string> buffer = split(tempEstructura.at(i),'|');

				}

				HeaderSize = regis.tellg();
			}

			string ReadLine;
			int i = 0,Regis=0;
			vector<int> Registros;
			while(getline(regis,ReadLine)){
				vector<string> buffer;
				buffer = (split(ReadLine,'|'));
				const char * css = buffer.at(0).c_str();
				if (ReadLine[0] != '*'){
					Registros.push_back(i);
					cout<<Regis<<")\t";
					for (int j = 0;j<buffer.size()-1;j++)
						cout<<buffer[j]<<"\t";

					cout<<"\n";
					Regis++;
				}
				i++;
			}
			regis.close();
			

			//ofstream RegisO;
			//RegisO.open(c,ofstream::app);
			int Selection;
			cout<<"Ingrese registro a eliminar"<<endl;
			cin>>Selection;//validar
			
			

			Selection = Registros[Selection];
			stringstream SSNewAvail;
			SSNewAvail << Selection;
			string SNewAvail = SSNewAvail.str();
			//Selection = atoi(SNewAvail.c_str());
			
			
			//Index.erase(KEY);
			//BTree.remove(KEY)

			string NewAvail;
			if (SNewAvail.size() < 2)
				NewAvail = "0000"+SNewAvail;
			else if (SNewAvail.size() < 3)
				NewAvail = "000"+SNewAvail;
			else if(SNewAvail.size()< 4)
				NewAvail = "00"+SNewAvail;
			else if(SNewAvail.size()< 5)
				NewAvail = "0"+SNewAvail;
			else
				NewAvail = SNewAvail;
			
				
			//LONGITUD REGISTRO (SUMA DE LONGITUD INDIVIDUAL + CAMPOS(por los |))
			
			
			//RegisO.seekp((Selection*LongitudRegistro)+HeaderSize);
			string Deleted = "*"+AvailList;
			//RegisO.write(Deleted.c_str(),Deleted.size());
			FILE* OutF = fopen(c,"r+");
			fseek(OutF,(Selection*LongitudRegistro)+HeaderSize,SEEK_SET);
			
			//cout << Selection <<" "<< LongitudRegistro <<" "<<HeaderSize << endl;
			
			fputs(Deleted.c_str(),OutF);

			fseek(OutF,0,SEEK_SET);
			fputs(NewAvail.c_str(),OutF);
			fclose(OutF);

			string IndexW = "Index"+file;
			ofstream NewIndex(IndexW.c_str());
			for (int i = 0;i<Index.size();i++)
				NewIndex<<Index[i].getKey()<<"\t"<<Index[i].getRRN()<<"\n";
			
		}
	}

	void buscarRegistro(){
		string file = workingFile();
		const char * c = file.c_str();
		if(strcmp(c,"NULL")==0){
			cout<<"oyy vey schlomo mcShekels"<<endl;
		}else{
			ifstream regis(c);
			if(regis.is_open()){
				string line, buscar;
				int i=0;//linea donde se encuentra la cadena
				getline(regis, line);
				getline(regis, line);
				cout<<"Ingrese el valor del campo que desea buscar:"<<endl;
				cin>>buscar;
				const char* b = buscar.c_str();
				if(strcmp(b, "")==0){
					cout<<"Ese no es un valor válido"<<endl;
				}else{
					bool encontrado = false;
					while(getline(regis, line)){
					i++;
					size_t found = line.find(buscar);
					if (found!=string::npos){
						cout<<buscar<<" encontrado en linea "<<i<<" posición "<<found<<endl;
						encontrado = true;
					}
					}
					if(!encontrado){
						cout<<"No se encontró"<<endl;
					}
				}
			}else{
				cout<<"Error abriendo archivo "<<file<<endl;
			}
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
			int i = 0,Regis = 0;
			while(getline(regis,ReadLine)){
				vector<string> buffer = split(ReadLine,'|');
				const char * css = buffer.at(0).c_str();
				if (ReadLine[0] != '*'){
					cout<<Regis<<")\t";
					for (int j = 0;j<buffer.size();j++)
						cout<<buffer[j]<<"\t";
					cout<<"\n";
					Regis++;
				}
				i++;
			}
		}
	}


	void BuscarRegistroIndice(){
		string file = workingFile();
		const char * c = file.c_str();
		string buscar;
		int POSEncontrado = -1,LongitudRegistro = 0,HeaderSize = 0;
		if(strcmp(c,"NULL")==0){
			cout<<"oyy vey schlomo mcShekels"<<endl;
		}else{
			ifstream regis(c);
			int HeaderSize = 0;
			int LongitudRegistro = 0;
			
			if(regis.is_open()){
				string line;
				getline(regis, line);
				vector<string> tempHeader = split(line,'$');
				LongitudRegistro = atoi(tempHeader[2].c_str());
				
				getline(regis,line);
				HeaderSize = regis.tellg();
				regis.close();
			}
			cout<<"Ingrese el valor del campo que desea buscar:"<<endl;
			cin>>buscar;
			const char* b = buscar.c_str();
			if(strcmp(b, "")==0){
				cout<<"Ese no es un valor válido"<<endl;
			}else{
				for (int i = 0;i<Index.size();i++){
					if (Index[i].getKey() == atoi(b)){
						POSEncontrado = i;
						break;
					}
				}
			}

			if (POSEncontrado != -1){
				FILE* SearchFile = fopen(c,"r+");
				fseek(SearchFile,(POSEncontrado*LongitudRegistro)+HeaderSize,SEEK_SET);
				char RegisFind[LongitudRegistro];
				fread(RegisFind,1,LongitudRegistro,SearchFile);
				cout << RegisFind <<endl;
				fclose(SearchFile);
			}

		}	
	}

	void GenerarDatos(){
		string file = workingFile();
		srand (time(NULL));

		const char * c = file.c_str();
		if(strcmp(c,"NULL")==0){
			cout<<"oyy vey schlomo mcShekels"<<endl;
		}else{
			ifstream regis(c);
			vector<string> Header1;
			vector<vector<string> > Campos;
			vector<string> buffer;
			int FieldNum = 0;
			
			if(regis.is_open()){			
				fstream fs;
				string line;
				getline(regis, line);
				Header1 = split(line,'$');
				FieldNum = atoi(Header1[1].c_str());
				getline(regis,line);
				vector<string> tempCampos = split(line,'$');
				for (int i = 0; i<tempCampos.size();i++){
					buffer = split(tempCampos.at(i),'|');
					Campos.push_back(buffer);
				}			
				regis.close();
			}

			ofstream OutF;
			OutF.open(c,ofstream::app);
			for(int i = 0 ;i<10000;i++){
				stringstream StringOutFile;
				
				for (int j = 0; j<FieldNum;j++){
					if (!strcmp(Campos[j][1].c_str(),"1")){
						int Input = 0;
						int Long = atoi(Campos[j][2].c_str());
						for (int k = 0 ;k<Long;k++){
							Input = rand() % 94 + 32;
							if (Input == 124)
								Input--;
							StringOutFile<< (char)Input;
						}
					}else if (!strcmp(Campos[j][1].c_str(),"2")){
						int Input = 0;
						int Long = atoi(Campos[j][2].c_str());
						int MOD = (pow(10.0,Long));
						Input = rand() % MOD;
						StringOutFile<< setw(Long) << setfill('0') << Input;

						if (!strcmp(Campos[j][3].c_str(),"1")){
							Index.push_back(IndexClass(Input,i));
							BTree.insertar(Input);
						}

					}
					StringOutFile<<"|";
				}
				StringOutFile<<"\n";
				OutF<<StringOutFile.str();
			}
			OutF.close();
			quickSort(&Index[0],0,Index.size()-1);

			string IndexW = "Index"+file;
			ofstream NewIndex(IndexW.c_str());
			for (int i = 0;i<Index.size();i++)
				NewIndex<<Index[i].getKey()<<"\t"<<Index[i].getRRN()<<"\n";

			string AFile = "BTree"+file;
			ofstream NewArbol(AFile.c_str());
			BTree.write(NewArbol);

		}		
	}

	void Reindexar(){
		string file = workingFile();
		Index.clear();
		ifstream regis;
		const char * c = file.c_str();

		int KeyCampo = -1;
		if(strcmp(c,"NULL")==0){
			cout<<"oyy vey schlomo mcShekels"<<endl;
		}else{
			regis.open(c);
			vector<vector<string> > Campos;
			string line;
			
			if(regis.is_open()){
				getline(regis, line);
				getline(regis,line);
				vector<string> tempCampos = split(line,'$');
				for (int i = 0; i<tempCampos.size();i++){
					vector<string> buffer = split(tempCampos.at(i),'|');
					Campos.push_back(buffer);
				}			
			}

			for (int i = 0 ; i< Campos.size() ; i++){
				if (!strcmp(Campos[i][3].c_str(),"1")){
					KeyCampo = i;
					break;
				}
			}

			int RRN = 0;
			while(getline(regis,line)){
				if (!(line[0] == '*')){
					vector<string> buffer = split(line,'|');
					Index.push_back(IndexClass(atoi(buffer[KeyCampo].c_str()),RRN));
				}
				RRN++;
			}

			regis.close();
			quickSort(&Index[0],0,Index.size()-1);

			string IndexW = "Index"+file;
			ofstream NewIndex(IndexW.c_str());
			for (int i = 0;i<Index.size();i++)
				NewIndex<<Index[i].getKey()<<"\t"<<Index[i].getRRN()<<"\n";
		}
	}
	//ugly hacks everyfuckingwhere
