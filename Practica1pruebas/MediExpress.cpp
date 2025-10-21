#include "MediExpress.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

// Constructor y Carga de Datos (con constructor de Laboratorio corregido)
MediExpress::MediExpress(const std::string& file_meds, const std::string& file_labs) {
    std::cout << "Inicializando MediExpress..." << std::endl;
    cargarMedicamentos(file_meds);
    cargarLaboratorios(file_labs);
    enlazarMedicamentosConLabs();
    std::cout << "Inicializacion completada." << std::endl;
}

void MediExpress::cargarMedicamentos(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de medicamentos: " << filename << std::endl;
        return;
    }
    std::string line;
    getline(file, line);
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string idnum, idalpha, nombre;
        getline(ss, idnum, ';');
        getline(ss, idalpha, ';');
        getline(ss, nombre, ';');
        medication.insertar(Pamedicamento(idnum, idalpha, nombre));
    }
    std::cout << "Cargados " << medication.tamlog() << " medicamentos." << std::endl;
}

// CORREGIDO: Uso del constructor y getters de Laboratorio
void MediExpress::cargarLaboratorios(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de laboratorios: " << filename << std::endl;
        return;
    }
    std::string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();
        std::stringstream ss(line);
        std::string id_str, nombre, dir, cp, full_loc, ciudad, prov;
        getline(ss, id_str, ';');
        getline(ss, nombre, ';');
        getline(ss, dir, ';');
        getline(ss, cp, ';');
        getline(ss, full_loc);

        size_t pos_open = full_loc.find('(');
        size_t pos_close = full_loc.find(')');
        if (pos_open != std::string::npos && pos_close != std::string::npos) {
            ciudad = full_loc.substr(0, pos_open);
            prov = full_loc.substr(pos_open + 1, pos_close - pos_open - 1);
        } else {
            ciudad = prov = full_loc;
        }

        // Llamada al constructor corregida (ahora con 5 parámetros)
        labs.insertarFinal(Laboratorio(std::stoi(id_str), nombre, dir, cp, prov));
    }
    std::cout << "Cargados " << labs.tam() << " laboratorios." << std::endl;
}

void MediExpress::enlazarMedicamentosConLabs() {
    ListaEnlazada<Laboratorio>::Iterador it = labs.iterador();
    int med_idx = 0;
    while (it != labs.end() && med_idx < medication.tamlog()) {
        Laboratorio* lab_actual = &(*it);
        if (med_idx < medication.tamlog()) medication[med_idx++].suministrarMed(lab_actual);
        if (med_idx < medication.tamlog()) medication[med_idx++].suministrarMed(lab_actual);
        ++it;
    }
    std::cout << "Enlace automatico completado." << std::endl;
}


void MediExpress::suministrarMed(Pamedicamento* pa, Laboratorio* l) {
    if (pa) pa->suministrarMed(l);
}

Laboratorio* MediExpress::buscarLab(const std::string& nombreLab) {
    std::string nombre_lower = nombreLab;
    std::transform(nombre_lower.begin(), nombre_lower.end(), nombre_lower.begin(), ::tolower);
    for (ListaEnlazada<Laboratorio>::Iterador it = labs.iterador(); it != labs.end(); ++it) {
        std::string lab_nombre_lower = (*it).getNombre();
        std::transform(lab_nombre_lower.begin(), lab_nombre_lower.end(), lab_nombre_lower.begin(), ::tolower);
        if (lab_nombre_lower.find(nombre_lower) != std::string::npos) return &(*it);
    }
    return nullptr;
}

VDinamico<Laboratorio*> MediExpress::buscarLabCiudad(const std::string& nombreCiudad) {
    VDinamico<Laboratorio*> resultado;
    std::string ciudad_lower = nombreCiudad;
    std::transform(ciudad_lower.begin(), ciudad_lower.end(), ciudad_lower.begin(), ::tolower);
    for (ListaEnlazada<Laboratorio>::Iterador it = labs.iterador(); it != labs.end(); ++it) {
        std::string lab_prov_lower = (*it).getProvincia();
        std::transform(lab_prov_lower.begin(), lab_prov_lower.end(), lab_prov_lower.begin(), ::tolower);
        if (lab_prov_lower.find(ciudad_lower) != std::string::npos) {
            resultado.insertar(&(*it));
        }
    }
    return resultado;
}

VDinamico<Pamedicamento*> MediExpress::buscarCompuesto(const std::string& nombrePA) {
    VDinamico<Pamedicamento*> resultado;
    std::string pa_lower = nombrePA;
    std::transform(pa_lower.begin(), pa_lower.end(), pa_lower.begin(), ::tolower);
    for (unsigned int i = 0; i < medication.tamlog(); ++i) {
        std::string med_nombre_lower = medication[i].get_nombre();
        std::transform(med_nombre_lower.begin(), med_nombre_lower.end(), med_nombre_lower.begin(), ::tolower);
        if (med_nombre_lower.find(pa_lower) != std::string::npos) resultado.insertar(&medication[i]);
    }
    return resultado;
}

VDinamico<Pamedicamento*> MediExpress::getMedicamSinLab() {
    VDinamico<Pamedicamento*> resultado;
    for (unsigned int i = 0; i < medication.tamlog(); ++i) {
        if (medication[i].get_suministrador() == nullptr) resultado.insertar(&medication[i]);
    }
    return resultado;
}

void MediExpress::BuscarGranada() {
    std::cout << "\n--- Laboratorios en Granada o provincia ---" << std::endl;
    VDinamico<Laboratorio*> labs_granada = buscarLabCiudad("Granada");
    if (labs_granada.tamlog() == 0) {
        std::cout << "No se encontraron laboratorios en Granada." << std::endl;
    } else {
        std::cout << "Se encontraron " << labs_granada.tamlog() << " laboratorios:" << std::endl;
        for (unsigned int i = 0; i < labs_granada.tamlog(); ++i) std::cout << "- " << *(labs_granada[i]) << std::endl;
    }
}

void MediExpress::BuscarJaen() {
    std::cout << "\n--- Numero de laboratorios en Jaen ---" << std::endl;
    VDinamico<Laboratorio*> labs_jaen = buscarLabCiudad("Jaen");
    std::cout << "Hay " << labs_jaen.tamlog() << " laboratorios en Jaen." << std::endl;
}

void MediExpress::ByMostrarMadrid() {
    std::cout << "\n--- Laboratorios en Madrid ---" << std::endl;
    VDinamico<Laboratorio*> labs_madrid = buscarLabCiudad("Madrid");
    std::cout << "Hay " << labs_madrid.tamlog() << " laboratorios en Madrid." << std::endl;
    std::cout << "Mostrando los 10 primeros:" << std::endl;
    for (unsigned int i = 0; i < labs_madrid.tamlog() && i < 10; ++i) std::cout << "- " << *(labs_madrid[i]) << std::endl;
}

void MediExpress::Aceites() {
    std::cout << "\n--- Laboratorios que suministran productos 'ACEITE' ---" << std::endl;
    VDinamico<Pamedicamento*> meds_aceite = buscarCompuesto("ACEITE");
    if (meds_aceite.tamlog() == 0) {
        std::cout << "No se encontraron medicamentos que contengan 'ACEITE'." << std::endl;
        return;
    }
    VDinamico<Laboratorio*> labs_resultado;
    for (unsigned int i = 0; i < meds_aceite.tamlog(); ++i) {
        Laboratorio* lab = meds_aceite[i]->get_suministrador();
        if (lab) {
            bool ya_esta = false;
            for (unsigned int j = 0; j < labs_resultado.tamlog(); ++j) if (labs_resultado[j]->getId() == lab->getId()) { ya_esta = true; break; }
            if (!ya_esta) labs_resultado.insertar(lab);
        }
    }
    if (labs_resultado.tamlog() == 0) std::cout << "Ningun laboratorio suministra productos con 'ACEITE'" << std::endl;
    else {
        std::cout << "Se encontraron " << labs_resultado.tamlog() << " laboratorios:" << std::endl;
        for (unsigned int i = 0; i < labs_resultado.tamlog(); ++i) std::cout << "- " << *(labs_resultado[i]) << std::endl;
    }
}

void MediExpress::AsignarMadrid() {
    std::cout << "\nAsignando laboratorios de Madrid a medicamentos sin suministrador" << std::endl;
    VDinamico<Pamedicamento*> meds_sin_lab = getMedicamSinLab();
    VDinamico<Laboratorio*> labs_madrid = buscarLabCiudad("Madrid");
    int asignados = 0;
    if (labs_madrid.tamlog() > 0) {
        for (unsigned int i = 0; i < meds_sin_lab.tamlog(); ++i) {
            suministrarMed(meds_sin_lab[i], labs_madrid[i % labs_madrid.tamlog()]);
            asignados++;
        }
    }
    std::cout << "Se han asignado " << asignados << " medicamentos a laboratorios de Madrid" << std::endl;
}

void MediExpress::EliminarBruselas() {
    std::cout << "\nEliminando laboratorios de Bruselas" << std::endl;
    VDinamico<Laboratorio*> labs_bruselas = buscarLabCiudad("Bruselas");
    if (labs_bruselas.tamlog() == 0) {
        std::cout << "No se encontraron laboratorios en Bruselas para eliminar." << std::endl;
        return;
    }
    VDinamico<int> ids_a_eliminar;
    for (unsigned int i = 0; i < labs_bruselas.tamlog(); ++i) ids_a_eliminar.insertar(labs_bruselas[i]->getId());
    for (unsigned int i = 0; i < medication.tamlog(); ++i) {
        if (medication[i].get_suministrador()) {
            for (unsigned int j = 0; j < ids_a_eliminar.tamlog(); ++j) {
                if (medication[i].get_suministrador()->getId() == ids_a_eliminar[j]) {
                    suministrarMed(&medication[i], nullptr);
                    break;
                }
            }
        }
    }
    ListaEnlazada<Laboratorio>::Iterador it = labs.iterador();
    while (it != labs.end()) {
        bool hay_que_borrar = false;
        for (unsigned int i = 0; i < ids_a_eliminar.tamlog(); ++i) if ((*it).getId() == ids_a_eliminar[i]) { hay_que_borrar = true; break; }
        if (hay_que_borrar) it = labs.borra(it); else ++it;
    }
    std::cout << "Se han eliminado " << ids_a_eliminar.tamlog() << " laboratorios de Bruselas" << std::endl;
}
