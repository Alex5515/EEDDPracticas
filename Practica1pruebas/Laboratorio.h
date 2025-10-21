#ifndef LABORATORIO_H
#define LABORATORIO_H

#include <string>
#include <iostream>

class Laboratorio {
private:
    int id;
    std::string nombre;
    std::string direccion;
    std::string CP;
    std::string provincia;


public:
    // Constructor
    Laboratorio(int i = 0, std::string n = "", std::string dir = "", std::string cp = "", std::string prov = "")
        : id(i), nombre(n), direccion(dir), CP(cp), provincia(prov){}

    int getId() const { return id; }
    std::string getNombre() const { return nombre; }
    std::string getCp() const { return CP; }
    std::string getProvincia() const { return provincia; }


    // Sobrecargamos el operador para imprimir
    friend std::ostream& operator<<(std::ostream& os, const Laboratorio& lab) {
        os << "ID: " << lab.id << ", Nombre: " << lab.nombre << ", CP: " << lab.CP << ", Provincia: " << lab.provincia;
        return os;
    }
};

#endif
