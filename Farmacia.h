#ifndef PRACTICA1_FARMACIA_H
#define PRACTICA1_FARMACIA_H
/**
 * @file Farmacia.h
 * @brief Declaración de la clase Farmacia (según UML de la práctica).
 */

#include <string>
#include "VDinamico.h"

class PaMedicamento;
class MediExpress;

/**
 * @class Farmacia
 * @brief Representa una farmacia con un pequeño almacén de medicamentos.
 */
class Farmacia {
private:
    // Atributos del UML
    std::string cif;
    std::string provincia;
    std::string localidad;
    std::string nombre;
    std::string direccion;
    std::string codPostal;

    // Relaciones
    MediExpress* linkMedi;                 ///< Enlace a MediExpress (no-ownership)
    VDinamico<PaMedicamento*> dispenses;   ///< Medicamentos que dispensa (punteros, sin copia)

public:
    // Constructores
    Farmacia();
    Farmacia(const std::string& _cif, const std::string& _prov, const std::string& _loc,
             const std::string& _nom, const std::string& _dir, const std::string& _cp);

    // Getters básicos
    const std::string& getCIF() const;
    const std::string& getNombre() const;
    const std::string& getLocalidad() const;
    const std::string& getProvincia() const;
    const std::string& getDireccion() const;  ///< <-- añadido
    const std::string& getCodPostal() const;  ///< <-- añadido

    // Integración
    void setLinkMedi(MediExpress* medi);

    // Operaciones del UML
    PaMedicamento* pedidoMedicam(int id_num);          ///< Pide a MediExpress si no lo tiene.
    PaMedicamento* buscaMedicam(int id_num) const;     ///< Busca por id en su dispensario.
    void dispensaMedicam(PaMedicamento* pa);           ///< Añade al dispensario si no está.

    // Comparadores para uso en AVL (clave = CIF)
    bool operator<(const Farmacia& other) const { return cif < other.cif; }
    bool operator>(const Farmacia& other) const { return cif > other.cif; }
    bool operator==(const Farmacia& other) const { return cif == other.cif; }
    bool operator==(const std::string& cif_) const { return cif == cif_; }
};

#endif //PRACTICA1_FARMACIA_H
