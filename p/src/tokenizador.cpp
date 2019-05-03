#include "tokenizador.h"

#include <fstream>
#include <sys/stat.h>
#include <algorithm>
#include <sys/mman.h> 
#include <fcntl.h>
#include <cstring>

std::list<std::string>* PTokens = nullptr;

//////////////////////////////////////////////////////////////////////////////
////// 			    CONSTRUCTORES                                        ///// 
//////////////////////////////////////////////////////////////////////////////
Tokenizador::Tokenizador (
	const std::string& delimitadoresPalabra,
	const bool& kcasosEspeciales, 
	const bool& minuscSinAcentos)
	: 	_delimiters(delimitadoresPalabra),
		_casosEspeciales(kcasosEspeciales),
		_pasarAminuscSinAcentos(minuscSinAcentos),
	  	_delimitersAux(delimitadoresPalabra),
	  	_delimitersLength(_delimiters.length())

{}
Tokenizador::Tokenizador (const Tokenizador& p_tk) {
	_delimiters = p_tk._delimiters;
	_delimitersAux = p_tk._delimitersAux;
	_casosEspeciales = p_tk._casosEspeciales;
	_pasarAminuscSinAcentos = p_tk._pasarAminuscSinAcentos;
	_delimitersLength = _delimiters.length();
}
Tokenizador::Tokenizador (const std::string& p_del) {
	_delimiters = p_del;
	_casosEspeciales = false;
	_pasarAminuscSinAcentos = false;
}
Tokenizador::Tokenizador () {
	_delimiters = ",;:.-/+*\\ '\"{}[]()<>�!�?&#=\t\n\r@";
	_casosEspeciales = true;
	_pasarAminuscSinAcentos = false;
}
Tokenizador::~Tokenizador () {
	/*_delimitersAux = "";
	_delimiters = "";
	_casosEspeciales = false;
	_pasarAminuscSinAcentos = false;*/
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
////// 					GENERICO                                          ///// 
//////////////////////////////////////////////////////////////////////////////
void
Tokenizador::Generico(char*& p_der) const {
	bool parar = false;
	char* pos_izq = p_der;
	//std::cout << "========> GENERICO\n";
	while (!parar) {
	//	std::cout << "Gp_izq->" << *pos_izq << "<- Gp_der:->" << *p_der << "<-"<< std::endl;
		parar = true;

		if (*p_der == ':') {
			URL(pos_izq, p_der);
		} else if (*p_der == '@') {
			MAIL(pos_izq, p_der);
		} else if (*p_der == '.' || *p_der == ',') {
			Punto(pos_izq, p_der);
		} else if (*p_der == '-') {
			Guion(pos_izq, p_der);
		} else if (EsDelimiter(*p_der)) {
			++p_der; // Saltar el delimitador
			if ((p_der - pos_izq) > 1)
				PTokens->push_back(ObtenerString(pos_izq,p_der-1));
		} else if (*p_der == '\0') {
			PTokens->push_back(ObtenerString(pos_izq,p_der));
		// ELSE: seguir iterando
		} else {
			parar = false;
			++p_der;
		}
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
// COMPRUEBA SI EL CARACTER ES DELIMITADOR Y NO SE ENCUENTRA
// EN LAS EXCEPCIONES DE LA URL
/////////////////////////////////////////////////////////////
bool
Tokenizador::EsURLDelimiter( char*& p_caracter) const {
	return (*p_caracter == ' ') || 
		(_delimiters.find(*p_caracter) != std::string::npos
		//(EsDelimiter(*p_caracter)
			&& _URLdelimiters.find(*p_caracter) == std::string::npos);
}
/////////////////////////////////////////////////////////////
// POSICIONA LOS PUNTEROS AL PRINCIPIO Y FINAL DEL TOKEN
// QUE FORMAN LA URL.
/////////////////////////////////////////////////////////////
void
Tokenizador::URL(char*& p_izq, char*& p_der) const {
	// Solo hay una URL a obtener si el inicio y fin dado son dir. mem. distintas
	if (p_izq != p_der) {
		if (EsDelimiter(*p_der+1) || *(p_der+1) == '\0') { 
			if (EsDelimiter(*p_der)) {
				//StoreToken(p_izq, p_der);
				PTokens->push_back(ObtenerString(p_izq,p_der));
				return;
			}
			PTokens->push_back(ObtenerString(p_izq,p_der+1));
			++p_der;
		} else if (EsURLIndicador(ObtenerString(p_izq, p_der))){
			while (*p_der != '\0' && !EsURLDelimiter(p_der)) {
				++p_der;
			}
			PTokens->push_back(ObtenerString(p_izq,p_der));
		} else {
			PTokens->push_back(ObtenerString(p_izq,p_der+1));
		}
	} else {
		++p_der;	
	}
}

//////////////////////////////////////////////////////////////////////////////
////// 						MAIL                                          ///// 
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
// OBTIENE EL TOKEN MAIL Y HACE PUSH A LA LISTA DE TOKENS
/////////////////////////////////////////////////////////////
void
Tokenizador::MAIL(char*& p_izq, char*& p_der) const {
	if (p_izq != p_der){
		// Guardar la posicion del primer @
		char* pos_arroba = p_der++; // saltar el @
		bool parar = false;
		// iterar hasta encontrar un delimitador y obtenerString()
		while (!parar && *p_der != ' ' && *p_der != '\0') {
			if (*p_der != '@' || !EsDelimiter('@')) {
				++p_der;
			} else {
				parar = true;
			}
		}
		if (!parar)
			PTokens->push_back(ObtenerString(p_izq, p_der));
		else {
			PTokens->push_back(ObtenerString(p_izq, pos_arroba));
			PTokens->push_back(ObtenerString(pos_arroba+1, p_der));
			++p_der; // Saltar el segundo @
		}
	}
	// Viene de Tokenizar()
	else if (p_izq == p_der && !EsDelimiter('@')) {
		//bool parar = false;
		char* pos_aux = p_izq;
		while (*p_der != ' ' && *p_der != '\0') {
			++p_der;
		}
		PTokens->push_back(ObtenerString(pos_aux, p_der));
	} else {
		++p_der; // Saltar el @ suelto
	}
}

bool
Tokenizador::EsAcronimoDel(char* p) const{
	return (*p == '.') || EsDelimiter(*p);
}
void
Tokenizador::AcronimoAux1(char* &p_izq, char*& p_der) const {
	if (EsDelimiter(*(p_der+1)) || EsDelimiter(*(p_der-1))) {
		//std::cout << "Ap_izq->" << *p_izq << "<- Ap_der+1:->" << *(p_der+1) << "<-"<< std::endl;
		//std::cout << "no es acronimo\n";
		PTokens->push_back(ObtenerString(p_izq, p_der++));
	// Si es acronimo :D seguir iterando"
	} else {
		//std::cout << "ES acronimo\n";
		++p_der; // Saltar el .
		bool parar = false;
		while (!parar && *p_der != '\0') {
			//std::cout << "<- Ap_der:->" << *p_der << "<-"<< std::endl;
			parar = ((*p_der == '.' && EsAcronimoDel(p_der+1))) || *p_der == ' ';
			++p_der;
		}
		PTokens->push_back(ObtenerString(p_izq, p_der-1));
	}
}

void
Tokenizador::AcronimoAux2(char* &p_izq, char*& p_der) const {
	if (EsDelimiter(*(p_der-1)) || EsDelimiter(*(p_der+1))) {
		//StoreToken(p_izq, ++p_der); // Saltar el caracter q
		PTokens->push_back(ObtenerString(p_izq, ++p_der));
	} else {
		++p_der; // Saltar el .
		bool parar = false;
		while (!parar && *p_der != '\0') {
			parar = ((*p_der == '.' && EsDelimiter(*(p_der+1)))) || *p_der == ' ';
			++p_der;
		}
		if (*(p_der-1) == ' ')
			PTokens->push_back(ObtenerString(p_izq, p_der-1));
		else
			PTokens->push_back(ObtenerString(p_izq, p_der));
	}
}

void
Tokenizador::Acronimo(char*& p_izq, char*& p_der) const {
	if (p_izq != p_der) {
		if (EsDelimiter('.'))
			AcronimoAux1(p_izq, p_der);
		else
			AcronimoAux2(p_izq, p_der);
	} 
	else if (*(p_der+1) == '.' || EsDelimiter('.')){
		++p_der;
	} 
	else {
		char* aux = p_izq;
		p_der+=3;
		PTokens->push_back(ObtenerString(aux,p_der));
	}
}

//////////////////////////////////////////////////////////////////////////////
////// 						Guion                                        ///// 
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
void
Tokenizador::Guion(char*& p_izq, char*& p_der) const {
	if (!EsDelimiter('-')) {
		GuionAux1(p_izq, p_der);
	}
	else
		GuionAux2(p_izq, p_der);
}
/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
void Tokenizador::GuionAux1(char* &p_izq, char* &p_der) const {
	if (p_izq == p_der)
		++p_der;

	while (!EsDelimiter(*p_der) && *p_der != '\0') {
		++p_der;
	}
	PTokens->push_back(ObtenerString(p_izq,p_der));
}
/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
void Tokenizador::GuionAux2(char* &p_izq, char* &p_der) const {
	// EL guion es forma una palabra compuesta?
	if (EsDelimiter(*(p_der-1)) || EsDelimiter(*(p_der+1))) {
		++p_der;  // Dado que no es compuesta, devolver token
	} else {
		bool parar = false;
		while (!parar && *p_der != ' ' && *p_der != '\0') {
			if (*p_der == '-' && (EsDelimiter(*(p_der-1)) || EsDelimiter(*(p_der+1)))) {
				parar = true;
			} else if (*p_der != '-' && EsDelimiter(*p_der)) {
				parar = true;
			} else {
				++p_der;
			}
		}
		PTokens->push_back(ObtenerString(p_izq,p_der));
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
		EliminarMinusAcentos(s);
	}
	// IF: CASOSESPECIALES == FALSE
	if (!_casosEspeciales) {
		TokenizarGeneral(s, p_tokens);
	// ELSE: TOKENIZAR CON LOS CASOS ESPECIALES
	} else if (!p_str.empty()) {
		TokenizarEspecial(s, p_tokens);
	}
}
////////////////////////////////////////////////////////////////////
// Tokeniza el fichero i guardando la salida en el fichero f (una
// palabra en cada l�nea del fichero). Devolver� true si se realiza
////////////////////////////////////////////////////////////////////
bool
Tokenizador::Tokenizar (std::string& p_fin, std::string& p_fout) const {
	std::list<std::string> tokens;

    struct stat s;
    const char* file_name = p_fin.c_str();
    int fd = open (file_name, O_RDONLY);
    /* Get the size of the file. */
    fstat (fd, &s);
    int size = s.st_size;
    char* f = (char *) mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);
	
	// Abrir fichero de salida
 	std::ofstream out(p_fout);
 	
 	char p = '\n';
    for (int i = 0; i < size; ++i) {

    	char* pendline = f+i;
    	while (*pendline != '\n') {
    		++pendline;	
    	}

    	std::string Stokenizar = ObtenerString(f+i, pendline+1);
    	tokens.clear();
    	//if (_casosEspeciales) 
		// EliminarMinusAcentos(Stokenizar);
    	//TokenizarEspecial( Stokenizar, tokens );
    	TokenizarGeneral(Stokenizar, tokens);
    	
		for (std::string s : tokens) {
			char* buffer = &s.at(0);
			out.write(buffer, s.size());
			out.write(&p, 1);
		}

    	i += pendline - (f+i);
    }

   	out.close();

	return true;
}
////////////////////////////////////////////////////////////////////////////
// Tokeniza el fichero i guardando la salida en un fichero de nombre i
// a�adi�ndole extensi�n .tk (sin eliminar previamente la extensi�n de i
// por ejemplo, del archivo pp.txt se generar�a el resultado en pp.txt.tk),
////////////////////////////////////////////////////////////////////////////
bool
Tokenizador::Tokenizar (const std::string& p_fin) const {
	std::string in(p_fin);
	std::string out(p_fin+".tk");
	return Tokenizar(in, out);
}
/////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////
bool 
Tokenizador::TokenizarListaFicheros (const std::string& p_i) const {
	std::string s = p_i;
    FILE* fp = fopen(s.c_str(), "r");
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1) {
       line[read-1] = 0;
       std::string in = line;
       std::string out = in+".tk";
       Tokenizar(in, out);
    }

    fclose(fp);
    if (line) free(line);

	return true;
}
/////////////////////////////////////////////////////////////
// Tokeniza un directorio, incluyendo los subdirectorios
/////////////////////////////////////////////////////////////
bool 
Tokenizador::TokenizarDirectorio (const std::string& p_dir) const {
	//struct stat dir;
	//int err = stat(p_dir.c_str(), &dir);
	std::string cmd = "find " +p_dir+ " -follow |sort > .lista_fich";
	system(cmd.c_str());
	return TokenizarListaFicheros(".lista_fich");
}

//////////////////////////////////////////////////////////////////////////////
////// 			NUMEROS                                                  ///// 
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
// caso especial para los decimales
/////////////////////////////////////////////////////////////
void 
Tokenizador::Decimal(char*& p_izq, char*& p_der) const {
	//	std::clog << "DECIMAL" << std::endl;
	// EL . NO es primer caracter
	if 	(p_izq == p_der) { DecimalAux2(p_izq, p_der); }
	// Controlar el caso donde se anyade 0[,.] 
	else 				 { DecimalAux1(p_izq, p_der); }
}
/////////////////////////////////////////////////////////////
// Distinta posici�n de memoria
/////////////////////////////////////////////////////////////
void 
Tokenizador::DecimalAux1(char*& p_izq, char*& p_der) const {
	++p_der;
	bool parar = false;
	while(*p_der != '\0' && *p_der != ' ' && !parar) {
		parar = true;
		char c = *p_der;
		if (c == '.' || c == ',') {
			if (*(p_der+1) == ' ') {
				PTokens->push_back(ObtenerString(p_izq,p_der-1));
				++p_der;
			} else if (!isdigit(*(p_der+1))){
				Acronimo(p_izq, p_der);
			} else { parar = false; ++p_der; }
		} else if (*p_der == 'E' && EsDelimiter(',')) {
			PTokens->push_back(ObtenerString(p_izq,p_izq));
			p_der = p_izq+2;
		} else if (EsDelimiter(*p_der)){
			PTokens->push_back(ObtenerString(p_izq,p_der));
			++p_der;
		}else { parar = false; ++p_der; }
	}

	if (!parar)
		PTokens->push_back(ObtenerString(p_izq,p_der));
}
/////////////////////////////////////////////////////////////
// Misma posici�n de memoria
/////////////////////////////////////////////////////////////
void 
Tokenizador::DecimalAux2(char*& p_izq, char*& p_der) const {
	//std::clog << "==========>DECIMALAux2" << std::endl;
	char* pizq = p_izq;
	bool parar = false;
	while(*p_der != '\0' && *p_der != ' ' && !parar) {
		parar = true;
		char c = *p_der;
		if (c == '.' || c == ',') {
			if (*(p_der+1) == ' ') {
				PTokens->push_back("0" + ObtenerString(pizq,p_der-1));
				++p_der;
				break;
			} else if (*(p_der+1) < '0' || *(p_der+1) > '9'){
				Acronimo(++pizq, p_der);
				break;
			} else { parar = false; ++p_der; }
		} else if (EsDelimiter(*p_der)) {
			PTokens->push_back("0" + ObtenerString(pizq,p_der));
			++p_der;
			break;
		}else { parar = false; ++p_der; }
	}
	if (!parar)
		PTokens->push_back("0" + ObtenerString(pizq,p_der));
}


//////////////////////////////////////////////////////////////////////////////
////// 			FUNCIONES AUXILIARES                                     ///// 
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
// Comprueba a que caso especial pertenece el . leido
/////////////////////////////////////////////////////////////
void
Tokenizador::Punto(char*& p_izq, char*& p_der) const {
//	std::cout << "========> PUNTOOOO\n";
//	std::cout << "<- Pp_der:->" << *p_der << "<- Pp_der+1:->" << *(p_der+1) << "<-"<< std::endl;
	// IF: es un token decimal
	//if (is_digit(*(p_der+1))) {
	char c = *(p_der+1);
	if (c >= '0' && c <='9'){
		//std::cout << "Numero" << std::endl;
		Decimal(p_izq, p_der);
	//	std::cout << "Acronimo\n";
	}else Acronimo(p_izq, p_der);
}
/////////////////////////////////////////////////////////////
// Tokeniza el string sin tener en cuenta los casos especiales
/////////////////////////////////////////////////////////////
void 
Tokenizador::TokenizarEspecial(std::string& s, std::list<std::string>& p_tokens) const {
	// Puntero a la lista de tokens
	PTokens = &p_tokens;
	// Puntero a la primera posici�n del string
	char* it = &s.at(0);
	// Iterar String hasta llegar al final
	while (*it != '\0') {
		if (*it == '.' || *it == ',') {				
			Punto(it, it);
		} else if (*it == '@') {
			MAIL(it, it);
		}else {									// NO DETECTA ESPECIALES
			Generico(it);
		}
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
// LAS POSIC�ON DE MEMORIA P_I y P_F
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
// Y PASA A MIN�SCULAS LOS CARACTERES
/////////////////////////////////////////////////////////////
const char* tr = "aaaaaaNNeeeeiiiiN�oooooNNuuuuNNNaaaaaaNNeeeeiiiiN�oooooNNuuuuNNN";
void
Tokenizador::EliminarMinusAcentos (std::string& s) const {
	long unsigned int i = 0;
	unsigned char a;
	char* c = &s.at(0);
	for (;i < s.size(); ++i) {
		a = *c;
		if (a >= 192){
			*c = tr[a - 192];
		} else{
			*c = tolower(*c);
		}
		++c;
	}
}
/////////////////////////////////////////////////////////////
// COMPRUEBA SI EL CARACTER P_D ES UN DELIMITADOR (_delimiter)
// ESTA FUNCION SOLO SE USA EN LOS CASOS ESPECIALES. ADEM�S
// DE LOS DELIMITADORES COMPRUEBA EL ESPACIO (' ')
/////////////////////////////////////////////////////////////
bool
Tokenizador::EsDelimiter(const char& p_d) const{
	// IF: _delimiters NO es cadena VACIA
	if (_delimitersLength > 0) {
		int i = 0;
		int s = _delimiters.size();
		const char* d = _delimiters.data();//&_delimiters.at(0);
		for (;i<s; ++d) {
			if (p_d == *d || p_d == ' ') {
				return true;
			}
			++i;
		}
	}
	// ELSE: como _delimiters es vacio, solo comprobar el espacio
	return (p_d == ' ');
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
void
Tokenizador::SetDelimitadoresPalabraAux (std::string p_del) {
	_delimitersAux = p_del;
}
//////////////////////////////////////////////////////////////////////////////
////// 			SOBRECARGA DE OPERADORES                                 ///// 
//////////////////////////////////////////////////////////////////////////////
std::ostream& 
operator<< (std::ostream& p_os, const Tokenizador& p_tk)
{	
	if (p_tk._casosEspeciales)
		p_os << "DELIMITADORES: " 					  << p_tk._delimitersAux
			 << " TRATA CASOS ESPECIALES: " 		  << p_tk._casosEspeciales 
			 << " PASAR A MINUSCULAS Y SIN ACENTOS: " << p_tk._pasarAminuscSinAcentos;
	 else 
	 	p_os << "DELIMITADORES: " << p_tk._delimiters << std::endl;

	return p_os;
}
