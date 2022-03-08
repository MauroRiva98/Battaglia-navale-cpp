#include <iostream>
#include <limits>
#include <string>
#include <memory>
#include <random>
#include <chrono>
#include "Flotta.h"
#include "Griglia.h"
#include "Nave.h"

using namespace std;

template <typename T> //Se l'input è un numero tra 1 e 10 lo restituisce diminuito di 1, se è una lettera (maiuscola o minuscola) restituisce il numero corrispondente
int input_cella (T x) {
	if((int) x >= 1 && (int) x <= 10)
		x = x - 1;
	else if((int) x >= (int) 'A' && (int) x <= (int) 'J')
		x = x - 65;
	else if((int) x >= (int) 'a' && (int) x <= (int) 'j')
		x = x - 97;
	else
		x = -1;
	return x;
}

Nave* crea_nave(string nome, int caselle_occupate, Griglia* g){
	char direzione = 'N';
	bool verticale = true;
	do{ //Chiedo al giocatore se vuole posizionare la nave in verticale o in orizzontale
		cout << "Vuoi posizionare la pedina in verticale o in orizzontale [v/o]? ";
		cin >> direzione;
		if(cin.fail() || (direzione != 'v' && direzione != 'o' && direzione != 'V' && direzione != 'O')){
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << endl;
		}
	}while(direzione != 'v' && direzione != 'o' && direzione != 'V' && direzione != 'O');
	if(direzione == 'o' || direzione == 'O')
		verticale = false;
	int cella[2];
	bool collisione = false;
	bool fuori_mappa = false;
	do{ //Prima di inserire la nave nella tabella controllo se non esce dai bordi della mappa e se non si sovrappone con altre navi
		do{ //Chiedo al giocatore di selezionare la prima casella in cui inserire la nave (quella più in alto se il posizionamento è in verticale, quella più a sinistra se è in orizzontale)
			int riga;
			char col;
			if(verticale)
				cout << endl << "Inserisci la riga della cella piu' in alto [1-10]: ";
			else
				cout << endl << "Inserisci la riga della cella piu' a sinistra [1-10]: ";
			cin >> riga;
			if(verticale)
				cout << endl << "Inserisci la colonna della cella piu' in alto [A-J]: ";
			else
				cout << endl << "Inserisci la colonna della cella piu' a sinistra [A-J]: ";
			cin >> col;
			cella[0] = input_cella(riga);
			cella[1] = input_cella(col);
		}while(cella[0] < 0 || cella[1] < 0);
		int array[caselle_occupate];
		if(verticale){
			for(int i = 0;i < caselle_occupate;i++){
				array[i] = (cella[0] + i);
			}
		}
		else{
			for(int i = 0;i < caselle_occupate;i++){
				array[i] = (cella[1] + i);
			}
		}
		collisione = false;
		fuori_mappa = false;
		if(verticale){
			if((caselle_occupate + cella[0]) > 10)
				fuori_mappa = true;
		}
		else{
			if((caselle_occupate + cella[1]) > 10)
				fuori_mappa = true;
		}
		for(int i = 0;i < caselle_occupate && !collisione && !fuori_mappa;i++){
			if(verticale){
				if(g->get_casella(array[i], cella[1]) != '~')
					collisione = true;
			}
			else{
				if(g->get_casella(cella[0], array[i]) != '~')
					collisione = true;
			}
		}
		if(!collisione && !fuori_mappa){
			for(int i = 0;i < caselle_occupate;i++)
				if(verticale)
					g->set_casella(array[i], cella[1], '|');
				else
					g->set_casella(cella[0], array[i], '-');
		}
		else if(collisione)
			cout << endl << "Collisione con un'altra nave!" << endl;
		else if(fuori_mappa)
			cout << endl << "Nave fuori dalla mappa!" << endl;
	}while(collisione || fuori_mappa);
	return new Nave(nome, caselle_occupate, verticale, cella);
}

Nave* crea_nave_IA(string nome, int caselle_occupate, Griglia* g){
	bool verticale;
	int cella[2];
	bool collisione = false;
	bool fuori_mappa = false;
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	uniform_int_distribution<std::mt19937::result_type> dist1(0,1);
	uniform_int_distribution<std::mt19937::result_type> dist9(0,9);
	do{
		if(dist1(rng) == 1)
			verticale = true;
		else
			verticale = false;
		collisione = false;
		fuori_mappa = false;
		cella[0] = dist9(rng);
		cella[1] = dist9(rng);
		int array[caselle_occupate];
		if(verticale){
			for(int i = 0;i < caselle_occupate;i++){
				array[i] = (cella[0] + i);
			}
		}
		else{
			for(int i = 0;i < caselle_occupate;i++){
				array[i] = (cella[1] + i);
			}
		}
		if(verticale){
			if((caselle_occupate + cella[0]) > 10)
				fuori_mappa = true;
		}
		else{
			if((caselle_occupate + cella[1]) > 10)
				fuori_mappa = true;
		}
		for(int i = 0;i < caselle_occupate && !collisione && !fuori_mappa;i++){
			if(verticale){
				if(g->get_casella(array[i], cella[1]) != '~')
					collisione = true;
			}
			else{
				if(g->get_casella(cella[0], array[i]) != '~')
					collisione = true;
			}
		}
		if(!collisione && !fuori_mappa){
			for(int i = 0;i < caselle_occupate;i++)
				if(verticale)
					g->set_casella(array[i], cella[1], '|');
				else
					g->set_casella(cella[0], array[i], '-');
		}
	}while(collisione || fuori_mappa);
	return new Nave(nome, caselle_occupate, verticale, cella);
}

Flotta::Flotta(){
}

Flotta::Flotta(bool player){
	unique_ptr <Griglia> g(new Griglia);
	if(player){
		g->visualizza(2);
		cout << endl << endl << "Portaerei:" << endl << endl;
		portaerei = unique_ptr <Nave> (crea_nave("Portaerei", 5, g.get()));
		system("cls");

		g->visualizza(2);
		cout << endl << endl << "Corazzata:" << endl << endl;
		corazzata = unique_ptr <Nave> (crea_nave("Corazzata", 4, g.get()));
		system("cls");

		g->visualizza(2);
		cout << endl << endl << "Incrociatore 1:" << endl << endl;
		incrociatore1 = unique_ptr <Nave> (crea_nave("Incrociatore 1", 3, g.get()));
		system("cls");

		g->visualizza(2);
		cout << endl << endl << "Incrociatore 2:" << endl << endl;
		incrociatore2 = unique_ptr <Nave> (crea_nave("Incrociatore 2", 3, g.get()));
		system("cls");

		g->visualizza(2);
		cout << endl << endl << "Sottomarino:" << endl << endl;
		sottomarino = unique_ptr <Nave> (crea_nave("Sottomarino", 3, g.get()));
		system("cls");

		g->visualizza(2);
		cout << endl << endl << "Cacciatorpediniere:" << endl << endl;
		cacciatorpediniere = unique_ptr <Nave> (crea_nave("Cacciatorpediniere", 2, g.get()));
		system("cls");
	}
	else{
		portaerei = unique_ptr <Nave> (crea_nave_IA("Portaerei", 5, g.get()));
		corazzata = unique_ptr <Nave> (crea_nave_IA("Corazzata", 4, g.get()));
		incrociatore1 = unique_ptr <Nave> (crea_nave_IA("Incrociatore 1", 3, g.get()));
		incrociatore2 = unique_ptr <Nave> (crea_nave_IA("Incrociatore 2", 3, g.get()));
		sottomarino = unique_ptr <Nave> (crea_nave_IA("Sottomarino", 3, g.get()));
		cacciatorpediniere = unique_ptr <Nave> (crea_nave_IA("Cacciatorpediniere", 2, g.get()));
	}
}

Flotta::~Flotta(){
}

Nave* Flotta::get_nave(string nome){
	  if(nome == (string) "Portaerei")
	    return this->portaerei.get();
	  else if(nome == (string) "Corazzata")
		return this->corazzata.get();
	  else if(nome == (string) "Incrociatore 1")
	 	return this->incrociatore1.get();
	  else if(nome == (string) "Incrociatore 2")
	  	 return this->incrociatore2.get();
	  else if(nome == (string) "Sottomarino")
	  	 return this->sottomarino.get();
	  else if(nome == (string) "Cacciatorpediniere")
	  	 return this->cacciatorpediniere.get();
	return NULL;
}

void Flotta::reset_nave(string nome){ //Dealloca dalla memoria la nave affondata
	if(nome == (string) "Portaerei")
		this->portaerei.reset(NULL);
	else if(nome == (string) "Corazzata")
		this->corazzata.reset(NULL);
	else if(nome == (string) "Incrociatore 1")
		this->incrociatore1.reset(NULL);
	else if(nome == (string) "Incrociatore 2")
		this->incrociatore2.reset(NULL);
	else if(nome == (string) "Sottomarino")
		this->sottomarino.reset(NULL);
	else if(nome == (string) "Cacciatorpediniere")
		this->cacciatorpediniere.reset(NULL);
}
