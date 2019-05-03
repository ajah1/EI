
#include "indexadorHash.h"
#include <fstream>
#include <stack>
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
		_tok.SetDelimitadoresPalabraAux(delimitadores);

		_directorioIndice = dirIndice;
		_tipoStemmer = tStemmer;
		_almacenarEnDisco = almEnDisco;
		_almacenarPosTerm = almPosTerm;

		ObtenerPalParada();
}

bool
IndexadorHash::IndexarDirectorio(const std::string& dirAIndexar) {
	struct stat dir;
	// Compruebo la existencia del directorio
	int err=stat(dirAIndexar.c_str(), &dir);

	if(err==-1 || !S_ISDIR(dir.st_mode)){
		return false;
	}
	else {
		// Hago una lista en un fichero con find>fich
		//std::string cmd="find "+dirAIndexar+" -follow |sort > .lista_fich"; //Extrae todas las rutas de los ficheros a un archivo .lista_fich
		// Asegurarme que borro los .tk
		std::string cmd = "rm -f "+dirAIndexar+"/*.tk";
		system(cmd.c_str());
		cmd="find "+dirAIndexar+" -type f > .lista_fich";
		system(cmd.c_str()); //Ejecuta el comando cmd en el tablero de comandos
		return Indexar(".lista_fich");
	}
}



bool 
IndexadorHash::Indexar (const std::string& ficheroDocumentos) {
	
	// Tokenizar los documentos
	_tok.TokenizarListaFicheros(ficheroDocumentos);

	// Indexar los tokens
	indexar_tokens(ficheroDocumentos);

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

		// Si _l_docs pasa a ser 0 borrar i del _indice


		for (auto& i : _indice) {
			auto& lista_doc = i.second.apuntarListaDocs();
			// Comprobar si en _l_docs está el documento
			auto d = lista_doc.find(id_doc);
			if (d != lista_doc.end()) { // i(indice) se encuentra en el d(docuemnto)
				//std::cout << i.first << "  \n" << i.second << '\n';
				// Restar la ft
				i.second.decftc(d->second.getft()); // dec a la ftc la ft del d
				// Borrar d de la _l_docs
				lista_doc.erase(id_doc);
				//std::cout << i.second << '\n';
				//std::cout << "_-----------\n";
			}
		}

		std::stack<std::string> s;

		for (auto it=_indice.begin(); it!=_indice.end(); ++it) {
			auto l = it->second.apuntarListaDocs();
			if (l.size() == 0) {
				s.push(it->first);
				//_informacionColeccionDocs.setnumTotalPalDiferentes(_informacionColeccionDocs.getnumTotalPalDiferentes()-1);
			}
		}

		std::string i = "";
		while (!s.empty()) {
			i = s.top();
			s.pop();
			_indice.erase(i);
		}


		// Restar contadores de informacionCOlecccionDOcs
		auto& d_borrar = _indiceDocs.find(nomDoc)->second;
		_informacionColeccionDocs.setnumTotalPalSinParada(
			_informacionColeccionDocs.getnumTotalPalSinParada()-d_borrar.getnumPalSinParada());
			//long int _numTotalPal; // No total de palabras en la colección
		_informacionColeccionDocs.setnumTotalPal(
			_informacionColeccionDocs.getnumTotalPal()-d_borrar.getnumPal());
			//long int _numDocs; // No total de documentos en la colección
		_informacionColeccionDocs.setnumDocs(_informacionColeccionDocs.getnumDocs()-1);
			//long int _numTotalPalDiferentes;
		_informacionColeccionDocs.setnumTotalPalDiferentes(
			_indice.size());
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

IndexadorHash::IndexadorHash(const std::string& directorioIndexacion) {

}

bool
IndexadorHash::GuardarIndexacion() const {
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
IndexadorHash::indexar_tokens(const std::string& ficherodocumentos) {

	// Abrir la lista de ficheros 
    FILE* fp = fopen(ficherodocumentos.c_str(), "r");
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    // Contadores para informacionTermino
   	long int aux_idDoc = 1;
	int aux_numPal = 0, aux_numParada = 0;
	std::unordered_set<std::string> aux_diferentes;
	std::unordered_set<std::string> dif;

	// Contadores para InformacionColeccionDocs
	int aux_numdocs = 0, aux_numtotalpal = 0, aux_numTotalPalSinParada = 0;
	int aux_numTotalPalDiferentes = 0, aux_tamBytes = 0;
	int aux_posicion = 0;
	
	// Por cada fichero tokenizado, procesar sus tokens
    while ((read = getline(&line, &len, fp)) != -1) {
       line[read-1] = 0;
       std::string aux_line = line;
       std::string f_tokens = aux_line+".tk";
       // std::cout << f_tokens << '\n';
       ++aux_numdocs;		// Update numero de documentos con tokens
       _documentos.insert({aux_line, aux_idDoc});

   		// Abrir fichero con los tokens
       	std::ifstream file;
       	file.open(f_tokens);
       	std::string token = "";
       	// Indexar el token leido
       	aux_posicion = 0;
       	while (getline(file, token)) {
       		// un nuevo token
       		//std::cout << "leyendo token->" << token << "<-\n";
       		// comrpobar si es parado o vacio
       		if (token != "") {
       			++aux_numPal;
	       		if (!EsParada(token)) {
	       			// Indexar el token
	       			if  (_indice.find(token)  == _indice.end()) {
	       				//std::cout << "indexar el token:-->" << token << "<--\n";
	       				
	       				_indice.insert({token, InformacionTermino(1)});
	       				// Añadir en la lista de documentos del termino el primer documento
	       				auto* lista_doc = &_indice.find(token)->second.apuntarListaDocs();
	       				lista_doc->insert({aux_idDoc, InfTermDoc(1, aux_posicion)});
	       				aux_diferentes.insert(token);
	       				dif.insert(token);

	       			// Ya está indexado, actualizar informacionTerminod el token
		       		} else {
		       			//std::cout << "actualizar ft" << std::endl;
		       			actualizar_token_indexado(token, aux_idDoc, aux_posicion);
						// IF: p_token no pertenece a diferentes THEN insertar el token
						if (aux_diferentes.find(token) == aux_diferentes.end()) {
							aux_diferentes.insert(token);
						}
		       		}
		       		
	       		} else {
	       			++aux_numParada;
	       		}
	       		++aux_posicion;
	       	}
       	}

       	// informacionColeccionDocs
       	aux_numtotalpal += aux_numPal;
       	aux_numTotalPalDiferentes += aux_diferentes.size();
       	aux_numTotalPalSinParada += (aux_numPal-aux_numParada);
       	
       	// InformacionTermino
       	_indiceDocs.insert({aux_line,InfDoc(aux_idDoc++, aux_numPal, (aux_numPal-aux_numParada), aux_diferentes.size(), GetFileSize(aux_line))});
       	
       	// informacionColeccionDocs
       	aux_tamBytes += GetFileSize(aux_line);					// Actualizar B
		aux_numPal = aux_numParada = 0;	// Setear auxiliares
		aux_diferentes.clear();

    }

    // Información recogida de la colección de documentos indexada
	_informacionColeccionDocs.setnumDocs (aux_numdocs);
	_informacionColeccionDocs.setnumTotalPal (aux_numtotalpal);
	_informacionColeccionDocs.settamBytes (aux_tamBytes); 
	_informacionColeccionDocs.setnumTotalPalSinParada (aux_numTotalPalSinParada);
	_informacionColeccionDocs.setnumTotalPalDiferentes(dif.size());

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
IndexadorHash::actualizar_token_indexado(std::string& p_token, int aux_idDoc, int aux_posicion) {
	// Acceder al token de _indice
	InformacionTermino* inft = &_indice.find(p_token)->second;
	inft->incFTC();

	// Del token actualizar su _l_docs
	auto lista_doc = &_indice.find(p_token)->second.apuntarListaDocs();
	auto busca = lista_doc->find(aux_idDoc);
	// IF no existe el documento THEN insertar en _l_docs
	if (busca == lista_doc->end())
		lista_doc->insert({aux_idDoc, InfTermDoc(1, aux_posicion)});
	else { // THEN actualizar el existente
		busca->second.UpdatePosTerm(aux_posicion);
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
			<< "\tidDoc: " 				<< infdoc->getidDoc()
			<< "\tnumPal: " 			<< infdoc->getnumPal()
			<< "\tnumPalSinParada: " 	<< infdoc->getnumPalSinParada()
			<< "\tnumPalDiferentes: " 	<< infdoc->getnumPalDiferentes()
			<< "\ttamBytes: " 			<< infdoc->gettamBytes() << '\n';
		return true;
	} else {
		return false;
	}
}
void 
IndexadorHash::ListarInfColeccDocs() const {
	std::cout
		<< "numDocs: " 					<< _informacionColeccionDocs.getnumDocs()
		<< "\tnumTotalPal: " 			<< _informacionColeccionDocs.getnumTotalPal()
		<< "\tnumTotalPalSinParada: " 	<< _informacionColeccionDocs.getnumTotalPalSinParada()
		<< "\tnumTotalPalDiferentes: " 	<< _informacionColeccionDocs.getnumTotalPalDiferentes()
		<< "\ttamBytes: " 				<< _informacionColeccionDocs.gettamBytes() 
		<< '\n';
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
			//_tok.EliminarMinusAcentos(line);
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

void IndexadorHash::CopiaIndice(std::unordered_map<std::string, InformacionTermino> p_indice) {
	for (auto& i : p_indice) {
		_indice.insert({i.first, i.second});
	}
}
void IndexadorHash::CopiaIndiceDocs(std::unordered_map<std::string, InfDoc> p_indiceDocs) {
	for (auto& i : p_indiceDocs) {
		_indiceDocs.insert({i.first, i.second});
	}
}

void IndexadorHash::CopiaindicePregunta(std::unordered_map<std::string, InformacionTerminoPregunta> p_indicepregunta) {
	for (auto& i : p_indicepregunta) {
		_indicePregunta.insert({i.first, i.second});
	}
}
void IndexadorHash::CopiaStopWords(std::unordered_set<std::string> p_parada) {
	for (auto& i : p_parada) {
		_stopWords.insert(i);
	}
}

void IndexadorHash::CopiaICD (const InfColeccionDocs& p_id) {
	_informacionColeccionDocs.setnumDocs(p_id.getnumDocs());
	_informacionColeccionDocs.setnumTotalPal(p_id.getnumTotalPal());
	_informacionColeccionDocs.settamBytes(p_id.gettamBytes());
	_informacionColeccionDocs.setnumTotalPalSinParada(p_id.getnumTotalPalSinParada());
	_informacionColeccionDocs.setnumTotalPalDiferentes(p_id.getnumTotalPalDiferentes());
}

IndexadorHash::IndexadorHash(const IndexadorHash& p_i) {
	//std::unordered_map<std::string, InformacionTermino> _indice;
	CopiaIndice(p_i._indice);
	//std::unordered_map<std::string, InfDoc> _indiceDocs;
	CopiaIndiceDocs(p_i._indiceDocs);
	//InfColeccionDocs _informacionColeccionDocs;
	CopiaICD(p_i._informacionColeccionDocs);

	//std::unordered_map<std::string, InformacionTerminoPregunta> _indicePregunta;
	CopiaindicePregunta(p_i._indicePregunta);


	// InformacionPregunta _infPregunta;
	//_infPregunta(p_i._infPregunta);

	//std::unordered_set<std::string> _stopWords;
	CopiaStopWords(p_i._stopWords);

	_pregunta = p_i._pregunta;
	_ficheroStopWords = p_i._ficheroStopWords;
	_tok = p_i._tok;
	_directorioIndice = p_i._directorioIndice;
	_tipoStemmer = p_i._tipoStemmer;
	_almacenarEnDisco = p_i._almacenarEnDisco;
	_almacenarPosTerm = p_i._almacenarPosTerm;

}
