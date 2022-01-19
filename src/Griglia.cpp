#include "Griglia.h"
#include <iostream>
#include <windows.h>

using namespace std;

Griglia::Griglia(){
	tabella = new char*[10];
	for(int i = 0;i < 10;i++){
		tabella[i] = new char[10];
		for(int j = 0;j < 10;j++){
			tabella[i][j] = '~';
		}
	}
}

Griglia::~Griglia(){
	for(int i = 0;i < 10;i++)
		delete [] this->tabella[i];
	delete [] this->tabella;
}

void Griglia::visualizza(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for(int i = 0;i < 10;i++){
			for(int j = 0;j < 10;j++){
				if(tabella[i][j] == '~')
					SetConsoleTextAttribute(hConsole, 9);
				else
					SetConsoleTextAttribute(hConsole, 7);
				cout << ' ' << tabella[i][j];
			}
			cout << endl;
	}
	SetConsoleTextAttribute(hConsole, 7);
}

void Griglia::visualizza(int mod){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if(mod > 0){
		SetConsoleTextAttribute(hConsole, 15);
		char c = 'A';
		cout << ' ' << ' ';
		for(int t = 0;t < 10;t++){
			cout << ' ' << c;
			c++;
		}
		cout << endl;
	}
	for(int i = 0;i < 10;i++){
			for(int j = 0;j < 10;j++){
				if((mod == 0 || mod == 2) && j == 0)
					if(i < 9){
						SetConsoleTextAttribute(hConsole, 15);
						cout << ' ' << (i+1);
						if(tabella[i][j] == '~')
							SetConsoleTextAttribute(hConsole, 9);
						else
							SetConsoleTextAttribute(hConsole, 7);
						cout << ' ' << tabella[i][j];
					}
					else{
						SetConsoleTextAttribute(hConsole, 15);
						cout << (i+1);
						if(tabella[i][j] == '~')
							SetConsoleTextAttribute(hConsole, 9);
						else
							SetConsoleTextAttribute(hConsole, 7);
						cout << ' ' << tabella[i][j];
					}
				else{
					if(tabella[i][j] == '~')
						SetConsoleTextAttribute(hConsole, 9);
					else
						SetConsoleTextAttribute(hConsole, 7);
					cout << ' ' << tabella[i][j];
				}
			}
			cout << endl;
		}
	SetConsoleTextAttribute(hConsole, 7);
}

char Griglia::get_casella(int riga, int colonna){
	return this->tabella[riga][colonna];
}

void Griglia::set_casella(int riga, int colonna, char carattere){
	this->tabella[riga][colonna] = carattere;
}

char** Griglia::get_tabella(){
	return this->tabella;
}


