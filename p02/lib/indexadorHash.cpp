#include "indexadorHash.h"
#include "stemmer.h"

IndexadorHash::IndexadorHash() {

}

IndexadorHash::IndexadorHash(const std::string& fichStopWords, 
	const std::string& delimitadores,
	const bool& detectComp, 
	const bool& minuscSinAcentos, 
	const std::string& dirIndice, 
	const int& tStemmer, 
	const bool& almEnDisco, 
	const bool& almPosTerm) {

}
IndexadorHash::IndexadorHash(const std::string& directorioIndexacion) {

}
IndexadorHash::IndexadorHash(const IndexadorHash&) {

}
IndexadorHash::~IndexadorHash() {

}
IndexadorHash& 
IndexadorHash::operator= (const IndexadorHash& p_i) {

}
bool 
IndexadorHash::Indexar(const std::string& ficheroDocumentos) {

}
bool 
IndexadorHash::IndexarDirectorio(const std::string& dirAIndexar) {

}
bool 
IndexadorHash::GuardarIndexacion() const {

}
bool 
IndexadorHash::RecuperarIndexacion (const std::string& directorioIndexacion) {

}
void 
IndexadorHash::ImprimirIndexacion() const {

}
bool 
IndexadorHash::IndexarPregunta(const std::string& preg) {

}
bool 
IndexadorHash::DevuelvePregunta(std::string& preg) const {

}
bool 
IndexadorHash::DevuelvePregunta(const std::string& word, InformacionTerminoPregunta& inf) const {

}
bool 
IndexadorHash::DevuelvePregunta(InformacionPregunta& inf) const {

}
void 
IndexadorHash::ImprimirIndexacionPregunta() {

}
void 
IndexadorHash::ImprimirPregunta() {

}
bool 
IndexadorHash::Devuelve(const std::string& word, InformacionTermino& inf) const {

}
bool 
IndexadorHash::Devuelve(const std::string& word, const std::string& nomDoc, InfTermDoc& InfDoc) const {

}
bool 
IndexadorHash::Existe(const std::string& word) const {

}
bool 
IndexadorHash::Borra(const std::string& word) {

}
bool 
IndexadorHash::BorraDoc(const std::string& nomDoc) {

}
void 
IndexadorHash::VaciarIndiceDocs() {

}
void 
IndexadorHash::VaciarIndicePreg() {

}
bool 
IndexadorHash::Actualiza(const std::string& word, const InformacionTermino& inf) {

}
bool 
IndexadorHash::Inserta(const std::string& word, const InformacionTermino& inf) {

}
int 
IndexadorHash::NumPalIndexadas() const {

}
std::string 
IndexadorHash::DevolverFichPalParada () const {

}
void 
IndexadorHash::ListarPalParada() const {

}
int 
IndexadorHash::NumPalParada() const {

}
std::string 
IndexadorHash::DevolverDelimitadores () const {

}
bool 
IndexadorHash::DevolverCasosEspeciales () const {

}
bool 
IndexadorHash::DevolverPasarAminuscSinAcentos () const {

}
bool 
IndexadorHash::DevolverAlmacenarPosTerm () const {

}
std::string 
IndexadorHash::DevolverDirIndice () const {
}
int 
IndexadorHash::DevolverTipoStemming () const {

}
bool 
IndexadorHash::DevolverAlmEnDisco () const {

}
void 
IndexadorHash::ListarInfColeccDocs() const {

}
void 
IndexadorHash::ListarTerminos() const {

}
bool 
IndexadorHash::ListarTerminos(const std::string& nomDoc) const {

}
bool 
IndexadorHash::ListarDocs(const std::string& nomDoc) const {

}