#pragma once

#include "tokenizador.h"
#include "indexadorInformacion.h"
#include <unordered_set>


class IndexadorHash {

public:

private:
	IndexadorHash();
	/* Este constructor se pone en la parte privada porque no se permitirá
	crear un indexador sin inicializarlo convenientemente. La inicialización
	la decidirá el alumno*/

	std::unordered_map<std::string, InformacionTermino> indice;
	// Índice de términos indexados accesible por el término

	std::unordered_map<std::string, InfDoc> indiceDocs;
	// Índice de documentos indexados accesible por el nombre del documento

	InfColeccionDocs informacionColeccionDocs;
	// Información recogida de la colección de documentos indexada

	std::string pregunta;
	// Pregunta indexada actualmente. Si no hay ninguna indexada, contendría el valor “”

	std::unordered_map<std::string, InformacionTerminoPregunta> indicePregunta;
	// Índice de términos indexados en una pregunta. Se almacenará en memoria 

	InformacionPregunta infPregunta;
	// Información recogida de la pregunta indexada. Se almacenará en memoria principal

	std::unordered_set<std::string> stopWords;
	/* Palabras de parada. Se almacenará en memoria principal. El filtrado
	de palabras de parada se realizará teniendo en cuenta el parámetro
	minuscSinAcentos. Es decir que se le aplicará el mismo proceso a las
	palabras de parada almacenadas en el fichero antes de realizar el
	filtrado (p.ej. si se aplica el pasar a minúsculas los términos del
	documento a indexar, para comprobar si se ha de eliminar el término,
	éste se comparará con la versión de palabras de parada en minúsculas)*/

	std::string ficheroStopWords;
	// Nombre del fichero que contiene las palabras de parada

	Tokenizador tok;
	/* Tokenizador que se usará en la indexación. Se inicializará con los
	parámetros del constructor: detectComp y minuscSinAcentos, los cuales
	determinarán qué término se ha de indexar (p.ej. si se activa
	minuscSinAcentos, entonces se guardarán los términos en minúsculas y sin
	acentos)*/

	std::string directorioIndice;
	/* “directorioIndice” será el directorio del disco duro donde se
	almacenará el índice. En caso que contenga la cadena vacía se creará en
	el directorio donde se ejecute el indexador*/

	int tipoStemmer;
	/* 0 = no se aplica stemmer: se indexa el término tal y como aparece
	tokenizado
	// Los siguientes valores harán que los términos a indexar se les
	aplique el stemmer y se almacene solo dicho stem.
	// 1 = stemmer de Porter para español
	// 2 = stemmer de Porter para inglés
	// Para el stemmer de Porter se utilizarán los archivos
	stemmer.cpp y stemmer.h, concretamente las funciones de nombre
	“stemmer”*/

	bool almacenarEnDisco;
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

	bool almacenarPosTerm;
	/* Si es true se almacenará la posición en la que aparecen los términos
	dentro del documento en la clase InfTermDoc*/

};