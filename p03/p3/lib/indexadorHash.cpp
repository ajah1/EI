#include <cstring>
#include "indexadorHash.h"
#include <fstream>
#include <stack>
#include <sys/stat.h>
#include <algorithm>
/////////////////////////////////////////////////////////////////
// MAPA PARA LISTAR DADO UNA RUTA DE UN DOCUMENTO EL ID ASOCIADO
////////////////////////////////////////////////////////////////
std::unordered_map<std::string, long int> _documentos;


IndexadorHash::IndexadorHash() {}

IndexadorHash::~IndexadorHash() {}

IndexadorHash::IndexadorHash (
	const std::string& fichStopWords, const std::string& delimitadores,
	const bool& detectComp, const bool& minuscSinAcentos, 
	const std::string& dirIndice, 		
	const int& tStemmer, 
	const bool& almEnDisco, const bool& almPosTerm) {

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
IndexadorHash::IndexarPregunta(const std::string& preg) {
	std::list<std::string> tokens;
	std::string p_aux = preg;
	std::transform(p_aux.begin(), p_aux.end(), p_aux.begin(), ::tolower);
	_tok.TokenizarGeneral(p_aux, tokens);

	// Posicion actual del token
	int posicion 	= 0,
	// contadores para _infPregunta 
		aux_ntp 	= 0,	//numTotalPal
		aux_ntpsp 	= 0;	//numTotalPalSinParada

	// IF(al menos un token es valido) THEN Vaciar los campos
	_indicePregunta.clear();
	_infPregunta.~InformacionPregunta();

	for (auto& t : tokens) {
		if (t != "") {
			if (!EsParada(t)) {
				// IF: t no está indexado THEN insertar en _indicePregunta
				if (_indicePregunta.find(t) == _indicePregunta.end()) {
					_indicePregunta.insert({t, InformacionTerminoPregunta(0)});
				}
				// Actualizar la _ft del t. Y anyadir la nueva posicion
				auto& t_infpreg = _indicePregunta.find(t)->second;
				t_infpreg.IncFt();					// ++_ft
				t_infpreg.NuevaPosicion(posicion);	// _posTerm.push_back(posicion)
				
				++aux_ntpsp;
			}

			// Calcular la posicion del sigueinte token
			++posicion;
			// 
			++aux_ntp;
		}
	}


	if (_indicePregunta.size() > 0) {
		// indexar la pregunta
		_pregunta = p_aux;
		// actualizar _infPregunta
		_infPregunta.setNumTotalPal(aux_ntp);
		_infPregunta.setNumTotalPalSinParada(aux_ntpsp);
		_infPregunta.setNumTotalPalDiferentes(_indicePregunta.size());
		return true;
	}

	return false;
}

bool 	 
IndexadorHash::DevuelvePregunta(
	const std::string& word, 
	InformacionTerminoPregunta& inf) const {

	if (_indicePregunta.find(word) != _indicePregunta.end()) {
		inf = _indicePregunta.find(word)->second;
		return true;
	}

	return false;
}
bool 
IndexadorHash::DevuelvePregunta (std::string& preg) const {
	preg = _pregunta;
	return _indicePregunta.size() > 0;
}
bool 
IndexadorHash::DevuelvePregunta(InformacionPregunta& inf) const {
	inf = _infPregunta;
	return _indicePregunta.size() > 0;
}

/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
bool
IndexadorHash::IndexarDirectorio(const std::string& dirAIndexar) {
	struct stat dir;
	int err = stat(dirAIndexar.c_str(), &dir);

	if(err==-1 || !S_ISDIR(dir.st_mode)){
		return false;
	} else {
		// Borrar los *.tk que otras ejecuciones
		std::string cmd = "rm -f "+dirAIndexar+"/*.tk";
		system(cmd.c_str());
		// Obtener la lista de ficheros a indexar
		cmd="find "+dirAIndexar+" -type f | sort -n > .lista_fich";
		system(cmd.c_str());
		return Indexar(".lista_fich");
	}
}
/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
bool 
IndexadorHash::Indexar (const std::string& ficheroDocumentos) {
	// Tokenizar los documentos
	_tok.TokenizarListaFicheros(ficheroDocumentos);
	// Indexar los tokens
	indexar_tokens(ficheroDocumentos);

	return true;
}
/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
bool 
IndexadorHash::Devuelve(const std::string& word, InformacionTermino& inf) {
	if (_indice.find(word) != _indice.end()) {
		inf = _indice.find(word)->second;
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
bool 
IndexadorHash::BorraDoc(const std::string& nomDoc) {
	// Comprobar si el documeto existe
	if (_indiceDocs.find(nomDoc) != _indiceDocs.end()) {
		// Borrar Por cada token indexado la informacion del documento
		int id_doc = getDocId(nomDoc);		// Necesitamos el id del documento
		for (auto& i : _indice) {
			// Referencia a la variable privada _l_docs
			auto& lista_doc = i.second.apuntarListaDocs();
			// Comprobar si en _l_docs está el documento
			auto d = lista_doc.find(id_doc);
			if (d != lista_doc.end()) {				// Restar la ft
				i.second.decftc(d->second.getft()); // dec a la ftc la ft del d
				lista_doc.erase(id_doc);			// Borrar d de la _l_docs
			}
		}

		std::stack<std::string> s;
		// Lista en una pila S los indices cuya _l_docs pasa a ser 0
		for (auto it=_indice.begin(); it!=_indice.end(); ++it) {
			auto l = it->second.apuntarListaDocs();	// Referencia a _l_docs
			if (l.size() == 0) {s.push(it->first);}	// IF _l_docs.size() == 0 THEN store i key
		}
		// Borrar de _indice aquellos i que almacena la pila
		std::string i = "";
		while (!s.empty()) {
			i = s.top();
			s.pop();
			_indice.erase(i);
		}

		// Actualizar los valores de la infColDocs
		auto& d_borrar = _indiceDocs.find(nomDoc)->second; 
		_informacionColeccionDocs.setnumTotalPalSinParada(
			_informacionColeccionDocs.getnumTotalPalSinParada()-d_borrar.getnumPalSinParada());
		_informacionColeccionDocs.setnumTotalPal(
			_informacionColeccionDocs.getnumTotalPal()-d_borrar.getnumPal());
		_informacionColeccionDocs.setnumDocs(_informacionColeccionDocs.getnumDocs()-1);
		_informacionColeccionDocs.setnumTotalPalDiferentes(_indice.size());
		_informacionColeccionDocs.settamBytes(
			_informacionColeccionDocs.gettamBytes()-d_borrar.gettamBytes());

		// Borrar el documento 
		_indiceDocs.erase(nomDoc);		// de _indiceDOcs
		_documentos.erase(nomDoc);		// del auxiliar _documentos

		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////
// DADA LA RUTA DE UN FICHERO DEVUELVE EL ID ASOCIADO
/////////////////////////////////////////////////////////////
int
IndexadorHash::getDocId (std::string p_ruta) const { 
	if (_documentos.find(p_ruta) != _documentos.end())
		return _documentos.find(p_ruta)->second;
	return -1;
}

/////////////////////////////////////////////////////////////
// TRUE SI EL STRING ESTA EN LA VARIABLE PRIVADA _STOPWORDS
/////////////////////////////////////////////////////////////
bool 
IndexadorHash::Devuelve(const std::string& word, const std::string& nomDoc, InfTermDoc& InfDoc) {
	// SI word esta indexado
	if (_indice.find(word) != _indice.end()) {
		// Obtener el id de un documento a partir del string
		int id_doc = getDocId( nomDoc );
		// comprobar que el id obtenido pertenece a _l_docs
		auto& lista_doc = _indice.find(word)->second.apuntarListaDocs();
		if (id_doc != -1 && lista_doc.find(id_doc) != lista_doc.end()) {
			InfDoc = lista_doc.find(id_doc)->second;
			return true;
		}
	}

	return false;
}
/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
bool 
IndexadorHash::Existe(const std::string& word) const {
	return _indice.find(word) != _indice.end();
}
/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
void
IndexadorHash::indexar_tokens(const std::string& ficherodocumentos) {

	// Abrir la lista de ficheros 
    FILE* fp = fopen(ficherodocumentos.c_str(), "r");
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    // Contadores para informacionTermino
   	long int aux_idDoc = 1;
	int aux_numPal = 0, aux_numParada = 0,
	// Contadores para InformacionColeccionDocs
		aux_numdocs = 0, 				aux_numtotalpal = 0, aux_numTotalPalSinParada = 0,
		aux_numTotalPalDiferentes = 0, 	aux_tamBytes = 0, 	 aux_posicion = 0;
	// 
	std::unordered_set<std::string> aux_diferentes, dif;

	
	// Por cada fichero tokenizado, procesar sus tokens
    while ((read = getline(&line, &len, fp)) != -1) {
       line[read-1] = 0;
       std::string aux_line = line;
       std::string f_tokens = aux_line+".tk";

       ++aux_numdocs;								// Update numero de documentos con tokens
       _documentos.insert({aux_line, aux_idDoc});	// Inserta el documento en la lista

   		// Abrir fichero con los tokens
       	std::ifstream file;
       	file.open(f_tokens);
       	std::string token = "";

       	// Indexar el token leido
       	aux_posicion = 0;							// Posicion del token a 0
       	while (getline(file, token)) {
       		if (token != "") {		// comrpobar si es parado o vacio
       			++aux_numPal;
	       		if (!EsParada(token)) {
	       			// Indexar el token
	       			if  (_indice.find(token)  == _indice.end()) {
	       				_indice.insert({token, InformacionTermino(1)});				// Insertar el indice
	       				// Añadir en la lista de documentos del termino el primer documento
	       				auto* lista_doc = &_indice.find(token)->second.apuntarListaDocs();
	       				lista_doc->insert({aux_idDoc, InfTermDoc(1, aux_posicion)});
	       				aux_diferentes.insert(token);
	       				dif.insert(token);
	       			// Ya está indexado, actualizar informacionTerminod el token
		       		} else {
		       			actualizar_token_indexado(token, aux_idDoc, aux_posicion);
						// IF: p_token no pertenece a diferentes THEN insertar el token
						if (aux_diferentes.find(token) == aux_diferentes.end()) {
							aux_diferentes.insert(token);
						}
		       		}		       		
	       		} else { ++aux_numParada; }
	       		
	       		++aux_posicion;
	       	}
       	}

       	// informacionColeccionDocs
       	aux_numtotalpal 			+= aux_numPal;
       	aux_numTotalPalDiferentes 	+= aux_diferentes.size();
       	aux_numTotalPalSinParada 	+= (aux_numPal-aux_numParada);
       	// InformacionTermino
       	_indiceDocs.insert({aux_line,InfDoc(aux_idDoc++, 
       		aux_numPal, 
       		(aux_numPal-aux_numParada), 
       		aux_diferentes.size(), 
       		GetFileSize(aux_line))}
       	);
       	// informacionColeccionDocs
       	aux_tamBytes 	+= GetFileSize(aux_line);	// Actualizar B
		aux_numPal 		= aux_numParada = 0;		// Setear auxiliares
		aux_diferentes.clear();

    }

    // Información de la colección de documentos indexada
	_informacionColeccionDocs.setnumDocs (aux_numdocs);
	_informacionColeccionDocs.setnumTotalPal (aux_numtotalpal);
	_informacionColeccionDocs.settamBytes (aux_tamBytes); 
	_informacionColeccionDocs.setnumTotalPalSinParada (aux_numTotalPalSinParada);
	_informacionColeccionDocs.setnumTotalPalDiferentes(dif.size());

    fclose(fp);
    if (line) free(line);
}

/////////////////////////////////////////////////////////////
// DADA LA RUTA DE UN ARCHIVO DEVUELVE EL SIZE EN BYTES
/////////////////////////////////////////////////////////////
double 
IndexadorHash::GetFileSize(std::string filename){
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

/////////////////////////////////////////////////////////////
// TRUE SI EL STRING ESTA EN LA VARIABLE PRIVADA _STOPWORDS
/////////////////////////////////////////////////////////////
bool
IndexadorHash::EsParada(const std::string& p_token) {
	return (_stopWords.find(p_token) != _stopWords.end());
}

/////////////////////////////////////////////////////////////
// GUARDAR EN _stopwords LAS PALABRAS DE PARADA
/////////////////////////////////////////////////////////////
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
////// 			     FUNCIONES AUX                                     		///// 
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
// TRUE SI EL STRING ESTA EN LA VARIABLE PRIVADA _STOPWORDS
/////////////////////////////////////////////////////////////
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
	else // THEN actualizar el existente
		busca->second.UpdatePosTerm(aux_posicion);
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
	CopiaIndice(p_i._indice);
	CopiaIndiceDocs(p_i._indiceDocs);
	CopiaICD(p_i._informacionColeccionDocs);
	CopiaindicePregunta(p_i._indicePregunta);
	CopiaStopWords(p_i._stopWords);

	// InformacionPregunta _infPregunta;
	//_infPregunta(p_i._infPregunta);

	_pregunta 			= p_i._pregunta;
	_ficheroStopWords 	= p_i._ficheroStopWords;
	_tok 				= p_i._tok;
	_directorioIndice 	= p_i._directorioIndice;
	_tipoStemmer 		= p_i._tipoStemmer;
	_almacenarEnDisco 	= p_i._almacenarEnDisco;
	_almacenarPosTerm 	= p_i._almacenarPosTerm;
}
std::string 
IndexadorHash::GetNombreDocumento (int p_idDoc) const {
	
	// Obtener la ruta del archivo asociado al p_idDOc
	bool find = false;
	std::string ruta("");
	for (auto it = _documentos.begin(); it != _documentos.end() && !find; ++it) {
		if (it->second == p_idDoc) {
			ruta = it->first;
			find = true;
		}
	}

	// Borra la ruta y la extensión del archivo
	std::list<std::string> url;
	Tokenizador tokenizer("/", false, false);
	tokenizer.Tokenizar(ruta, url);

	// Mover el iterador hasta la última posición
	auto it = url.begin();
	for (int i = 1; i < url.size(); ++i) {
		it++;
	}

	// Guardar el contenido del iterador
	std::string last_token = *it;
	// Obtener el nombre sin la extension
	std::string nombreDoc = last_token.substr(0, last_token.find('.'));

	return nombreDoc;
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
int 
IndexadorHash::NumPalIndexadas() const {
	return _indice.size();
}

//////////////////////////////////////////////////////////////////////////////
////// 			     IMPRIMIR INFO                                    ///// 
//////////////////////////////////////////////////////////////////////////////
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
bool 
IndexadorHash::ListarDocs (const std::string& nomDoc) {
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
	}
	return false;
}

/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
IndexadorHash::IndexadorHash(const std::string& directorioIndexacion) {
	_directorioIndice = directorioIndexacion;
	LeerIndice();
	LeerIndiceDocs();
	LeerInformacionColeccionDocs();
	LeerTokenizador();
	LeerVariables();
	ObtenerPalParada();
}

void
IndexadorHash::LeerVariables() {
	std::ifstream file;
	std::string s = _directorioIndice+"/variables.txt";
	file.open(s.c_str());
	std::string linea;

	while(!file.eof()){
		getline(file, _pregunta);
		getline(file, _ficheroStopWords);
		getline(file, linea);
		_tipoStemmer=atoi(linea.c_str());
		getline(file, linea);
		_almacenarEnDisco=atoi(linea.c_str());
		getline(file, linea);
		_almacenarPosTerm=atoi(linea.c_str());
	}
}

void
IndexadorHash::LeerTokenizador() {
	std::ifstream file;
	std::string s = _directorioIndice+"/tokenizador.txt";
	file.open(s.c_str());
	std::string linea;

	while (getline(file, linea)) {
		_tok.CasosEspeciales(atoi(linea.c_str()));
		getline(file, linea);
		_tok.DelimitadoresPalabra(linea);
		getline(file, linea);
		_tok.PasarAminuscSinAcentos(atoi(linea.c_str()));
	}
}

void IndexadorHash::LeerInformacionColeccionDocs() {
	std::string linea = "";
	std::string s = _directorioIndice+"/informacionColeccionDocs.txt";
	
	std::ifstream f;
	f.open(s.c_str());

	while(getline(f, linea)){
		_informacionColeccionDocs.setnumDocs((atoi(linea.c_str())));
		getline(f, linea);
		_informacionColeccionDocs.setnumTotalPal((atoi(linea.c_str())));
		getline(f, linea);
		_informacionColeccionDocs.setnumTotalPalSinParada((atoi(linea.c_str())));
		getline(f, linea);
		_informacionColeccionDocs.setnumTotalPalDiferentes((atoi(linea.c_str())));
		getline(f, linea);
		_informacionColeccionDocs.settamBytes((atoi(linea.c_str())));
	}
	f.close();
}

void IndexadorHash::LeerIndiceDocs() {
	std::string linea, termino;
	std::list<std::string> tokens;
	InfDoc id;

	std::string s = _directorioIndice+"/indiceDocs.txt";
	std::ifstream f;
	f.open(s.c_str());

	_tok.DelimitadoresPalabra("\t ");

	while (getline(f, linea)) {
		
		_tok.TokenizarGeneral(linea,tokens);

		for(auto t = tokens.begin(); t != tokens.end(); ++t){
			termino = (*t);
			++t; ++t;
			id.setidDoc((atoi((*t).c_str())));
			++t; ++t;
			id.setNumPal((atoi((*t).c_str())));
			++t; ++t;
			id.setNumPalSinParada((atoi((*t).c_str())));
			++t; ++t;
			id.setNumPalDiferentes((atoi((*t).c_str())));
			++t; ++t;
			id.setTamBytes((atoi((*t).c_str())));
		}
		_indiceDocs.insert({termino,id});
	}

	f.close();
}

void
IndexadorHash::LeerIndice() {
	std::string file, linea, termino;
	std::list<std::string> tokens;
	//std::list<int> lista;
	

	//std::list<std::string>::iterator itAux;
	//std::unordered_map<long int,InfTermDoc> l_docs_aux;

	std::ifstream f;

	std::string s = _directorioIndice+"/indice.txt";
	f.open(s.c_str());

	InfTermDoc itd;

	_tok.DelimitadoresPalabra("\t ");//Para tokenizar los indices
	_tok.CasosEspeciales(false);
	_tok.PasarAminuscSinAcentos(false);

	while(getline(f, linea)){
		std::cout << linea << '\n';
		termino = "";
		//l_docs_aux.clear();
		//lista.clear();
		InformacionTermino it;

		_tok.Tokenizar(linea,tokens);
		//std::cout << "linea: " << linea << std::endl;
		for(auto t= tokens.begin();t!= tokens.end();++t){
			int fd = 0, idDoc = 0;
			if (t == tokens.begin()) {
				//termino=(*t);
				// en el tokenizador general el termino puede tener en medio \t
				// u otros caracteres raros, por eso no valo solo con coger el valor
				// del primer iterador
				bool entrad = false;
				while (*t != "F") {
					termino += *t;
					++t;
					entrad = true;
				}
				if (!entrad) {
				++t;
				}
				++t;

				//++t;
				it.setFtc((atoi((*t).c_str())));
				++t;
				++t;
				fd = atoi((*t).c_str());
			} else {	// Empieza en Id.DOc
				//++t;//numero a leer
				idDoc = std::stoi(*t);//atoi((*t).c_str());
				++t;//ft
				++t;// valor a leer
				itd.setFt(atoi((*t).c_str()));
				++t;
				// Leer ft posiciones
				for (int i = 0; i < itd.getft(); ++i) {
					//lista.push_back(atoi((*t).c_str()));
					itd.getposterm().push_back(atoi((*t).c_str()));
					if (i != itd.getft()-1)
						++t;
				}
				//itd.setPosTerm(lista);
				//l_docs_aux.insert({idDoc,itd});
				it.getldocs().insert({idDoc,itd});
			}
		}
		//it.setDocs(l_docs_aux);
		_indice.insert({termino,it});
	}
	f.close();
}

/////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////
bool
IndexadorHash::GuardarIndexacion() const {
	struct stat dir;
	// Compruebo la existencia del directorio
	int err = stat(_directorioIndice.c_str(), &dir);
	//Tokenizador tokaux(tok);

	if(err==-1 || !S_ISDIR(dir.st_mode)){//Si no existe el directorio lo creamos
		std::string cmd = "mkdir " + _directorioIndice;//+ " rm -f "+_directorioIndice+"/.*"; //Crea el directorio
		system(cmd.c_str()); //Ejecuta el comando cmd en el tablero de comandos
	}

	GuardarIndice();
	GuardarIndiceDocs();
	GuardarInformacionColeccionDocs();
	GuardarTokenizador();
	GuardarVariables();

	return false;
}

void
IndexadorHash::GuardarIndice () const{
	std::ofstream ofile;
	std::string s =  _directorioIndice+"/indice.txt";
	ofile.open(s.c_str());

	for(auto i = _indice.begin(); i != _indice.end(); ++i){
		ofile << i->first << '\t'<< i->second << '\n';
	}

	ofile.close();
}
void
IndexadorHash::GuardarIndiceDocs () const {
	std::ofstream ofile;
	std::string s =  _directorioIndice+"/indiceDocs.txt";
	ofile.open(s.c_str(), std::ios::out);

	//for(auto& id : _indiceDocs){
	for(auto id = _indiceDocs.begin(); id != _indiceDocs.end(); ++id){
		ofile << id->first << '\t' << id->second << '\n';
	}

	ofile.close();
}
void 
IndexadorHash::GuardarInformacionColeccionDocs () const {
	std::ofstream ofile;
	std::string s =  _directorioIndice+"/informacionColeccionDocs.txt";
	ofile.open(s.c_str(), std::ios::out);

	ofile << _informacionColeccionDocs.getnumDocs() << '\n'
		<< _informacionColeccionDocs.getnumTotalPal() << '\n'
		<< _informacionColeccionDocs.getnumTotalPalSinParada() << '\n'
		<< _informacionColeccionDocs.getnumTotalPalDiferentes() << '\n'
		<< _informacionColeccionDocs.gettamBytes();

	ofile.close();
}
void 
IndexadorHash::GuardarTokenizador () const {
	std::ofstream ofile;
	std::string s =  _directorioIndice+"/tokenizador.txt";
	ofile.open(s.c_str(), std::ios::out);

	ofile 
		<< _tok.CasosEspeciales() << '\n' 
		<< _tok.DelimitadoresPalabra() << '\n' 
		<< _tok.PasarAminuscSinAcentos();

	ofile.close();
}
void
IndexadorHash::GuardarVariables () const {
	std::ofstream ofile;
	std::string s =  _directorioIndice+"/variables.txt";
	ofile.open(s.c_str(), std::ios::out);

	ofile 
		<< _pregunta << '\n' 
		<< _ficheroStopWords << '\n' 
		<< _tipoStemmer << '\n'
		<< _almacenarEnDisco  << '\n'
		<< _almacenarPosTerm;

	ofile.close();
}
