
#include "tokenizador.h"

#include <fstream>
#include <sys/stat.h>
#include <cstdlib>
#include <algorithm>

//////////////////////////////////////////////////////////////////////////////
////// 			    CONSTRUCTORES                                        ///// 
//////////////////////////////////////////////////////////////////////////////
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


//////////////////////////////////////////////////////////////////////////////
////// 						MAIL                                          ///// 
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
// COMPRUEBA SI P_CARACTER, PERTENECE A LA LISTA DE 
// DELIMITADORES QUE NO AFECTAN DENTRO DURANTE EL MAIL
/////////////////////////////////////////////////////////////
bool
Tokenizador::EsMailDelimiter(const char* p_caracter) const {
	return (_MAILdelimiters.find(p_caracter) != std::string::npos) || 
			(*p_caracter == ' ');
}
/////////////////////////////////////////////////////////////
// OBTIENE EL TOKEN MAIL Y HACE PUSH A LA LISTA DE TOKENS
/////////////////////////////////////////////////////////////
void
Tokenizador::MAIL(char*& p_izq, char*& p_der, std::list<std::string>& p_l) const {
	// Viene de Generico()
	if (p_izq != p_der){
		// Guardar la posicion del primer @
		char* pos_arroba = p_der++; // saltar el @

		bool parar = false;
		// iterar hasta encontrar un delimitador y obtenerString()
		while (!parar && *p_der != ' ' && *p_der != '\0') {
			if (*p_der == '@' && EsDelimiter('@')) {
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
	// Viene de Tokenizar()
	else if (p_izq == p_der && !EsDelimiter('@')) {
		bool parar = false;
		char* pos_aux = p_izq;
		while (!parar && *p_der != ' ' && *p_der != '\0') {
			p_der++;
		}
			p_l.push_back(ObtenerString(pos_aux, p_der));
	} else {
		p_der++; // Saltar el @ suelto
	}
}


//////////////////////////////////////////////////////////////////////////////
////// 					GENERICO                                          ///// 
//////////////////////////////////////////////////////////////////////////////
void
Tokenizador::Generico(char* &p_der, std::list<std::string>& p_tokens) const {
	//std::cout << "\n[LOG] Generico() \n";
	bool parar = false, token = false;
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
			!EsDelimiter(*(p_der-1)) &&
			!EsDelimiter(*(p_der+1))) {
				// Si pos+1 es un punto no es acronimo
				if (*(p_der+1) == '.') {
					p_der += 2;
				// LLamada a Acronimo()
				} else {
					Acronimo(pos_izq, p_der, p_tokens);
					parar = true;
				}
		} else if (*p_der == '-' &&
			!EsDelimiter(*(p_der-1)) &&
			!EsDelimiter(*(p_der+1))) {
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
		} else if (EsDelimiter(*p_der)) {
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
	bool token = false, parar = false;
	char* pos_der = nullptr;

	while (!parar) {
		// Casos para el .
		if (*p_der == '-') {
			// ...
			if (p_izq == p_der) {
				p_der++;
				parar = true;
			} // A.
			else if (!EsDelimiter(*(p_der-1))) {
				// A.
				if (EsDelimiter(*(p_der+1)) || *(p_der+1) == '-' && EsDelimiter('-')) {
					pos_der = p_der++;
					parar = token = true;
				// A.A
				} else { p_der += 1;}
			// ..A
			} else if (!EsDelimiter(*(p_der+1))) {
				p_der++;
				parar = true;
			}
		// Encuentra un delimitador para y muestra token
		} else if (EsDelimiter(*p_der) || *p_der == '\0') {
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
			else if (!EsDelimiter(*(p_der-1))) {
				// A.
				if (EsDelimiter(*(p_der+1)) || *(p_der+1) == '.') {
					pos_der = p_der++;
					parar = token = true;
				// A.A
				} else { p_der += 1;}
			// ..A
			} else if (!EsDelimiter(*(p_der+1))) {
				p_der++;
				parar = true;
			}
		// Encuentra un delimitador para y muestra token
		} else if (EsDelimiter(*p_der) || *p_der == '\0') {
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



//////////////////////////////////////////////////////////////////////////////
////// 						URL                                          ///// 
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
// COMPRUEBA SI P_INDICADOR, PERTENECE A LA LISTA DE 
// INDICADORES ACEPTADOS EN UNA URL (http, https, ...)
/////////////////////////////////////////////////////////////
bool
Tokenizador::EsURLIndicador(const std::string& p_indicador) const {
	return (_URLindc.find(p_indicador) != std::string::npos);
}
/////////////////////////////////////////////////////////////
// COMPRUEBA SI EL CARACTER ES UNO DE LOS CARÁCTERES QUE NO 
// PUEDEN DELIMITAR A UNA URL
/////////////////////////////////////////////////////////////
bool
Tokenizador::EsURLDelimiter(const char* p_caracter) const {
	return (_URLdelimiters.find(*p_caracter) != std::string::npos) || 
			(*p_caracter == ' ');
}
/////////////////////////////////////////////////////////////
// POSICIONA LOS PUNTEROS AL PRINCIPIO Y FINAL DEL TOKEN
// QUE FORMAN LA URL.
/////////////////////////////////////////////////////////////
void
Tokenizador::URL(char*& p_izq, char*& p_der, std::list<std::string>& p_l) const {
	// Solo hay una URL a obtener si el inicio y fin dado son dir. mem. distintas
	if (p_izq != p_der) {
		// IF: el indicador se encuntra a final de string o luego viene un _del
		if (*(p_der + 1) == '\0' && EsDelimiter(*p_der)) {
			// THEN: NO es una URL, devolver el token actual
			p_l.push_back(ObtenerString(p_izq, p_der));
		}
		// ELSE: el indicador es aceptado por la URL (http, https, ...)
		else if (EsURLIndicador(ObtenerString(p_izq, p_der-1))) { // -1 para no coger los :
			p_der++; // Saltar los :
			// Leer carácteres hasta encontrar un delimitador de la URL
			while (!EsURLDelimiter(p_der) && (*p_der != '\0')) {
				p_der++; // siguiente posición de memoria a leer
			}
			// Devolver el token de la URL
			p_l.push_back(ObtenerString(p_izq, p_der+1));
		}
		// Si no hay nada que hacer saltar el caracter
		p_der++;
	}
}




//////////////////////////////////////////////////////////////////////////////
////// 			TOKENIZADOR                                              ///// 
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//	Dado un String devuelve la lista de tokens
/////////////////////////////////////////////////////////////
void 
Tokenizador::Tokenizar (const std::string& p_str, std::list<std::string>& p_tokens) const {
	// Liberar memoria de la lista y poner size a cero
	p_tokens.clear();
	// Copia del String a tokenizar
	std::string s(p_str);
	// IF: pasar == true, eliminar antes de tokenizar
	if (_pasarAminuscSinAcentos) {
		s.assign(EliminarMinusAcentos(p_str));
	}
	// IF: CASOSESPECIALES == FALSE
	if (!_casosEspeciales) {
		TokenizarGeneral(s, p_tokens);
	// ELSE: TOKENIZAR CON LOS CASOS ESPECIALES
	} else if (!p_str.empty()) {
		TokenizarEspecial(s, p_tokens);
	}
}
/////////////////////////////////////////////////////////////
//	 
/////////////////////////////////////////////////////////////
bool
Tokenizador::Tokenizar (const std::string& p_fin, const std::string& p_fout) const {
	using namespace std;

	ifstream i;
	ofstream f;
	std::string cadena;
	std::list<std::string> tokens;

	i.open(p_fin.c_str());
	if (!i) {
		std::cerr << "ERROR: No existe el archivo: " << p_fin << std::endl;
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
	std::list<std::string>::iterator itS;
	for (itS= tokens.begin(); itS != tokens.end(); itS++) {
		f << (*itS) << std::endl;
	}

	f.close();
	
	return true;
}
/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
bool 
Tokenizador::Tokenizar (const std::string& p_i) const {
	return false;
}
/////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////
bool 
Tokenizador::TokenizarListaFicheros (const std::string& p_i) const {
	return false;
}
/////////////////////////////////////////////////////////////
// Tokeniza un directorio
/////////////////////////////////////////////////////////////
bool 
Tokenizador::TokenizarDirectorio (const std::string& p_dir) const {
	struct stat dir;

	// Compruebo la existencia del directorio
	int err = stat(p_dir.c_str(), &dir);

	if (err == -1 || !S_ISDIR(dir.st_mode))
		return false;
	else {
		// Hago una lista en un fichero con find>fich
		std::string cmd = "find " +p_dir+ " -follow |sort > .lista_fich";
		system(cmd.c_str());
		return TokenizarListaFicheros(".lista_fich");
	}
}

//////////////////////////////////////////////////////////////////////////////
////// 			FUNCIONES AUXILIARES                                     ///// 
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
// Tokeniza el string sin tener en cuenta los casos especiales
/////////////////////////////////////////////////////////////
void 
Tokenizador::TokenizarEspecial(std::string& s, std::list<std::string>& p_tokens) const {
	// Puntero a la primera posición del string
	char* it = &s.at(0);
	// Para no apuntar fuera del String
	bool primera_it = true;
	// Iterar String hasta llegar al final
	while (*it != '\0') {
		if (*it == '@') {							// MAIL
			MAIL(it, it, p_tokens);
		} else if (*it == ':') {					// URL
			URL(it, it, p_tokens);
		} else if (*it == '-' && !primera_it) {		// GUIONES
			Guion(it, it, p_tokens);
		} else if (*it == '.' && !primera_it 		// ACRONIMO
					&& !EsDelimiter(*(it-1)) 
					&& !EsDelimiter(*(it+1))) {
			Acronimo(it, it, p_tokens);
		} else {									// NO DETECTA ESPECIALES
			Generico(it, p_tokens);
		}
		primera_it = false;
	}
}
/////////////////////////////////////////////////////////////
// Tokeniza el string sin tener en cuenta los casos especiales
/////////////////////////////////////////////////////////////
void 
Tokenizador::TokenizarGeneral(std::string& s, std::list<std::string>& p_tokens) const {
	using namespace std;
	string::size_type lastPos 	= s.find_first_not_of(_delimiters, 0);
	string::size_type pos 		= s.find_first_of(_delimiters, lastPos);

	while(string::npos != pos || string::npos != lastPos) {
		p_tokens.push_back(s.substr(lastPos, pos - lastPos));
		lastPos = s.find_first_not_of(_delimiters, pos);
		pos 	= s.find_first_of(_delimiters, lastPos);
	}
}
/////////////////////////////////////////////////////////////
// DEVUELVE EL STRING QUE SE ECUENTRA ENTRE
// LAS POSICÍON DE MEMORIA P_I y P_F
/////////////////////////////////////////////////////////////
std::string
Tokenizador::ObtenerString(const char* p_i, const char* p_f) const {
	std::string s_out;
	// IF: inicio y fin apuntan a la misma dir.
	if (p_i != p_f) {
		// Asignar al tring (p_f-p_i) B's desde p_i
		s_out.assign(p_i, (p_f - p_i));
	// THEN: Asignar el string 1B desde la p_i
	} else {
		s_out.assign(p_i, 1);
	}
	return s_out;
}
/////////////////////////////////////////////////////////////
// ELIMINA LOS ACENTOS DE LAS VOCALES a, e, i, o, u
// Y PASA A MINÚSCULAS LOS CARACTERES
/////////////////////////////////////////////////////////////
std::string
Tokenizador::EliminarMinusAcentos (const std::string& p_str) const {
	std::string str_out = p_str;
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
/////////////////////////////////////////////////////////////
// COMPRUEBA SI EL CARACTER P_D ES UN DELIMITADOR (_delimiter)
// ESTA FUNCION SOLO SE USA EN LOS CASOS ESPECIALES. ADEMÁS
// DE LOS DELIMITADORES COMPRUEBA EL ESPACIO (' ')
/////////////////////////////////////////////////////////////
bool
Tokenizador::EsDelimiter(const char p_d) const{
	bool es_delimitador = false;
	// IF: _delimiters NO es cadena VACIA
	if (_delimiters.length() > 0) {
		for (const char* d = &_delimiters.at(0); *d != '\0'; ++d) {
			if (p_d == *d || p_d == ' ') {
				es_delimitador = true;
			}
		}
	// ELSE: como _delimiters es vacio, solo comprobar el espacio
	}else {
		es_delimitador = (p_d == ' ');
	} 	

	return es_delimitador;
}


//////////////////////////////////////////////////////////////////////////////
////// 			     GETTERS-SETTERS                                     ///// 
//////////////////////////////////////////////////////////////////////////////
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
void 
Tokenizador::DelimitadoresPalabra (const std::string& p_nuevoDelimiters) {
	_delimiters = "";
	for (char d : p_nuevoDelimiters) {
		if (_delimiters.find_first_of(d) == std::string::npos) {
			_delimiters += d;
		}
	}
}
void
Tokenizador::AnyadirDelimitadoresPalabra (const std::string& p_newDel) {
	std::string aux = p_newDel;
	aux.erase(std::unique(aux.begin(), aux.end()), aux.end());
	for (char s : aux) {
		if (_delimiters.find(s) == std::string::npos) {
			_delimiters += s;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////
////// 			SOBRECARGA DE OPERADORES                                 ///// 
//////////////////////////////////////////////////////////////////////////////
std::ostream& 
operator<< (std::ostream& p_os, const Tokenizador& p_tk)
{	
	p_os << "DELIMITADORES: " 					  << p_tk._delimitersAux 
		 << " TRATA CASOS ESPECIALES: " 		  << p_tk._casosEspeciales 
		 << " PASAR A MINUSCULAS Y SIN ACENTOS: " << p_tk._pasarAminuscSinAcentos;

	return p_os;
}