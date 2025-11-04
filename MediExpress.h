#ifndef MEDIEXPRESS_H
#define MEDIEXPRESS_H
// MediExpress.h
/**
 * @file MediExpress.h
 * @brief Declaración de la clase MediExpress (según UML de la práctica).
 */

#include <string>
#include "AVL.h"
#include "VDinamico.h"
#include "ListaEnlazada.h"

class Farmacia;
class PaMedicamento;
class Laboratorio;

/**
 * @class MediExpress
 * @brief Sistema de gestión y suministro de medicamentos a farmacias.
 *
 * Estructuras:
 *  - AVL<Farmacia> pharmacy
 *  - VDinamico<PaMedicamento> medications
 *  - ListaEnlazada<Laboratorio> labs
 */
class MediExpress {
private:
    AVL<Farmacia> pharmacy;                 ///< Árbol de farmacias (clave: CIF)
    VDinamico<PaMedicamento> medications;   ///< Vector de medicamentos (objetos propietarios)
    ListaEnlazada<Laboratorio> labs;        ///< Lista de laboratorios

public:
    // Constructores
    MediExpress();

    // Acceso a estructuras (por si se necesitan en pruebas)
    AVL<Farmacia>& avlFarmacias();
    VDinamico<PaMedicamento>& getMedications();
    ListaEnlazada<Laboratorio>& getLabs();

    /* ========================
       Funciones del UML
       ======================== */

    /// @brief Enlaza un medicamento y un laboratorio (no crea copias).
    void suministrarMed(PaMedicamento* pa, Laboratorio* l);

    /// @brief Busca un laboratorio por nombre exacto. Devuelve puntero o nullptr.
    Laboratorio* buscarLab(const std::string& nombreLab);

    /// @brief Busca laboratorios por ciudad/localidad. Devuelve punteros.
    VDinamico<Laboratorio*> buscarLabCiudad(const std::string& nombreCiudad);

    /// @brief Busca medicamento por id_num. Devuelve puntero o nullptr.
    PaMedicamento* buscarCompuesto(int id_num);

    /// @brief Devuelve todos los medicamentos que no tienen laboratorio asignado.
    VDinamico<PaMedicamento*> getMedicamSinLab();

    /// @brief Suministra a una farmacia el medicamento id_num (enlaza sin copias) y lo dispensa.
    PaMedicamento* suministrarFarmacia(Farmacia& f, int id_num);

    /// @brief Busca una farmacia por CIF en el AVL. Devuelve puntero o nullptr.
    Farmacia* buscarFarmacia(const std::string& cif);

    /// @brief Busca laboratorios cuyo nombre contenga la cadena dada (parcial).
    VDinamico<Laboratorio*> buscarLabs(const std::string& nombrePA);

    /* ========================
       Carga de datos (pruebas)
       ======================== */

    /// @brief Inserta una farmacia en el AVL y enlaza su relación con MediExpress.
    void insertarFarmacia(const Farmacia& f);

    /// @brief Inserta un medicamento en el vector.
    void insertarMedicamento(const PaMedicamento& m);

    /// @brief Inserta un laboratorio en la lista.
    void insertarLaboratorio(const Laboratorio& l);
};


#endif //MEDIEXPRESS_H

