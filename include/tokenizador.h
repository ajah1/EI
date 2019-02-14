#pragma once

#include <iostream>
#include <string>
#include <list>

class Tokenizador {
	
	friend std::ostream& operator<<(std::ostream&, const Tokenizador&);
	/* // cout << “DELIMITADORES: “ << delimiters << “ TRATA CASOS ESPECIALES:
	“ << casosEspeciales << “ PASAR A MINUSCULAS Y SIN ACENTOS: “ <<
	pasarAminuscSinAcentos;
	// Aunque se modifique el almacenamiento de los delimitadores por temas
	de eficiencia, el campo delimiters se imprimirá con el std::string con el que
	se inicializó el tokenizador */

public:
	Tokenizador (
		const std::string& delimitadoresPalabra, 
		const bool& kcasosEspeciales, 
		const bool& minuscSinAcentos);
	/* Inicializa delimiters a delimitadoresPalabra; casosEspeciales a
	kcasosEspeciales; pasarAminuscSinAcentos a minuscSinAcentos*/
	Tokenizador (const Tokenizador&);
	Tokenizador (const std::string&);
	Tokenizador ();
	/*Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
	casosEspeciales a true; pasarAminuscSinAcentos a false */

	~Tokenizador (); // Pone delimiters=""
	
	Tokenizador& operator= (const Tokenizador& p_tk);
	
	void Tokenizar (const std::string& p_str, std::list<std::string>& p_tokens) const;
	/* Tokeniza str devolviendo el resultado en tokens. La lista tokens se
	vaciará antes de almacenar el resultado de la tokenización.*/

	bool Tokenizar (const std::string& p_i, const std::string& p_f) const;
	/* Tokeniza el fichero i guardando la salida en el fichero f (una
	palabra en cada línea del fichero). Devolverá true si se realiza la
	tokenización de forma correcta; false en caso contrario enviando a cerr
	el mensaje correspondiente (p.ej. que no exista el archivo i)*/

	bool Tokenizar (const std::string & p_i) const;
	/* Tokeniza el fichero i guardando la salida en un fichero de nombre i
	añadiéndole extensión .tk (sin eliminar previamente la extensión de i
	por ejemplo, del archivo pp.txt se generaría el resultado en pp.txt.tk),
	y que contendrá una palabra en cada línea del fichero. Devolverá true si
	se realiza la tokenización de forma correcta; false en caso contrario
	enviando a cerr el mensaje correspondiente (p.ej. que no exista el
	archivo i)*/

	bool TokenizarListaFicheros (const std::string& p_i) const;
	/* Tokeniza el fichero i que contiene un nombre de fichero por línea
	guardando la salida en ficheros (uno por cada línea de i) cuyo nombre
	será el leído en i añadiéndole extensión .tk, y que contendrá una
	palabra en cada línea del fichero leído en i. Devolverá true si se
	realiza la tokenización de forma correcta de todos los archivos que
	contiene i; devolverá false en caso contrario enviando a cerr el mensaje
	correspondiente (p.ej. que no exista el archivo i, o que se trate de un
	directorio, enviando a “cerr” los archivos de i que no existan o que
	sean directorios; luego no se ha de interrumpir la ejecución si hay
	algún archivo en i que no exista)*/
	
	bool TokenizarDirectorio (const std::string& p_i) const;
	/* Tokeniza todos los archivos que contenga el directorio i, incluyendo
	los de los subdirectorios, guardando la salida en ficheros cuyo nombre
	será el de entrada añadiéndole extensión .tk, y que contendrá una
	palabra en cada línea del fichero. Devolverá true si se realiza la
	tokenización de forma correcta de todos los archivos; devolverá false en
	caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no
	exista el directorio i, o los ficheros que no se hayan podido tokenizar)*/

	void DelimitadoresPalabra(const std::string& p_nuevoDelimiters);
	/* Cambia “delimiters” por “nuevoDelimiters” comprobando que no hayan
	delimitadores repetidos (de izquierda a derecha), en cuyo caso se
	eliminarían los que hayan sido repetidos (por la derecha)*/
	
	void AnyadirDelimitadoresPalabra(const std::string& p_nuevoDelimiters); //
	/* Añade al final de “delimiters” los nuevos delimitadores que aparezcan
	en “nuevoDelimiters” (no se almacenarán caracteres repetidos)*/
	
	std::string DelimitadoresPalabra() const;
	// Devuelve “delimiters”
	
	void CasosEspeciales (const bool& p_nuevoCasosEspeciales);
	// Cambia la variable privada “casosEspeciales”
	
	bool CasosEspeciales ();
	// Devuelve el contenido de la variable privada “casosEspeciales”
	
	void PasarAminuscSinAcentos (const bool& p_PasarAminuscSinAcentos);
	/* Cambia la variable privada “pasarAminuscSinAcentos”. Atención al
	formato de codificación del corpus (comando “file” de Linux). Para la
	corrección de la práctica se utilizará el formato actual (ISO-8859).*/
	
	bool PasarAminuscSinAcentos ();
	// Devuelve el contenido de la variable privada “pasarAminuscSinAcentos”

	// Devuelve el substring formado entre dos punteros inicial y final
	std::string ObtenerString (const char* p_i, const char* p_f) const;
	// Comprobaciones para el caso especial acronicmo
	void Acronimo(char* &p_izq, char* &p_it, std::list<std::string>&) const;
	// Devuelve true si _delimiters contiene el char
	bool SDelimitador(const char p_d) const;
	// Comprobaciones para el tokenizador general
	void Generico(char* &p_it, std::list<std::string>&) const;
	// Quita los acentos y pasa a minúsculas las mayúsculas 
	std::string EliminarMinusAcentos(const std::string&) const;
	// URL
	void URL(char* &p_izq, char* &p_der, std::list<std::string>&) const;	
	// Devuelve True si el string pertenece a _indicadores
	bool EsIndicador(const std::string&) const;
	// True si el caracter es delimitador para la URL
	bool esURLDelimiter(const char* p_caracter) const;

private:
	std::string _delimitersAux;
	
	// String para la URL
	std::string _indicadores = "http https ftp";
	std::string _URLdelimiters = "_:/.?&-=#@";

	std::string _delimiters;
 	/* Delimitadores de términos. Aunque se
	modifique la forma de almacenamiento interna para mejorar la eficiencia, este
	campo debe permanecer para indicar el orden en que se introdujeron los
	delimitadores*/

	bool _casosEspeciales;
	/* Si true detectará palabras compuestas y casos especiales. Sino,
	trabajará al igual que el algoritmo propuesto en la sección “Versión del
	tokenizador vista en clase”*/
	
	bool _pasarAminuscSinAcentos;
	/* Si true pasará el token a minúsculas y quitará acentos, antes de
	realizar la tokenización*/
};