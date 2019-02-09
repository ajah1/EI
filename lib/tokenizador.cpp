
#include "../include/tokenizador.h"

#include <fstream>

#include <sys/stat.h>
#include <cstdlib>

using namespace std;

/*
á, é, í, ó, ú, à, è, ì, ò, ù


No tekinzar palabras vacías o de longitud cero
SIEMPRE espacio en blanco como delimitador (cuanto especial = true)
1.URL 2.Decimales 3.E-mail 4.Acrónimos 5.Guiones

ALGORITMO GENERAL (recorriendo de izquierda a derecha)
	*Encontrar un delimitador o un blanco
		*Si es especial se ignora
		*Si no especial toke sin delimitador


=>Eliminar acentos
=>Pasar de mayúsculas a míniculas
*/

Tokenizador::Tokenizador (
	const std::string& delimitadoresPalabra,
	const bool& kcasosEspeciales, 
	const bool& minuscSinAcentos)
	: _delimiters(delimitadoresPalabra),
	  _delimitersAux(delimitadoresPalabra),
	  _casosEspeciales(kcasosEspeciales),
	  _pasarAminuscSinAcentos(minuscSinAcentos)

{}

Tokenizador::Tokenizador (const Tokenizador& p_tk) {
	_delimiters = p_tk._delimiters;
	_delimitersAux = p_tk._delimitersAux;
	_casosEspeciales = p_tk._casosEspeciales;
	_pasarAminuscSinAcentos = p_tk._pasarAminuscSinAcentos;
}

Tokenizador::Tokenizador () {
	_delimiters = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
	_casosEspeciales = true;
	_pasarAminuscSinAcentos = false;
}

Tokenizador::~Tokenizador () {
	_delimitersAux = "";
	_delimiters = "";
	_casosEspeciales = false;
	_pasarAminuscSinAcentos = false;
}

Tokenizador& 
Tokenizador::operator= (const Tokenizador& p_tk) {
	if (this != &p_tk) {
		_delimitersAux = p_tk._delimitersAux;
		_delimiters = p_tk._delimiters;
		_casosEspeciales = p_tk._casosEspeciales;
		_pasarAminuscSinAcentos = p_tk._pasarAminuscSinAcentos;
	}

	return *this;
}


void
Tokenizador::EliminarMinusAcentos (std::string& p_str) const{

	std::clog << "[LOG] EliminarMinusAcentos \n";

	// á, é, í, ó, ú, à, è, ì, ò, ù
	// Á, É, Í, Ó, Ú, À, È, Ì, Ò, Ù
	/*
		   **´**	**`** (-1)
	a -> 193, 225
	e -> 201, 233
	i -> 205, 237
	o -> 211, 243
	u -> 218, 250
	*/
	for (char* it = &p_str.at(0); *it != '\0'; ++it) {
		if (*it == 193 || *it == 225) {
			*it = 97;
		}
	}
}


std::string
Tokenizador::ObtenerString(const char* p_i, const char* p_f) const {
	// std::cout << "p_i: " << *p_i << std::endl;
	// std::cout << "p_f: " << *p_f << std::endl;

	// Size del substring
	int s_size = p_f - p_i + 1;
	//std::cout << "s_size: " << s_size << std::endl;

	// Copiar en un string s_size characteres desde p_i 
	std::string s_out(p_i, s_size);
	//std::cout << "Substring: " << s_out << std::endl;

	return s_out;
}

void 
Tokenizador::Tokenizar (const std::string& p_str, std::list<std::string>& p_tokens) const {
	
	if (_pasarAminuscSinAcentos) {
		//EliminarMinusAcentos(p_str);
	}

	if (!_casosEspeciales) {
	//	std::clog << "[LOG] Tokenizar sin especiales \n";
		string::size_type lastPos 	= p_str.find_first_not_of(_delimiters, 0);
		string::size_type pos 		= p_str.find_first_of(_delimiters, lastPos);

		while(string::npos != pos || string::npos != lastPos) {
			p_tokens.push_back(p_str.substr(lastPos, pos - lastPos));
			lastPos = p_str.find_first_not_of(_delimiters, pos);
			pos 	= p_str.find_first_of(_delimiters, lastPos);
		}
	} else {

		string str(p_str);
		char* it = &str.at(0);

		bool primera_it = true;

		std::cout << "\n*it: " << *it << " *(it+1): " << *(it+1) << '\n';

		while (*it != '\0') {
			// CASO ESPECIAL ACRONIMOS
			if (*it == '.' && !primera_it) {
				std::clog << "[LOG] EspecialAcronimo() \n";
				EspecialAcronimo(it, it);
			} 
			// CASO GENERAL
			else {
				// tirar para lante hasta econtrar un " " o 
				// encontrar un delimitador
				std::clog << "[LOG] NormanMan() \n";
				NormanMan(it);
				//it++;
			}

			primera_it = false;
			std::cout << "\n*it-2: " << *(it-2) << " *it: " << *it << " *(it+1): " << *(it+1);
			std::cin.get();
		}
	}
}

std::string
Tokenizador::NormanMan(char* &p_it) const {

	char* p_actual = p_it;

	bool parar = false;
	bool fake = false;
	bool single = false;

	while (!parar) {
		// U.
			// U.* => generico
			// U.U => acronimo
		// U* => generico
		// UU => seguir
		
		//"Hack.4.Good p1 "
		//"...U.S.A p1 e..g. p2. La"

		if (*p_it != '.' && *(p_it+1) == '.') {				// U.
			if (EsDelimitador(*(p_it+2))) {					// U.* => generico
				p_it += 2;
			} else {										// U.U => acronimo
				EspecialAcronimo(p_actual, ++p_it);
				fake = true;
			}
			parar =  true;
		}
		else if (EsDelimitador(*p_it)) {					// * => _del para
			std::cout << "encuentra el delimitador: **" << *p_it << "**\n"; 
			p_it++;
			parar = true;
		}
		else if (*(p_it+1) == '\0') {					// U\0 => fin del string
			std::cout << "*it-1: " << *(p_it-1) << std::endl;
			std::cout << "entra fin string" << std::endl;
			p_it++;
			parar = single = true;
		}
		else if (EsDelimitador(*p_it+1)){ 					// U* => _del para
				//|| *(p_it+1) == '\0') {	
			//"Hola que tal "
			std::cout << "*it+1: " << *(p_it+1) << std::endl;
			std::cout << "entra EsDelimitador+1" << std::endl;
			p_it += 2;
			parar = true;
		}

		else {
			std::cout << "iterar: " << *p_it << std::endl;
			p_it++;
		}
	}

	/*
	    p1
	*/

	if (!fake && (p_it - p_actual) > 1) {
		if (single)
			std::cout << "NormanMan:-->" << ObtenerString(p_actual, p_it-1) << "<--\n";
		else
			std::cout << "NormanMan:-->" << ObtenerString(p_actual, p_it-2) << "<--\n";
	}

	return "";
}

//"U..S.A p1 e..g. p2. La"
//U, S.A, p1, e, g, p2, La,
std::string
Tokenizador::EspecialAcronimo(char* &p_izq, char* &p_it) const {
	/*
		A.A => seguir buscando
		A.* => generar substring hasta la izq
		*.A => no es acronimo
		*.* => no es acronimo
	*/

	/***final del string****/

	// donde empieza el substring
	char* copia_it = p_izq;

	bool estabaClaro = false;
	bool parar = false;
	/*
	a.Tokenizar("Hack.4.Good p1 ", lt1);
	a.Tokenizar("U.A.Aaa p1 ", lt1);
	*/
	std::cout << "ACRONIMOASDSDA::::" << std::endl;
	std::cout << "pactu: " << *copia_it << std::endl;
	
	while (!parar) {
		std::cout << "it: " << *p_it << " *it+1: " << *(p_it+1) << std::endl;

		if (*p_it == ' ') {
			parar = true;
		}
		// Parte derecha no es delimitador
		else if (!EsDelimitador(*(p_it+1))) {	// ?.A
			if (!EsDelimitador(*(p_it-1))) {	// A.A
				p_it += 2;						// apuntar al siguiente bloque
				estabaClaro = true;
			} else {							// *.A
				p_it += 1;
				parar = true;
			}
		}
		// Parte izquierda no es delimitador
		else if (!EsDelimitador(*(p_it-1))) {	// A.*
			p_it += 2;
			parar = true;
		}
		// Izq y Der delimitan
		else  { 								// *.* 
			p_it += 2;
			parar = true;
		}
	}

	if (estabaClaro)
		std::cout << "EspecialAcronimo:-->" << ObtenerString(copia_it, p_it-1) << "<--\n";


	return "";
}

bool
Tokenizador::EsDelimitador(const char p_d) const{
	bool es_delimitador = false;

	if (_casosEspeciales == true) {
		// ' ' siempre se considera delimitador
		if (_delimiters.empty()) {
			es_delimitador = p_d == ' ';
		} else {
			for (const char* d = &_delimiters.at(0); *d != '\0'; ++d) {
				if (p_d == *d || p_d == ' ') {
					es_delimitador = true;
				}
			}	
		}
	} else if (!_delimiters.empty()) {
		for (const char* d = &_delimiters.at(0); *d != '\0'; ++d) {
			if (p_d == *d) {
				es_delimitador = true; 
			}
		}
	}
	return es_delimitador;
}

bool
Tokenizador::Tokenizar (const std::string& p_fin, const std::string& p_fout) const {
	ifstream i;
	ofstream f;
	string cadena;
	list<string> tokens;

	i.open(p_fin.c_str());
	if (!i) {
		cerr << "ERROR: No existe el archivo: " << p_fin << endl;
		return false;
	} else {
		while (!i.eof()) {
			cadena="";
			getline(i, cadena);
			if (cadena.length()!=0) {
				Tokenizar(cadena, tokens);
			}
		}
	}

	i.close();
	
	f.open(p_fout.c_str());
	list<string>::iterator itS;
	for (itS= tokens.begin(); itS != tokens.end(); itS++) {
		f << (*itS) << endl;
	}

	f.close();
	
	return true;
}

bool 
Tokenizador::Tokenizar (const std::string& p_i) const {
	return false;
}

bool 
Tokenizador::TokenizarListaFicheros (const std::string& p_i) const {
	return false;
}

bool 
Tokenizador::TokenizarDirectorio (const std::string& p_dir) const {
	struct stat dir;

	// Compruebo la existencia del directorio
	int err = stat(p_dir.c_str(), &dir);

	if (err == -1 || !S_ISDIR(dir.st_mode))
		return false;
	else {
		// Hago una lista en un fichero con find>fich
		string cmd = "find " +p_dir+ " -follow |sort > .lista_fich";
		system(cmd.c_str());
		return TokenizarListaFicheros(".lista_fich");
	}
}

void 
Tokenizador::DelimitadoresPalabra (const std::string& p_nuevoDelimiters) {
	_delimiters = "";

	for (char d : p_nuevoDelimiters) {
		if (_delimiters.find_first_of(d) == string::npos) {
			_delimiters += d;
		}
	}
}

void
Tokenizador::AnyadirDelimitadoresPalabra (const std::string& p_nuevoDelimiters) {
	// No repetidos
	bool addDelimiter = true;
	for (std::string::const_iterator it = _delimiters.begin(); it != _delimiters.end(); ++it) {
		if (p_nuevoDelimiters.compare(&*it) == true)
			addDelimiter = false;
	}

	if (addDelimiter)
		_delimiters.append(p_nuevoDelimiters);
}

std::string 
Tokenizador::DelimitadoresPalabra() const {
	return _delimiters;
}

void
Tokenizador::CasosEspeciales (const bool& p_nuevoCasosEspeciales) {
	_casosEspeciales = p_nuevoCasosEspeciales;
}

bool 
Tokenizador::CasosEspeciales () {
	return _casosEspeciales;
}

void 
Tokenizador::PasarAminuscSinAcentos (const bool& p_PasarAminuscSinAcentos) {
	_pasarAminuscSinAcentos = p_PasarAminuscSinAcentos;
}

bool 
Tokenizador::PasarAminuscSinAcentos () {
	return _pasarAminuscSinAcentos;
}

std::ostream& 
operator<< (std::ostream& p_os, const Tokenizador& p_tk)
{	
	p_os << "DELIMITADORES: " 					 << p_tk._delimitersAux 
		 << "TRATA CASOS ESPECIALES: " 			 << p_tk._casosEspeciales 
		 << "PASAR A MINUSCULAS Y SIN ACENTOS: " << p_tk._pasarAminuscSinAcentos;

	return p_os;
}