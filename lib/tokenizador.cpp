
#include "../include/tokenizador.h"

#include <fstream>

#include <sys/stat.h>
#include <cstdlib>

#include <algorithm>

using namespace std;

int str_size = 0;

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

Tokenizador::Tokenizador (const std::string& p_del) {
	_delimiters = p_del;
	_casosEspeciales = true;
	_pasarAminuscSinAcentos = false;
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


std::string
Tokenizador::EliminarMinusAcentos (const std::string& p_str) const {
	string str_out = p_str;
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
	
	return str_out;
}

bool
Tokenizador::SDelimitador(const char p_d) const{
	bool es_delimitador = false;

	if (_delimiters.length() > 0) {
		for (const char* d = &_delimiters.at(0); *d != '\0'; ++d) {
			if (p_d == *d || p_d == ' ') {
				es_delimitador = true;
			}
		}
	}else {
		es_delimitador = (p_d == ' ');
	} 	

	return es_delimitador;
}

void 
Tokenizador::Tokenizar (const std::string& p_str, std::list<std::string>& p_tokens) const {
	
	//p_tokens.clear();
	for (int i = 0; i < p_tokens.size(); ++i) {
		p_tokens.pop_back();
	}
	p_tokens.clear();

	std::string s;

	if (_pasarAminuscSinAcentos) {
		s.assign(EliminarMinusAcentos(p_str));
	} else {
		s.assign(p_str);
	}

	int s_size = s.length();

	if (!_casosEspeciales) {
		string::size_type lastPos 	= s.find_first_not_of(_delimiters, 0);
		string::size_type pos 		= s.find_first_of(_delimiters, lastPos);

		while(string::npos != pos || string::npos != lastPos) {
			p_tokens.push_back(s.substr(lastPos, pos - lastPos));
			lastPos = s.find_first_not_of(_delimiters, pos);
			pos 	= s.find_first_of(_delimiters, lastPos);
		}
	} else {
		char* it = &s.at(0);

		// Para no salir del string por la izq
		bool primera_it = true;

		while (*it != '\0') {
			//std::cout << ":ASDASD";
			if (*it == '@') {
				MAIL(it, it, p_tokens);
			}
			else if (*it == ':') {
				URL(it, it, p_tokens);
			} else if (*it == '-' && !primera_it) {
					Guion(it, it, p_tokens); 
			} else if (*it == '.' && 
				!primera_it &&
				!SDelimitador(*(it-1)) &&
				!SDelimitador(*(it+1))) {
					Acronimo(it, it, p_tokens);
			} else {
				Generico(it, p_tokens);
			}

			primera_it = false;
		}
	}
}

bool
Tokenizador::esMailDelimiter(const char* p_caracter) const {
	return (_maildelimiters.find(p_caracter) != std::string::npos) || 
			(*p_caracter == ' ');
}

void
Tokenizador::MAIL(char* &p_izq, char* &p_der, std::list<string>& p_l) const {
	
	//pal1 @iuii.ua.es p1 p2

	/*
		izq != der => Generico()
			iterar hasta encontrar un delimitador y obtenerString()
		ELSE => Main() => devolver un token norma
			¿@ es _delimiter?
				SI => no incluir el @
				NO => incluir el @
	*/

	//pal1 @iuii.ua.es p1 p2

	if (p_izq != p_der){
		// Guardar la posicion del primer @
		char* pos_arroba = p_der++; // saltar el @

		bool parar = false;
		while (!parar && *p_der != ' ' && *p_der != '\0') {
			//std::cout << "mizq:->" << *p_izq << "<-mder: " << *p_der << "<-" <<std::endl;
			if (*p_der == '@' && SDelimitador('@')) {
				p_l.push_back(ObtenerString(p_izq, pos_arroba));
				p_der = pos_arroba + 1;
				parar = true;
			} else {
				p_der++;
			}
		}
		if (!parar)
			p_l.push_back(ObtenerString(p_izq, p_der));
		 
	} 
	else if (p_izq == p_der && !SDelimitador('@')) {
		//std::cout << "entra == " << std::endl;
		bool parar = false;
		char* pos_aux = p_izq;
		while (!parar && *p_der != ' ' && *p_der != '\0') {
		//	std::cout << "==izq:->" << *p_izq << "<-==der: " << *p_der << "<-" <<std::endl;
			p_der++;
		}
			p_l.push_back(ObtenerString(pos_aux, p_der));
	}

	else {
		p_der++; // Saltar el @ suelto
	}
}


bool
Tokenizador::EsIndicador(const std::string& p_indicador) const {
	return (_indicadores.find(p_indicador) != std::string::npos);
}

bool
Tokenizador::esURLDelimiter(const char* p_caracter) const {
	return (_URLdelimiters.find(p_caracter) != std::string::npos) || 
			(*p_caracter == ' ');
}

void
Tokenizador::URL(char* &p_izq, char* &p_der, std::list<string>& p_l) const {

	// IF p_izq != p_der THEN viene de generico()
	if (p_izq != p_der) {
		// comprobar el caso "http: otra" ó "http:"
		if (*(p_der + 1) == '\0' && SDelimitador(*p_der)) {
			p_l.push_back(ObtenerString(p_izq, p_der));
		}
		// SI el indicador es correcto (http, https, ...)
		else if (EsIndicador(ObtenerString(p_izq, p_der-1))) { // -1 para no coger los :
			p_der++; // Saltar los :

			while (!esURLDelimiter(p_der) && (*p_der != '\0')) {
				p_der++; // seguir iterando el string
			}
			p_l.push_back(ObtenerString(p_izq, p_der+1));
		}
		
		p_der++;
	}
}

void
Tokenizador::Generico(char* &p_der, std::list<std::string>& p_tokens) const {
	//std::cout << "\n[LOG] Generico() \n";
	bool parar = false;
	bool token = false;
	char* pos_izq = p_der;

	while (!parar) {
		//std::cout << "Gp_izq->" << *pos_izq << "<- Gp_der:->" << *p_der << "<-"<< std::endl;

		if (*p_der == ':') {
			URL(pos_izq, p_der, p_tokens);
			parar = true;
		} else if (*p_der == '@') {
			MAIL(pos_izq, p_der, p_tokens);
			parar = true;
					// En otro caso: encuntra un delimitador ó \0
		} else if (*p_der == '.' &&
			!SDelimitador(*(p_der-1)) &&
			!SDelimitador(*(p_der+1))) {
				// Si pos+1 es un punto no es acronimo
				if (*(p_der+1) == '.') {
					p_der += 2;
				// LLamada a Acronimo()
				} else {
					Acronimo(pos_izq, p_der, p_tokens);
					parar = true;
				}
		} else if (*p_der == '-' &&
			!SDelimitador(*(p_der-1)) &&
			!SDelimitador(*(p_der+1))) {
				// Si pos+1 es un punto no es acronimo
				if (*(p_der+1) == '-') {
					p_der += 2;
				// LLamada a Acronimo()
				} else {
					Guion(pos_izq, p_der, p_tokens);
					parar = true;
				}
		// En otro caso: encuntra un delimitador ó \0
		//std::cout << "Encuentra el delimitador:-->" << *p_der << "<--" << std::endl;
		} else if (SDelimitador(*p_der)) {
			parar = token = true;
			p_der++;
		} else if (*p_der == '\0') {
			parar = true;
			p_tokens.push_back(ObtenerString(pos_izq, p_der));
		// ELSE: seguir iterando
		} else {
			p_der++;
		}
	}

	if (token && (p_der - pos_izq) > 1) {
		p_tokens.push_back(ObtenerString(pos_izq, p_der-1));
	}
}

void
Tokenizador::Guion(char* &p_izq, char* &p_der, std::list<std::string>& p_tokens) const {
	//std::cout << "\n[LOG] Acronimo() \n";
	bool token = false;
	bool parar = false;

	char* pos_der = nullptr;

	while (!parar) {
		// Casos para el .
		if (*p_der == '-') {
			// ...
			if (p_izq == p_der) {
				p_der++;
				parar = true;
			} // A.
			else if (!SDelimitador(*(p_der-1))) {
				// A.
				if (SDelimitador(*(p_der+1)) || *(p_der+1) == '-' && SDelimitador('-')) {
					pos_der = p_der++;
					parar = token = true;
				// A.A
				} else { p_der += 1;}
			// ..A
			} else if (!SDelimitador(*(p_der+1))) {
				p_der++;
				parar = true;
			}
		// Encuentra un delimitador para y muestra token
		} else if (SDelimitador(*p_der) || *p_der == '\0') {
			pos_der = p_der;
			parar = token = true;
		// seguir iterando
		} else {
			p_der++;
		}
	}

	if (token) {
		p_tokens.push_back(ObtenerString(p_izq, pos_der));
	}
}

void
Tokenizador::Acronimo(char* &p_izq, char* &p_der, std::list<std::string>& p_tokens) const {
	//std::cout << "\n[LOG] Acronimo() \n";
	bool token = false;
	bool parar = false;

	char* pos_der = nullptr;

	while (!parar) {
		// Casos para el .
		if (*p_der == '.') {
			// ...
			if (p_izq == p_der) {
				p_der++;
				parar = true;
			} // A.
			else if (!SDelimitador(*(p_der-1))) {
				// A.
				if (SDelimitador(*(p_der+1)) || *(p_der+1) == '.') {
					pos_der = p_der++;
					parar = token = true;
				// A.A
				} else { p_der += 1;}
			// ..A
			} else if (!SDelimitador(*(p_der+1))) {
				p_der++;
				parar = true;
			}
		// Encuentra un delimitador para y muestra token
		} else if (SDelimitador(*p_der) || *p_der == '\0') {
			pos_der = p_der;
			parar = token = true;
		// seguir iterando
		} else {
			p_der++;
		}
	}

	if (token) {
		p_tokens.push_back(ObtenerString(p_izq, pos_der));
	}
}

std::string
Tokenizador::ObtenerString(const char* p_i, const char* p_f) const {
	std::string s_out;

	if (p_i == p_f) {
		s_out.assign(p_i, 1);
	} else {
		s_out.assign(p_i, (p_f - p_i));
	}

	return s_out;
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
Tokenizador::AnyadirDelimitadoresPalabra (const std::string& p_newDel) {
	std::string aux = p_newDel;

	// Eliminar caracteres repetidos
	aux.erase(std::unique(aux.begin(), aux.end()), aux.end());

	// Anyadir los caracter que no son delimitadores
	for (char s : aux) {
		if (_delimiters.find(s) == string::npos) {
			_delimiters += s;
		}
	}
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
		 << " TRATA CASOS ESPECIALES: " 			 << p_tk._casosEspeciales 
		 << " PASAR A MINUSCULAS Y SIN ACENTOS: " << p_tk._pasarAminuscSinAcentos;

	return p_os;
}