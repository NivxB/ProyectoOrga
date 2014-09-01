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

using namespace std;//ayy lmao

string workingFile();

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
	string file = workingFile();
	string Index = "../Index"+file;
	file = "../"+file;
	srand (time(NULL));
	cout << Index << endl;

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

		ofstream OutF,IndexF;
		OutF.open(c,ofstream::app);
		IndexF.open(Index.c_str());
		for(int i = 0 ;i<100;i++){
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
						IndexF<<Input<<"\t"<<i<<"\n";
					}

				}
				StringOutFile<<"|";
			}
			StringOutFile<<"\n";
			OutF<<StringOutFile.str();
		}
		OutF.close();
		IndexF.close();
	}
}

string workingFile(){
	int nLines = 0;
	ifstream estruc("../estructuras.dat");
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

		ifstream estruc2("../estructuras.dat");//ni idea de por que falla si uso el mismo estruc de arriba
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
