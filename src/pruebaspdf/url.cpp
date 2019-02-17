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

int main(void)
{
	Tokenizador a(",", true, false);
	list<string> tokens;

	string s = "p0 http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date=22-01-2013&newspaper=catedraTelefonicaUA@iuii.ua.es p1 p2";
	a.Tokenizar(s, tokens);
	imprimirListaSTL(tokens);
	/* La lista de tokens a devolver debería contener (NO lo detecta como URL
	porque no se ha detectado ningún delimitador especial que activase el caso
	URL, igual que tampoco detecta el de e-mail porque tampoco está el @ o el .
	como delimitador): "p0,
	http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date
	=22-01-2013&newspaper=catedraTelefonicaUA@iuii.ua.es, p1, p2"*/

	a.DelimitadoresPalabra("@");
	a.Tokenizar(s, tokens);
	imprimirListaSTL(tokens);
	/* La lista de tokens a devolver debería contener (NO lo detecta como URL,
	pero sí el de e-mail): "p0,
	http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date
	=22-01-2013&newspaper=catedraTelefonicaUA@iuii.ua.es, p1, p2"*/

	a.DelimitadoresPalabra("/ ");
	a.Tokenizar(s, tokens);
	imprimirListaSTL(tokens);
	/* La lista de tokens a devolver debería contener (lo detecta como URL): "p0,
	http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date
	=22-01-2013&newspaper=catedraTelefonicaUA@iuii.ua.es, p1, p2"*/

	a.DelimitadoresPalabra("_:/.?&-=#@");
	a.Tokenizar(s, tokens);
	imprimirListaSTL(tokens);
	/* La lista de tokens a devolver debería contener: "p0,
	http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date
	=22-01-2013&newspaper=catedraTelefonicaUA@iuii.ua.es, p1, p2"*/

	a.DelimitadoresPalabra("/&");
	s = "p0 hhttp://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date=22-01-2013 p1 p2";
	a.Tokenizar(s, tokens);
	imprimirListaSTL(tokens);
	/* La lista de tokens a devolver debería contener: "p0, hhttp:,
	intime.dlsi.ua.es:8080, dossierct, index.jsp?lang=es, status=probable,
	date=22-01-2013, p1, p2"
	// No detecta una URL porque comienza por “hhttp:”*/

	s = "p0 Http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date=22-01-2013 p1 p2";
	a.Tokenizar(s, tokens);
	imprimirListaSTL(tokens);
	/* La lista de tokens a devolver debería contener (no detecta una URL porque
	comienza por “Http:” y porque pasarAminuscSinAcentos == false): "p0, Http:,
	intime.dlsi.ua.es:8080, dossierct, index.jsp?lang=es, status=probable,
	date=22-01-2013, p1, p2"*/

	a.Tokenizar("http:////ab.", tokens);
	imprimirListaSTL(tokens);
	// La lista de tokens: “http:////ab.”

	a.PasarAminuscSinAcentos(true);
	a.Tokenizar(s, tokens);
	imprimirListaSTL(tokens);
	/*La lista de tokens a devolver debería contener: 
	"p0, http://intime.dlsi.ua.es:8080/dossierct/index.jsp?lang=es&status=probable&date
	=22-01-2013&newspaper=catedraTelefonicaUA@iuii.ua.es, p1, p2"
	// Sí detecta una URL porque pasarAminuscSinAcentos == true, y ya que se pasa
	el token a minúsculas y quitará acentos antes de realizar la tokenización,
	“Http:” se convertirá a “http:”*/

	return 0;
}