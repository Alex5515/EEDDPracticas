/**
* @file PaMedicamento.cpp
 * @brief Implementación de PaMedicamento.
 */

#include "PaMedicamento.h"
#include "Laboratorio.h"

PaMedicamento::PaMedicamento() : id_num(-1), servidoPor(nullptr) {}

PaMedicamento::PaMedicamento(int idNum, const std::string& idAlpha, const std::string& nom)
    : id_num(idNum), id_alpha(idAlpha), nombre(nom), servidoPor(nullptr) {}

int PaMedicamento::getIdNum() const { return id_num; }
const std::string& PaMedicamento::getIdAlpha() const { return id_alpha; }
const std::string& PaMedicamento::getNombre() const { return nombre; }
Laboratorio* PaMedicamento::getLaboratorio() const { return servidoPor; }

void PaMedicamento::setLaboratorio(Laboratorio* lab) { servidoPor = lab; }
void PaMedicamento::setNombre(const std::string& nom) { nombre = nom; }
