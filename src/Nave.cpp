#include "Nave.h"
#include <iostream>
#include <vector>

using namespace std;

Nave::Nave(){
	this->nome = "Placeholder";
	this->caselle_occupate = 0;
	this->punti_vita = 0;
	this->verticale = false;
	this->valore_cella_fisso = -1;
	this->vettore_valori_cella.push_back(-1);
}

Nave::Nave(string n, int c, bool v, int prima_cella[]){
	this->nome = n;
	this->caselle_occupate = c;
	this->punti_vita = c;
	this->verticale = v;
	if(v){
		this->valore_cella_fisso = prima_cella[1]; //Valore colonna
		for(int i = 0;i < c;i++){
			this->vettore_valori_cella.push_back(prima_cella[0] + i);
		}
	}
	else{
		this->valore_cella_fisso = prima_cella[0]; //Valore riga
		for(int i = 0;i < c;i++){
			this->vettore_valori_cella.push_back(prima_cella[1] + i);
		}
	}
}

Nave::~Nave(){
}

// prefix decrement
Nave& Nave::operator--(){
	this->punti_vita = this->punti_vita - 1;
    return *this; // return new value by reference
}

// postfix decrement
Nave Nave::operator--(int){
	Nave old = *this; // copy old value
    operator--();  // prefix increment
    return old;    // return old value
}

bool Nave::is_verticale(){
	return this->verticale;
}

int Nave::get_valore_cella_fisso(){
	return this->valore_cella_fisso;
}

vector<int> Nave::get_vettore_valori_cell(){
	return this->vettore_valori_cella;
}

int Nave::get_punti_vita(){
	return this->punti_vita;
}

string Nave::get_nome(){
	return this->nome;
}
