
#include "indexadorInformacion.h"


//////////////////////////////////////////////////////////////////////////////
////// 			InfTermDoc                       						///// 
//////////////////////////////////////////////////////////////////////////////
InfTermDoc::InfTermDoc (const InfTermDoc& p_itd) :
	_ft(p_itd._ft),
	_posTerm(p_itd._posTerm)
{}

InfTermDoc::InfTermDoc () : 
	_ft(0)
{}

InfTermDoc::~InfTermDoc () {
	_ft = 0;
	_posTerm.clear();
}

InfTermDoc& 
InfTermDoc::operator= (const InfTermDoc& p_itd) {
	if (this != &p_itd) {
		_ft = p_itd._ft;
		_posTerm.clear();
		_posTerm = p_itd._posTerm;
	}
	return *this;
}


//////////////////////////////////////////////////////////////////////////////
////// 			InformacionTermino                       			///// 
//////////////////////////////////////////////////////////////////////////////
InformacionTermino::InformacionTermino (const InformacionTermino& p_it) :
	_ftc(p_it._ftc),
	_l_docs(p_it._l_docs) 
{}

InformacionTermino::InformacionTermino () :
	_ftc(0) 
{}

InformacionTermino::~InformacionTermino () {
	_ftc = 0;
	_l_docs.clear();
}

InformacionTermino& 
InformacionTermino::operator= (const InformacionTermino& p_it) {
	if (this != &p_it) {
		_ftc = p_it._ftc;
		_l_docs.clear();
		_l_docs = p_it._l_docs;
	}
	return *this;
}


//////////////////////////////////////////////////////////////////////////////
////// 			InfoDOc 				                      			///// 
//////////////////////////////////////////////////////////////////////////////
InfDoc::InfDoc (const InfDoc& p_id) : 
	_idDoc(p_id._idDoc),
	_numPal(p_id._numPal),
	_numPalSinParada(p_id._numPalSinParada),
	_numPalDiferentes(p_id._numPalDiferentes),
	_tamBytes(p_id._tamBytes),
	_fechaModificacion(p_id._fechaModificacion)
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
InfDoc::operator= (const InfDoc& p_id) {
	if (this != &p_id) {
		_idDoc				= p_id._idDoc;
		_numPal 			= p_id._numPal;
		_numPalSinParada 	= p_id._numPalSinParada;
		_numPalDiferentes 	= p_id._numPalDiferentes;
		_tamBytes 			= p_id._tamBytes;
		_fechaModificacion 	= p_id._fechaModificacion;
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
////// 			InfColeccionDocs 				                      			///// 
//////////////////////////////////////////////////////////////////////////////
InfColeccionDocs::InfColeccionDocs (const InfColeccionDocs& p_id) :
	_numDocs(p_id._numDocs),
	_numTotalPal(p_id._numTotalPal),
	_tamBytes(p_id._tamBytes),
	_numTotalPalSinParada(p_id._numTotalPalSinParada),
	_numTotalPalDiferentes(p_id._numTotalPalDiferentes)
{}

InfColeccionDocs::InfColeccionDocs () :
	_numDocs(0.0),
	_numTotalPal(0.0),
	_tamBytes(0.0),
	_numTotalPalSinParada(0.0),
	_numTotalPalDiferentes(0.0)
{}

InfColeccionDocs::~InfColeccionDocs () {
	_numDocs = _numTotalPal = _tamBytes = 0.0;
	_numTotalPalSinParada = _numTotalPalDiferentes = 0.0;
}

InfColeccionDocs&
InfColeccionDocs::operator= (const InfColeccionDocs& p_id) {
	if (this != &p_id) {
		_numDocs 				= p_id._numDocs;
		_numTotalPal 			= p_id._numTotalPal;
		_tamBytes 				= p_id._tamBytes;
		_numTotalPalSinParada 	= p_id._numTotalPalSinParada;
		_numTotalPalDiferentes 	= p_id._numTotalPalDiferentes;
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
////// 			InformacionTerminoPregunta 				                 ///// 
//////////////////////////////////////////////////////////////////////////////
InformacionTerminoPregunta::InformacionTerminoPregunta (const InformacionTerminoPregunta& p_itp) :
	_ft(p_itp._ft),
	_posTerm(p_itp._posTerm)
{}

InformacionTerminoPregunta::InformacionTerminoPregunta () :
	_ft(0)	
{}

InformacionTerminoPregunta::~InformacionTerminoPregunta () {
	_ft = 0;
	_posTerm.clear();
}

InformacionTerminoPregunta& 
InformacionTerminoPregunta::operator= (const InformacionTerminoPregunta& p_itp) {
	if (this != &p_itp) {
		_ft = p_itp._ft;
		_posTerm.clear();
		_posTerm = p_itp._posTerm;
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
operator<< (std::ostream& p_os, const InfTermDoc& p) {
	p_os << "ft: " << p._ft;

	for (int posicion : p._posTerm) {
		p_os << "\t" << posicion;	
	}
	return p_os;
}
std::ostream&
operator<< (std::ostream& p_os, const InformacionTermino& p) {
	p_os << "Frecuencia total: " << p._ftc << "\tfd: " << p._l_docs.size();

	for (auto it = p._l_docs.begin(); it != p._l_docs.end(); ++it) {
		p_os << "\tId.Doc: " << it->first << "\t" << it->second;
	}
	return p_os;
}
std::ostream& 
operator<< (std::ostream& p_os, const InfDoc& p_id) {
	return p_os 
	<< "\tidDoc: " 				<< p_id._idDoc 
	<< "\tnumPal: "  			<< p_id._numPal
	<< "\tnumPalSinParada: " 	<< p_id._numPalSinParada 
	<< "\tnumPalDiferentes: " 	<< p_id._numPalDiferentes
	<< "\ttamBytes: " 			<< p_id._tamBytes;	
}
std::ostream& 
operator<< (std::ostream& s, const InfColeccionDocs& p) {
	return  s 
	<< "numDocs: " 					<< p._numDocs 
	<< "\tnumTotalPal: " 			<< p._numTotalPal 
	<< "\tnumTotalPalSinParada: " 	<< p._numTotalPalSinParada
 	<< "\tnumTotalPalDiferentes: " 	<< p._numTotalPalDiferentes 
 	<< "\ttamBytes: " 				<< p._tamBytes;
}
std::ostream& 
operator<< (std::ostream& s, const InformacionTerminoPregunta& p) {
	s << "ft: " << p._ft;

	for (int posicion : p._posTerm) {
		s << "\t" << posicion;
	}
	return s;
}
std::ostream& 
operator<< (std::ostream& s, const InformacionPregunta& p) {
	return s 
	<< "numTotalPal: " 				<< p._numTotalPal 
	<< "\tnumTotalPalSinParada: " 	<< p._numTotalPalSinParada 
	<< "\tnumTotalPalDiferentes: " 	<< p._numTotalPalDiferentes;
}


//////////////////////////////////////////////////////////////////////////////
////// 			FUNCIONES PARTE PRIVADA                                 ///// 
//////////////////////////////////////////////////////////////////////////////
long int 
InfDoc::getidDoc() const { 
	return _idDoc; 
} 
int 
InfDoc::getnumPal() const { 
	return _numPal; 
}
int 
InfDoc::getnumPalSinParada() const { 
	return _numPalSinParada; 
}
int 
InfDoc::getnumPalDiferentes() const { 
	return _numPalDiferentes; 
}
int 
InfDoc::gettamBytes() const { 
	return _tamBytes; 
}

InfDoc::InfDoc(const int p_idDoc, const int p_numPal, const int p_numPalSinParada, 
			const int p_numPalDiferentes, const int p_tamBytes) :
	_idDoc(p_idDoc),
	_numPal(p_numPal),
	_numPalSinParada(p_numPalSinParada),
	_numPalDiferentes(p_numPalDiferentes),
	_tamBytes(p_tamBytes)
{}

std::unordered_map<long int, InfTermDoc>& 
InformacionTermino::apuntarListaDocs() {
	return _l_docs;
}

InfTermDoc::InfTermDoc (int p_ft, int p_pos) {
	_ft = p_ft;
	_posTerm.push_back(p_pos);
}

void 
InformacionTermino::setDocs (std::unordered_map<long int, InfTermDoc> l) {
	for (auto& d : l) {
		_l_docs.insert({d.first, d.second});
	}
}