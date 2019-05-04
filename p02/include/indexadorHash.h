#pragma once

#include "tokenizador.h"
#include "indexadorInformacion.h"
#include <unordered_set>

class IndexadorHash {

friend std::ostream& operator<<(std::ostream& s, const IndexadorHash& p);

public:
	IndexadorHash(const std::string& fichStopWords, const std::string& delimitadores,
	const bool& detectComp, const bool& minuscSinAcentos, const std::string&
	dirIndice, const int& tStemmer, const bool& almEnDisco, const bool&
	almPosTerm);

	IndexadorHash(const std::string& directorioIndexacion);
	IndexadorHash(const IndexadorHash&);
	~IndexadorHash();
	
	IndexadorHash& operator= (const IndexadorHash&);

	bool Indexar(const std::string& ficheroDocumentos);
	bool IndexarDirectorio(const std::string& dirAIndexar);
	bool GuardarIndexacion() const;
	bool RecuperarIndexacion (const std::string& directorioIndexacion);
	
	void ImprimirIndexacion() const;

	bool IndexarPregunta(const std::string& preg);
	bool DevuelvePregunta(std::string& preg) const;
	bool DevuelvePregunta(const std::string& word, InformacionTerminoPregunta& inf) const;
	bool DevuelvePregunta(InformacionPregunta& inf) const;
	
	void ImprimirIndexacionPregunta();
	void ImprimirPregunta();
	
	bool Devuelve(const std::string& word, InformacionTermino& inf);
	bool Devuelve(const std::string& word, const std::string& nomDoc, InfTermDoc& InfDoc);
	bool Existe(const std::string& word) const;
	
	bool Borra(const std::string& word);
	bool BorraDoc(const std::string& nomDoc);
	
	void VaciarIndiceDocs();
	void VaciarIndicePreg();
	
	bool Actualiza(const std::string& word, const InformacionTermino& inf);
	bool Inserta(const std::string& word, const InformacionTermino& inf);
	
	int NumPalIndexadas() const;
	std::string DevolverFichPalParada () const;
	void ListarPalParada() const;
	int NumPalParada() const;
	
	std::string DevolverDelimitadores () const;
	bool DevolverCasosEspeciales () ;
	bool DevolverPasarAminuscSinAcentos () ;
	bool DevolverAlmacenarPosTerm () const;
	std::string DevolverDirIndice () const;
	int DevolverTipoStemming () const;
	bool DevolverAlmEnDisco () const;
	
	void ListarInfColeccDocs() const;
	void ListarTerminos() const;
	bool ListarTerminos(const std::string& nomDoc) const;
	bool ListarDocs(const std::string& nomDoc);
	
private:

	/////////////////////////////////////////////////////////////////
	///  FUNCIONES AUX
	/////////////////////////////////////////////////////////////////
	// Leer las stopwords del fichero
	void ObtenerPalParada();
	// Indexar los tokens guardados en ficheros
	void indexar_tokens(const std::string&);
	// Comprobar si el token es stop word
	bool EsParada(const std::string& p_token);
	// Actualizar el objeto InformacionTermino de un indice ya indexado
	void actualizar_token_indexado(std::string& p_token, int aux_idDoc, int aux_posicion);
	// OBtener el size en B de un archivo
	double GetFileSize(std::string filename);
	// Dado un id de doc devuelve su nombre en un string
	int getDocId (std::string p_ruta) const;
	// _indice
	void CopiaIndice(std::unordered_map<std::string, InformacionTermino> p_indice);
	// _indiceDOcs
	void CopiaIndiceDocs(std::unordered_map<std::string, InfDoc> p_indiceDocs);
	// _indicePregunta
	void CopiaindicePregunta(std::unordered_map<std::string, InformacionTerminoPregunta> p_indicepregunta);
	// _stopwords
	void CopiaStopWords(std::unordered_set<std::string> p_parada);
	// copia ICD
	void CopiaICD (const InfColeccionDocs& p_id);

	// 
	void GuardarIndice () const;
	void GuardarIndiceDocs () const;
	void GuardarInformacionColeccionDocs () const;
	void GuardarTokenizador() const;
	void GuardarVariables () const;
	//
	void LeerIndice();
	void LeerIndiceDocs(); 
	void LeerInformacionColeccionDocs();
	void LeerTokenizador();
	void LeerVariables ();
	IndexadorHash();

	std::unordered_map<std::string, InformacionTermino> _indice;
	// Índice de términos indexados accesible por el término
	std::unordered_map<std::string, InfDoc> _indiceDocs;
	// Índice de documentos indexados accesible por el nombre del documento
	InfColeccionDocs _informacionColeccionDocs;
	// Información recogida de la colección de documentos indexada
	std::string _pregunta;
	// Pregunta indexada actualmente. Si no hay ninguna indexada, contendría el valor “”
	std::unordered_map<std::string, InformacionTerminoPregunta> _indicePregunta;
	// Índice de términos indexados en una pregunta. Se almacenará en memoria 
	InformacionPregunta _infPregunta;
	// Información recogida de la pregunta indexada. Se almacenará en memoria principal
	std::unordered_set<std::string> _stopWords;
	std::string _ficheroStopWords;
	// Nombre del fichero que contiene las palabras de parada
	Tokenizador _tok;
	/* Tokenizador que se usará en la indexación.*/
	std::string _directorioIndice;
	/* “directorioIndice” será el directorio del disco duro donde se
	almacenará el índice. En caso que contenga la cadena vacía se creará en
	el directorio donde se ejecute el indexador*/
	int _tipoStemmer;

	bool _almacenarEnDisco;

	bool _almacenarPosTerm;
};
