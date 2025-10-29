#ifndef PRACTICA1_FARMACIA_H
#define PRACTICA1_FARMACIA_H

#include <string>
#include "VDinamico.h"

class PaMedicamento; // <-- clase correcta
class MediExpress;

class Farmacia {
private:
    std::string _cif, _provincia, _localidad, _nombre, _direccion, _codPostal;
    VDinamico<PaMedicamento*> _dispense;   // stock por puntero, sin copias
    MediExpress* _linkMedi = nullptr;

public:
    Farmacia() = default;
    explicit Farmacia(const std::string& cif): _cif(cif) {}
    Farmacia(std::string cif, std::string nombre, std::string direccion,
             std::string localidad, std::string provincia, std::string cp = "")
        : _cif(std::move(cif)), _provincia(std::move(provincia)), _localidad(std::move(localidad)),
          _nombre(std::move(nombre)), _direccion(std::move(direccion)), _codPostal(std::move(cp)) {}

    // getters
    const std::string& cif() const { return _cif; }
    const std::string& provincia() const { return _provincia; }
    const std::string& localidad() const { return _localidad; }
    const std::string& nombre() const { return _nombre; }
    const std::string& direccion() const { return _direccion; }
    const std::string& codPostal() const { return _codPostal; }

    void setLinkMedi(MediExpress* m) { _linkMedi = m; }

    // API del UML (y overload práctico)
    PaMedicamento* buscaMedicam(int id_num) const;
    PaMedicamento* buscaMedicam(const std::string& id_num) const;

    void dispensaMedicam(PaMedicamento* pa);

    PaMedicamento* pedidoMedicam(int id_num);
    PaMedicamento* pedidoMedicam(const std::string& id_num);

    // Para AVL: orden por CIF
    bool operator<(const Farmacia& o) const { return _cif < o._cif; }
};

std::ostream& operator<<(std::ostream& os, const Farmacia& f);

#endif //PRACTICA1_FARMACIA_H
