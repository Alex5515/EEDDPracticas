/**
* @file Farmacia.cpp
 * @brief Implementación de la clase Farmacia.
 */

#include "Farmacia.h"
#include "PaMedicamento.h"
#include "MediExpress.h"

Farmacia::Farmacia() : linkMedi(nullptr) {}

Farmacia::Farmacia(const std::string& _cif, const std::string& _prov, const std::string& _loc,
                   const std::string& _nom, const std::string& _dir, const std::string& _cp)
    : cif(_cif), provincia(_prov), localidad(_loc), nombre(_nom),
      direccion(_dir), codPostal(_cp), linkMedi(nullptr) {}

const std::string& Farmacia::getCIF() const        { return cif; }
const std::string& Farmacia::getNombre() const     { return nombre; }
const std::string& Farmacia::getLocalidad() const  { return localidad; }
const std::string& Farmacia::getProvincia() const  { return provincia; }
const std::string& Farmacia::getDireccion() const  { return direccion; }   // <-- añadido
const std::string& Farmacia::getCodPostal() const  { return codPostal; }   // <-- añadido

void Farmacia::setLinkMedi(MediExpress* medi) { linkMedi = medi; }

PaMedicamento* Farmacia::buscaMedicam(int id_num) const {
    for (unsigned i = 0; i < dispenses.tamlog(); ++i) {
        PaMedicamento* pa = dispenses[i];
        if (pa && pa->operator==(id_num)) return pa;
    }
    return nullptr;
}

void Farmacia::dispensaMedicam(PaMedicamento* pa) {
    if (!pa) return;
    if (buscaMedicam(pa->getIdNum()) == nullptr) {
        dispenses.insertar(pa);
    }
}

PaMedicamento* Farmacia::pedidoMedicam(int id_num) {
    PaMedicamento* ya = buscaMedicam(id_num);
    if (ya) return ya;
    if (linkMedi) {
        return linkMedi->suministrarFarmacia(*this, id_num);
    }
    return nullptr;
}

