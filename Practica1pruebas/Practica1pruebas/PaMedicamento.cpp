#include "PaMedicamento.h"
#include <utility>

Pamedicamento::Pamedicamento() : _id_num(""), _id_alpha(""), _nombre("")
{

}
Pamedicamento::Pamedicamento( const std::string &id_num, const std::string &id_alpha, const std::string &nombre) : _id_num(id_num), _id_alpha(id_alpha), _nombre(nombre)

{

}

const std::string &Pamedicamento::get_id_num() const{
    return _id_num;
}

void Pamedicamento::set_id_num(const std::string &id_num) {
    _id_num = id_num;
}
const std::string &Pamedicamento::get_id_alpha() const{
    return _id_alpha;
}

void Pamedicamento::set_id_alpha(const std::string &id_alpha) {
    _id_alpha = id_alpha;
}
const std::string &Pamedicamento::get_nombre() const {
    return _nombre;
}
void Pamedicamento::set_nombre(const std::string &nombre) {
    _nombre = nombre;
}