#include "PaMedicamento.h"
#include "Laboratorio.h"


Pamedicamento::Pamedicamento(const std::string& id_num, const std::string& id_alpha, const std::string& nombre)
    : _id_num(id_num), _id_alpha(id_alpha), _nombre(nombre), _suministrador(nullptr) {
}


const std::string& Pamedicamento::get_id_num() const {
    return _id_num;
}
const std::string& Pamedicamento::get_id_alpha() const {
    return _id_alpha;
}
const std::string& Pamedicamento::get_nombre() const {
    return _nombre;
}
Laboratorio* Pamedicamento::get_suministrador() const {
    return _suministrador;
}


void Pamedicamento::set_id_num(const std::string& id_num) {
    _id_num = id_num;
}
void Pamedicamento::set_id_alpha(const std::string& id_alpha) {
    _id_alpha = id_alpha;
}
void Pamedicamento::set_nombre(const std::string& nombre) {
    _nombre = nombre;
}


void Pamedicamento::suministrarMed(Laboratorio* lab) {
    _suministrador = lab;
}


bool Pamedicamento::operator<(const Pamedicamento& other) const {
    return std::stoi(_id_num) < std::stoi(other._id_num);
}
bool Pamedicamento::operator==(const Pamedicamento& other) const {
    return _id_num == other._id_num;
}

std::ostream& operator<<(std::ostream& os, const Pamedicamento& med) {
    os << "Medicamento(ID: " << med.get_id_num() << ", Nombre: " << med.get_nombre();
    if (med.get_suministrador()) {
        os << ", Suministrador: " << med.get_suministrador()->getNombre();
    }
    os << ")";
    return os;
}

