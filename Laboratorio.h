#ifndef LABORATORIO_H
#define LABORATORIO_H

#include <iostream>
#include <string>

class Laboratorio {
private:
    // ====== Campos EXACTOS del UML ======
    int         id;           // id : Integer
    std::string nombreLab;    // nombreLab : String
    std::string direccion;    // direccion : String
    std::string codPostal;    // codPostal : String
    std::string localidad;    // localidad : String

public:
    // ====== Constructores ======
    // Orden acorde al UML: (id, nombreLab, direccion, codPostal, localidad)
    Laboratorio(int i = 0,
                std::string nombre = "",
                std::string dir = "",
                std::string cp = "",
                std::string loc = "")
        : id(i), nombreLab(std::move(nombre)), direccion(std::move(dir)),
          codPostal(std::move(cp)), localidad(std::move(loc)) {}

    // ====== Getters/Setters en nomenclatura UML ======
    int                  getId()           const { return id; }
    const std::string&   getNombreLab()    const { return nombreLab; }
    const std::string&   getDireccion()    const { return direccion; }
    const std::string&   getCodPostal()    const { return codPostal; }
    const std::string&   getLocalidad()    const { return localidad; }

    void setId(int v)                         { id = v; }
    void setNombreLab(const std::string& v)   { nombreLab = v; }
    void setDireccion(const std::string& v)   { direccion = v; }
    void setCodPostal(const std::string& v)   { codPostal = v; }
    void setLocalidad(const std::string& v)   { localidad = v; }

    // ====== ALIAS DE COMPATIBILIDAD (para no romper tu código existente) ======
    // Antes usabas: getNombre(), getDireccion(), getCp(), getCiudad(), getProvincia()
    // Mapeamos a los nuevos campos del UML:
    std::string getNombre()      const { return nombreLab; }       // alias de nombreLab
    std::string getCp()          const { return codPostal; }       // alias de codPostal
    std::string getCiudad()      const { return localidad; }       // alias de localidad
    std::string getProvincia()   const { return localidad; }       // alias de localidad (no hay "provincia" en el UML)

    void setNombre(const std::string& v)   { nombreLab  = v; }
    void setCp(const std::string& v)       { codPostal  = v; }
    void setCiudad(const std::string& v)   { localidad  = v; }
    void setProvincia(const std::string& v){ localidad  = v; }

    // ====== Operador de impresión ======
    friend std::ostream& operator<<(std::ostream& os, const Laboratorio& lab) {
        os << "Laboratorio{ id=" << lab.id
           << ", nombreLab='" << lab.nombreLab
           << "', localidad='" << lab.localidad
           << "', codPostal='" << lab.codPostal
           << "', direccion='" << lab.direccion
           << "' }";
        return os;
    }

    // ====== Comparadores útiles (por id) opcionales ======
    bool operator<(const Laboratorio& o)  const { return id <  o.id; }
    bool operator==(const Laboratorio& o) const { return id == o.id; }
};

#endif //LABORATORIO_H
