#pragma once

#include <unordered_map>
#include <list>
#include <iostream>

class InfTermDoc {
friend std::ostream& operator<<(std::ostream&, const InfTermDoc&);

public:
	InfTermDoc (const InfTermDoc &);
	InfTermDoc ();  // Inicializa ft = 0
	InfTermDoc (int p_ft, int p_pos);
	~InfTermDoc (); // Pone ft = 0
	InfTermDoc & operator= (const InfTermDoc &);
	/////////////////////////////////////////////////////////////////
	///  FUNCIONES AUX
	/////////////////////////////////////////////////////////////////
	// Inserta la posicion en la lista de posicion e incrementa la ft
	void UpdatePosTerm(int pos) {++_ft; _posTerm.push_back(pos);}
	// Getter para la _ft
	int getft() const {return _ft;}
	void setFt(int ft) { _ft = ft;}
	void setPosTerm(std::list<int>& lista) {_posTerm = lista;}
	std::list<int>& getposterm() { return _posTerm; }
	void pushTerm(int& term) { _posTerm.push_back(term);}

private:
	int _ft;
	std::list<int> _posTerm;
};


class InformacionTermino {
friend std::ostream& operator<<(std::ostream& s, const InformacionTermino& p);

public:
	InformacionTermino (const InformacionTermino &);
	InformacionTermino (int p_ftc) {_ftc = p_ftc;}
	InformacionTermino ();  // Inicializa ftc = 0
	~InformacionTermino (); // Pone ftc = 0 y vacía l_docs
	InformacionTermino & operator= (const InformacionTermino &);
	/////////////////////////////////////////////////////////////////
	///  FUNCIONES AUX
	/////////////////////////////////////////////////////////////////
	// Incrementar en 1 la _ftc
	void incFTC () 		{ ++_ftc;}
	// Decrementar a la ftc el valor d
	void decftc (int d) {_ftc -= d;}
	// Getter para la ftc
	int getftc () 	const	{return _ftc;};
	// Devuelve una referencia al campo privado _l_docs
	std::unordered_map<long int, InfTermDoc>& apuntarListaDocs();
	//
	void setFtc(int ftc) { _ftc = ftc;} 
	//
	void setDocs (std::unordered_map<long int, InfTermDoc> l);
	//
	const std::unordered_map<long int, InfTermDoc>& getDocs() const {
		return _l_docs;
	}
	std::unordered_map<long int, InfTermDoc>& getDocs() {
		return _l_docs;
	}
	//
	int getNumDocs() const { return _l_docs.size(); }
	const std::unordered_map<long int, InfTermDoc>& getldocs() const {
		return _l_docs;
	}
	std::unordered_map<long int, InfTermDoc>&
	getldocs () { return _l_docs; }


private:
	int _ftc;
	std::unordered_map<long int, InfTermDoc> _l_docs;
};


struct Fecha
{
	int _d, _m, _y;
	
	Fecha  () { _d = _m = _y = 0; }
	~Fecha () { _d = _m = _y = 0; }
	Fecha& operator= (const Fecha& p_f) {
		if (this != &p_f){
			_d = p_f._d;
			_m = p_f._m;
			_y = p_f._y;
		}
		return *this;
	}
};


class InfDoc {
friend std::ostream& operator<<(std::ostream& s, const InfDoc& p);

public:
	InfDoc (const InfDoc &);
	InfDoc ();
	~InfDoc ();
	InfDoc& operator= (const InfDoc &);
	InfDoc(const int p_idDoc, const int p_numPal, const int p_numPalSinParada, 
			const int p_numPalDiferentes, const int p_tamBytes);
	/////////////////////////////////////////////////////////////////
	///  FUNCIONES AUX
	/////////////////////////////////////////////////////////////////
	// Getters
	long int getidDoc() const;
	int getnumPal() const;
	int getnumPalSinParada() const;
	int getnumPalDiferentes() const;
	int gettamBytes() const;
	std::string getDocNombre (int id) const;
	//
	void setidDoc(int i) {_idDoc = i;}
	void setNumPal(int i) {_numPal = i;}
	void setNumPalSinParada(int i) {_numPalSinParada = i;}
	void setNumPalDiferentes(int i) {_numPalDiferentes = i;}
	void setTamBytes(int i) {_tamBytes = i;}
private:
	long int _idDoc;
	int _numPal;
	int _numPalSinParada;
	int _numPalDiferentes;
	int _tamBytes;
	Fecha _fechaModificacion; /* fecha y hora de modificación del documento.*/
};


class InfColeccionDocs {
friend std::ostream& operator<<(std::ostream& s, const InfColeccionDocs& p);

public:
	InfColeccionDocs (const InfColeccionDocs &);
	InfColeccionDocs ();
	~InfColeccionDocs ();
	InfColeccionDocs & operator= (const InfColeccionDocs &);

	/////////////////////////////////////////////////////////////////
	///  FUNCIONES AUX
	/////////////////////////////////////////////////////////////////
	// Getters y Setters
	void setnumDocs (long int n) {_numDocs = n;}
	void setnumTotalPal (long int n) {_numTotalPal = n;}
	void settamBytes (long int n) {_tamBytes = n;}
	void setnumTotalPalSinParada (long int n) {_numTotalPalSinParada = n;}
	void setnumTotalPalDiferentes (long int n) {_numTotalPalDiferentes = n;}
	long int getnumDocs () const {return _numDocs;} 
	long int getnumTotalPal () const {return _numTotalPal;} 
	long int gettamBytes () const {return _tamBytes;} 
	long int getnumTotalPalSinParada () const {return _numTotalPalSinParada;}
	long int getnumTotalPalDiferentes () const {return _numTotalPalDiferentes;}

private:
	long int _numDocs; // No total de documentos en la colección
	long int _numTotalPal; // No total de palabras en la colección
	long int _tamBytes; // Tamaño total en bytes de la colección
	long int _numTotalPalSinParada; // No total de palabras sin stop-words en la colección
	long int _numTotalPalDiferentes;
};


class InformacionTerminoPregunta {
friend std::ostream& operator<<(std::ostream& s, const InformacionTerminoPregunta& p);

public:
	InformacionTerminoPregunta (const InformacionTerminoPregunta &);
	InformacionTerminoPregunta ();
	~InformacionTerminoPregunta ();
	InformacionTerminoPregunta & operator= (const InformacionTerminoPregunta&);
	
	//
	InformacionTerminoPregunta (int p_ft) { _ft = p_ft; }
	//
	void IncFt() { _ft++; }
	//	
	void NuevaPosicion(int p_posicion) { _posTerm.push_back(p_posicion); }
	//
	int getft() const { return _ft; }

private:
	int _ft;
	std::list<int> _posTerm;
};


class InformacionPregunta {
friend std::ostream& operator<<(std::ostream& s, const InformacionPregunta& p);

public:
	InformacionPregunta (const InformacionPregunta &);
	InformacionPregunta ();
	~InformacionPregunta ();
	InformacionPregunta & operator= (const InformacionPregunta &);

	//
	void setNumTotalPal (int p) {_numTotalPal += p;} 
	void setNumTotalPalSinParada (int p) {_numTotalPalSinParada += p;}
	void setNumTotalPalDiferentes (int p) {_numTotalPalDiferentes += p;}
	int getnumTotalPalSinParada() { return _numTotalPalSinParada; }
private:
	long int _numTotalPal; // No total de palabras en la pregunta
	long int _numTotalPalSinParada; // No total de palabras sin stop-words en la pregunta
	long int _numTotalPalDiferentes;
	/*No total de palabras diferentes en la pregunta que no sean stop-words
	(sin acumular la frecuencia de cada una de ellas)*/
};