
#include "../include/tokenizador.h"

#include <fstream>

#include <sys/stat.h>
#include <cstdlib>

using namespace std;

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


std::string
Tokenizador::EliminarMinusAcentos (const std::string& p_str) const{

	//std::clog << "\n [LOG] EliminarMinusAcentos \n";

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

	string str_out = p_str;
	for (int i = 0; i < p_str.length(); ++i) {
		if(str_out[i] == (char)192 || str_out[i] == (char)193 || str_out[i] == (char)224 || str_out[i] == (char)225) {
			str_out[i] = 'a';
		} else if(str_out[i] == (char)200 || str_out[i]== (char)201 || str_out[i] == (char)232 || str_out[i] == (char)233) {
			str_out[i] = 'e';
		}else if(str_out[i] == (char)204 || str_out[i] == (char)205 || str_out[i] == (char)236 || str_out[i] == (char)237) {
			str_out[i] = 'i';
		}else if(str_out[i] == (char)210 || str_out[i] == (char)211 || str_out[i] == (char)242 || str_out[i] == (char)243) {
			str_out[i] = 'o';
		}else if(str_out[i] == (char)217 || str_out[i] == (char)218 || str_out[i] == (char)249 || str_out[i] == (char)250) {
			str_out[i] = 'u';
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
	} else {
		es_delimitador = (p_d == ' ');
	} 	

	return es_delimitador;
}

void 
Tokenizador::Tokenizar (const std::string& p_str, std::list<std::string>& p_tokens) const {
	
	p_tokens.clear();

	std::string s(p_str);

	if (_pasarAminuscSinAcentos) {
		s = EliminarMinusAcentos(p_str);
	}

	if (!_casosEspeciales) {
		//sstd::cout << "\n[LOG] Tokenizar sin especiales\n";
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
			//std::clog << "\n[B_MAIN] *it: " <<  *it 
			//		  << " *(it+1): " 			<< *(it+1);

			if (*it == '.' && 
				!primera_it &&
				!SDelimitador(*(it-1)) &&
				!SDelimitador(*(it+1))) {
					Acronimo(it, it, p_tokens);
			} else {
				Generico(it, p_tokens);
			}

			primera_it = false;

			//std::cout << "-------------------------" << std::endl;
			//cin.get();
		}
	}
}

void
Tokenizador::Generico(char* &p_der, std::list<std::string>& p_tokens) const {
	//std::clog << "\n[LOG] Generico() \n";

	bool parar = false;
	bool token = false;
	char* pos_izq = p_der;

	// "U.S.A.U.S.A .p1"
	while (!parar) {
		//std::cout << "Iterar: ->" << *p_der << "<-" << std::endl;
		// Si a medias encuentra un caso especial
		if (*p_der == '.' &&
			!SDelimitador(*(p_der-1)) &&
			!SDelimitador(*(p_der+1))) {
				if (*(p_der+1) == '.') {
					p_der += 2;
				} else {
					Acronimo(pos_izq, p_der, p_tokens);
					parar = true;
					token = false;
				}
		// En otro caso: comprobar delimitadores
		} else if (SDelimitador(*p_der)) {
			parar = token = true;
			p_der++;
		// En otro caso: final de string
		} else if (*p_der == '\0') {
			parar = token = true;
			p_der++;
		}
		else {
			p_der++;
		}
	}

	if (token && (p_der - pos_izq) > 1) {
		p_tokens.push_back(ObtenerString(pos_izq, p_der-1));
		/*std::cout << "Generico:-->" 
				  << ObtenerString(pos_izq, p_der-1) 
				  << "<--\n";*/
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

// "U..S.A p1 e..g. p2. La"

void
Tokenizador::Acronimo(char* &p_izq, char* &p_der, std::list<std::string>& p_tokens) const {
	//std::cout << "\n[LOG] Acronimo() \n";

	bool token = false;
	bool parar = false;

	char* pos_der = nullptr;

	while (!parar) {
		//std::cout << "*izq: " 	<< *p_izq 
		//		  << "  *der: " << *p_der 
		//		  << std::endl;

		// casos para el .
		if (*p_der == '.') {
			if (p_izq == p_der) {
				p_der++;
				parar = true;
				token = false;
			}
			// A.
			else if (!SDelimitador(*(p_der-1))) {
				// A.
				if (SDelimitador(*(p_der+1)) || *(p_der+1) == '.') {
					pos_der = p_der;
					parar = token = true;
					p_der++;
					/*std::cout << "pizq: " << *p_izq;
					std::cout << "\npder: " << *pos_der;
					std::cout << "\npder+1: " << *(p_der+1) << "\n";*/
				// A.A
				} else {
					//std::cout << "A.A" << std::endl;
					p_der += 1;
				}
			// *.
			} else {
				// ..A
				if (!SDelimitador(*(p_der+1))) {
				//	std::cout << ".A" << std::endl;
					p_der++;
					parar = true;
					token = false;
				}
			}
		// cuando encuentra un delimitador para y muestra token
		} else if (SDelimitador(*p_der) || *p_der == '\0') {
			//std::cout << "encuentra delimitadores" << std::endl;
			pos_der = p_der;
			parar = token = true;
		}
		// seguir incrementando
		else {
			// "U..S.A p1 e..g. p2. La"
			//std::cout << "nada" << std::endl;
			p_der++;
		}
	}

	if (token) {
		p_tokens.push_back(ObtenerString(p_izq, pos_der));
		//std::cout << "Acronimo:-->" 
		//	  << ObtenerString(p_izq, pos_der)
		//	  << "<--\n";
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