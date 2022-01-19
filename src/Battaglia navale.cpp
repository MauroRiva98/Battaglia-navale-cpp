//============================================================================
// Name        : Battaglia.cpp
// Author      : Mauro Riva [1053644]
// Version     :
// Copyright   : Your copyright notice
// Description : Battaglia navale
//============================================================================

#include <iostream>
#include <exception>
#include <stdlib.h>
#include <limits>
#include <string>
#include <memory>
#include <random>
#include <chrono>
#include <list>
#include <iterator>
#include "Nave.h"
#include "Griglia.h"
#include "Flotta.h"
#include "Tabellone.h"

using namespace std;

int menu(){
	cout << "Menu principale\n\n";
	cout << "1) Giocatore vs Computer" << endl;
	cout << "2) Giocatore vs Giocatore" << endl << endl;
	cout << "Cosa vuoi fare? ";

	int risposta;

	try{
		cin >> risposta;
		if(cin.fail())
			throw invalid_argument("NaN");
		if(risposta != 1 && risposta != 2)
			throw out_of_range("Valore inserito non corretto!");
	}
	catch (const out_of_range &e) {
		system("cls");
		cout << endl << e.what() << endl << endl;
		return -1;
	}
	catch (const invalid_argument &e) {
		// clear error state
		cin.clear();
		// discard 'bad' character(s)
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		system("cls");
		cout << endl << e.what() << endl << endl;
		return -1;
	}

	return risposta;
}

template <typename T>
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

void visualizza_tabella(Griglia* g){
	g->visualizza(2);
}

void turno_giocatore(Tabellone* playerA, Tabellone* playerB){
	int riga;
	char colonna;
	int casella[2];
	bool casella_gia_colpita = false;
	visualizza_tabella(playerB);
	do{
		cout << endl << "Inserisci la riga della cella che vuoi colpire [1-10]: ";
		cin >> riga;
		cout << endl << "Inserisci la colonna della cella che vuoi colpire [A-J]: ";
		cin >> colonna;
		casella[0] = input_cella(riga);
		casella[1] = input_cella(colonna);
		casella_gia_colpita = false;
		if(casella[0] > 0 && casella[1] > 0)
			if(playerB->get_casella(casella[0], casella[1]) == 'X' || playerB->get_casella(casella[0], casella[1]) == 'O'){
				casella_gia_colpita = true;
				cout << endl << "Hai gia' colpito questa casella!" << endl;
			}
	}while(casella[0] < 0 || casella[1] < 0 || casella_gia_colpita);
	if(playerB->get_casella(casella[0], casella[1]) == '~'){
		cout << endl << "Colpo mancato!" << endl;
		playerB->set_casella(casella[0], casella[1], 'O');
	}
	else if(playerB->get_casella(casella[0], casella[1]) == '|' || playerB->get_casella(casella[0], casella[1]) == '-'){
		playerB->colpisci_nave(casella[0], casella[1]);
		playerB->set_casella(casella[0], casella[1], 'X');
		playerB->operator --();
	}
}

void turno_IA(Tabellone* playerA, Tabellone* playerB, bool& priorita, list<int>& righe, list<int>& colonne){
	int casella[2];
	bool casella_gia_colpita = false;
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	uniform_int_distribution<std::mt19937::result_type> dist9(0,9);
	visualizza_tabella(playerB);
	if(priorita && righe.empty())
		priorita = false;
	if(!priorita){
		do{
			casella_gia_colpita = false;
			casella[0] = dist9(rng);
			casella[1] = dist9(rng);
			if(playerB->get_casella(casella[0], casella[1]) == 'X' || playerB->get_casella(casella[0], casella[1]) == 'O')
				casella_gia_colpita = true;
		}while(casella_gia_colpita);
	}
	else{
		casella[0] = righe.front();
		righe.pop_front();
		casella[1] = colonne.front();
		colonne.pop_front();
	}
	cout << endl << "Riga: " << (casella[0] + 1) << endl;
	cout << endl << "Colonna: " << (char)(casella[1] + 65) << endl;
	if(playerB->get_casella(casella[0], casella[1]) == '~'){
		cout << endl << "Colpo mancato!" << endl;
		playerB->set_casella(casella[0], casella[1], 'O');
	}
	else if(playerB->get_casella(casella[0], casella[1]) == '|' || playerB->get_casella(casella[0], casella[1]) == '-'){
		int status_nave = playerB->colpisci_nave(casella[0], casella[1]);
		playerB->set_casella(casella[0], casella[1], 'X');
		playerB->operator --();
		if(status_nave == 0){ //Nave colpita
			if(!priorita){
				priorita = true;
				if(casella[1] < 9){ //Casella a destra
					if(playerB->get_casella(casella[0], (casella[1] + 1)) != 'X' && playerB->get_casella(casella[0], (casella[1] + 1)) != 'O'){
						righe.push_back(casella[0]);
						colonne.push_back(casella[1] + 1);
					}
				}
				if(casella[0] < 9){ //Casella sotto
					if(playerB->get_casella((casella[0] + 1), casella[1]) != 'X' && playerB->get_casella((casella[0] + 1), casella[1]) != 'O'){
						righe.push_back(casella[0] + 1);
						colonne.push_back(casella[1]);
					}
				}
				if(casella[1] > 0){ //Casella a sinistra
					if(playerB->get_casella(casella[0], (casella[1] - 1)) != 'X' && playerB->get_casella(casella[0], (casella[1] - 1)) != 'O'){
						righe.push_back(casella[0]);
						colonne.push_back(casella[1] - 1);
					}
				}
				if(casella[0] > 0){ //Casella sopra
					if(playerB->get_casella((casella[0] - 1), casella[1]) != 'X' && playerB->get_casella((casella[0] - 1), casella[1]) != 'O'){
						righe.push_back(casella[0] - 1);
						colonne.push_back(casella[1]);
					}
				}
			}
			else{
				bool verticale = false;
				if(casella[0] < 9){
					if(playerB->get_casella((casella[0] + 1), casella[1]) == 'X')
						verticale = true;
				}
				if(casella[0] > 0){
					if(playerB->get_casella((casella[0] - 1), casella[1]) == 'X')
						verticale = true;
				}
				if(verticale){
					if(casella[0] > 0)
						if(playerB->get_casella((casella[0] - 1), casella[1]) != 'X' && playerB->get_casella((casella[0] - 1), casella[1]) != 'O'){
							righe.push_front(casella[0] - 1);
							colonne.push_front(casella[1]);
						}
					if(casella[0] < 9)
						if(playerB->get_casella((casella[0] + 1), casella[1]) != 'X' && playerB->get_casella((casella[0] + 1), casella[1]) != 'O'){
							righe.push_front(casella[0] + 1);
							colonne.push_front(casella[1]);
						}
				}
				else{
					if(casella[1] > 0)
						if(playerB->get_casella(casella[0], (casella[1] - 1)) != 'X' && playerB->get_casella(casella[0], (casella[1] - 1)) != 'O'){
							righe.push_front(casella[0]);
							colonne.push_front(casella[1] - 1);
						}
					if(casella[1] < 9)
						if(playerB->get_casella(casella[0], (casella[1] + 1)) != 'X' && playerB->get_casella(casella[0], (casella[1] + 1)) != 'O'){
							righe.push_front(casella[0]);
							colonne.push_front(casella[1] + 1);
						}
				}
				std::list<int>::iterator iter = righe.begin();
				std::list<int>::iterator end  = righe.end();
				std::list<int>::iterator iter2 = colonne.begin();
				while(iter != end){
					if(verticale){
						if(*iter2 != casella[1]){
							iter = righe.erase(iter);
							iter2 = colonne.erase(iter2);
						}
						else{
							++iter;
							++iter2;
						}
					}
					else{
						if(*iter != casella[0]){
							iter = righe.erase(iter);
							iter2 = colonne.erase(iter2);
						}
						else{
							++iter;
							++iter2;
						}
					}
				}
			}
		}
		else if(status_nave == 1){ //Nave affondata
			priorita = false;
			righe.clear();
			colonne.clear();
		}
	}
}

int main() {
	int modalita;
	bool multiplayer;

	do {
		modalita = menu();
	} while(modalita < 0);
	system("cls");

	if(modalita == 2)
		multiplayer = true;
	else
		multiplayer = false;

	//Posizionamento navi Player 1
	cout << "Player 1: " << endl << endl;
	unique_ptr <Tabellone> player1(new Tabellone(true));

	//Posizionamento navi Player 2
	if(multiplayer)
		cout << "Player 2: " << endl << endl;
	unique_ptr <Tabellone> player2(new Tabellone(multiplayer));

	//Partita
	bool vittoria = false;
	bool priorita = false;
	list <int> righe;
	list <int> colonne;
	while(!vittoria){
		//Turno Player 1
		system("cls");
		cout << "Turno giocatore 1:" << endl << endl;
		turno_giocatore(player1.get(), player2.get());
		cout << endl;
		system("pause");

		//Turno player 2
		system("cls");
		if(multiplayer){
			cout << "Turno giocatore 2:" << endl << endl;
			turno_giocatore(player2.get(), player1.get());
		}
		else{
			cout << "Turno computer:" << endl << endl;
			turno_IA(player2.get(), player1.get(), priorita, righe, colonne);
		}
		cout << endl;
		system("pause");

		//Vittoria
		if(player1.get()->get_punti_rimasti() == 0){
			vittoria = true;
			system("cls");
			if(multiplayer)
				cout << "Vittoria del Player 2!" << endl << endl;
			else
				cout << "Vittoria del computer!" << endl << endl;
		}
		else if(player2.get()->get_punti_rimasti() == 0){
			vittoria = true;
			system("cls");
			cout << "Vittoria del Player 1!" << endl << endl;
		}
	}
	system("PAUSE");
	return 0;
}
