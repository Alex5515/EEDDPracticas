#ifndef MEDIEXPRESS_H
#define MEDIEXPRESS_H

#include <string>
#include "VDinamico.h"
#include "ListaEnlazada.h"
#include "Pamedicamento.h"
#include "Laboratorio.h"

class MediExpress {
private:
    VDinamico<Pamedicamento> medication;
    ListaEnlazada<Laboratorio> labs;

    // Métodos privados para la inicialización
    void cargarMedicamentos(const std::string& filename);
    void cargarLaboratorios(const std::string& filename);
    void enlazarMedicamentosConLabs();

public:

    MediExpress(const std::string& file_meds, const std::string& file_labs);

    void suministrarMed(Pamedicamento* pa, Laboratorio* l);
    Laboratorio* buscarLab(const std::string& nombreLab);
    VDinamico<Laboratorio*> buscarLabCiudad(const std::string& nombreCiudad);
    VDinamico<Pamedicamento*> buscarCompuesto(const std::string& nombrePA);
    VDinamico<Pamedicamento*> getMedicamSinLab();

    void BuscarGranada();
    void BuscarJaen();
    void ByMostrarMadrid();
    void Aceites();
    void AsignarMadrid();
    void EliminarBruselas();
};

#endif //MEDIEXPRESS_H

