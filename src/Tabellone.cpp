#include <iostream>
#include <vector>
#include <windows.h>
#include <list>
#include <iterator>
#include "Tabellone.h"
#include "Griglia.h"
#include "Flotta.h"

using namespace std;

void inizializza_tabella(char** tabella, Nave* n){
	vector <int> vettore = n->get_vettore_valori_cell();
	if(n->is_verticale()){
		int colonna = n->get_valore_cella_fisso();
		for(unsigned int i = 0;i < vettore.size();i++)
			tabella[vettore.at(i)][colonna] = '|';
	}
	else{
		int riga =  n->get_valore_cella_fisso();
		for(unsigned int i = 0;i < vettore.size();i++)
			tabella[riga][vettore.at(i)] = '-';
	}
}

Tabellone::Tabellone(){
	this->punti_rimasti = 0;
}

Tabellone::Tabellone(bool player) : Griglia(), Flotta(player){
	this->punti_rimasti = 20;
	inizializza_tabella(this->get_tabella(), this->get_nave("Portaerei"));
	inizializza_tabella(this->get_tabella(), this->get_nave("Corazzata"));
	inizializza_tabella(this->get_tabella(), this->get_nave("Incrociatore 1"));
	inizializza_tabella(this->get_tabella(), this->get_nave("Incrociatore 2"));
	inizializza_tabella(this->get_tabella(), this->get_nave("Sottomarino"));
	inizializza_tabella(this->get_tabella(), this->get_nave("Cacciatorpediniere"));
}

Tabellone::~Tabellone(){
}

void Tabellone::visualizza(int mod){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	char** tab = this->get_tabella();
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
							if(tab[i][j] == '~')
								SetConsoleTextAttribute(hConsole, 9);
							else if(tab[i][j] == 'X')
								SetConsoleTextAttribute(hConsole, 12);
							else if(tab[i][j] == 'O')
								SetConsoleTextAttribute(hConsole, 14);
							else
								SetConsoleTextAttribute(hConsole, 7);
							if(tab[i][j] == '|' || tab[i][j] == '-'){
								SetConsoleTextAttribute(hConsole, 9);
								cout << ' ' << '~';
							}
							else
								cout << ' ' << tab[i][j];
						}
						else{
							SetConsoleTextAttribute(hConsole, 15);
							cout << (i+1);
							if(tab[i][j] == '~')
								SetConsoleTextAttribute(hConsole, 9);
							else if(tab[i][j] == 'X')
								SetConsoleTextAttribute(hConsole, 12);
							else if(tab[i][j] == 'O')
								SetConsoleTextAttribute(hConsole, 14);
							else
								SetConsoleTextAttribute(hConsole, 7);
							if(tab[i][j] == '|' || tab[i][j] == '-'){
								SetConsoleTextAttribute(hConsole, 9);
								cout << ' ' << '~';
							}
							else
								cout << ' ' << tab[i][j];
						}
					else{
						if(tab[i][j] == '~')
							SetConsoleTextAttribute(hConsole, 9);
						else if(tab[i][j] == 'X')
							SetConsoleTextAttribute(hConsole, 12);
						else if(tab[i][j] == 'O')
							SetConsoleTextAttribute(hConsole, 14);
						else
							SetConsoleTextAttribute(hConsole, 7);
						if(tab[i][j] == '|' || tab[i][j] == '-'){
							SetConsoleTextAttribute(hConsole, 9);
							cout << ' ' << '~';
						}
						else
							cout << ' ' << tab[i][j];
					}
				}
				cout << endl;
			}
		SetConsoleTextAttribute(hConsole, 7);
}

// prefix decrement
Tabellone& Tabellone::operator--(){
	this->punti_rimasti = this->punti_rimasti - 1;
    return *this; // return new value by reference
}

int Tabellone::colpisci_nave(int riga, int colonna){
	list <Nave*> lista;
	if(this->get_nave("Portaerei") != NULL)
		lista.push_back(this->get_nave("Portaerei"));
	if(this->get_nave("Corazzata") != NULL)
		lista.push_back(this->get_nave("Corazzata"));
	if(this->get_nave("Incrociatore 1") != NULL)
		lista.push_back(this->get_nave("Incrociatore 1"));
	if(this->get_nave("Incrociatore 2") != NULL)
		lista.push_back(this->get_nave("Incrociatore 2"));
	if(this->get_nave("Sottomarino") != NULL)
		lista.push_back(this->get_nave("Sottomarino"));
	if(this->get_nave("Cacciatorpediniere") != NULL)
		lista.push_back(this->get_nave("Cacciatorpediniere"));

	for (auto const& i : lista) {
	    if(i->is_verticale()){
	    	if(i->get_valore_cella_fisso() == colonna){
	    		for(unsigned int j = 0;j < i->get_vettore_valori_cell().size();j++){
	    			if(i->get_vettore_valori_cell().at(j) == riga){
	    				i->operator --();
	    				if(i->get_punti_vita() == 0){
	    					cout << endl << i->get_nome() << " affondato/a! " << endl;
	    					this->reset_nave(i->get_nome());
	    					return 1;
	    				}
	    				else
	    					cout << endl << "Nave colpita!" << endl;
	    				return 0;
	    			}
	    		}
	    	}
	    }
	    else{
	    	if(i->get_valore_cella_fisso() == riga){
	    		for(unsigned int j = 0;j < i->get_vettore_valori_cell().size();j++){
	    			if(i->get_vettore_valori_cell().at(j) == colonna){
	    				i->operator --();
	    				if(i->get_punti_vita() == 0){
	    					cout << endl << i->get_nome() << " affondato/a! " << endl;
	    					this->reset_nave(i->get_nome());
	    					return 1;
	    				}
	    				else
	    					cout << endl << "Nave colpita!" << endl;
	    		    	return 0;
	    		    }
	    		 }
	    	}
	    }
	}
	return -1;
}

int Tabellone::get_punti_rimasti(){
	return this->punti_rimasti;
}
