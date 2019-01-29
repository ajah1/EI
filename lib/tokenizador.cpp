
#include "../include/tokenizador.h"

#include <sys/stat.h>
#include <cstdlib>

using namespace std;

Tokenizador::Tokenizador (
	const std::string& delimitadoresPalabra,
	const bool& kcasosEspeciales, 
	const bool& minuscSinAcentos) {

}

Tokenizador::Tokenizador (const Tokenizador&) {

}

Tokenizador::Tokenizador () {

}

Tokenizador::~Tokenizador () {

}

Tokenizador& 
Tokenizador::operator= (const Tokenizador& p_tk) {
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
	return "";
}

void 
Tokenizador::CasosEspeciales (const bool& p_nuevoCasosEspeciales) {

}

bool 
Tokenizador::CasosEspeciales () {
	return false;
}

void 
Tokenizador::PasarAminuscSinAcentos (const bool& p_PasarAminuscSinAcentos) {

}

bool 
Tokenizador::PasarAminuscSinAcentos () {
	return false;
}

std::ostream& 
operator<< (std::ostream& p_os, const Tokenizador& p_tk)
{	
	p_os << "vacio";
	return p_os;
}