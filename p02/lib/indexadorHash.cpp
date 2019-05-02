
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

bool 
IndexadorHash::Devuelve(const std::string& word, InformacionTermino& inf) {
	if (_indice.find(word) != _indice.end()) {
		inf = _indice.find(word)->second;
		return true;
	}
	return false;
}

std::unordered_map<std::string, long int> _documentos;

bool 
IndexadorHash::BorraDoc(const std::string& nomDoc) {
	// Comprobar si el documeto existe
	if (_indiceDocs.find(nomDoc) != _indiceDocs.end()) {
		int id_doc = getDocId(nomDoc);	// Necesitamos el id del documento
		// 1. Borrar Por cada token indexado la informacion del documento
		for (auto& i : _indice) {
			auto& lista_doc = i.second.apuntarListaDocs();
			// Comprobar si en _l_docs está el documento
			auto d = lista_doc.find(id_doc);
			if (d != lista_doc.end()) { // i(indice) se encuentra en el d(docuemnto)
				std::cout << "i(indice) se encuentra en el d(docuemnto)\n";
				std::cout << i.first << "  " << i.second << '\n';
				// Restar la ft
				i.second.decftc(d->second.getft()); // dec a la ftc la ft del d
				// Borrar d de la _l_docs
				lista_doc.erase(id_doc);
				std::cout << i.second << '\n';
				std::cout << "_-----------\n";
				//std::cout << "borrar de la l_doc: " << lista_doc.size() << std::endl;
			}
		};

		// Si _l_docs pasa a ser 0 borrar i del _indice
		for (auto& i : _indice) {
			auto& lista_doc = i.second.apuntarListaDocs();
			std::cout << "con lista size: " << lista_doc.size() << std::endl;
			std::cout << i.second << std::endl;
			if (lista_doc.size() == 0) {
				std::cout << "Borrar la i: " << i.first << std::endl;
				_indice.erase(i.first);
			}
		};

		// Restar contadores de informacionCOlecccionDOcs
		auto& d_borrar = _indiceDocs.find(nomDoc)->second;
		_informacionColeccionDocs.setnumTotalPalSinParada(
			_informacionColeccionDocs.getnumTotalPalSinParada()-d_borrar.getnumPalSinParada());
			//long int _numTotalPal; // No total de palabras en la colección
		_informacionColeccionDocs.setnumTotalPal(
			_informacionColeccionDocs.getnumTotalPal()-d_borrar.getnumPal());
			//long int _numDocs; // No total de documentos en la colección
		_informacionColeccionDocs.setnumDocs(_informacionColeccionDocs.getnumDocs());
			//long int _numTotalPalDiferentes;
		_informacionColeccionDocs.setnumTotalPalDiferentes(
			_informacionColeccionDocs.getnumTotalPalDiferentes()-d_borrar.getnumPalDiferentes());
			//long int _tamBytes; // Tamaño total en bytes de la colección
		_informacionColeccionDocs.settamBytes(
			_informacionColeccionDocs.gettamBytes()-d_borrar.gettamBytes());




		// Borrar de _indiceDOcs 
		_indiceDocs.erase(nomDoc);
		// Borrar del auxiliasr _documentos
		_documentos.erase(nomDoc);

		return true;
	}	

	return false;
}



int 
IndexadorHash::getDocId (std::string p_ruta) const { 
	if (_documentos.find(p_ruta) != _documentos.end())
		return _documentos.find(p_ruta)->second;
	return -1;
}

bool 
IndexadorHash::Devuelve(const std::string& word, const std::string& nomDoc, InfTermDoc& InfDoc) {
	// 1. word esta indexado
	if (_indice.find(word) != _indice.end()) {
		// 2. esta en la lista de documentos
			// Obtener el string de un documento a partir del id
			// comprobar que el id obtenido pertenece a _l_docs
		int id_doc = getDocId( nomDoc );
		auto& lista_doc = _indice.find(word)->second.apuntarListaDocs();
		if (id_doc != -1 && lista_doc.find(id_doc) != lista_doc.end()) {
			InfDoc = lista_doc.find(id_doc)->second;
			return true;
		}
	}

	return false;
}

bool 
IndexadorHash::Existe(const std::string& word) const {
	return _indice.find(word) != _indice.end();
}

void
IndexadorHash::indexar_tokens() {

	// Abrir la lista de ficheros 
    FILE* fp = fopen("listaFicheros_corto.txt", "r");
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    // Contadores para informacionTermino
   	long int aux_idDoc = 1;
	int aux_numPal = 0, aux_numParada = 0, aux_numPalDiferentes = 0;

	// Contadores para InformacionColeccionDocs
	int aux_numdocs = 0, aux_numtotalpal = 0, aux_numTotalPalSinParada = 0;
	int aux_numTotalPalDiferentes = 0, aux_tamBytes = 0;
	
	// Por cada fichero tokenizado, procesar sus tokens
    while ((read = getline(&line, &len, fp)) != -1) {
       line[read-1] = 0;
       std::string aux_line = line;
       std::string f_tokens = aux_line+".tk";
       std::cout << f_tokens << '\n';
       ++aux_numdocs;		// Update numero de documentos con tokens
       _documentos.insert({aux_line, aux_idDoc});

   		// Abrir fichero con los tokens
       	std::ifstream file;
       	file.open(f_tokens);
       	std::string token = "";
       	// Indexar el token leido
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
	       				// Añadir en la lista de documentos del termino el primer documento
	       				auto* lista_doc = &_indice.find(token)->second.apuntarListaDocs();
	       				lista_doc->insert({aux_idDoc, InfTermDoc(1, -1000)});
	       				

	       				++aux_numPalDiferentes;
	       			// Ya está indexado, actualizar informacionTerminod el token
		       		} else {
		       			//std::cout << "actualizar ft" << std::endl;
		       			actualizar_token_indexado(token, aux_idDoc);
		       		}
	       		} else {
	       			++aux_numParada;
	       		}
	       	}
       	}

       	// informacionColeccionDocs
       	aux_numtotalpal += aux_numPal;
       	aux_numTotalPalDiferentes += aux_numPalDiferentes;
       	aux_numTotalPalSinParada += (aux_numPal-aux_numParada);
       	
       	// InformacionTermino
       	_indiceDocs.insert({aux_line,InfDoc(aux_idDoc++, aux_numPal, (aux_numPal-aux_numParada), aux_numPalDiferentes, GetFileSize(aux_line))});
       	
       	// informacionColeccionDocs
       	aux_tamBytes += GetFileSize(aux_line);					// Actualizar B
		aux_numPal = aux_numParada = aux_numPalDiferentes = 0;	// Setear auxiliares

    }

    // Información recogida de la colección de documentos indexada
	_informacionColeccionDocs.setnumDocs (aux_numdocs);
	_informacionColeccionDocs.setnumTotalPal (aux_numtotalpal);
	_informacionColeccionDocs.settamBytes (aux_tamBytes); 
	_informacionColeccionDocs.setnumTotalPalSinParada (aux_numTotalPalSinParada);
	_informacionColeccionDocs.setnumTotalPalDiferentes(aux_numTotalPalDiferentes);

    fclose(fp);
    if (line) free(line);

}

double 
IndexadorHash::GetFileSize(std::string filename){
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

void
IndexadorHash::actualizar_token_indexado(std::string& p_token, int aux_idDoc) {
	// Acceder al token de _indice
	InformacionTermino* inft = &_indice.find(p_token)->second;
	inft->incFTC();

	// Del token actualizar su _l_docs
	auto lista_doc = &_indice.find(p_token)->second.apuntarListaDocs();
	auto busca = lista_doc->find(aux_idDoc);
	// IF no existe el documento THEN insertar en _l_docs
	if (busca == lista_doc->end())
		lista_doc->insert({aux_idDoc, InfTermDoc(1, -1000)});
	else { // THEN actualizar el existente
		busca->second.UpdatePosTerm(-1000);
	}
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