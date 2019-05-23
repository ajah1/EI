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

a.IndexarPregunta("pal1 pal4 pal2 pal3 pal1. pal2, pal4 pal2 Yo EL LA Yo el");

if(a.Buscar(1000))
	a.ImprimirResultadoBusqueda(10000);
/*
a.CambiarFormulaSimilitud(1);

if(a.Buscar(100))
	a.ImprimirResultadoBusqueda(1000);
*/
}
/*
0 DFR fichero1 0 3.55086 pal1 pal4 pal2 pal3 pal1. pal2, pal4 pal2 Yo EL LA Yo el
0 DFR fichero2 1 2.52059 pal1 pal4 pal2 pal3 pal1. pal2, pal4 pal2 Yo EL LA Yo el
0 BM25 fichero1 0 -0.660377 pal1 pal4 pal2 pal3 pal1. pal2, pal4 pal2 Yo EL LA Yo el
0 BM25 fichero2 1 -1.00132 pal1 pal4 pal2 pal3 pal1. pal2, pal4 pal2 Yo EL LA Yo el
*/