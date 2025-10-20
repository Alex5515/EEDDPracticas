
#ifndef PRACTICA1_PAMEDICAMENTO_H
#define PRACTICA1_PAMEDICAMENTO_H

#include <string>
#include <iostream> // Para sobrecarga de operador <<

class Pamedicamento {
private:
    std::string _id_num;
    std::string _id_alpha;
    std::string _nombre;

public:
    Pamedicamento();
    Pamedicamento(const std::string &id_num, const std::string &id_alpha, const std::string &nombre);

    const std::string &get_id_num() const;
    void set_id_num(const std::string &id_num);

    const std::string &get_id_alpha() const;
    void set_id_alpha(const std::string &id_alpha);

    const std::string &get_nombre() const;
    void set_nombre(const std::string &nombre);

    // Operador < para ordenar por id_num
    bool operator<(const Pamedicamento& other) const {
        return std::stoi(_id_num) < std::stoi(other._id_num);
    }

    // Operador == para búsqueda
    bool operator==(const Pamedicamento& other) const {
        return _id_num == other._id_num;
    }

    // Operador << para imprimir
    friend std::ostream& operator<<(std::ostream& os, const Pamedicamento& med) {
        os << "(Id_number=" << med._id_num
           << " id_alpha=" << med._id_alpha
           << " Nombre=" << med._nombre << ")";
        return os;
    }
};


#endif //PRACTICA1_PAMEDICAMENTO_H