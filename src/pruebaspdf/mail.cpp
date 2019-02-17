#include <iostream> 
#include <string>
#include <list> 
#include "tokenizador.h"

using namespace std;

void imprimirListaSTL(const list<string>& cadena)
{
        list<string>::const_iterator itCadena;
        for(itCadena=cadena.begin();itCadena!=cadena.end();itCadena++)
        {
                cout << (*itCadena) << ", ";
        }
        cout << endl;
}

int main () {
	Tokenizador a("", true, false);
	list<string> tokens;

	a.DelimitadoresPalabra("@.&");
	a.Tokenizar("catedraTelefonicaUA@@iuii.ua.es p1 p2", tokens);
	imprimirListaSTL(tokens);
	/*// La lista de tokens a devolver debería contener (el @@ hace que no se
	detecte como email, y para “iuii.ua.es” lo detecta como acrónimo):
	"catedraTelefonicaUA, iuii.ua.es, p1, p2"*/

	a.Tokenizar("pal1 cat@iuii@cd.ua.es p1 p2", tokens);
	imprimirListaSTL(tokens);
	/*// La lista de tokens a devolver debería contener (se separa por el primer @
	porque al detectar el segundo @ comprueba que no es un email, y al ser el @ un
	delimitador, detectaría un email en el segundo @): "pal1, cat, iuii@cd.ua.es,
	p1, p2"*/

	a.DelimitadoresPalabra("@.-_");
	a.Tokenizar("-catedraTelefonicaUA@iuii.ua.es @p1 p2 ", tokens);
	imprimirListaSTL(tokens);
	/*// La lista de tokens a devolver debería contener:
	"catedraTelefonicaUA@iuii.ua.es, p1, p2"*/

	a.Tokenizar("@p2@@dot.com@p1-p2", tokens);
	imprimirListaSTL(tokens);
}