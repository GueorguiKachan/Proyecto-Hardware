#include <map>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

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
	
	string T[tamanyoEntrada][tamanyoSalida];
	int C[tamanyoEntrada][tamanyoSalida];

	for (int i = 0; i < tamanyoSalida; i++){
		C[i][0] = i;
	}
	for (int j = 0; j < tamanyoEntrada; j++){
		C[0][j] = j;	
	}

	int x,y,z;

	for(int i=1; i < tamanyoSalida; i++){
		cout << "Letra i " << sSalida[i] << endl;
		for(int j=1; j < tamanyoEntrada; j++){
			cout << "Letra j " << sEntrada[j] << endl;
			x = C[i-1][j] + 1;
			y = C[i][j-1] + 1;
			if(sEntrada[j-1] == sSalida[i-1]){
				z = C[i-1][j-1];
			}else{
				z = C[i-1][j-1] + 1;
			}
			int aux =  min(x,y);
			int minimo = min(aux,z);
			C[i][j] = minimo;
			if(minimo == x){ // (min=x) => borrar 
				string xd =  "b";
				xd.append(to_string(i));
				cout << "Minimo x-" << xd  << endl;
				T[i][j] = xd;
			}else if(minimo == y){ // (min=y) => insert
				string xd =  "i";
				xd.append(to_string(i));
				xd += ":";
				xd += sEntrada[i];// xd.append(sEntrada[i]);
				cout << "Minimo y - "<< xd << endl;
				T[i][j] = xd;
			}else if(minimo == z && sEntrada[i] == sSalida[j]){ // (min=z) & A[i]=B[j] => nada
				cout << "Minimo z- n" << endl;
				T[i][j] = "n";	
			}else{ // (min=z) & A[i]!=B[j] => sustit
				string xd =  "s";
				xd.append(to_string(i));
				xd += ":";
				xd += sEntrada[i];
				cout << "Minimo z- " << xd << endl;
				T[i][j] = xd;
			}	
		}
	}

}

int main(int argc, char *argv[]){
	hallarCambios(argv[1], argv[2]);
}