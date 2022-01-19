#ifndef FLOTTA_H_
#define FLOTTA_H_

#include "Nave.h"
#include "Griglia.h"
#include <memory>

using namespace std;

class Flotta {
private:
	unique_ptr <Nave> portaerei;
	unique_ptr <Nave> corazzata;
	unique_ptr <Nave> incrociatore1;
	unique_ptr <Nave> incrociatore2;
	unique_ptr <Nave> sottomarino;
	unique_ptr <Nave> cacciatorpediniere;
public:
	Flotta();
	Flotta(bool player);
	virtual ~Flotta();
	Nave* get_nave(string nome);
	void reset_nave(string nome);
};

template <typename T>
int input_cella (T x);

Nave* crea_nave(string nome, int caselle_occupate, Griglia* g);

Nave* crea_nave_IA(string nome, int caselle_occupate, Griglia* g);

#endif /* FLOTTA_H_ */
