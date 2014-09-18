	#include <iostream>
	#include <fstream>
	#include <cstring>
	#include <string>
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
	void Compactar();
	void createStructure();
	void imprimirEstructura(vector<campo>);
	void AddRegistro();
	void DeleteRegistro();
	void ListarRegistros();
	void buscarRegistro();
	void BuscarRegistroIndice();
	void buscarRegistroArbol();
	void GenerarDatos();
	void Reindexar();
	void cruzarIndices();
	void cruzarArboles();
	void cruzarDURO();
	int validarEntradaInt();
	int validarEntradaInt(int opc);
	void LoadTree(string FileName,Tree*);

	int binary_search(IndexClass[],int ,int , IndexClass ,int );

	vector<IndexClass> Index;
	//Tree BTree(32);

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


	bool COMPARAR(string S1,string S2){
		if (S1.length() != S2.length())
			return false;
		const char* ALGO1 = S1.c_str();
		const char* ALGO2 = S2.c_str();
		
		for (int i = 0; i<S1.length() ; i++){
			if (ALGO1[i] != ALGO2[i])
				return false;
		}


		return true;	
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
			cout<<"-------------------------\n1. Crear nueva estructura\n2. Ver registros en estructura\n3. Modificar\n4. Registros\n5. Reindexar\n6. Cruzar\n7. Generar Datos\n8. Compactar\n9. Salir"<<endl;
			sel = validarEntradaInt(9);//validar sel

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
						SelectionRegistro= validarEntradaInt(4);
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
								selBuscar=validarEntradaInt(2);
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
				case 7:{
					cout << "Generando 10,000 registros..." << endl;
					GenerarDatos();
					cout << "Finalizado" << endl;
					break;
				}
				case 9:{
					cont = false;
					break;
				}
				case 8:{
					Compactar();
					break;
				}
				case 6:{
					int selBuscar;
					cout<<"-------------------------\n1. Indice\n2. Arbol\n3. Duro\n"<<endl;
					selBuscar=validarEntradaInt(3);
					switch(selBuscar){
						case 1:{
							cruzarIndices();
							break;
						}
						case 2:{
							cruzarArboles();
							break;
						}
						case 3:{
							cruzarDURO();
						}
					}
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
		vector<string> Estructura ;
		ifstream estruc("estructuras.dat");
		string line = "NULL";
		if(estruc.is_open()){
			while(getline(estruc,line)){
				nLines++;
	    		cout<<nLines<<'\t'<<line<<'\n';
	    		Estructura.push_back(line);
	    	}    
		}else{
			cout<<"Error al abrir el archivo estructuras.dat"<<endl;
		}
		estruc.close();

		if(nLines>0){//hay alguna manera de hacer esto sin tener que abrir el archivo dos veces, lo se en mi corazon
			cout<<"Ingrese el numero del archivo sobre el que desea trabajar: "<<endl;
			int sel;
			sel=validarEntradaInt(nLines);//validar sel
			
			line = Estructura[sel - 1];

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
		fieldNumber = validarEntradaInt();//numero de campos
		LongitudRegistro += fieldNumber+1;
		vector<campo> campos;

		for(int i=0;i<fieldNumber;i++){
			string fieldName;
			int isKey=0;
			char k;
			cout<<"Ingrese el nombre del campo "<<(i+1)<<endl;//validar que no hayan otros con el mismo nombre
			cin>>fieldName;
			cout<<"Ingrese el tipo:\n1: Caracter\n2: Entero\n3: Real"<<endl;//to do: real, validar
			int fieldT = validarEntradaInt(3);
			cout<<"Ingrese el tamaño:"<<endl;//validar
			int fieldS = validarEntradaInt(200);
			LongitudRegistro+=fieldS;
			if(!thereIsKey&&fieldT==2){
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
		Tree BTree(32);
		LoadTree(file,&BTree);
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
								enterInt = validarEntradaInt();
								if(enterInt>=(pow(10, size))){
									cout<<"Ha ingresado un entero de mayor tamaño. Por favor ingrese un entero de tamaño "<<size<<" o menor."<<endl;
								}else if(enterInt<1){
									cout<<"Ha ingresado un entero negativo. Por gavor solo ingrese valores positivos"<<endl;
								}else{
									cout << Campos[i][3] << endl;
									if(!strcmp(Campos[i][3].c_str(),"1")){
										int dontWrite = binary_search(&Index[0],0,Index.size(),IndexClass(enterInt,-1),1); 
										if(dontWrite != -1){
											cout<<"La llave primaria ya se encuentra"<<endl;
											i=Campos.size();
										}else{
											NewIndexL.setKey(enterInt);
											BTree.insertar(enterInt);
										}
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

					char NewAvail[6];
					fseek(OutAvail,(AvailNum*LongitudRegistro)+HeaderSize,SEEK_SET);
					fgetc(OutAvail);
					for(int i = 0 ; i < 5 ; i++)
						NewAvail[i] = fgetc(OutAvail);
					NewAvail[6] = '$';
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
		Tree BTree(32);
		LoadTree(file,&BTree);
		int KeyCampo = 0;
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
				//cout<< "1 " << regis.tellg()<<endl;
				vector<string> tempHeader = split(line,'$');
				AvailList = tempHeader[0];

				LongitudRegistro = atoi(tempHeader[2].c_str());
				getline(regis,line);
				//cout<< "2 " << regis.tellg()<<endl;

				vector<string> tempEstructura = split(line,'$');
				for (int i = 0; i<tempEstructura.size();i++){
					vector<string> buffer = split(tempEstructura.at(i),'|');
					if (!strcmp(buffer[3].c_str(),"1")){
						KeyCampo = i;
						break;
					}
				}

				HeaderSize = regis.tellg();
			}

			string ReadLine;
			int i = 0,Regis=0;
			vector<int> Registros;
			while(getline(regis,ReadLine)){
				vector<string> buffer;
				buffer = (split(ReadLine,'|'));
				//const char * css = buffer.at(0).c_str();
				if (ReadLine[0] != '*'){
					Registros.push_back(i);
					cout<<Regis<<")\t";
					for (int j = 0;j<buffer.size();j++){
						cout<<buffer[j]<<"\t";
					}

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

			int POSEncontrado = binary_search(&Index[0],0,Index.size(),IndexClass(-1,Selection),0);

			int KeyEliminar = Index[POSEncontrado].getKey();


			BTree.remove(Index[KeyEliminar].getKey());
			Index.erase(Index.begin() + KeyEliminar);

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
			
			string BTreeW = "BTree"+file;
			ofstream NewBTree(BTreeW.c_str());
			BTree.write(NewBTree);


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
				POSEncontrado = binary_search(&Index[0],0,Index.size(),IndexClass(atoi(b),-1),1);
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

		Tree BTree(32);
		LoadTree(file,&BTree);

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

			vector<vector<int> > vec;


			for (int j = 0; j<FieldNum;j++){
				if (!strcmp(Campos[j][1].c_str(),"2")){
					int Long = atoi(Campos[j][2].c_str());
					int MOD = (pow(10.0,Long));
					
					vector<int> v;
					for(int i=0;i<10000;i++){
						v.push_back(i%MOD);
					}

					random_shuffle(v.begin(),v.end());
					vec.push_back(v);
				}
			}			

			for(int i = 0 ;i<10000;i++){
				stringstream StringOutFile;
				int h=0;
				
				for (int j = 0; j<FieldNum;j++){
					if (!strcmp(Campos[j][1].c_str(),"1")){
						h++;
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
						//Input = rand() % MOD;
						Input = vec.at(j-h).back();
						
						vec.at(j-h).pop_back();

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

	int binary_search(IndexClass array[],int first,int last, IndexClass search_key,int TYPE) // 1 - KEY 0 - RRN
	{
		int index = -1;
		//cout << "FIS "<< first << endl;
		//cout << "SEC "<< last << endl;
		if (TYPE == 1){
			if (first > last)
				index = -1;
			else{
				int mid = (first + last)/2;
				//cout << "MID "<< mid << endl;
			 	if (search_key == array[mid])
			 		index = mid;
			 	else
					if (search_key < array[mid])
			 			index = binary_search(array,first, mid-1, search_key,TYPE);
					else
						index = binary_search(array, mid+1, last, search_key,TYPE);
			} // end if
		}else{
			if (first > last)
				index = -1;
			else{
				int mid = (first + last)/2;
			 	if (search_key.EqualRRN(array[mid]))
			 		index = mid;
			 	else
					if (search_key.LessThanRRN(array[mid]))
			 			index = binary_search(array,first, mid-1, search_key,TYPE);
					else
						index = binary_search(array, mid+1, last, search_key,TYPE);
			} // end if
		}
		return index;
	}// end binarySearch

	int validarEntradaInt(){
		string input;
		cin>>input;
		const char * c = input.c_str();
		return atoi(c);
	}

	int validarEntradaInt(int opc){//bound superior
		string input;
		bool cont=true;
		const char * c;	
		
		while(cont){
			cin>>input;
			c = input.c_str();

			if(atoi(c)>opc||atoi(c)==0){
				cout<<"Por favor ingrese un valor válido"<<endl;
			}else{
				cont = false;
			}
		}
		return atoi(c);
	}

	void cruzarIndices(){
		string file = workingFile();
		vector<IndexClass> Index2;
		for(int i=0;i<Index.size();i++){
			Index2.push_back(Index.at(i));
		}

		string file2 = workingFile();

		int encontrado=-1, encontrado2 = -1;
		const char* c = file.c_str();
		const char* c2 = file2.c_str();
		ifstream regis(c);
		ifstream regis2(c2);
		int HeaderSize = 0;
		int LongitudRegistro = 0;
		int HeaderSize2 = 0;
		int LongitudRegistro2 = 0;

		if(regis.is_open()){
				string line;
				getline(regis, line);
				vector<string> tempHeader = split(line,'$');
				LongitudRegistro = atoi(tempHeader[2].c_str());

				getline(regis,line);
				HeaderSize = regis.tellg();
				regis.close();
		}


		if(regis2.is_open()){
				string line;
				getline(regis2, line);
				vector<string> tempHeader = split(line,'$');
				LongitudRegistro2 = atoi(tempHeader[2].c_str());
				
				getline(regis2,line);
				HeaderSize2 = regis2.tellg();
				regis2.close();
		}


		for(int i=0;i<Index.size();i++){
			encontrado2 = binary_search(&Index2[0],0,Index2.size(),Index.at(i),1);
			//encontrado = binary_search(&Index[0],0,Index.size(),Index.at(i),1);

			if (encontrado2 != -1){
				
				FILE* SearchFile = fopen(c,"r+");
				fseek(SearchFile,(Index2[encontrado2].getRRN()*LongitudRegistro)+HeaderSize,SEEK_SET);
				char RegisFind[LongitudRegistro];
				fread(RegisFind,1,LongitudRegistro,SearchFile);

				//RegisFind[LongitudRegistro - 1] = ' '

				//cout << RegisFind << "ESPACIO ";

				for (int i = 0 ; i<LongitudRegistro-1 ; i++)
					cout<<RegisFind[i];
				cout << endl;

				fclose(SearchFile);
				FILE* SearchFile2 = fopen(c2,"r+");
				fseek(SearchFile2,(Index[i].getRRN()*LongitudRegistro2)+HeaderSize2,SEEK_SET);
				char RegisFind2[LongitudRegistro2];
				fread(RegisFind2,1,LongitudRegistro2,SearchFile2);
				//cout << RegisFind2<< endl;
				
				for (int i = 0 ; i<LongitudRegistro2-1 ; i++)
					cout<<RegisFind2[i];
				cout << endl<<endl;

				fclose(SearchFile2);
				encontrado2=-1;
			}
		}
	}

	void LoadTree(string FileName,Tree* BTree){
		string BTreeFile = "BTree"+FileName;
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
					BTree->insertar(atoi(Line.c_str()));
			}
		}
	}

	void cruzarArboles(){		
		string file = workingFile();
		vector<IndexClass> Index2;
		for(int i=0;i<Index.size();i++){
			Index2.push_back(Index.at(i));
		}

		string file2 = workingFile();

		int encontrado=-1, encontrado2 = -1;
		const char* c = file.c_str();
		const char* c2 = file2.c_str();
		ifstream regis(c);
		ifstream regis2(c2);
		int HeaderSize = 0;
		int LongitudRegistro = 0;
		int HeaderSize2 = 0;
		int LongitudRegistro2 = 0;

		if(regis.is_open()){
				string line;
				getline(regis, line);
				vector<string> tempHeader = split(line,'$');
				LongitudRegistro = atoi(tempHeader[2].c_str());
				
				getline(regis,line);
				HeaderSize = regis.tellg();
				regis.close();
		}


		if(regis2.is_open()){
				string line;
				getline(regis2, line);
				vector<string> tempHeader = split(line,'$');
				LongitudRegistro2 = atoi(tempHeader[2].c_str());
				
				getline(regis2,line);
				HeaderSize2 = regis2.tellg();
				regis2.close();
		}


		Tree BTree_1(32);
		Tree BTree_2(32);

		LoadTree(c,&BTree_1);
		LoadTree(c2,&BTree_2);

		for(int i=0;i<Index.size();i++){
			
			if (BTree_1.buscar(Index[i].getKey()) != NULL && BTree_2.buscar(Index[i].getKey()) != NULL){

				encontrado2 = binary_search(&Index2[0],0,Index2.size(),Index.at(i),1);
				//encontrado = binary_search(&Index[0],0,Index.size(),Index.at(i),1);

				if (encontrado2 != -1){
					
					FILE* SearchFile = fopen(c,"r+");
					fseek(SearchFile,(Index2[encontrado2].getRRN()*LongitudRegistro)+HeaderSize,SEEK_SET);
					char RegisFind[LongitudRegistro];
					fread(RegisFind,1,LongitudRegistro,SearchFile);
					//cout << RegisFind << " ESPACIO ";
					
					for (int i = 0 ; i<LongitudRegistro-1 ; i++)
						cout<<RegisFind[i];
					cout << endl;

					fclose(SearchFile);
					FILE* SearchFile2 = fopen(c2,"r+");
					fseek(SearchFile2,(Index[i].getRRN()*LongitudRegistro2)+HeaderSize2,SEEK_SET);
					char RegisFind2[LongitudRegistro2];
					fread(RegisFind2,1,LongitudRegistro2,SearchFile2);
					//cout << RegisFind2<< endl;

					for (int i = 0 ; i<LongitudRegistro2-1 ; i++)
						cout<<RegisFind2[i];
					cout << endl << endl;	

					fclose(SearchFile2);
					encontrado2=-1;
				}
			}
		}
	}

	void Compactar(){
		string filename = workingFile();

		int Success = rename(filename.c_str(),"OLD.dat");

		ofstream estruc(filename.c_str());
		ifstream oldfile("OLD.dat");
		if (Success == 0){
			string line;
			getline(oldfile,line);
			vector<string> buffer = split(line,'$');
			estruc<<"000-1$"<<buffer[1]<<"$"<<buffer[2]<<"$\n";

			getline(oldfile,line);
			estruc<<line<<'\n';

			while (getline(oldfile,line)){
				if (line[0] != '*'){
					estruc<<line<<'\n';
				}
			}

			estruc.close();
			oldfile.close();
		}

		if (remove("OLD.dat") != 0){
			cerr << "No se pudo remover el archivo temp";
		}

	}

	void cruzarDURO(){
		/*
		string File1 = workingFile();
		string File2 = workingFile();
		string line1;
		string line2;

		int KeyC1 = 0;
		int KeyC2 = 0;

		ifstream File_1(File1.c_str());
		ifstream File_2(File2.c_str());

		if (File_1.is_open() && File_2.is_open()){
			getline(File_1,line1);
			getline(File_2,line2);

			getline(File_1,line1);
			vector<string> tempEstructura = split(line1,'$');
			for (int i = 0; i<tempEstructura.size();i++){
				vector<string> bufferz = split(tempEstructura.at(i),'|');
				if (!strcmp(bufferz[3].c_str(),"1")){
					KeyC1 = i;
					break;
				}
			}

			getline(File_2,line2);

			tempEstructura = split(line2,'$');
			for (int i = 0; i<tempEstructura.size();i++){
				vector<string> bufferz = split(tempEstructura.at(i),'|');
				if (!strcmp(bufferz[3].c_str(),"1")){
					KeyC2 = i;
					break;
				}
			}


			while (getline(File_1,line1)){
				vector<string> buffer = split (line1,'|');

					//cout << "ANTES WHILE" << endl;

				while (getline(File_2,line2)){
					vector<string> buffer2 = split(line2 , '|');

					//if (buffer[0].compare( buffer2[0] ) == 0){
					int K1 = atoi(buffer[0].c_str());
					int K2 = atoi(buffer2[0].c_str());


					cout << K1 << "SS " << K2 <<endl;

					if (K1 == K2){
						for (int i = 0 ; i < buffer.size() - 1 ; i++ )
							cout << buffer[i]<<;
						cout << endl;

						for (int i = 0 ; i < buffer2.size() - 1 ; i++ )
							cout << buffer2[i];
						cout << endl<<endl;
						cout << line1 <<endl;
						cout << line2 << endl;
						cout << endl;
						break;

					}
				}
			}
		}
		*/
		cruzarIndices();
	}
	
	//ugly hacks everyfuckingwhere
