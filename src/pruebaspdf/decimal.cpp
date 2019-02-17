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
                cout << (*itCadena) << " <> ";
        }
        cout << std::endl << "------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
        cout << endl;
}

int main () {

	Tokenizador a(",", true, false);
	list<string> lt1;

	a.DelimitadoresPalabra("@.,&");
	a.Tokenizar("pal1 10.000,34 10,000.34 10.000.123.456.789.009,34 10,000,123,456,789,009.34 20.03 40,03 2005 10. 20, 10.0 20,0 La 20,12.456,7.8.9,", lt1);
	imprimirListaSTL(lt1);
	/* La lista de tokens a devolver debería contener (se muestra separada por
	tabuladores en lugar de por comas como ocurre en los ejemplos anteriores por
	cuestiones de claridad; todos los términos numéricos con puntos o comas se
	detectan como números y no como acrónimos, ya que los números se detectan
	antes que los acrónimos; para el “10.” no le aplicaría la heurística al
	aparecer el punto al final, el cual tampoco sería acrónimo ya que aparece un
	solo punto al final de la palabra, por lo que se extraería como “10”
	quitándole el punto delimitador): 
	"pal1 10.000,34 10,000.34 10.000.123.456.789.009,34 10,000,123,456,789,009.34 20.03 40,03 2005 10 20 10.0 20,0 La 20,12.456,7.8.9"*/
	
	a.Tokenizar(".34 ,56", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener: "0.34 0,56"*/
	
	a.Tokenizar("pal1 10.35% 10,35% 23.000,3% 23,5$ 23.05$ 23,05$ 11.1$ 11.05$ 3.4% 4,3%", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener: 
	"pal1 10.35 % 10,35 % 23.000,3 % 23,5 $ 23.05 $ 23,05 $ 11.1 $ 11.05 $ 3.4 % 4,3 %"*/
	
	a.Tokenizar("pal1 10.00a 10.000.a.000 10/12/85 1,23E+10", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener (no extraería números sino
	acrónimos): 
	"pal1 10.00a 10.000.a.000 10/12/85 1 23E+10"*/
	
	a.Tokenizar("pal1&10.00@10.000&abc@10/12/85", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener (extraería un número al
	encontrar el punto en “10.00” finalizado en el @ que es delimitador (igual con
	10.000); también extraería un email en “abc@10/12/85”): 
	"pal1 10.00 10.000 abc@10/12/85"*/
	
	a.Tokenizar(".34@@&,56", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener: 
	"0.34 0,56"*/
	
	a.Tokenizar("...10.000.a.000 ,,23.05 10/12/85 1,23E+10", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener (en “10.000.a.000” extraería
	un acrónimo del que quitaría los puntos del principio; en “,,23.05” quitaría
	la primera coma ya que va seguida de otra coma, pero en la segunda sí entraría
	en el caso de los números, y puesto que es el principio de la palabra le
	añadiría el cero inicial): 
	"10.000.a.000 0,23.05 10/12/85 1 23E+10"*/
	
	a.Tokenizar("...10.000.000 ,,23.05 10/12/85 1,23E+10", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener (en “10.000.000” extraería
	un número del que quitaría los puntos del principio, menos el último en el que
	añadiría el 0.; en “,,23.05” quitaría la primera coma ya que va seguida de
	otra coma, pero en la segunda sí entraría en el caso de los números, y puesto
	que es el principio de la palabra le añadiría el cero inicial): 
	"0.10.000.000 0,23.05 10/12/85 1 23E+10"*/
	
	a.Tokenizar("3..2 4,,,,5 ..35", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener (en “3..2” no lo detectaría
	ni como número ni como acrónimo ya que tiene dos puntos seguidos, pero al
	quitar el primer punto al ser delimitador, al analizar el siguiente punto que
	aparece al principio de una palabra, entonces entraría en el caso de los
	números por lo que le añadiría el cero del principio en “0.2”; algo similar
	ocurriría en “4,,,,5” salvo que no hay ambigüedad con el caso de los
	acrónimos): 
	"3 0.2 4 0,5 0.35"*/

	a.DelimitadoresPalabra("");
	a.Tokenizar("...10.000.a.000 ,,23.05 10/12/85 1,23E+10", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener (no quita las , ni los .
	porque ahora no son delimitadores, por lo que no detecta acrónimos ni
	números): 
	"...10.000.a.000 ,,23.05 10/12/85 1,23E+10"*/

	a.Tokenizar("3..2 4,,,,5 ..35", lt1);
	imprimirListaSTL(lt1);
	/*// La lista de tokens a devolver debería contener (no quita las , ni los .
	porque ahora no son delimitadores, por lo que no detecta acrónimos ni
	números): 
	"3..2 4,,,,5 ..35"*/
}