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

	Tokenizador a(",", true, false);
	list<string> lt1;


	a.DelimitadoresPalabra("@.&");
	a.Tokenizar("U.S....A.BC.D ", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener (al encontrar el segundo
	punto seguido no lo considera como acrónimo, por lo que extrae el primer
	acrónimo “U.S”; el siguiente token sí lo detecta como acrónimo “A.BC.D”):
	"U.S, A.BC.D"*/

	a.Tokenizar("...U.S.A... p1 e..g. p2. La", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener: "U.S.A, p1, e, g, p2, La"*/

	a.Tokenizar("...U.S.A@p1 e..g-p2. La", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener (el acrónimo U.S.A finaliza
	al encontrar el @ que es delimitador; “g-p2” no lo detecta como multipalabra
	ya que el guión no es delimitador, pero tras detectar el segundo punto que
	hace que no sea acrónimo y quita el punto delimitador del final): "U.S.A, p1,
	e, g-p2, La"*/


	a.DelimitadoresPalabra("");
	/*// Pero al estar activados los casos
	especiales el blanco sí se considerará separador*/

	a.Tokenizar("U.S.A .U.S.A .p1 p1 e.g. p2. La", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener (no detecta ningún acrónimo
	al no ser el . un delimitador): "U.S.A, .U.S.A, .p1, p1, e.g., p2., La"*/

	a.Tokenizar("...U.S.A p1 e..g. p2. La", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener: "...U.S.A, p1, e..g., p2.,
	La"*/

	a.Tokenizar("a&U.S.A p1 e.g. p2. La", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener: "a&U.S.A, p1, e.g., p2.,
	La"*/

	a.DelimitadoresPalabra("&");
	a.Tokenizar("a&U.S.A p1 e.g. p2. La", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener (porque & es delimitador,
	pero “U.S.A” no lo detecta como acrónimo ya que el punto no es delimitador):
	"a, U.S.A, p1, e.g., p2., La"*/

	return 0;
}