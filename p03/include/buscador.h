#pragma once

#include "indexadorHash.h"

#include <queue>

class ResultadoRI { 
	friend std::ostream& operator<<(std::ostream&, const ResultadoRI&); 
public: 
	ResultadoRI(const double& kvSimilitud, const long int& kidDoc, const int& np);
 	double VSimilitud() const; 
 	long int IdDoc() const; 
 	bool operator< (const ResultadoRI& lhs) const; 
 	ResultadoRI(const ResultadoRI&);
 	int GetNumPregunta() const {return numPregunta;}
 	double GetVSimilitud() const {return vSimilitud;}
private: 
	double vSimilitud; 
	long int idDoc; 
	int numPregunta; 
};

class Buscador: public IndexadorHash {

friend std::ostream& operator<<(std::ostream& s, const Buscador& p) {
	std::string preg; 
	
	s << "Buscador: " << std::endl; 

	if(p.DevuelvePregunta(preg)) 
		s << "\tPregunta indexada: " << preg << std::endl; 
	else 
		s << "\tNo hay ninguna pregunta indexada" << std::endl; 
	
	s << "\tDatos del indexador: " << std::endl << (IndexadorHash) p; 

	// Invoca a la sobrecarga de la salida del Indexador 
	return s; 
}

 public:
 	Buscador();
	Buscador(const std::string& directorioIndexacion, const int& f);
	Buscador(const Buscador&); 
	~Buscador(); 
	Buscador& operator= (const Buscador&); 
	bool Buscar(const int& numDocumentos = 99999);
	bool Buscar(const std::string& dirPreguntas, const int& numDocumentos, const int& numPregInicio, const int& numPregFin);
	void ImprimirResultadoBusqueda(const int& numDocumentos = 99999) const;
	bool ImprimirResultadoBusqueda(const int& numDocumentos, const std::string& nombreFichero) const;
	int DevolverFormulaSimilitud() const;

	bool CambiarFormulaSimilitud(const int& f);
	// Cambia el valor de “formSimilitud” a “f” si contiene un valor correcto (f == 0 || f == 1); 
	// Devolverá false si “f” no contiene un valor correcto, en cuyo caso no cambiaría el valor anterior de “formSimilitud” 
	void CambiarParametrosDFR(const double& kc); // Cambia el valor de “c = kc” 
	double DevolverParametrosDFR() const; 
	// Devuelve el valor de “c” 
	void CambiarParametrosBM25(const double& kk1, const double& kb); // Cambia el valor de “k1 = kk1; b = kb;” 
	void DevolverParametrosBM25(double& kk1, double& kb) const; // Devuelve el valor de “k1” y “b”

private: 
	int _formSimilitud; // 0: DFR, 1: BM25
	std::priority_queue< ResultadoRI > _docsOrdenados;
	double _c; // Constante del modelo DFR 
	double _k1; // Constante del modelo BM25 
	double _b; // Constante del modelo BM25 

	/////////////////////////////////////////////////////////////////////////////
	// FUNCIONES AUXILIARES
	/////////////////////////////////////////////////////////////////////////////
	double DFR (const int& p_idDoc);
	double BM25 (const int& p_idDoc);
	void LiberarCola(std::priority_queue< ResultadoRI>&) const;

};	