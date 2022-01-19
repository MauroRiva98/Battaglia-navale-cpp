#ifndef GRIGLIA_H_
#define GRIGLIA_H_

class Griglia {
private:
	char** tabella;
public:
	Griglia();
	virtual ~Griglia();
	virtual void visualizza();
	virtual void visualizza(int mod); //0 visualizza indice righe, 1 indice colonne, 2 entrambi
	char get_casella(int riga, int colonna);
	void set_casella(int riga, int colonna, char carattere);
	char** get_tabella();
};


#endif /* GRIGLIA_H_ */
