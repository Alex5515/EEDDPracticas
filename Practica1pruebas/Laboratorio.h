#ifndef LABORATORIO_H
#define LABORATORIO_H

#include <iostream>
#include <string>

class Laboratorio {
private:
    int id;
    std::string nombre;
    std::string direccion;
    std::string CP;
    std::string ciudad;
    std::string provincia;

public:

    Laboratorio(int i = 0, std::string n = "", std::string dir = "", std::string c = "", std::string prov = "")
        : id(i), nombre(n), direccion(dir), CP(c), provincia(prov) {
    }

    // --- GETTERS ---
    int getId() const { return id; }
    std::string getNombre() const { return nombre; }
    std::string getDireccion() const { return direccion; }
    std::string getCp() const { return CP; }
    std::string getCiudad() const { return ciudad; }
    std::string getProvincia() const { return provincia; }

    // Operador de impresión
    friend std::ostream& operator<<(std::ostream& os, const Laboratorio& lab) {
        os << "ID: " << lab.id << ", Nombre: " << lab.nombre << ", Provincia: " << lab.provincia;
        return os;
    }
};

#endif //LABORATORIO_H
