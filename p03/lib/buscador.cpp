#include "buscador.h"
#include "math.h"
#include <fstream>
/////////////////////////////////////////////////////////////////////////////
// CLASE RESULTADORI
/////////////////////////////////////////////////////////////////////////////
ResultadoRI::ResultadoRI(const double& kvSimilitud, const long int& kidDoc, const int& np) { 
	vSimilitud = kvSimilitud; 
	idDoc = kidDoc; 
	numPregunta = np; 
}
ResultadoRI::ResultadoRI(const ResultadoRI& p_r) { 
	vSimilitud 	= p_r.vSimilitud; 
	idDoc 		= p_r.idDoc; 
	numPregunta = p_r.numPregunta; 
}
double ResultadoRI::VSimilitud() const { 
	return vSimilitud; 
}
long int 
ResultadoRI::IdDoc() const { 
	return idDoc; 
} 
bool 
ResultadoRI::operator< (const ResultadoRI& lhs) const { 
	if(numPregunta == lhs.numPregunta) 
		return (vSimilitud < lhs.vSimilitud); 
	else 
		return (numPregunta > lhs.numPregunta); 
} 
std::ostream& 
operator<<(std::ostream &os, const ResultadoRI &res){ 
	return os 	
		<< res.vSimilitud 	<< "\t\t" 
		<< res.idDoc 		<< "\t" 
		<< res.numPregunta 
		<< std::endl;
}

/////////////////////////////////////////////////////////////////////////////
// FORMA CANONICA
/////////////////////////////////////////////////////////////////////////////
Buscador::Buscador(const std::string& directorioIndexacion, const int& f) : 
	IndexadorHash(directorioIndexacion),
	_formSimilitud(0),
	_c(2),
	_k1(1.2),
	_b(0.75),
	_esConjunto(false)
{}

Buscador::Buscador() :
	IndexadorHash()
{}

Buscador::~Buscador() {
	_formSimilitud = 0;
	_c = _k1 = _b = 0.0;
	for (long unsigned int i = 0; i < _docsOrdenados.size(); ++i)
		_docsOrdenados.pop();
}

Buscador& 
Buscador::operator= (const Buscador& p_b) {
	if (this != &p_b) {

	}

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// FUNCIONES AUXILIARES
/////////////////////////////////////////////////////////////////////////////
void
Buscador::LiberarCola(std::priority_queue< ResultadoRI>& cola) const{
	while (!cola.empty()) {
		cola.pop();
	}
}


/////////////////////////////////////////////////////////////////////////////
// GETTERS / SETTERS
/////////////////////////////////////////////////////////////////////////////
double
Buscador::DevolverParametrosDFR() const {
	return _c;
}
void 
Buscador::DevolverParametrosBM25(double& kk1, double& kb) const {
	kk1 = _k1;
	kb = _b;
}
int 
Buscador::DevolverFormulaSimilitud() const {
	return _formSimilitud;
}
bool 
Buscador::CambiarFormulaSimilitud(const int& f) {
	if (f < 0 || f > 1) {
		return false;
	}

	_formSimilitud = f;
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// SOBRECARGA OPERADOR SALIDA
/////////////////////////////////////////////////////////////////////////////
int numerodelapregunta = 0;
int numerodepreguntas = 0;
bool 
Buscador::Buscar
(	const std::string& dirPreguntas, 
	const int& numDocumentos,
	const int& numPregInicio, 
	const int& numPregFin) 
{
	numerodepreguntas = numPregFin - numPregInicio;
	// Borrar la memoria de _docsOrdenados
	while (!_docsOrdenados.empty()) {
		_docsOrdenados.pop();
	}
	std::ifstream query_file; 
	for (int i = numPregInicio; i <= numPregFin; ++i) {
		query_file.open(dirPreguntas+std::to_string(i)+".txt");
		std::string query(""), line("");
		while (std::getline(query_file, line)) {
			query += line;
		}
		query_file.close();
		numerodelapregunta = i;

		IndexarPregunta(query);
		Buscar(numDocumentos);
	}
	_esConjunto = true;
	return true;
}


bool 
Buscador::Buscar(const int& numDocumentos) {
	_esConjunto = false;
	// IF no hay pregunta indexada THEN return false
	std::string pregunta("");
	if (!DevuelvePregunta(pregunta)) {
		return false;
	}

	// Borrar la memoria de _docsOrdenados
	//while (!_docsOrdenados.empty()) {
	//	_docsOrdenados.pop();
	//}

	// Almacena todos los resultados
	std::priority_queue< ResultadoRI > aux_queue;

	// Delegado al metodo a aplicar
	double (Buscador::*fp_metodo)(const int&) = &Buscador::DFR;

	if (_formSimilitud == 1) {
		fp_metodo = &Buscador::BM25;
	}
	
	for (int i = 1; i <= getIndiceDocsSize(); ++i) {
		//std::cout << numerodelapregunta << std::endl;
		aux_queue.push(ResultadoRI((this->*fp_metodo)(i),i,numerodelapregunta));
	}

	if (!aux_queue.empty()) {
		// Hay menos documentos que los pedidos solo hacer push
		if (aux_queue.size() < numDocumentos) {
			while (!aux_queue.empty()) {
				_docsOrdenados.push(aux_queue.top());
				aux_queue.pop();
			}
		// Almacenrar numDocumentos de resultados de _docsOrdenados
		} else {
			for (int i = 0; i < numDocumentos; ++i) {
				_docsOrdenados.push(aux_queue.top());
				aux_queue.pop();
			}

			LiberarCola(aux_queue);
		}
		//std::cout << _docsOrdenados.size() << std::endl;
		return true;
	}

	return false;
}

int
Buscador::CalculateNQI (const std::string& token_pregunta) {

	auto& ref_indice = getIndice();
	int nqi = 0;

	if (ref_indice.find(token_pregunta) != ref_indice.end()) {
		nqi = ref_indice.find(token_pregunta)->second.getNumDocs();
	}

	return nqi;
}

double
Buscador::CalculateAVGDL () {

	int c_palabras_no_parada = 0;

	for (auto& doc : getIndiceDocs()) {
		c_palabras_no_parada +=  doc.second.getnumPalSinParada();
	}
	
	return 1.0 * c_palabras_no_parada / getIndiceDocs().size();
}

int
Buscador::CalculateFQID (const std::string& token_pregunta, const int& idDoc) {

	int fqid = 0;

	// Buscar el termino en _indice
	const auto& indice = getIndice().find(token_pregunta);
	if (indice != getIndice().end()) {
		// Buscar el el idDoc en la lista de docs en la que aparece i
		const auto& infotermdoc = indice->second.getDocs().find(idDoc);
		if (infotermdoc != indice->second.getDocs().end()) {
			fqid = infotermdoc->second.getft();
		}
	}

	return fqid;
}

double 
Buscador::BM25 (const int& docID) {
	//std::clog << "llamada a BM25 \n";

	// score(D, Q) : valor de similitud para el documento D respecto a la pregunta Q que realiza el usuario
	double score = 0.0;
	// n : número de términos (no de parada) de la query Q  
	//int n = getInfPregunta().getnumTotalPalSinParada(); //getIndicePregunta().size();
	// f(qi, D) : la frecuencia del término qi en el documento D
	int fqid = 0;
	// |D| : el número de palabras (no de parada) del documento D
	int Dnumpal = 0;
	// avgdl: la media de todas las |D| en la colección
	double avgdl = CalculateAVGDL();
	// N: cantidad de documentos en la colección
	int N = getIndiceDocs().size();
	// n(qi): número de documentos en los que aparece el término qi
	int nqi = 0;
	// constantes para configuración: k1 = 1,2 b = 0,75
	double k1 	= _k1;
	double b 	= _b;

	// Auxiliar Data
	double leftOperand 	= 0.0;	// IDF(qi)	
	double rightOperand = 0.0;	// Division
	// set Dnumpal value
	for (auto& d : getIndiceDocs()) {
		if (d.second.getidDoc() == docID) {
			Dnumpal = d.second.getnumPalSinParada();
		}
	}

	// Sumatorio desde i=1 hasta n
	//for (int i = 0; i < n; ++i) {
	for (auto& i_pregunta : getIndicePregunta()) {
		
		leftOperand = rightOperand = 0.0;

		nqi = CalculateNQI(i_pregunta.first);
		fqid = CalculateFQID(i_pregunta.first, docID);

		// Calculate left operand
		leftOperand = log10((N - nqi + 0.5) / (nqi + 0.5));
		// Calculate right operand
		rightOperand = (fqid * (k1 + 1)) / (fqid + k1 * (1 - b + b * (Dnumpal/avgdl)));

		// Update score
		score += leftOperand * rightOperand;
	}
	
	return score;
}


/*double
Buscador::CalculateAVRLD() {

	int c_palabras_no_parada = 0;

	for (auto& doc : getIndiceDocs()) {
		c_palabras_no_parada +=  doc.second.getnumPalSinParada();
	}
	
	return 1.0 * c_palabras_no_parada / getIndiceDocs().size();
}*/

/////////////////////////////////////////////// DFR /////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double
Buscador::DFR (const int& p_idDoc) {
	//std::clog << "llamada a DFR \n";
	//std::cout << "IDDOC: " << p_idDoc << "...................:" << std::endl;
	double sim = 0.0;
	// q: la query o pregunta que realiza el usuario 
	std::string q = GetPregunta();
	// d: el documento del que se calcula su valor de similitud sim(q, d) respecto a la pregunta q que realiza el usuario 
	//int d = p_idDoc;
	// k: número de términos (no de parada) de la query q 
	int k = getInfPregunta().getnumTotalPalSinParada();
	// wi,q: peso en la query del término i de la query q 
	double wiq = 0.0;
	// wi,d: peso en el documento del término i de la query q
	double wid = 0.0; 
	// ft: número total de veces que el término t aparece en toda la colección 
	int ft = 0;
	// ft,d: número de veces que el término t aparece en el documento d
	int ftd = 0;
	// t,q: número de veces que el término t aparece en la query q 
	int ftq = 0;
	// nt: número de documentos en los que aparece el término t 
	int nt = 0;
	// ld: longitud en palabras (no de parada) del documento 
	int ld = 0;
	// avr_ld: media en palabras (no de parada) del tamaño de los documentos 
	double avr_ld = CalculateAVGDL();
	// N: cantidad de documentos en la colección 
	int N = getIndiceDocs().size();
	// Valor recomendado de c = 2
	int c = _c;


	// Set ld value
	for (auto& d : getIndiceDocs()) {
		if (d.second.getidDoc() == p_idDoc) {
			ld = d.second.getnumPalSinParada();
		}
	}

	// Auxiliar data
	//double leftOperand = 0.0;
	//double rightOperand = 0.0;

	// for (i=i ... k)
	for (auto& termino_pregunta : getIndicePregunta()) {

		//////////////////////////// calculate wiq
		// Wtq = ftq / k
		ftq = CalculateFTQ(termino_pregunta.first);
		wiq = 1.0 * ftq / k;

		//////////////////////////// calculate wid
		// wid = leftOperand * rightOperand
		ftd = CalculateFTD(termino_pregunta.first, p_idDoc);
		ft 	= CalculateFT(termino_pregunta.first);
		nt 	= CalculateNT(termino_pregunta.first);
		if (ftd == 0 || ft == 0 || nt == 0) {
			
		} else {
			double lambdat = 1.0 * ft / N;
			double ftdd = CalculateFTDD(ftd, c, avr_ld, ld);

			double leftOperand = 
				(log2(1 + lambdat) + ftdd * log2((1 + lambdat) / (lambdat)));
			double rightOperand = 
				(ft + 1) / (nt * (ftdd + 1));

			wid = leftOperand * rightOperand;
			
			// Update sim value
			sim += wiq * wid;
		}
	}



	return sim;
}
// ft,d: número de veces que el término t aparece en el documento d
int
Buscador::CalculateFTD(const std::string& token, const int& d) {
	int ftd = 0;

	const auto& ref_indice = getIndice().find(token);
	if (ref_indice != getIndice().end()) {
		const auto& ref_ldocs = ref_indice->second.getldocs().find(d);
		if (ref_ldocs != ref_indice->second.getldocs().end()) {
			ftd = ref_ldocs->second.getft();
		}
	}

	return ftd;
}

double
Buscador::CalculateFTDD(const int& ftd, const double& c, const double& avr_ld, const int& ld) {
	return (double)ftd * log2(1 + (c * avr_ld) / ld);
}

int
Buscador::CalculateNT(const std::string& termino_pregunta) {
	int nt = 0;

	const auto& ref_indice = getIndice();
	if (ref_indice.find(termino_pregunta) != ref_indice.end()) {
		nt = ref_indice.find(termino_pregunta)->second.getNumDocs();
	}

	return nt;
}

int
Buscador::CalculateFTQ(const std::string& termino_pregunta) {
	int ftq = 0;

	const auto& r_indice_pregunta = getIndicePregunta();
	if (r_indice_pregunta.find(termino_pregunta) != r_indice_pregunta.end()) {
		ftq = r_indice_pregunta.find(termino_pregunta)->second.getft();
	}

	return ftq;
}

int
Buscador::CalculateFT(const std::string& termino_pregunta) {
	//std::cout << "E:E :" << termino_pregunta << std::endl;
	auto& ref_indice = getIndice();
	int nqi = 0;
	auto aux = ref_indice.find(termino_pregunta);
	if (aux != ref_indice.end()) {
		nqi = ref_indice.find(termino_pregunta)->second.getftc();
	}

	return nqi;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





void
Buscador::ImprimirResultadoBusqueda(const int& numDocumentos) {
	int n_docs = numDocumentos;

	// Si numDocumentos es mayor que los guardados imprimir todos
	if (numDocumentos > _docsOrdenados.size())
		n_docs = _docsOrdenados.size();

	//0 DFR fichero1 0 1.5 pal1 pal7
	std::priority_queue<ResultadoRI> aux(_docsOrdenados);


	if (!_esConjunto) {

		for (int i = 0; i < n_docs; ++i) {
			ResultadoRI top_r(_docsOrdenados.top());

			std::string formula = "DFR";
			if (_formSimilitud == 1) formula = "BM25";

			std::cout
				<< top_r.GetNumPregunta() 	<< ' '
				<< formula 	 			<< ' '
				<< GetNombreDocumento(top_r.getidDoc()) << ' '
				<< i << ' '
				<< top_r.GetVSimilitud() << ' '
				<< GetPregunta() << '\n';

			_docsOrdenados.pop();
		}
	} else {
		//for (int i = 0; i < _docsOrdenados.size(); ++i) {
		for (int i = _docsOrdenados.size(); i > 0; --i) {
		
			ResultadoRI top_r(_docsOrdenados.top());

			std::string formula = "DFR";
			if (_formSimilitud == 1) formula = "BM25";

			std::cout
				<< top_r.GetNumPregunta() 	<< ' '
				<< formula 	 			<< ' '
				<< GetNombreDocumento(top_r.getidDoc()) << ' '
				<< i << ' '
				<< top_r.GetVSimilitud() << ' '
				<< "ConjuntoDePreguntas" << '\n';

			_docsOrdenados.pop();
		}
	}


	LiberarCola (aux);
	
}



