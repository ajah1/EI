#include "tokenizador.h"
#include "indexadorHash.h"
#include <sys/resource.h>
#include <list>
#include <string>
#include <iostream>

using namespace std;

double getcputime(void) {
	struct timeval tim;
	struct rusage ru;
	getrusage(RUSAGE_SELF, &ru);
	tim=ru.ru_utime;
	double t=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
	tim=ru.ru_stime;
	t+=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
	return t;
}

main() {
	long double aa=getcputime();
	/*IndexadorHash b("./StopWordsEspanyol.txt", ".,:", false, false, "./indicePruebaEspanyol", 0, false, true);
	b.Indexar("listaFicheros.txt");*/
	cout << "Ha tardado " << getcputime() - aa << " segundos" << endl;
}