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

    void buscarYMostrarLabsGranada();
    void indicarCuantosLabsJaen();
    void indicarYMostrarLaboratoriosMadrid();
    void mostrarLabsSuministranAceites();
    void asignarLabsAMedicamentosSinSuministrador();

    // Función para el trabajo en parejas
    void eliminarLabsBruselas();
};

#endif // MEDIEXPRESS_H
