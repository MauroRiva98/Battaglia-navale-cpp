#ifndef TABELLONE_H_
#define TABELLONE_H_

#include "Flotta.h"
#include "Nave.h"
#include "Griglia.h"

using namespace std;

class Tabellone : public Griglia, public Flotta {
private:
	int punti_rimasti;
public:
	Tabellone();
	Tabellone(bool player);
	virtual ~Tabellone();
	void visualizza(int mod); //0 visualizza indice righe, 1 indice colonne, 2 entrambi
	Tabellone& operator--();
	int colpisci_nave(int riga, int colonna);
	int get_punti_rimasti();
};

void inizializza_tabella(char** tabella, Nave* n);

#endif /* TABELLONE_H_ */
