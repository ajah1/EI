#pragma once

#include <iostream>
#include <list>

class Tokenizador {
	
	friend std::ostream& operator<<(std::ostream&, const Tokenizador&);
	
public:
	// CONSTRUCTORES
	Tokenizador (const std::string& delimitadoresPalabra, 
				 const bool& kcasosEspeciales, 
				 const bool& minuscSinAcentos);

	Tokenizador (const Tokenizador&);
	Tokenizador (const std::string&);
	Tokenizador ();
	~Tokenizador ();
	Tokenizador& operator= (const Tokenizador& p_tk);
	// TOKENIZADOR
	void Tokenizar (const std::string& p_str, std::list<std::string>& p_tokens) const;
	bool Tokenizar (std::string& p_i,  std::string& p_f) const;
	bool Tokenizar (const std::string& p_i) const;
	bool TokenizarListaFicheros (const std::string& p_i) const;
	bool TokenizarDirectorio (const std::string& p_i) const;
	// GETTERS
	std::string DelimitadoresPalabra() const;
	bool PasarAminuscSinAcentos ();
	bool CasosEspeciales ();
	// SETTERS
	void DelimitadoresPalabra(const std::string& p_nuevoDelimiters);
	void AnyadirDelimitadoresPalabra(const std::string& p_nuevoDelimiters);
	void PasarAminuscSinAcentos (const bool& p_PasarAminuscSinAcentos);
	void CasosEspeciales (const bool& p_nuevoCasosEspeciales);



private:
	std::string _delimiters 		= "";
	bool _casosEspeciales			= false;
	bool _pasarAminuscSinAcentos	= false;
	

	//////////////////////////////////////////////
	//VARIABLES'S AUXILIARES
	////////////////////////////////////////////////
	// Copia de los delimitadores iniciales
	std::string _delimitersAux	= "";
	// Indicadores aceptados por una URL
	std::string _URLindc 		= "http https ftp";
	// Delimitadores que no afectan en medio a la URL
	std::string _URLdelimiters 	= "_:/.?&-=#@";
	// Delimitadores que no afectan en medio a la URL
	std::string _MAILdelimiters = ".-_";
	// Length del string delimiters
	int 		_delimitersLength = 0;

	//////////////////////////////////////////////
	// GENERICO:  obtiene el token cuando no detecta especial
	////////////////////////////////////////////////
	void Generico(char*& p_it) const;
	//////////////////////////////////////////////
	// URL: obtiene el token URL
	////////////////////////////////////////////////
	void URL(char*& p_izq, char*& p_der) const;	
	// Devuelve True si el string pertenece a _indicadores
	bool EsURLIndicador(const std::string&) const;
	// True si el caracter es delimitador para la URL
	bool EsURLDelimiter( char*& p_caracter) const;
	//////////////////////////////////////////////
	// MAIL: obtiene el token MAIL
	////////////////////////////////////////////////
	void MAIL(char*& p_izq, char*& p_der) const;
	//////////////////////////////////////////////
	// ACRONIMO: obtiene el token acronimo
	////////////////////////////////////////////////
	void Acronimo(char*& p_izq, char*& p_it) const;
	void AcronimoAux1(char* &p_izq, char*& p_der) const;
	void AcronimoAux2(char* &p_izq, char*& p_der) const;
	bool EsAcronimoDel(char* p) const;
	//////////////////////////////////////////////
	// GUION: obtiene el token GUION
	////////////////////////////////////////////////
	void Guion(char*& p_izq, char*& p_der) const;
	void GuionAux1(char* &p_izq, char* &p_der) const;
	void GuionAux2(char* &p_izq, char* &p_der) const;
	//////////////////////////////////////////////
	// DIGITO: obtiene el token numero
	////////////////////////////////////////////////
	void Decimal(char*& p_izq, char*& p_der) const;
	void DecimalAux1(char*& p_izq, char*& p_der) const;
	void DecimalAux2(char*& p_izq, char*& p_der) const;
	////////////////////////////////////////////////
	//FUNCIONES AUXILIARES
	////////////////////////////////////////////////
	// Quita los acentos y pasa a minúsculas las mayúsculas 
	void EliminarMinusAcentos(std::string&) const;
	// Comprueba si un ., trata de un acronimo o un digito
	void Punto(char*& p_izq, char*& p_der) const;
	// Devuelve el string obtenido entre las dos posiciones de memoria
	std::string ObtenerString (const char* p_i, const char* p_f) const;
	// Devuelve true si _delimiters contiene el char
	bool EsDelimiter(const char& p_d) const;
	// Tokeniza el string sin tener en cuenta los casos especiales
	void TokenizarGeneral(std::string&, std::list<std::string>&) const;
	// Tokeniza el string teniendo en cuenta los casos especiales
	void TokenizarEspecial(std::string&, std::list<std::string>&) const;
};