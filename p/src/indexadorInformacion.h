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

	void UpdatePosTerm(int pos) {++_ft; _posTerm.push_back(pos);}
	int getft() {return _ft;}
private:
	int _ft; // Frecuencia del término en el documento
	std::list<int> _posTerm;
	/* Solo se almacenará esta información si el campo privado del indexador
	almacenarPosTerm == true*/
	/* Lista de números de palabra en los que aparece el término en el
	documento. Los números de palabra comenzarán desde cero (la primera
	palabra del documento). Se numerarán las palabras de parada. Estará
	ordenada de menor a mayor posición.*/
};


class InformacionTermino {
friend std::ostream& operator<<(std::ostream& s, const InformacionTermino& p);

public:
	InformacionTermino (const InformacionTermino &);
	InformacionTermino (int p_ftc) {_ftc = p_ftc;}
	InformacionTermino ();  // Inicializa ftc = 0
	~InformacionTermino (); // Pone ftc = 0 y vacía l_docs
	InformacionTermino & operator= (const InformacionTermino &);
	
	void incFTC() { ++_ftc;}
	void decftc(int d) {_ftc -= d;}
	int getftc() {return _ftc;};
	std::unordered_map<long int, InfTermDoc>& apuntarListaDocs();
private:
	int _ftc; // Frecuencia total del término en la colección
	std::unordered_map<long int, InfTermDoc> _l_docs;
	/* Tabla Hash que se accederá por el id del documento, devolviendo un
	objeto de la clase InfTermDoc que contiene toda la información de
	aparición del término en el documento*/
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

   	//idDoc: 1	numPal: 6	numPalSinParada: 4	numPalDiferentes: 3	tamBytes: 30
	InfDoc(const int p_idDoc, const int p_numPal, const int p_numPalSinParada, 
		const int p_numPalDiferentes, const int p_tamBytes);
	long int getidDoc() const;
	int getnumPal() const;
	int getnumPalSinParada() const;
	int getnumPalDiferentes() const;
	int gettamBytes() const;
	std::string getDocNombre (int id) const;

private:
	long int _idDoc;
	/* Identificador del documento. El primer documento indexado en la
	colección será el identificador 1*/
	int _numPal; 		  // No total de palabras del documento
	int _numPalSinParada; // No total de palabras sin stop-words del documento
	int _numPalDiferentes;
	/* No total de palabras diferentes que no sean stop-words (sin acumular
	la frecuencia de cada una de ellas)*/
	int _tamBytes; 		  // Tamaño en bytes del documento
	Fecha _fechaModificacion;
	/* Atributo correspondiente a la fecha y hora de modificación del
	documento. El tipo “Fecha/hora” lo elegirá/implementará el alumno*/
};

class InfColeccionDocs {
friend std::ostream& operator<<(std::ostream& s, const InfColeccionDocs& p);

public:
	InfColeccionDocs (const InfColeccionDocs &);
	InfColeccionDocs ();
	~InfColeccionDocs ();
	InfColeccionDocs & operator= (const InfColeccionDocs &);

	void setnumDocs (long int n) {_numDocs = n;} // No total de documentos en la colección
	void setnumTotalPal (long int n) {_numTotalPal = n;} // No total de palabras en la colección
	void settamBytes (long int n) {_tamBytes = n;} // Tamaño total en bytes de la colección
	void setnumTotalPalSinParada (long int n) {_numTotalPalSinParada = n;} // No total de palabras sin stop-words en la colección
	void setnumTotalPalDiferentes (long int n) {_numTotalPalDiferentes = n;}

	long int getnumDocs () const {return _numDocs;} // No total de documentos en la colección
	long int getnumTotalPal () const {return _numTotalPal;} // No total de palabras en la colección
	long int gettamBytes () const {return _tamBytes;} // Tamaño total en bytes de la colección
	long int getnumTotalPalSinParada () const {return _numTotalPalSinParada;} // No total de palabras sin stop-words en la colección
	long int getnumTotalPalDiferentes () const {return _numTotalPalDiferentes;}

private:
	long int _numDocs; // No total de documentos en la colección
	long int _numTotalPal; // No total de palabras en la colección
	long int _tamBytes; // Tamaño total en bytes de la colección
	long int _numTotalPalSinParada; // No total de palabras sin stop-words en la colección
	long int _numTotalPalDiferentes;
	/* No total de palabras diferentes en la colección que no sean stop-
	words (sin acumular la frecuencia de cada una de ellas)*/
	
};


class InformacionTerminoPregunta {
friend std::ostream& operator<<(std::ostream& s, const InformacionTerminoPregunta& p);

public:
	InformacionTerminoPregunta (const InformacionTerminoPregunta &);
	InformacionTerminoPregunta ();
	~InformacionTerminoPregunta ();
	InformacionTerminoPregunta & operator= (const InformacionTerminoPregunta&);

private:
	int _ft;
	 // Frecuencia total del término en la pregunta
	std::list<int> _posTerm;
	/* Solo se almacenará esta información si el campo privado del indexador
	almacenarPosTerm == true*/
	/* Lista de números de palabra en los que aparece el término en la
	pregunta. Los números de palabra comenzarán desde cero (la primera
	palabra de la pregunta). Se numerarán las palabras de parada. Estará
	ordenada de menor a mayor posición.*/
};


class InformacionPregunta {
friend std::ostream& operator<<(std::ostream& s, const InformacionPregunta& p);

public:
	InformacionPregunta (const InformacionPregunta &);
	InformacionPregunta ();
	~InformacionPregunta ();
	InformacionPregunta & operator= (const InformacionPregunta &);
private:
	long int _numTotalPal; // No total de palabras en la pregunta
	long int _numTotalPalSinParada; // No total de palabras sin stop-words en la pregunta
	long int _numTotalPalDiferentes;
	/*No total de palabras diferentes en la pregunta que no sean stop-words
	(sin acumular la frecuencia de cada una de ellas)*/
};