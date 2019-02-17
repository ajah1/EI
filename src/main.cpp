#include <iostream> 
#include <string>
#include <list> 
#include "tokenizador.h"
#include <map>
#include <vector>
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

	std::string str_out = "áéíóú ÁÉÍÓÚ Ññ ";
	list<string> lt1, lt2;

	Tokenizador a("-#:/.", true, true); 
	list<string> tokens; 

	a.Tokenizar(str_out, tokens);
	imprimirListaSTL(tokens);

/*
	typedef std::multimap<int, int> MyMapType;

	MyMapType myMap;

	myMap.insert( std::pair<int, int>('á', 'a') );
	myMap.insert( std::pair<int, int>('Á', 'a') );
	myMap.insert( std::pair<int, int>('à', 'a') );
	myMap.insert( std::pair<int, int>('À', 'a') );

	myMap.insert( std::pair<int, int>('é', 'e') );
	myMap.insert( std::pair<int, int>('É', 'e') );
	myMap.insert( std::pair<int, int>('è', 'e') );
	myMap.insert( std::pair<int, int>('È', 'e') );

	myMap.insert( std::pair<int, int>('í', 'i') );
	myMap.insert( std::pair<int, int>('Í', 'i') );
	myMap.insert( std::pair<int, int>('ì', 'i') );
	myMap.insert( std::pair<int, int>('Ì', 'i') );

	myMap.insert( std::pair<int, int>('ó', 'o') );
	myMap.insert( std::pair<int, int>('Ó', 'o') );
	myMap.insert( std::pair<int, int>('ò', 'o') );
	myMap.insert( std::pair<int, int>('Ò', 'o') );

	myMap.insert( std::pair<int, int>('ú', 'u') );
	myMap.insert( std::pair<int, int>('Ú', 'u') );
	myMap.insert( std::pair<int, int>('ù', 'u') );
	myMap.insert( std::pair<int, int>('Ù', 'u') );

	myMap.insert( std::pair<int, int>('Ñ', 'ñ') );


	std::map<int, int>::iterator it;

	for (char* p = &str_out.at(0); *p != '\0'; ++p) {
		it = myMap.find(*p);
		if (it != myMap.end()) {
			*p = myMap.find(*p)->second;
		}
	}

	std::cout << str_out << std::endl;
	*/
/*
	for (int i = 0; i < p_str.length(); ++i) {
		if(str_out[i] == (char)192 || str_out[i] == (char)193 || str_out[i] == (char)224 || str_out[i] == (char)225) {
			str_out[i] = 'a';
		}else if(str_out[i] == (char)200 || str_out[i] == (char)201 || str_out[i] == (char)232 || str_out[i] == (char)233) {
			str_out[i] = 'e';
		}else if(str_out[i] == (char)204 || str_out[i] == (char)205 || str_out[i] == (char)236 || str_out[i] == (char)237) {
			str_out[i] = 'i';
		}else if(str_out[i] == (char)210 || str_out[i] == (char)211 || str_out[i] == (char)242 || str_out[i] == (char)243) {
			str_out[i] = 'o';
		}else if(str_out[i] == (char)217 || str_out[i] == (char)218 || str_out[i] == (char)249 || str_out[i] == (char)250) {
			str_out[i] = 'u';
		}else if(str_out[i] == (char)209){
			str_out[i] = (char)241; // tolower no funciona con la Ñ
		}else{
			str_out[i]=(char)tolower(str_out[i]);
		}
	}
*/
}
