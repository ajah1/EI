#include <iostream> 
#include <string>
#include "buscador.h"
#include "indexadorHash.h"

using namespace std;

main() {
IndexadorHash b("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, false);

b.Indexar("./listaFicheros_corto.txt");
b.GuardarIndexacion();

Buscador a("./indicePrueba", 0);
string preg;
double kk1; double kb;

/*
Buscador:
#########
Stemming: 0
Parametros DFR: 2
Parametros BM25: 1.2 0.75
Formula similitud: 0
Formula similitud: 1
No se ha podido cambiar la formula de similitud
Pregunta indexada: 
Pregunta indexada: pal7
*/

cout << "Buscador:" << endl << "#########" << endl;
cout << "Stemming: " << a.DevolverTipoStemming () << endl;
cout << "Parametros DFR: " << a.DevolverParametrosDFR() << endl;
a.DevolverParametrosBM25(kk1, kb);
cout << "Parametros BM25: " << kk1 << " " << kb << endl;
cout << "Formula similitud: " << a.DevolverFormulaSimilitud() << endl;
if(a.CambiarFormulaSimilitud(1))
	cout << "Formula similitud: " << a.DevolverFormulaSimilitud() << endl;
else
	cout << "No se ha podido cambiar la formula de similitud" << endl;

if(a.CambiarFormulaSimilitud(2))
	cout << "Formula similitud: " << a.DevolverFormulaSimilitud() << endl;
else
	cout << "No se ha podido cambiar la formula de similitud" << endl;

a.DevuelvePregunta(preg) ;
cout << "Pregunta indexada: " << preg << endl;
a.IndexarPregunta("pal7");
a.DevuelvePregunta(preg) ;
cout << "Pregunta indexada: " << preg << endl;
}
