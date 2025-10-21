#include "MediExpress.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>



MediExpress::MediExpress(const std::string& file_meds, const std::string& file_labs) {
    cargarMedicamentos(file_meds);
    cargarLaboratorios(file_labs);
    enlazarMedicamentosConLaboratorios();
    std::cout << "Inicializacion de datos completada" << std::endl;
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
        medicacion.insertar(Pamedicamento(idnum, idalpha, nombre));
    }
    std::cout << "Cargados " << medicacion.tamlog() << " medicamentos." << std::endl;
}

void MediExpress::cargarLaboratorios(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de laboratorios: " << filename << std::endl;
        return;
    }
    std::string line;
    getline(file, line);

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string id_str, nombre, dir, ciudad, prov, pais, tel;
        getline(ss, id_str, ';');
        getline(ss, nombre, ';');
        getline(ss, dir, ';');
        getline(ss, ciudad, ';');
        getline(ss, prov, ';');
        getline(ss, pais, ';');
        getline(ss, tel, ';');
        laboratorios.insertarFinal(Laboratorio(std::stoi(id_str), nombre, dir, ciudad, prov, pais, tel));
    }
    std::cout << "Cargados " << laboratorios.tam() << " laboratorios." << std::endl;
}

void MediExpress::enlazarMedicamentosConLaboratorios() {
    ListaEnlazada<Laboratorio>::Iterador iterador0 = laboratorios.iterador();
    int med_idx = 0;

    while (iterador0 != laboratorios.end() && med_idx < medicacion.tamlog()) {
        Laboratorio& lab_actual = *iterador0;

        if (med_idx < medicacion.tamlog()) {
            medicacion[med_idx].suministrarMed(&lab_actual);
            med_idx++;
        }
        if (med_idx < medicacion.tamlog()) {
            medicacion[med_idx].suministrarMed(&lab_actual);
            med_idx++;
        }
        ++iterador0;
    }
}

void MediExpress::buscarYMostrarLabsGranada() {
    std::cout << "\nLaboratorios en Granada" << std::endl;
    int count = 0;
    for (ListaEnlazada<Laboratorio>::Iterador iterador1 = laboratorios.iterador(); iterador1 != laboratorios.end(); ++iterador1) {
        if ((*iterador1).getProvincia() == "Granada") {
            std::cout << *iterador1 << std::endl;
            count++;
        }
    }
    if (count == 0) {
        std::cout << "No se encontraron laboratorios en Granada" << std::endl;
    }
}

void MediExpress::indicarCuantosLabsJaen() {
    std::cout << "\nNumero de laboratorios en Jaen" << std::endl;
    int count = 0;
    for (ListaEnlazada<Laboratorio>::Iterador iterador2 = laboratorios.iterador(); iterador2 != laboratorios.end(); ++iterador2) {
        if ((*iterador2).getProvincia() == "Jaén") {
            count++;
        }
    }
    std::cout << "Hay " << count << " laboratorios en Jaen." << std::endl;
}

void MediExpress::indicarYMostrarLaboratoriosMadrid() {
    std::cout << "\n Laboratorios en Madrid" << std::endl;
    VDinamico<Laboratorio*> labs_madrid;
    for (ListaEnlazada<Laboratorio>::Iterador iterador3 = laboratorios.iterador(); iterador3 != laboratorios.end(); ++iterador3) {
        if ((*iterador3).getProvincia() == "Madrid") {
            labs_madrid.insertar(&(*iterador3));
        }
    }
    std::cout << "Hay " << labs_madrid.tamlog() << " laboratorios en Madrid" << std::endl;
    std::cout << "Mostrando los 10 primeros: " << std::endl;
    for (unsigned i = 0; i < 10 && i < labs_madrid.tamlog(); ++i) {
        std::cout << "- " << *labs_madrid[i] << std::endl;
    }
}

void MediExpress::asignarLabsAMedicamentosSinSuministrador() {
    std::cout << "\n Asignando laboratorios de Madrid a medicamentos sin suministrador " << std::endl;
    VDinamico<Laboratorio*> labs_madrid;
    for (ListaEnlazada<Laboratorio>::Iterador iterador4 = laboratorios.iterador(); iterador4 != laboratorios.end(); ++iterador4) {
        if ((*iterador4).getProvincia() == "Madrid") {
            labs_madrid.insertar(&(*iterador4));
        }
    }
    int lab_idx = 0;
    int asignados = 0;
    for (unsigned i = 0; i < medicacion.tamlog(); ++i) {
        if (medicacion[i].get_suministrador() == nullptr) {
            if (lab_idx < labs_madrid.tamlog()) {
                medicacion[i].suministrarMed(labs_madrid[lab_idx]);
                lab_idx++;
                asignados++;
            } else {
                break;
            }
        }
    }
    std::cout << "Se han asignado " << asignados << " medicamentos a laboratorios de Madrid." << std::endl;
}

void MediExpress::eliminarLabsBruselas() {
    std::cout << "\n--- Eliminando laboratorios de Bruselas ---" << std::endl;

    std::vector<int> id_a_eliminar;
    ListaEnlazada<Laboratorio>::Iterador it_busqueda = laboratorios.iterador();
    while (it_busqueda != laboratorios.end()) {
        std::string ciudad = (*it_busqueda).getCiudad();
        std::string provincia = (*it_busqueda).getProvincia();
        //convertimos a minúsculas para comparar
        std::transform(ciudad.begin(), ciudad.end(), ciudad.begin(), ::tolower);
        std::transform(provincia.begin(), provincia.end(), provincia.begin(), ::tolower);

        if (ciudad.find("bruselas") != std::string::npos || provincia.find("bruselas") != std::string::npos) {
            id_a_eliminar.push_back((*it_busqueda).getId());
        }
        ++it_busqueda;
    }

    if (id_a_eliminar.empty()) {
        std::cout << "No se encontraron laboratorios en Bruselas para eliminar." << std::endl;
        return;
    }

    //Desenlazar los medicamentos asociados
    for (unsigned int i = 0; i < medicacion.tamlog(); ++i) {
        if (medicacion[i].get_suministrador() != nullptr) {
            int lab_id = medicacion[i].get_suministrador()->getId();
            for (unsigned int j = 0; j < id_a_eliminar.size(); ++j) {
                if (lab_id == id_a_eliminar[j]) {
                    medicacion[i].suministrarMed(nullptr); // Desenlazamos
                    break;
                }
            }
        }
    }
    ListaEnlazada<Laboratorio>::Iterador iterador_borrado = laboratorios.iterador();
    while (iterador_borrado != laboratorios.end()) {
        int lab_id_actual = (*iterador_borrado).getId();
        bool borramos = false;

        for (unsigned int i = 0; i < id_a_eliminar.size(); ++i) {
            if (lab_id_actual == id_a_eliminar[i]) {
                borramos = true;
                break;
            }
        }

        if (borramos) {
            ListaEnlazada<Laboratorio>::Iterador iterador_a_borrar = iterador_borrado;
            ++iterador_borrado;
            //Borramos el nodo usando la copia
            laboratorios.borra(iterador_a_borrar);
        } else {
            ++iterador_borrado;
        }
    }

    std::cout << "Se han eliminado " << id_a_eliminar.size() << " laboratorios de Bruselas" << std::endl;
}

void MediExpress::mostrarLabsSuministranAceites() {
    std::cout << "\n Laboratorios que suministran productos con ACEITE en el nombre" << std::endl;

    VDinamico<Pamedicamento> medicamentos_con_aceite;
    std::string busqueda = "aceite";

    for (unsigned int i = 0; i < medicacion.tamlog(); ++i) {
        std::string nombre_original = medicacion[i].get_nombre();
        std::string nombre_lower = nombre_original;

        // Convertir a minúsculas
        std::transform(nombre_lower.begin(), nombre_lower.end(), nombre_lower.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        if (nombre_lower.find(busqueda) != std::string::npos) {
            medicamentos_con_aceite.insertar(medicacion[i]);
        }
    }

    if (medicamentos_con_aceite.tamlog() == 0) {
        std::cout << "No se encontraron medicamentos que contengan 'ACEITE'." << std::endl;
        return;
    }

    VDinamico<Laboratorio*> resultados_lab;
    for (unsigned int i = 0; i < medicamentos_con_aceite.tamlog(); ++i) {
        Laboratorio* lab_actual = medicamentos_con_aceite[i].get_suministrador();

        if (lab_actual != nullptr) {
            bool ya_aparece = false;
            // Comprobar si este laboratorio ya lo hemos añadido
            for (unsigned int j = 0; j < resultados_lab.tamlog(); ++j) {
                if (resultados_lab[j]->getId() == lab_actual->getId()) {
                    ya_aparece = true;
                    break;
                }
            }
            // Si no estaba, lo añadimos
            if (!ya_aparece) {
                resultados_lab.insertar(lab_actual);
            }
        }
    }

    if (resultados_lab.tamlog() == 0) {
        std::cout << "Ningun laboratorio suministra productos con 'ACEITES'." << std::endl;
        return;
    }

    std::cout << "Se encontraron " << resultados_lab.tamlog() << " laboratorios que suministran 'ACEITES':" << std::endl;
    for (unsigned int i = 0; i < resultados_lab.tamlog(); ++i) {
        std::cout << "- " << *(resultados_lab[i]) << std::endl;
    }
}



