
#include "indexadorHash.h"
#include <fstream>
#include <sys/stat.h>

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
	_tok.TokenizarListaFicheros("no_se_usa_este_parametro");

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

   	long int aux_idDoc = 1;
	int aux_numPal = 0; 		  // No total de palabras del documento
	int aux_numParada = 0; // No total de palabras stop-words del documento
	int aux_numPalDiferentes = 0;
	//No total de palabras diferentes que no sean stop-words (sin acumular
	//la frecuencia de cada una de ellas)

	int aux_numdocs = 0;
	int aux_numtotalpal = 0;
	int aux_numTotalPalSinParada = 0;
	int aux_numTotalPalDiferentes = 0;
	int aux_tamBytes = 0;
	
    while ((read = getline(&line, &len, fp)) != -1) {
       line[read-1] = 0;
       std::string aux_line = line;
       std::string f_tokens = aux_line+".tk";
       std::cout << f_tokens << '\n';
       ++aux_numdocs;


   		// while(leerTokens)
       	std::ifstream file;
       	file.open(f_tokens);
       	std::string token = "";
       	while (getline(file, token)) {
       		// un nuevo token
       		std::cout << "leyendo token->" << token << "<-\n"; 
       		// comrpobar si es parado o vacio
       		if (token != "") {
       			++aux_numPal;
	       		if (!EsParada(token)) {
	       			// Indexar el token
	       			if  (_indice.find(token)  == _indice.end()) {
	       				std::cout << "indexar el token:-->" << token << "<--\n";
	       				_indice.insert({token, InformacionTermino(1)});
	       				++aux_numPalDiferentes;
	       			// Ya está indexado, actualizar informacionTerminod el token
		       		} else {
		       			//std::cout << "actualizar ft" << std::endl;
		       			actualizar_token_indexado(token);
		       		}
	       		} else {
	       			++aux_numParada;
	       		}
	       	}
       	}

       	aux_numtotalpal += aux_numPal;
       	aux_numTotalPalDiferentes += aux_numPalDiferentes;
       	aux_numTotalPalSinParada += (aux_numPal-aux_numParada);


       	//idDoc: 1	numPal: 6	 numPalSinParada: 4	numPalDiferentes: 3	tamBytes: 30
       	_indiceDocs.insert(
       		{aux_line, 
       		InfDoc(aux_idDoc++, aux_numPal, (aux_numPal-aux_numParada), aux_numPalDiferentes, GetFileSize(aux_line))}
   		);
       	aux_tamBytes += GetFileSize(aux_line);
       	   		
	// Tamaño total en bytes de la colección

		aux_numPal = 0; 		  
		aux_numParada = 0;
		aux_numPalDiferentes = 0;

    }

    fclose(fp);
    if (line) free(line);

	_informacionColeccionDocs.setnumDocs (aux_numdocs);
	// No total de documentos en la colección
	_informacionColeccionDocs.setnumTotalPal (aux_numtotalpal); 
	// No total de palabras en la colección
	_informacionColeccionDocs.settamBytes (aux_tamBytes); 
	_informacionColeccionDocs.setnumTotalPalSinParada (aux_numTotalPalSinParada); 
	// No total de palabras sin stop-words en la colección
	_informacionColeccionDocs.setnumTotalPalDiferentes(aux_numTotalPalDiferentes);
}

double IndexadorHash::GetFileSize(std::string filename){
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
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
int 
IndexadorHash::NumPalIndexadas() const {
	return _indice.size();
}
bool 
IndexadorHash::ListarDocs(const std::string& nomDoc) {
	//corpus_corto/fichero1.txt	idDoc: 1	numPal: 6	numPalSinParada: 4	numPalDiferentes: 3	tamBytes: 30
	if (_indiceDocs.find(nomDoc) != _indiceDocs.end()) {
		InfDoc* infdoc = &_indiceDocs.find(nomDoc)->second;
		std::cout 	
			<< nomDoc
			<< " idDoc: " 				<< infdoc->getidDoc()
			<< "	numPal: " 			<< infdoc->getnumPal()
			<< "	numPalSinParada: " 	<< infdoc->getnumPalSinParada()
			<< " numPalDiferentes: " 	<< infdoc->getnumPalDiferentes()
			<< " tamBytes: " 			<< infdoc->gettamBytes() << '\n';
		return true;
	} else {
		std::cout << "no puede listar el documento \n";
		return false;
	}
}
void 
IndexadorHash::ListarInfColeccDocs() const {
	std::cout
		<< "numDocs: " << _informacionColeccionDocs.getnumDocs()
		<< " numTotalPal: " << _informacionColeccionDocs.getnumTotalPal()
		<< " numTotalPalSinParada: " << _informacionColeccionDocs.getnumTotalPalSinParada()
		<< " numTotalPalDiferentes: " << _informacionColeccionDocs.getnumTotalPalDiferentes()
		<< " tamBytes: " << _informacionColeccionDocs.gettamBytes() << '\n';
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