#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <map>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

int offset = 0;
string A = "ABBC";
string B = "BABB";
enum Transf { borrar, insert, sustit, nada };

Transf** T  = nullptr;
int size = 0;
int capacity = 0; 

//int C[5][5];

int min( int a, int b, int c){
    if (a <= b && a <= c) {
        return a;
    }else if (b <= c) {
        return b;
    }
    return c;
}


//Transf T[4][4];
/*
void compSec(){
    int i, j, x, y, z;
    for (i = 0; i < 5; i++){
        C[i][0] = i;
    }
    for (j = 0; j < 5; j++){
        C[0][j] = j;
    }
    for (i = 1; i < 5; i ++){
        for (j = 1; j < 5; j++){
            x = *C[i-1, j] + 1;
            y = *C[i, j-1] + 1;
            if (A[i] == B[j]){
                z =*C[i-1, j-1];
            }else{
                z =*C[i-1,j-1] + 1;
            }
            *C[i,j] = min(x,y,z);
            if (min(x,y,z) == x) {
                T[i][j] = Transf::borrar;
            }else if (min (x,y,z) == y){
                T[i][j] = Transf::insert;
            }else if (A[i] == B[j]) {
                T[i][j] = Transf::nada;
            }else{
                T[i][j] = Transf::sustit;
            }
        }
    }
}*/



/*
void deshacer(string ultimaVersion, string ficheroBk, string versionRestaurada){
    ifstream fin1; fin1.open(ultimaVersion);
    ifstream fin2; fin2.open(ficheroBk);
	ofstream fout; fout.open(versionRestaurada);
    if (fin1.is_open() && fin2.is_open() && fout.is_open()){
        stringstream strStream;
        strStream << fin1.rdbuf(); //read the file
        string stringUltimaVersion = strStream.str(); //str holds the content of the file

        string auxFicheroBk;
        char c;
        string aux;
        int index;
        while(EOF != (c= fin2.get())){
            aux = "";
            switch (c){
                case 'b':{ 
                    c = fin2.get();
                    while(c != '\n' && c != EOF){
                        aux = aux + c;
                        c = fin2.get();
                    }
                    index = stoi(aux);
                    cout << "borrar la posicion " << index<<endl;
                    stringUltimaVersion.erase(index+ offset);
                    offset --;
                
                    break;}
                case 's':{ 
                    while(':' != (c=fin2.get())){
                        aux = aux + c;
                    }
                    index = stoi(aux);
                    c = fin2.get();
                    cout << "sustituir la posicion " << index << " por el caracter " << c <<endl;
                    stringUltimaVersion[index + offset] = c;
                    break;}
                case 'i':{
                    while(':' != (c=fin2.get())){
                        aux = aux + c;
                    }
                    index = stoi(aux);
                    c = fin2.get();
                    cout << "insertar tras la posicion " << index << " el caracter " << c <<endl;
                    aux = c;
                    stringUltimaVersion.insert(index+ offset, aux);
                    offset ++;
                    break;}
            }
        }
        fout << stringUltimaVersion;
    }

    fin1.close(); fin2.close(); fout.close();

}*/

void mostrarT (){
    for (int i = 0; i <= 3; i++){
        for (int j = 0; j <=3; j++){
            cout << T[i][j]<<endl;
        }
    }
}

static void res( int i, int j){
    cout << i <<":"<<j<<endl;
    int k;

    if (i == 0){
        for (k = 0; k < j; k++){
            cout << "Anadir" << B[k]<< "en el lugar" << k<<endl;
        }
    }else if (j == 0){
        for (k = 0; k < i; k++){
            cout << "Borrar car numero " << k << endl;
        }
    }else{
        switch (T[i-1][j-1]){
            case borrar: res(i-1, j); cout << "borrar " <<i <<endl; break;
            case insert: res(i, j-1);cout<< "insert " << i <<" : "<<B[j-1] <<endl; break;
            case sustit: res(i-1,j-1);cout<< "sustit " << i << " de A por numero "<< j<<" de B" <<endl; break;
            case nada: res(i-1, j-1); cout<<"nada"<<endl; break;
            default: cout <<"mal"<<endl;
        }
    }
}

void inicializarT (int filas, int columnas){
    T = new Transf*[filas];
    for (int i = 0; i < filas; i++){
        T[i] = new Transf[columnas];
    }
}

void borrarT (int filas){
    for(int i = 0; i < filas; i++){
        delete[] T[i];
        delete[] T;
    }
}




void hallarCambios(string ficheroEntrada, string ficheroSalida){
	ifstream t(ficheroEntrada);
	stringstream buffer;
	buffer << t.rdbuf();

	string sEntrada = buffer.str();

	ifstream s(ficheroSalida);
	std:stringstream buffer2;
	buffer2 << s.rdbuf();

	string sSalida = buffer2.str();

	const int tamanyoSalida = sSalida.size();
	const int tamanyoEntrada = sEntrada.size();
	//Transf T[tamanyoEntrada][tamanyoSalida];
    
    inicializarT(tamanyoEntrada ,tamanyoSalida);
    cout << "Se ha inicializado " <<endl;
	int C[tamanyoEntrada + 1][tamanyoSalida + 1];

	for (int i = 0; i <= tamanyoSalida; i++){
		C[i][0] = i;
        cout << " i: " <<i <<endl;
	}
	for (int j = 0; j <= tamanyoEntrada; j++){
		C[0][j] = j;
        cout << " j: " <<j <<endl;	
	}

	int x,y,z;

	for(int i=1; i <= tamanyoSalida; i++){
		//cout << "Letra i " << sSalida[i] << endl;
		for(int j=1; j <= tamanyoEntrada; j++){
			//cout << "Letra j " << sEntrada[j] << endl;
			x = C[i-1][j] + 1;
			y = C[i][j-1] + 1;
			if(sEntrada[i-1] == sSalida[j-1]){
				z = C[i-1][j-1];
			}else{
				z = C[i-1][j-1] + 1;
			}
			int minimo = min(x,y,z);
			C[i][j] = minimo;
			if(minimo == x){ // (min=x) => borrar 
				//string xd =  "b";
				//xd.append(to_string(i));
				//cout << "Minimo x-" << xd  << endl;
                cout << "borrar" <<endl;
				T[i-1][j-1] = Transf::borrar;
			}else if(minimo == y){ // (min=y) => insert
				//string xd =  "i";
				//xd.append(to_string(i));
				//xd += ":";
				//xd += sEntrada[i];// xd.append(sEntrada[i]);
				// cout << "Minimo y - "<< xd << endl;
                cout << "insert" <<endl;
				T[i-1][j-1] = Transf::insert;
			}else if(minimo == z && sEntrada[i-1] == sSalida[j-1]){ // (min=z) & A[i]=B[j] => nada
				//cout << "Minimo z- n" << endl;
                cout << "nada" <<endl;
				T[i-1][j-1] = Transf::nada;	
			}else{ // (min=z) & A[i]!=B[j] => sustit
				//string xd =  "s";
				//xd.append(to_string(i));
				//xd += ":";
				//xd += sEntrada[i];
				//cout << "Minimo z- " << xd << endl;
                cout << "sustit" <<endl;
				T[i-1][j-1] = Transf::sustit;
			}	
		}
	}
    cout << "antes de res() "<<endl;
    mostrarT();
    res(tamanyoEntrada , tamanyoSalida);
    borrarT(tamanyoEntrada);
}


int main(int argc, char *argv[]){
	hallarCambios(argv[1], argv[2]);
}

    /*string fichero;
    string comando;
    cout << "Inserte fichero a tratar versiones: "<<endl;
    cin >> fichero;
    while(1){
        cout << "Comandos disponibles: "<<endl;
        cout << "'L' -> listar las versiones anteriores" <<endl;

        cin >> comando;
        //switch(str2int(comando)){
            //case str2int("L"): {
                execl("./","ls -l",NULL);
              //  }
          //  }
          break;
    }*/
    //compSec();
    //res(4,4);
    //deshacer("a.txt", "c.txt", "b.txt");



