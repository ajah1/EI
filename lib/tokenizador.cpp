
#include "../include/tokenizador.h"

#include <sys/stat.h>
#include <cstdlib>

using namespace std;

Tokenizador::Tokenizador (
	const std::string& delimitadoresPalabra,
	const bool& kcasosEspeciales, 
	const bool& minuscSinAcentos) 
	: _delimiters(delimitadoresPalabra),
	  _casosEspeciales(kcasosEspeciales),
	  _pasarAminuscSinAcentos(minuscSinAcentos)

{}

Tokenizador::Tokenizador (const Tokenizador& p_tk) {
	_delimiters = p_tk._delimiters;
	_casosEspeciales = p_tk._casosEspeciales;
	_pasarAminuscSinAcentos = p_tk._pasarAminuscSinAcentos;
}

Tokenizador::Tokenizador () {
	_delimiters = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
	_casosEspeciales = true;
	_pasarAminuscSinAcentos = false;
}

Tokenizador::~Tokenizador () {
	_delimiters = "";
	_casosEspeciales = false;
	_pasarAminuscSinAcentos = false;
}

Tokenizador& 
Tokenizador::operator= (const Tokenizador& p_tk) {
	_delimiters = p_tk._delimiters;
	_casosEspeciales = p_tk._casosEspeciales;
	_pasarAminuscSinAcentos = p_tk._pasarAminuscSinAcentos;
	return *this;
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
Tokenizador::Tokenizar (const std::string& p_i, const std::string& p_f) const {
	return false;
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

}

void 
Tokenizador::AnyadirDelimitadoresPalabra (const std::string& p_nuevoDelimiters) {

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
	p_os << "vacio";
	return p_os;
}