// Farmacia.cpp
#include "Farmacia.h"
#include "PaMedicamento.h"  // getId(), getNombre(), ...  :contentReference[oaicite:1]{index=1}
#include "MediExpress.h"    // suministrarFarmacia(Farmacia&, int)
#include <cstdlib>

static inline int to_int(const std::string& s) {
    // conversión segura básica; si falla -> 0 (ajusta si quieres tratar errores)
    try { return std::stoi(s); } catch (...) { return 0; }
}

PaMedicamento* Farmacia::buscaMedicam(const std::string& id_num) const {
    return buscaMedicam(to_int(id_num));
}

PaMedicamento* Farmacia::buscaMedicam(int id_num) const {
    for (unsigned i = 0; i < _dispense.tamlog(); ++i) {
        // Ahora la comparación es por int usando PaMedicamento::getId()
        if (_dispense[i] && _dispense[i]->getIdNum() == id_num) return _dispense[i];
    }
    return nullptr;
}

void Farmacia::dispensaMedicam(PaMedicamento* pa){
    if (pa) _dispense.insertar(pa);
}

PaMedicamento* Farmacia::pedidoMedicam(const std::string& id_num){
    // delega en la versión int para llamar a la firma correcta de MediExpress
    return pedidoMedicam(to_int(id_num));
}

PaMedicamento* Farmacia::pedidoMedicam(int id_num){
    if (PaMedicamento* m = buscaMedicam(id_num)) return m;
    if (!_linkMedi) return nullptr;
    // Llama a suministrarFarmacia(Farmacia&, int) — firma del UML
    return _linkMedi->suministrarFarmacia(*this, id_num);
}

std::ostream& operator<<(std::ostream& os, const Farmacia& f){
    os << "Farmacia(CIF=" << f.cif()
       << ", Nombre=" << f.nombre()
       << ", " << f.localidad()
       << ", " << f.provincia()
       << ", CP=" << f.codPostal() << ")";
    return os;
}

