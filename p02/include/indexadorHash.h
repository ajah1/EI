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

	// Leer las stopwords del fichero
	void ObtenerPalParada();
	// Indexar los tokens guardados en ficheros
	void indexar_tokens();
	// Comprobar si el token es stop word
	bool EsParada(const std::string& p_token);
	// Actualizar el objeto InformacionTermino de un indice ya indexado
	void actualizar_token_indexado(std::string& p_token, int aux_idDoc);
	// OBtener el size en B de un archivo
	double GetFileSize(std::string filename);
	// Dado un id de doc devuelve su nombre en un string
	int getDocId (std::string p_ruta) const;

	IndexadorHash();
	/* Este constructor se pone en la parte privada porque no se permitirá
	crear un indexador sin inicializarlo convenientemente. La inicialización
	la decidirá el alumno*/

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
	/* Palabras de parada. Se almacenará en memoria principal. El filtrado
	de palabras de parada se realizará teniendo en cuenta el parámetro
	minuscSinAcentos. Es decir que se le aplicará el mismo proceso a las
	palabras de parada almacenadas en el fichero antes de realizar el
	filtrado (p.ej. si se aplica el pasar a minúsculas los términos del
	documento a indexar, para comprobar si se ha de eliminar el término,
	éste se comparará con la versión de palabras de parada en minúsculas)*/

	std::string _ficheroStopWords;
	// Nombre del fichero que contiene las palabras de parada

	Tokenizador _tok;
	/* Tokenizador que se usará en la indexación. Se inicializará con los
	parámetros del constructor: detectComp y minuscSinAcentos, los cuales
	determinarán qué término se ha de indexar (p.ej. si se activa
	minuscSinAcentos, entonces se guardarán los términos en minúsculas y sin
	acentos)*/

	std::string _directorioIndice;
	/* “directorioIndice” será el directorio del disco duro donde se
	almacenará el índice. En caso que contenga la cadena vacía se creará en
	el directorio donde se ejecute el indexador*/

	int _tipoStemmer;
	/* 0 = no se aplica stemmer: se indexa el término tal y como aparece
	tokenizado
	// Los siguientes valores harán que los términos a indexar se les
	aplique el stemmer y se almacene solo dicho stem.
	// 1 = stemmer de Porter para español
	// 2 = stemmer de Porter para inglés
	// Para el stemmer de Porter se utilizarán los archivos
	stemmer.cpp y stemmer.h, concretamente las funciones de nombre
	“stemmer”*/

	bool _almacenarEnDisco;
	/* Esta opción está ideada para poder indexar colecciones de documentos
	lo suficientemente grandes para que su indexación no quepa en memoria,
	por lo que si es true se almacenará la mínima parte de los índices de
	los documentos en memoria principal, p.ej. solo la estructura “indice”
	para saber las palabras indexadas, guardando únicamente punteros a las
	posiciones de los archivos almacenados en disco que contendrán el resto
	de información asociada a cada término (lo mismo para indiceDocs). Se
	valorará el equilibrio para conseguir una indexación independientemente
	del tamaño del corpus a indexar, pero reduciendo el número de accesos a
	disco. Para los datos de la indexación de la pregunta no habría que
	hacer nada. En caso de que esta variable tenga valor false, se
	almacenará todo el índice en memoria principal (tal y como se ha
	descrito anteriormente).*/

	bool _almacenarPosTerm;
	/* Si es true se almacenará la posición en la que aparecen los términos
	dentro del documento en la clase InfTermDoc*/

};