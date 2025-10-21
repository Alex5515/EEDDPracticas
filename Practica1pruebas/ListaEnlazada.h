#ifndef PRACTICA2_LISTAENLAZADA_H
#define PRACTICA2_LISTAENLAZADA_H

#include <iostream>
#include <stdexcept>

template <class T>
class ListaEnlazada {
private:

    struct Nodo {
        T dato;
        Nodo* sig;
        Nodo(const T& d, Nodo* s = nullptr) : dato(d), sig(s) {}
    };

    Nodo* _inicio; // Puntero al nodo inicio
    Nodo* _fin;    // Puntero al nodo final
    int _tam;      // Contador para el tamaño

    // Funcion para liberar memoria
    void libera() {
        Nodo* actual = _inicio;
        while (actual != nullptr) {
            Nodo* a_borrar = actual;
            actual = actual->sig;
            delete a_borrar;
        }
        _inicio = nullptr;
        _fin = nullptr;
        _tam = 0;
    }

    // Función para copiar otra lista
    void copia(const ListaEnlazada<T>& origen) {
        _inicio = nullptr;
        _fin = nullptr;
        _tam = 0;

        Nodo* actual_origen = origen._inicio;
        while (actual_origen != nullptr) {
            // Usamos el dato por const referencia ya que solo estamos leyendo de origen
            insertarFinal(actual_origen->dato);
            actual_origen = actual_origen->sig;
        }
    }

public:
    class Iterador {
    private:
        Nodo* actual;
        ListaEnlazada* lista_referencia;

        //muy importante el friend visto en clase
        friend class ListaEnlazada;

    public:
        Iterador(Nodo* a = nullptr, ListaEnlazada* l = nullptr) : actual(a), lista_referencia(l) {}

        // Operador de incremento
        Iterador& operator++() {
            if (actual == nullptr) {
                //lanzamos una excepcion si llegamos a una posicion no valida
                if (actual == nullptr) {
                     throw std::out_of_range("Intento de avanzar en un iterador nulo o al final.");
                }
            }
            actual = actual->sig;
            return *this;
        }

        Iterador operator++(int) {
            Iterador temp = *this;
            ++(*this);
            return temp;
        }

        T& operator*() {
            if (actual == nullptr) {
                throw std::out_of_range("Intento de quitar la referencia de un iterador nulo");
            }
            return actual->dato;
        }

        const T& operator*() const {
            if (actual == nullptr) {
                throw std::out_of_range("Intento de quitar la referencia de un iterador nulo ");
            }
            return actual->dato;
        }

        bool operator==(const Iterador& other) const {
            return actual == other.actual;
        }

        bool operator!=(const Iterador& other) const {
            return actual != other.actual;
        }


    };

    // Constructor por defecto
    ListaEnlazada() : _inicio(nullptr), _fin(nullptr), _tam(0) {}

    // Destructor
    ~ListaEnlazada() {
        libera();
    }

    // Constructor copia
    ListaEnlazada(const ListaEnlazada<T>& origen) {
        copia(origen);
    }

    // Operador de asignacion
    ListaEnlazada<T>& operator=(const ListaEnlazada<T>& origen) {
        if (this != &origen) {
            libera();
            copia(origen);
        }
        return *this;
    }

    Iterador iterador() {
        return Iterador(_inicio, this);
    }

    Iterador end() {
        return Iterador(nullptr, this);
    }

    T& inicio() {
        if (vacia()) {
            throw std::out_of_range("La lista está vacía.");
        }
        return _inicio->dato;
    }

    //Obtenemos el final
    T& fin() {
        if (vacia()) {
            throw std::out_of_range("La lista está vacía.");
        }
        return _fin->dato;
    }

    //Insertamos al principio
    void insertarInicio(T& dato) {
        Nodo* nuevo = new Nodo(dato, _inicio);
        _inicio = nuevo;
        if (_fin == nullptr) { // Si la lista estaba vacía
            _fin = nuevo;
        }
        _tam++;
    }

    //Insertamos al final
    void insertarFinal(const T& dato) {
        Nodo* nuevo = new Nodo(dato);
        if (vacia()) {
            _inicio = nuevo;
            _fin = nuevo;
        } else {
            _fin->sig = nuevo;
            _fin = nuevo;
        }
        _tam++;
    }

    //Insertamos un dato en la posición anterior
    void insertaDelante(Iterador& i, T& dato) {
        if (i.actual == nullptr) {
            throw std::invalid_argument("El iterador no es válido o apunta al final.");
        }

        if (i.actual == _inicio) {
            insertarInicio(dato);
        } else {
            Nodo* anterior = _inicio;
            while (anterior != nullptr && anterior->sig != i.actual) {
                anterior = anterior->sig;
            }
            if (anterior == nullptr) {
                throw std::out_of_range("El iterador no pertenece a esta lista o es invalido");
            }
            Nodo* nuevo = new Nodo(dato, i.actual);
            anterior->sig = nuevo;
            _tam++;
        }
    }

    //Insertamos un dato en la posición siguiente
    void insertaDetras(Iterador& i, T& dato) {
        if (i.actual == nullptr) {
            throw std::invalid_argument("El iterador no es válido o apunta al final.");
        }

        Nodo* nuevo = new Nodo(dato, i.actual->sig);
        i.actual->sig = nuevo;
        _tam++;

        if (i.actual == _fin) {
            _fin = nuevo;
        }
    }

    // Borramos el elemento situado en el comienzo
    void borrarInicio() {
        if (vacia()) {
            throw std::out_of_range("No se puede borrar: La lista está vacía.");
        }
        Nodo* a_borrar = _inicio;
        _inicio = _inicio->sig;
        delete a_borrar;
        _tam--;
        if (_inicio == nullptr) { // Si se borró el único elemento
            _fin = nullptr;
        }
    }

    // Borramos el elemento situado en el final
    void borrarFinal() {
        if (vacia()) {
            throw std::out_of_range("No se puede borrar: La lista está vacía.");
        }

        if (_inicio == _fin) { // si solo hay uno
            delete _inicio;
            _inicio = nullptr;
            _fin = nullptr;
        } else {
            Nodo* anterior = _inicio;
            while (anterior->sig != _fin) {
                anterior = anterior->sig;
            }
            delete _fin;
            _fin = anterior;
            _fin->sig = nullptr;
        }
        _tam--;
    }

    // Borramos el elemento
    void borra(Iterador& i) {
        if (i.actual == nullptr) {
            throw std::invalid_argument("El iterador no es válido o ya fue borrado.");
        }

        if (i.actual == _inicio) {
            borrarInicio();
            i.actual = nullptr;
        } else {
            // Buscamos el nodo anterior
            Nodo* anterior = _inicio;
            while (anterior != nullptr && anterior->sig != i.actual) {
                anterior = anterior->sig;
            }

            if (anterior == nullptr) {
                throw std::out_of_range("El iterador no pertenece a la lista o es invalido");
            }

            Nodo* a_borrar = i.actual;
            anterior->sig = a_borrar->sig;

            if (a_borrar == _fin) {
                _fin = anterior;
            }

            delete a_borrar;
            _tam--;
            i.actual = nullptr;
        }
    }

    //Devolvemos el número de elementos
    int tam() const {
        return _tam;
    }

    // Comprobamos si la lista está vacía
    bool vacia() const {
        return _inicio == nullptr;
    }

    ListaEnlazada<T> concatena(const ListaEnlazada<T>& l) const {
        ListaEnlazada<T> resultado = *this;
        Nodo* actual_l = l._inicio;
        while (actual_l != nullptr) {
            resultado.insertarFinal(const_cast<T&>(actual_l->dato));
            actual_l = actual_l->sig;
        }
        return resultado;
    }

    ListaEnlazada<T> operator+(const ListaEnlazada<T>& l) const {
        return concatena(l);
    }

    friend std::ostream& operator<<(std::ostream& os, const ListaEnlazada<T>& l) {
        os << "[ ";
        Nodo* actual = l._inicio;
        while (actual != nullptr) {
            os << actual->dato;
            if (actual->sig != nullptr) {
                os << ", ";
            }
            actual = actual->sig;
        }
        os << " ]";
        return os;
    }
};

#endif