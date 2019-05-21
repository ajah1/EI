#include "buscador.h"
#include "math.h"

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
	_b(0.75)
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

bool 
Buscador::Buscar(const int& numDocumentos) {
	// IF no hay pregunta indexada THEN return false
	std::string pregunta("");
	if (!DevuelvePregunta(pregunta)) {
		return false;
	}

	// Borrar la memoria de _docsOrdenados
	while (!_docsOrdenados.empty()) {
		_docsOrdenados.pop();
	}

	// Almacena todos los resultados
	std::priority_queue< ResultadoRI > aux_queue;

	// Delegado al metodo a aplicar
	double (Buscador::*fp_metodo)(const int&) = &Buscador::DFR;

	if (_formSimilitud == 1) {
		fp_metodo = &Buscador::BM25;
	}

	for (int i = 1; i <= getIndiceDocsSize(); ++i) {
		aux_queue.push(ResultadoRI((this->*fp_metodo)(i),i,0));
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

		return true;
	}

	return false;
}

double 
Buscador::BM25 (const int& Doc) {
	/*std::clog << "llamada a BM25 \n";
	double sim=0.0;
	double idf,numpal,medianumpal;

	std::unordered_map<std::string, InfDoc> aux;
	aux = getIndiceDocs();

	//std::cout << "size: " << aux.size() << '\n';

	for(auto i=getIndiceDocs().begin();i!=getIndiceDocs().end();++i){

		if(i->second.getidDoc()==Doc){
			numpal=i->second.getnumPalSinParada();
		}
		medianumpal+=i->second.getnumPalSinParada();
	}

	medianumpal=medianumpal/getIndiceDocs().size();

	InformacionTermino it;
	for(auto i=getIndicePregunta().begin();i!=getIndicePregunta().end();++i){//Para cada término de la pregunta
		if(getIndice().find(i->first)!=getIndice().end()){
			it=getIndice().find(i->first)->second;
			idf=log10((getIndiceDocs().size()-(it.getDocs().size()+0.5))/(it.getDocs().size()+0.5));
			if(it.getDocs().find(Doc)!=it.getDocs().end()){
				sim+=fabs(idf*((it.getDocs().find(Doc)->second.getft()*(_k1+1))/(it.getDocs().find(Doc)->second.getft()+(_k1*(1-_b+(_b*(numpal/medianumpal)))))));
			}
			else{
				//Numeros absolutos --> fabs(num);
				sim+=fabs(idf*(0*(_k1+1))/(0+(_k1*(1-_b+(_b*(numpal/medianumpal))))));
			}
		}

	}*/
	
	return sim;
}

void
Buscador::ImprimirResultadoBusqueda(const int& numDocumentos) const {
	// Si numDocumentos es mayor que los guardados imprimir todos
	if (numDocumentos > _docsOrdenados.size()) {
		std::cout << "sin implementar \n";
	} else {
		//0 DFR fichero1 0 1.5 pal1 pal7
		std::priority_queue<ResultadoRI> aux(_docsOrdenados);
		for (int i = 0; i < numDocumentos; ++i) {
			ResultadoRI top_r(aux.top());

			std::string formula = "DFR";
			if (_formSimilitud == 1) formula = "BM25";
			std::string pregunta = "ConjuntoDePreguntas";
			if (top_r.GetNumPregunta() == 0) {
				pregunta = GetPregunta();
			}

			std::cout
				<< top_r.GetNumPregunta() 	<< ' '
				<< formula 	 			<< ' '
				<< GetNombreDocumento(i+1) << ' '
				<< i << ' '
				<< top_r.GetVSimilitud() << ' '
				<< pregunta << '\n';

			aux.pop();
			std::cout << "size: " << _docsOrdenados.size() << std::endl;
		}

		LiberarCola (aux);
	}
	std::cout << "size: " << _docsOrdenados.size() << std::endl;
}

double 
Buscador::DFR (const int& p_idDoc) {
	std::clog << "llamada a DFR \n";
	return 9.9f;
}


