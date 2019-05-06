#include "buscador.h"

/////////////////////////////////////////////////////////////////////////////
// CLASE RESULTADORI
/////////////////////////////////////////////////////////////////////////////
ResultadoRI::ResultadoRI(const double& kvSimilitud, const long int& kidDoc, const int& np) { 
	vSimilitud = kvSimilitud; 
	idDoc = kidDoc; 
	numPregunta = np; 
}
double ResultadoRI::VSimilitud() const { 
	return vSimilitud; 
}
long int 
ResultadoRI::IdDoc() const { 
	return idDoc; 
} 
bool 
ResultadoRI::operator< (const ResultadoRI& lhs) const { 
	if(numPregunta == lhs.numPregunta) 
		return (vSimilitud < lhs.vSimilitud); 
	else 
		return (numPregunta > lhs.numPregunta); 
} 
std::ostream& 
operator<<(std::ostream &os, const ResultadoRI &res){ 
	return os 	
		<< res.vSimilitud 	<< "\t\t" 
		<< res.idDoc 		<< "\t" 
		<< res.numPregunta 
		<< std::endl;
}

/////////////////////////////////////////////////////////////////////////////
// FORMA CANONICA
/////////////////////////////////////////////////////////////////////////////
Buscador::Buscador(const std::string& directorioIndexacion, const int& f) : 
	IndexadorHash(directorioIndexacion),
	_formSimilitud(0),
	_c(2),
	_k1(1.2),
	_b(0.75)
{}

Buscador::~Buscador() {
	_formSimilitud = 0;
	_c = _k1 = _b = 0.0;
	for (long unsigned int i = 0; i < _docsOrdenados.size(); ++i)
		_docsOrdenados.pop();
}

Buscador& 
Buscador::operator= (const Buscador& p_b) {
	if (this != &p_b) {

	}

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// FUNCIONES AUXILIARES
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// GETTERS / SETTERS
/////////////////////////////////////////////////////////////////////////////
double
Buscador::DevolverParametrosDFR() const {
	return _c;
}
void 
Buscador::DevolverParametrosBM25(double& kk1, double& kb) const {
	kk1 = _k1;
	kb = _b;
}
int 
Buscador::DevolverFormulaSimilitud() const {
	return _formSimilitud;
}
bool 
Buscador::CambiarFormulaSimilitud(const int& f) {
	if (f < 0 || f > 1) {
		return false;
	}

	_formSimilitud = f;
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// SOBRECARGA OPERADOR SALIDA
/////////////////////////////////////////////////////////////////////////////

