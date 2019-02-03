
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
	_delimitersAux = p_tk._delimitersAux;
	_delimiters = p_tk._delimiters;
	_casosEspeciales = p_tk._casosEspeciales;
	_pasarAminuscSinAcentos = p_tk._pasarAminuscSinAcentos;
	return *this;
}


void
Tokenizador::EliminarMinusAcentos (const std::string p_str) {
/*
	string sn = p_str;

	std::cout << "+++++++++++++++++++++++++++++++++++++++++ \n";
	std::cout << p_str << std::endl; 

	char* it = sn;
	//á, é, í, ó, ú, à, è, ì, ò, ù
*/



}

void 
Tokenizador::Tokenizar (const std::string& p_str, std::list<std::string>& p_tokens) const {
	string::size_type lastPos 	= p_str.find_first_not_of(_delimiters, 0);
	string::size_type pos 		= p_str.find_first_of(_delimiters, lastPos);

	while(string::npos != pos || string::npos != lastPos) {
		p_tokens.push_back(p_str.substr(lastPos, pos - lastPos));
		lastPos = p_str.find_first_not_of(_delimiters, pos);
		pos 	= p_str.find_first_of(_delimiters, lastPos);
	}
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