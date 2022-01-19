#ifndef NAVE_H_
#define NAVE_H_

#include <string>
#include<vector>
using namespace std;

class Nave {
private:
	string nome;
	int caselle_occupate;
	int punti_vita;
	bool verticale;
	int valore_cella_fisso;
	vector<int> vettore_valori_cella;
public:
	Nave();
	Nave(string n, int c, bool v, int pc[]);
	~Nave();
	bool is_verticale();
	int get_valore_cella_fisso();
	vector<int> get_vettore_valori_cell();
	Nave& operator--();
	Nave operator--(int);
	int get_punti_vita();
	string get_nome();
};




#endif /* NAVE_H_ */
