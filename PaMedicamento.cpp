#include "PaMedicamento.h"
#include "Laboratorio.h"  // solo si necesitas imprimir datos del lab en operator<< (adelantada arriba)

PaMedicamento::PaMedicamento(int id_num, const std::string& id_alpha, const std::string& nombre)
    : id_num(id_num), id_alpha(id_alpha), nombre(nombre), servidoPor(nullptr) {}

// --- Getters ---
int PaMedicamento::getIdNum() const { return id_num; }
const std::string& PaMedicamento::getIdAlpha() const { return id_alpha; }
const std::string& PaMedicamento::getNombre() const { return nombre; }
Laboratorio* PaMedicamento::getSuministrador() const { return servidoPor; }
Laboratorio* PaMedicamento::getServidoPor() const { return servidoPor; }

// --- Setters ---
void PaMedicamento::setIdNum(int v) { id_num = v; }
void PaMedicamento::setIdAlpha(const std::string& v) { id_alpha = v; }
void PaMedicamento::setNombre(const std::string& v) { nombre = v; }
void PaMedicamento::setSuministrador(Laboratorio* lab) { servidoPor = lab; }
void PaMedicamento::setServidoPor(Laboratorio* lab) { servidoPor = lab; }

// --- Comparación por id_num ---
bool PaMedicamento::operator<(const PaMedicamento& other) const {
    return id_num < other.id_num;
}
bool PaMedicamento::operator==(const PaMedicamento& other) const {
    return id_num == other.id_num;
}

// --- Formato de salida ---
std::ostream& operator<<(std::ostream& os, const PaMedicamento& med) {
    os << "PaMedicamento(ID=" << med.getIdNum()
       << ", id_alpha=" << med.getIdAlpha()
       << ", Nombre=" << med.getNombre();
    if (med.getSuministrador()) {
        // Solo mostramos identificador/nombre mínimo del laboratorio para evitar dependencias fuertes
        os << ", Suministrador@" << med.getSuministrador();
    }
    os << ")";
    return os;
}
