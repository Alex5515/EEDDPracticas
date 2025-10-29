#ifndef MEDIEXPRESS_H
#define MEDIEXPRESS_H
// MediExpress.h

#include <string>
#include <iostream>
#include "VDinamico.h"
#include "ListaEnlazada.h"
#include "PaMedicamento.h"  // clase Pamedicamento (get_id_num, get_nombre, ...) :contentReference[oaicite:4]{index=4}
#include "Laboratorio.h"    // getters id/nombre/provincia                        :contentReference[oaicite:5]{index=5}
#include "AVL.h"
#include "Farmacia.h"

class MediExpress {
private:
    VDinamico<PaMedicamento> medication;   // medicamentos (objetos)                :contentReference[oaicite:6]{index=6}
    ListaEnlazada<Laboratorio> labs;       // laboratorios (lista enlazada)         :contentReference[oaicite:7]{index=7}
    AVL<Farmacia> pharmacy;                // AVL de farmacias
    VDinamico<Farmacia> farmVec;           // para benchmark lineal
    VDinamico<std::string> farmCIF500;     // buffer de 500 CIFs (Programa I)

    // Carga de datos
    void cargarMedicamentos(const std::string& filename);
    void cargarLaboratorios(const std::string& filename);
    void enlazarMedicamentosConLabs();

    void cargarFarmaciasAVL(const std::string& filename);
    void cargarFarmaciasVector(const std::string& filename);
    void prepararCIF500(const std::string& filename);

    // Utilidades
    PaMedicamento* buscarMedPorId(const std::string& idnum);
    VDinamico<Laboratorio*> buscarLabsPorNombreMed(const std::string& nombreParcial);

public:
    MediExpress(const std::string& file_meds, const std::string& file_labs, const std::string& file_farms);

    // ---- API existente + UML ----
    void suministrarMed(PaMedicamento* pa, Laboratorio* l);
    Laboratorio* buscarLab(const std::string& nombreLab);
    VDinamico<Laboratorio*> buscarLabCiudad(const std::string& nombreCiudad);

    // UML: buscarCompuesto(Integer id_num) y buscarCompuesto(String nombrePA)
    PaMedicamento* buscarCompuesto(int id_num);
    VDinamico<PaMedicamento*> buscarCompuesto(const std::string& nombrePA);

    // UML: buscarFarmacia(String cif)
    Farmacia* buscarFarmacia(const std::string& cif);

    // UML: buscarLabs(String nombrePA) → laboratorios que suministran meds cuyo nombre contiene nombrePA
    VDinamico<Laboratorio*> buscarLabs(const std::string& nombrePA);

    // UML: suministrarFarmacia(Farmacia f, Integer id_num)  (sobrecarga práctica por cif+string)
    PaMedicamento* suministrarFarmacia(Farmacia& f, int id_num);
    PaMedicamento* suministrarFarmacia(const std::string& cif, const std::string& med_id_num);

    // Programa I (benchmark)
    void programaI_benchmarkAVLvsVector();
    void mostrarAlturaAVL() const;
    void imprimir100Inorden() const;

    // Programa II
    void asignarMedicamentosAFarmaciasCiclico();
};



#endif //MEDIEXPRESS_H

