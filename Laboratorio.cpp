/**
* @file Laboratorio.cpp
 * @brief Implementación de Laboratorio.
 */

#include "Laboratorio.h"
#include <algorithm>

Laboratorio::Laboratorio() : id(-1) {}

Laboratorio::Laboratorio(int id_, const std::string& nombre, const std::string& dir,
                         const std::string& cp, const std::string& loc)
    : id(id_), nombreLab(nombre), direccion(dir), codPostal(cp), localidad(loc) {}

int Laboratorio::getId() const { return id; }
const std::string& Laboratorio::getNombre() const { return nombreLab; }
const std::string& Laboratorio::getDireccion() const { return direccion; }
const std::string& Laboratorio::getCodPostal() const { return codPostal; }
const std::string& Laboratorio::getLocalidad() const { return localidad; }

void Laboratorio::setNombre(const std::string& n) { nombreLab = n; }

bool Laboratorio::nombreContiene(const std::string& sub) const {
    if (sub.empty()) return true;
    std::string a = nombreLab, b = sub;
    std::transform(a.begin(), a.end(), a.begin(), ::toupper);
    std::transform(b.begin(), b.end(), b.begin(), ::toupper);
    return a.find(b) != std::string::npos;
}
