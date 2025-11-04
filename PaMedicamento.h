#ifndef PRACTICA1_PAMEDICAMENTO_H
#define PRACTICA1_PAMEDICAMENTO_H

/**
 * @file PaMedicamento.h
 * @brief Declaración de la clase PaMedicamento (principio activo / medicamento).
 */

#include <string>

class Laboratorio; // forward

/**
 * @class PaMedicamento
 * @brief Representa un medicamento / principio activo.
 */
class PaMedicamento {
private:
    int      id_num;        ///< Identificador numérico (único)
    std::string id_alpha;   ///< Identificador alfanumérico
    std::string nombre;     ///< Nombre del medicamento / PA
    Laboratorio* servidoPor;///< Laboratorio que lo sirve (puede ser nullptr)

public:
    /// @brief Ctor por defecto.
    PaMedicamento();

    /// @brief Ctor con datos.
    PaMedicamento(int idNum, const std::string& idAlpha, const std::string& nom);

    // Getters / setters básicos (sin copias pesadas)
    int getIdNum() const;
    const std::string& getIdAlpha() const;
    const std::string& getNombre() const;
    Laboratorio* getLaboratorio() const;

    void setLaboratorio(Laboratorio* lab);
    void setNombre(const std::string& nom);

    // Comparadores por id_num (para búsquedas)
    bool operator==(int id) const { return id_num == id; }
    bool operator<(const PaMedicamento& other) const { return id_num < other.id_num; }
};

#endif //PRACTICA1_PAMEDICAMENTO_H
