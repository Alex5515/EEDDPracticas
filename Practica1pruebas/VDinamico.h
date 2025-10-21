#ifndef PRACTICA1_VDINAMICO_H
#define PRACTICA1_VDINAMICO_H


#include <algorithm> // Para std::sort
#include <stdexcept> // Para std::out_of_range, std::invalid_argument
#include <climits>   // Para UINT_MAX

template <class T>
class VDinamico {
private:
    T* v;
    unsigned int tamal; // Tamaño lógico
    unsigned int tamaf; // Tamaño físico

    void redimensionar(unsigned int nuevo_tamaf) {
        T* nuevo_v = new T[nuevo_tamaf];
        for (unsigned int i = 0; i < tamal; ++i) {
            nuevo_v[i] = v[i];
        }
        delete[] v;
        v = nuevo_v;
        tamaf = nuevo_tamaf;
    }

public:
    // Constructor por defecto
    VDinamico() : v(new T[1]), tamal(0), tamaf(1) {}

    // Constructor dando un tamaño lógico inicial
    VDinamico(unsigned int tamlog) : tamal(tamlog) {
        if (tamlog == 0) {
            tamaf = 1;
        } else {
            tamaf = 1;
            while (tamaf < tamlog) {
                tamaf *= 2;
            }
        }
        v = new T[tamaf];
    }

    // Constructor copia
    VDinamico(const VDinamico<T>& origen) : tamal(origen.tamal), tamaf(origen.tamaf) {
        v = new T[tamaf];
        for (unsigned int i = 0; i < tamal; ++i) {
            v[i] = origen.v[i];
        }
    }

    // Constructor de copia parcial
    VDinamico(const VDinamico<T>& origen, unsigned int posicionInicial, unsigned int numElementos) {
        if (posicionInicial >= origen.tamal || posicionInicial + numElementos > origen.tamal) {
            throw std::out_of_range("Rango de copia parcial inválido.");
        }

        tamal = numElementos;
        if (tamal == 0) {
            tamaf = 1;
        } else {
            tamaf = 1;
            while (tamaf < tamal) {
                tamaf *= 2;
            }
        }
        v = new T[tamaf];
        for (unsigned int i = 0; i < numElementos; ++i) {
            v[i] = origen.v[posicionInicial + i];
        }
    }

    // Operador de asignación (=)
    VDinamico<T>& operator=(const VDinamico<T>& origen) {
        if (this != &origen) {
            delete[] v;
            tamal = origen.tamal;
            tamaf = origen.tamaf;
            v = new T[tamaf];
            for (unsigned int i = 0; i < tamal; ++i) {
                v[i] = origen.v[i];
            }
        }
        return *this;
    }

    // Operador [] para acceder a un dato para lectura/escritura
    T& operator[](unsigned int pos) {
        if (pos >= tamal) {
            throw std::out_of_range("Acceso fuera de rango.");
        }
        return v[pos];
    }

    const T& operator[](unsigned int pos) const {
        if (pos >= tamal) {
            throw std::out_of_range("Acceso fuera de rango.");
        }
        return v[pos];
    }

    // Insertar un dato en una posición
    void insertar(const T& dato, unsigned int pos = UINT_MAX) {
        if (tamal == tamaf) {
            redimensionar(tamaf * 2);
        }

        if (pos == UINT_MAX || pos >= tamal) { // Insertar al final
            v[tamal] = dato;
        } else { // Insertar en posición intermedia
            for (unsigned int i = tamal; i > pos; --i) {
                v[i] = v[i - 1];
            }
            v[pos] = dato;
        }
        tamal++;
    }

    // Eliminar un dato de una posición intermedia
    T borrar(unsigned int pos = UINT_MAX) {
        if (tamal == 0) {
            throw std::out_of_range("El vector está vacío.");
        }

        if (pos == UINT_MAX || pos >= tamal) { // Eliminar el último
            tamal--;
        } else { // Eliminar en posición intermedia
            for (unsigned int i = pos; i < tamal - 1; ++i) {
                v[i] = v[i + 1];
            }
            tamal--;
        }

        if (tamal * 3 < tamaf && tamaf > 1) { // Reducir tamaño físico si es necesario
            redimensionar(tamaf / 2);
        }
        return v[tamal]; // Retorna el elemento que estaba en la posición eliminada (ahora el último)
    }

    // Ordenar el vector de menor a mayor
    void ordenar() {
        std::sort(v, v + tamal);
    }

    // Ordenar el vector de mayor a menor (para el requisito inicial)
    void ordenarRev() {
        std::sort(v, v + tamal, [](const T& a, const T& b) {
            return !(a < b); // Usar el operador < de T para ordenar de forma inversa
        });
    }

    // Obtener el tamaño lógico
    unsigned int tamlog() const {
        return tamal;
    }

    // Destructor
    ~VDinamico() {
        delete[] v;
    }
};

#endif // PRACTICA1_VDINAMICO_H
