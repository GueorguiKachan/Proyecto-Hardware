#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
using namespace std;

int offset = 0;
string A = "ABBC";
string B = "BABB";
enum class Transf { borrar, insert, sustit, nada };



int C[5][5];

int min( int a, int b, int c){
    if (a <= b && a <= c) {
        return a;
    }else if (b <= c) {
        return b;
    }
    return c;
}


Transf T[4][4];

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
}


void res( int i, int j ){
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
        switch (T[i][j]){
            case Transf::borrar: res(i-1, j); cout << "borrar " <<i <<endl; break;
            case Transf::insert: res(i, j-1);cout<< "insert " << i <<" : "<<B[j-1] <<endl; break;
            case Transf::sustit: res(i-1,j-1);cout<< "sustit " << i << " de A por numero "<< j<<" de B" <<endl; break;
            case Transf::nada: res(i-1, j-1); break;
        }
    }
}

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

}

int main (){
    string fichero;
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
    }
    compSec();
    res(4,4);
    deshacer("a.txt", "c.txt", "b.txt");
}


