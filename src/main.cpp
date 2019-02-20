#include <iostream> 
#include <string>
#include <list> 
#include "tokenizador.h"

using namespace std;

///////// Comprobación de que vacíe la lista resultado

void imprimirListaSTL(const list<string>& cadena)
{
        list<string>::const_iterator itCadena;
        for(itCadena=cadena.begin();itCadena!=cadena.end();itCadena++)
        {
                cout << (*itCadena) << ", ";
        }
        cout << endl;
}

int
main(void)
{
        bool kCasosEspeciales = true, kpasarAminusculas = false;

        list<string> lt1, lt2;

Tokenizador a("-#", true, false); 
list<string> tokens; 

a.DelimitadoresPalabra("@.&");
a.Tokenizar("U.S.A p1 e.g. p2. La", tokens);
        imprimirListaSTL(tokens);

a.DelimitadoresPalabra("");
a.Tokenizar("U.S.A.U.S.A .p1 p1 e.g p2. La", tokens);
        imprimirListaSTL(tokens);

a.Tokenizar("a&U.S.A p1 e.g p2. La", tokens);
        imprimirListaSTL(tokens);

a.DelimitadoresPalabra("&");
a.Tokenizar("a&U.S.A p1 e.g p2. La", tokens);
        imprimirListaSTL(tokens);


a.PasarAminuscSinAcentos(true);
a.Tokenizar("a&U.S.A p1 e.g p2. La", tokens);
        imprimirListaSTL(tokens);

a.DelimitadoresPalabra(".&");
a.CasosEspeciales (false);
a.Tokenizar("a&U.S.A p1 e.g. p2. La", tokens);
        imprimirListaSTL(tokens);


                a.Tokenizar("MS#DOS OS_2 [high low]", tokens);
                imprimirListaSTL(tokens);

                a.AnyadirDelimitadoresPalabra("_ []");
                a.Tokenizar("MS#DOS OS_2 [high low]", tokens);
                imprimirListaSTL(tokens);

                a.DelimitadoresPalabra("_");
                a.Tokenizar("MS#DOS OS_2 [high low]", tokens);
                imprimirListaSTL(tokens);

                a.DelimitadoresPalabra(" _");
                a.Tokenizar("MS#DOS 10 España Éspáñé OS_2 [high low]", tokens);
                imprimirListaSTL(tokens);

                a.Tokenizar("áéíóú ÁÉÍÓÚ Ññ ", tokens);
                imprimirListaSTL(tokens);

                a.PasarAminuscSinAcentos(false);
                a.DelimitadoresPalabra(" _");
                a.Tokenizar("MS#DOS 10 España Éspáñé OS_2 [high low]", tokens);
                imprimirListaSTL(tokens);
}
