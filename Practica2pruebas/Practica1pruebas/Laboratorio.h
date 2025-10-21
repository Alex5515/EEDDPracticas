#ifndef LABORATORIO_H
#define LABORATORIO_H

#include <string>
#include <iostream>

class Laboratorio {
private:
    int id;
    std::string nombre;
    std::string direccion;
    std::string ciudad;
    std::string provincia;
    std::string pais;
    std::string telefono;

public:


    // Constructor
    Laboratorio(int i = 0, std::string n = "", std::string dir = "", std::string ciu = "", std::string prov = "", std::string p = "", std::string tel = "")
        : id(i), nombre(n), direccion(dir), ciudad(ciu), provincia(prov), pais(p), telefono(tel) {}

    int getId() const { return id; }
    std::string getNombre() const { return nombre; }
    std::string getCiudad() const { return ciudad; }
    std::string getProvincia() const { return provincia; }
    std::string getPais() const { return pais; }

    // Sobrecargamos el operador para imprimir
    friend std::ostream& operator<<(std::ostream& os, const Laboratorio& lab) {
        os << "ID: " << lab.id << ", Nombre: " << lab.nombre << ", Ciudad: " << lab.ciudad << ", Provincia: " << lab.provincia;
        return os;
    }
};

#endif // LABORATORIO_H
