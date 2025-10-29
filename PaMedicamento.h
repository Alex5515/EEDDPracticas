#ifndef PRACTICA1_PAMEDICAMENTO_H
#define PRACTICA1_PAMEDICAMENTO_H

#include <string>
#include <iostream>

// Adelanto para evitar inclusiones circulares
class Laboratorio;

class PaMedicamento {
private:
    // --- UML ---
    int         id_num;     // Identificador numérico
    std::string id_alpha;   // Identificador alfanumérico
    std::string nombre;     // Nombre del PA

    // Relación UML: servidoPor (Laboratorio l)
    Laboratorio* servidoPor = nullptr; // puntero, no copia

public:
    // --- Constructores ---
    PaMedicamento(int id_num = 0,
                  const std::string& id_alpha = "",
                  const std::string& nombre = "");

    // --- Getters (camelCase, recomendados) ---
    int                  getIdNum()     const;
    const std::string&   getIdAlpha()   const;
    const std::string&   getNombre()    const;
    Laboratorio*         getSuministrador() const;   // nombre claro
    Laboratorio*         getServidoPor() const;      // alias del UML

    // --- Setters (camelCase) ---
    void setIdNum(int v);
    void setIdAlpha(const std::string& v);
    void setNombre(const std::string& v);
    void setSuministrador(Laboratorio* lab);         // nombre claro
    void setServidoPor(Laboratorio* lab);            // alias del UML

    // --- Compatibilidad retro (snake_case) ---
    // Si tu código llama a estos nombres, seguirán funcionando.
    int                get_id_num() const            { return getIdNum(); }
    const std::string& get_id_alpha() const          { return getIdAlpha(); }
    const std::string& get_nombre() const            { return getNombre(); }
    Laboratorio*       get_suministrador() const     { return getSuministrador(); }
    void               set_id_num(int v)             { setIdNum(v); }
    void               set_id_alpha(const std::string& v){ setIdAlpha(v); }
    void               set_nombre(const std::string& v)  { setNombre(v); }
    void               suministrarMed(Laboratorio* lab)   { setSuministrador(lab); } // API usada en tu MediExpress

    // --- Operadores de comparación (por id_num) ---
    bool operator<(const PaMedicamento& other)  const;
    bool operator==(const PaMedicamento& other) const;
    bool operator>(const PaMedicamento& other)  const { return other < *this; }

    // --- Salida legible ---
    friend std::ostream& operator<<(std::ostream& os, const PaMedicamento& med);
};


#endif //PRACTICA1_PAMEDICAMENTO_H
