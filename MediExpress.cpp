/**
 * @file MediExpress.cpp
 * @brief Implementación de MediExpress adaptada a VDinamico y ListaEnlazada reales.
 */

#include "MediExpress.h"
#include "Farmacia.h"
#include "PaMedicamento.h"
#include "Laboratorio.h"

MediExpress::MediExpress() {}

AVL<Farmacia>& MediExpress::avlFarmacias() { return pharmacy; }
VDinamico<PaMedicamento>& MediExpress::getMedications() { return medications; }
ListaEnlazada<Laboratorio>& MediExpress::getLabs() { return labs; }

void MediExpress::suministrarMed(PaMedicamento* pa, Laboratorio* l) {
    if (pa) pa->setLaboratorio(l);
}

Laboratorio* MediExpress::buscarLab(const std::string& nombreLab) {
    typename ListaEnlazada<Laboratorio>::Iterador it = labs.iterador();
    while (it != labs.end()) {
        Laboratorio& ref = *it;
        if (ref.getNombre() == nombreLab)
            return &ref;
        ++it;
    }
    return nullptr;
}

VDinamico<Laboratorio*> MediExpress::buscarLabCiudad(const std::string& nombreCiudad) {
    VDinamico<Laboratorio*> out;
    typename ListaEnlazada<Laboratorio>::Iterador it = labs.iterador();
    while (it != labs.end()) {
        Laboratorio& ref = *it;
        if (ref.getLocalidad() == nombreCiudad)
            out.insertar(&ref);
        ++it;
    }
    return out;
}

PaMedicamento* MediExpress::buscarCompuesto(int id_num) {
    for (unsigned i = 0; i < medications.tamlog(); ++i) {
        if (medications[i].operator==(id_num))
            return &medications[i];
    }
    return nullptr;
}

VDinamico<PaMedicamento*> MediExpress::getMedicamSinLab() {
    VDinamico<PaMedicamento*> out;
    for (unsigned i = 0; i < medications.tamlog(); ++i) {
        if (medications[i].getLaboratorio() == nullptr)
            out.insertar(&medications[i]);
    }
    return out;
}

PaMedicamento* MediExpress::suministrarFarmacia(Farmacia& f, int id_num) {
    PaMedicamento* pa = buscarCompuesto(id_num);
    if (!pa) return nullptr;
    f.dispensaMedicam(pa);
    return pa;
}

Farmacia* MediExpress::buscarFarmacia(const std::string& cif) {
    VDinamico<Farmacia*> inorden = pharmacy.recorreInorden();
    for (unsigned i = 0; i < inorden.tamlog(); ++i) {
        if (inorden[i] && inorden[i]->operator==(cif))
            return inorden[i];
    }
    return nullptr;
}

VDinamico<Laboratorio*> MediExpress::buscarLabs(const std::string& nombrePA) {
    VDinamico<Laboratorio*> out;
    for (unsigned i = 0; i < medications.tamlog(); ++i) {
        const std::string& nom = medications[i].getNombre();
        std::string a = nom, b = nombrePA;
        std::transform(a.begin(), a.end(), a.begin(), ::toupper);
        std::transform(b.begin(), b.end(), b.begin(), ::toupper);
        if (a.find(b) != std::string::npos) {
            Laboratorio* L = medications[i].getLaboratorio();
            if (L) out.insertar(L);
        }
    }
    return out;
}

void MediExpress::insertarFarmacia(const Farmacia& f) {
    pharmacy.inserta(f);
    VDinamico<Farmacia*> ino = pharmacy.recorreInorden();
    for (unsigned i = 0; i < ino.tamlog(); ++i) {
        if (ino[i] && ino[i]->getCIF() == f.getCIF()) {
            ino[i]->setLinkMedi(this);
            break;
        }
    }
}

void MediExpress::insertarMedicamento(const PaMedicamento& m) {
    medications.insertar(m);
}

void MediExpress::insertarLaboratorio(const Laboratorio& l) {
    labs.insertarFinal(l);
}
