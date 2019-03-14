
#include "indexadorInformacion.h"


//////////////////////////////////////////////////////////////////////////////
////// 			InfTermDoc                       						///// 
//////////////////////////////////////////////////////////////////////////////
InfTermDoc::InfTermDoc (const InfTermDoc& p_td) :
	_ft(p_td._ft),
	_posTerm(p_td._posTerm)
{}

InfTermDoc::InfTermDoc () : 
	_ft(0)
{}

InfTermDoc::~InfTermDoc () {
	_ft = 0;
	_posTerm.clear();
}

InfTermDoc& 
InfTermDoc::operator= (const InfTermDoc& p_td) {
	if (this != &p_td) {
		_ft = p_td._ft;
		_posTerm.clear();
		_posTerm = p_td._posTerm;
	}
	return *this;
}


//////////////////////////////////////////////////////////////////////////////
////// 			InformacionTermino                       			///// 
//////////////////////////////////////////////////////////////////////////////
InformacionTermino::InformacionTermino (const InformacionTermino& p_t) :
	_ftc(p_t._ftc),
	_l_docs(p_t._l_docs) 
{}

InformacionTermino::InformacionTermino () :
	_ftc(0) 
{}

InformacionTermino::~InformacionTermino () {
	_ftc = 0;
	_l_docs.clear();
}

InformacionTermino& 
InformacionTermino::operator= (const InformacionTermino& p_t) {
	if (this != &p_t) {
		_ftc = p_t._ftc;
		_l_docs.clear();
		_l_docs = p_t._l_docs;
	}
	return *this;
}


//////////////////////////////////////////////////////////////////////////////
////// 			InfoDOc 				                      			///// 
//////////////////////////////////////////////////////////////////////////////
InfDoc::InfDoc (const InfDoc& p_d) : 
	_idDoc(p_d._idDoc),
	_numPal(p_d._numPal),
	_numPalSinParada(p_d._numPalSinParada),
	_numPalDiferentes(p_d._numPalDiferentes),
	_tamBytes(p_d._tamBytes),
	_fechaModificacion(p_d._fechaModificacion)
{}

InfDoc::InfDoc () : 
	_idDoc(0),
	_numPal(0),
	_numPalSinParada(0),
	_numPalDiferentes(0),
	_tamBytes(0)
{}

InfDoc::~InfDoc () {
	_idDoc = _numPal = _numPalSinParada = 0;
	_numPalDiferentes = _tamBytes = 0;
	_fechaModificacion.~Fecha();
}

InfDoc& 
InfDoc::operator= (const InfDoc& p_d) {
	if (this != &p_d) {
		_idDoc				= p_d._idDoc;
		_numPal 			= p_d._numPal;
		_numPalSinParada 	= p_d._numPalSinParada;
		_numPalDiferentes 	= p_d._numPalDiferentes;
		_tamBytes 			= p_d._tamBytes;
		_fechaModificacion 	= p_d._fechaModificacion;
	}
	return *this;
}


//////////////////////////////////////////////////////////////////////////////
////// 			InformacionTerminoPregunta 				                 ///// 
//////////////////////////////////////////////////////////////////////////////
InformacionTerminoPregunta::InformacionTerminoPregunta (const InformacionTerminoPregunta& p_tp) :
	_ft(p_tp._ft),
	_posTerm(p_tp._posTerm)
{}

InformacionTerminoPregunta::InformacionTerminoPregunta () :
	_ft(0)	
{}

InformacionTerminoPregunta::~InformacionTerminoPregunta () {
	_ft = 0;
	_posTerm.clear();
}

InformacionTerminoPregunta& 
InformacionTerminoPregunta::operator= (const InformacionTerminoPregunta& p_tp) {
	if (this != &p_tp) {
		_ft = p_tp._ft;
		_posTerm.clear();
		_posTerm = p_tp._posTerm;
	}
	return *this;
}


//////////////////////////////////////////////////////////////////////////////
////// 			InformacionPregunta      				                 ///// 
/////////////////////////////////////////////////////////////////////////////
InformacionPregunta::InformacionPregunta (const InformacionPregunta& p_ip) : 
	_numTotalPal(p_ip._numTotalPal),
	_numTotalPalSinParada(p_ip._numTotalPalSinParada),
	_numTotalPalDiferentes(p_ip._numTotalPalDiferentes)	
{}

InformacionPregunta::InformacionPregunta () : 
	_numTotalPal(0),
	_numTotalPalSinParada(0),
	_numTotalPalDiferentes(0)
{}

InformacionPregunta::~InformacionPregunta () {
	_numTotalPal = _numTotalPalSinParada = _numTotalPalDiferentes = 0;
}

InformacionPregunta& 
InformacionPregunta::operator= (const InformacionPregunta& p_ip) {
	if (this != &p_ip) {
		_numTotalPal 			= p_ip._numTotalPal;
		_numTotalPalSinParada 	= p_ip._numTotalPalSinParada;
		_numTotalPalDiferentes 	= p_ip._numTotalPalDiferentes;
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
////// 			SOBRECARGA DE OPERADORES                                 ///// 
//////////////////////////////////////////////////////////////////////////////
std::ostream& 
operator<< (std::ostream& p_os, const InfTermDoc& p_inft) {
	//p_os << "ft: " << p_inft._ft;
	//A continuación se mostrarían todos los elementos de p.posTerm (“posicion
	//TAB posicion TAB ... posicion, es decir nunca finalizará en un TAB”): s <<
	//“\t“ << posicion;
	return p_os;	
}
std::ostream&
operator<< (std::ostream& p_os, const InformacionTermino& p_t) {
	/*s << “Frecuencia total: “ << p.ftc << “\tfd: ” << p.l_docs.size();
	// A continuación se mostrarían todos los elementos de p.l_docs: s <<
	“\tId.Doc: “ << idDoc << “\t” << InfTermDoc;
	}*/	
	return p_os;
}
std::ostream& 
operator<< (std::ostream& p_os, const InfDoc& p_d) {
	return p_os;
}
std::ostream& 
operator<<(std::ostream& p_os, const InformacionTerminoPregunta& p_tp) {
	return p_os;
	/*s << “ft: “ << p.ft;
	// A continuación se mostrarían todos los elementos de p.posTerm (“posicion
	TAB posicion TAB ... posicion, es decir nunca finalizará en un TAB”): s <<
	“\t“ << posicion;*/
}
std::ostream& 
operator<< (std::ostream& p_os, const InformacionPregunta& p_ip) {
	return p_os;
}