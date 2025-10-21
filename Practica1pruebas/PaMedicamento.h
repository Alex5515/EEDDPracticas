#ifndef PRACTICA1_PAMEDICAMENTO_H
#define PRACTICA1_PAMEDICAMENTO_H

#include <string>
#include <iostream>

class Laboratorio;

class Pamedicamento {
private:

    std::string _id_num;
    std::string _id_alpha;
    std::string _nombre;
    Laboratorio* _suministrador;

public:
    Pamedicamento(const std::string& id_num = "", const std::string& id_alpha = "", const std::string& nombre = "");

    const std::string& get_id_num() const;
    const std::string& get_id_alpha() const;
    const std::string& get_nombre() const;
    Laboratorio* get_suministrador() const;

    void set_id_num(const std::string& id_num);
    void set_id_alpha(const std::string& id_alpha);
    void set_nombre(const std::string& nombre);

    void suministrarMed(Laboratorio* lab);

    bool operator<(const Pamedicamento& other) const;
    bool operator==(const Pamedicamento& other) const;
};

std::ostream& operator<<(std::ostream& os, const Pamedicamento& med);

#endif //PRACTICA1_PAMEDICAMENTO_H
