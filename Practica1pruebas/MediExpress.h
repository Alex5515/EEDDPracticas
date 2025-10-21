#ifndef MEDIEXPRESS_H
#define MEDIEXPRESS_H

#include "VDinamico.h"
#include "ListaEnlazada.h"
#include "PaMedicamento.h"
#include "Laboratorio.h"

class MediExpress {
private:
    VDinamico<Pamedicamento> medicacion;
    ListaEnlazada<Laboratorio> laboratorios;

    void cargarMedicamentos(const std::string& filename);
    void cargarLaboratorios(const std::string& filename);
    void enlazarMedicamentosConLaboratorios();

public:

    MediExpress(const std::string& file_meds, const std::string& file_labs);

    void MostrarLabsGranada();
    void LabsJaen();
    void MostrarLaboratoriosMadrid();
    void mostrarLabsSuministranAceites();
    void LabsAMedicamentosSinSuministrador();

    void eliminarLabsBruselas();
};

#endif
