#ifndef LABORATORIO_H
#define LABORATORIO_H
/**
 * @file Laboratorio.h
 * @brief Declaración de la clase Laboratorio.
 */

#include <string>

/**
 * @class Laboratorio
 * @brief Representa un laboratorio farmacéutico.
 */
class Laboratorio {
private:
    int id;                     ///< Identificador entero (único)
    std::string nombreLab;      ///< Nombre del laboratorio
    std::string direccion;      ///< Dirección
    std::string codPostal;      ///< Código postal
    std::string localidad;      ///< Localidad

public:
    Laboratorio();
    Laboratorio(int id_, const std::string& nombre, const std::string& dir,
                const std::string& cp, const std::string& loc);

    // Getters
    int getId() const;
    const std::string& getNombre() const;
    const std::string& getDireccion() const;
    const std::string& getCodPostal() const;
    const std::string& getLocalidad() const;

    // Setters mínimos
    void setNombre(const std::string& n);

    // Utilidades
    bool nombreContiene(const std::string& sub) const;  ///< búsqueda parcial por nombre
};


#endif //LABORATORIO_H
