
#include "indexadorHash.h"
#include <fstream>


IndexadorHash::IndexadorHash() {}
IndexadorHash::~IndexadorHash() {}

IndexadorHash::IndexadorHash(
	const std::string& fichStopWords, 
	const std::string& delimitadores,
	const bool& detectComp, 			
	const bool& minuscSinAcentos, 
	const std::string& dirIndice, 		
	const int& tStemmer, 
	const bool& almEnDisco, 			
	const bool& almPosTerm) {

		_ficheroStopWords = fichStopWords;

		_tok.DelimitadoresPalabra(delimitadores);
		_tok.CasosEspeciales(detectComp);
		_tok.PasarAminuscSinAcentos(minuscSinAcentos);

		_directorioIndice = dirIndice;
		_tipoStemmer = tStemmer;
		_almacenarEnDisco = almEnDisco;
		_almacenarPosTerm = almPosTerm;

		ObtenerPalParada();
}




bool 
IndexadorHash::Indexar (const std::string& ficheroDocumentos) {
	
	// Tokenizar los documentos
	_tok.TokenizarListaFicheros("noImporta");

	// Indexar los tokens
	indexar_tokens();



	return true;
}

void
IndexadorHash::indexar_tokens() {

	// Por cada fichero de tokens
    FILE* fp = fopen("listaFicheros_corto.txt", "r");
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1) {
       line[read-1] = 0;
       std::string aux_line = line;
       std::string f_tokens = aux_line+".tk";
       std::cout << f_tokens << '\n';

   		// while(leerTokens)
       	std::ifstream file;
       	file.open(f_tokens);
       	std::string token = "";
       	while (getline(file, token)) {
       		// un nuevo token
       		std::cout << "leyendo token->" << token << "<-\n"; 
       		// comrpobar si es parado o vacio
       		if (!EsParada(token) && token != "") {
       			// Indexar el token
       			if  (_indice.find(token)  == _indice.end()) {
       				//std::cout << "indexar el token:-->" << token << "<--\n";
       				_indice.insert({token, InformacionTermino(1)});
       			// Ya está indexado, actualizar informacionTerminod el token
	       		} else {
	       			std::cout << "actualizar ft" << std::endl;
	       			actualizar_token_indexado(token);
	       		}
       		}
       	}
    }

    fclose(fp);
    if (line) free(line);
}

void
IndexadorHash::actualizar_token_indexado(std::string& p_token) {
	InformacionTermino* inft = &_indice.find(p_token)->second;
	inft->incFTC();
}
bool
IndexadorHash::EsParada(const std::string& p_token) {
	return (_stopWords.find(p_token) != _stopWords.end());
}




void
IndexadorHash::ObtenerPalParada() {
	std::ifstream file;
	file.open(_ficheroStopWords);
	
	std::string line = "";

	if (!DevolverPasarAminuscSinAcentos()) {
		while (getline(file, line))
			_stopWords.insert(line);
	} else {
		while (getline(file, line)) {
			_tok.EliminarMinusAcentos(line);
			_stopWords.insert(line);
		}
	}

	file.close();
}


//////////////////////////////////////////////////////////////////////////////
////// 			     GETTERS-SETTERS                                     ///// 
//////////////////////////////////////////////////////////////////////////////
void 
IndexadorHash::ListarPalParada() const {
	for (auto word : _stopWords)
		std::cout << word << std::endl;
}

std::string 
IndexadorHash::DevolverDelimitadores () const {
	return _tok.DelimitadoresPalabra();
}
std::string
IndexadorHash::DevolverDirIndice () const {
	return _directorioIndice;
}
int
IndexadorHash::DevolverTipoStemming () const {
	return _tipoStemmer;
} 
bool
IndexadorHash::DevolverAlmEnDisco () const {
	return _almacenarEnDisco;
} 
bool
IndexadorHash::DevolverAlmacenarPosTerm () const {
	return _almacenarPosTerm;
} 
bool
IndexadorHash::DevolverPasarAminuscSinAcentos () {
	return _tok.PasarAminuscSinAcentos();
} 
bool
IndexadorHash::DevolverCasosEspeciales () {
	return _tok.CasosEspeciales();
} 
int 
IndexadorHash::NumPalParada() const {
	return _stopWords.size();
}
std::string
IndexadorHash::DevolverFichPalParada() const {
	return _ficheroStopWords;
}

std::ostream& operator<<(std::ostream& s, const IndexadorHash& p) { 
	return s 	
		<< "Fichero con el listado de palabras de parada: " 		
			<< p._ficheroStopWords << std::endl
		<< "Tokenizador: " 											
			<< p._tok << std::endl
		<< "Directorio donde se almacenara el indice generado: " 	
			<< p._directorioIndice << std::endl 
		<< "Stemmer utilizado: " 									
			<< p._tipoStemmer << std::endl 
		<< "Informacion de la coleccion indexada: " 				
			<< p._informacionColeccionDocs << std::endl 
		<< "Se almacenara parte del indice en disco duro: " 		
			<< p._almacenarEnDisco << std::endl 
		<< "Se almacenaran las posiciones de los terminos: " 		
			<< p._almacenarPosTerm;  
}