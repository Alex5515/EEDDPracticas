#ifndef PRACTICA2_LISTAENLAZADA_H
#define PRACTICA2_LISTAENLAZADA_H

#include <stdexcept>
#include <iostream>

template<class T>
class ListaEnlazada {
private:
    struct Nodo {
        T dato;
        Nodo* sig;
        Nodo(const T& d, Nodo* s = nullptr) : dato(d), sig(s) {}
    };

    Nodo* _inicio;
    Nodo* _fin;
    int _tam;

    void libera() {
        Nodo* actual = _inicio;
        while (actual != nullptr) {
            Nodo* a_borrar = actual;
            actual = actual->sig;
            delete a_borrar;
        }
        _inicio = _fin = nullptr;
        _tam = 0;
    }

public:
    class Iterador {
    private:
        Nodo* actual;
        ListaEnlazada* lista_referencia;
        friend class ListaEnlazada;
    public:
        Iterador(Nodo* a = nullptr, ListaEnlazada* l = nullptr) : actual(a), lista_referencia(l) {}

        Iterador& operator++() {
            if (actual == nullptr) throw std::out_of_range("Intento de avanzar en un iterador nulo o al final.");
            actual = actual->sig;
            return *this;
        }

        T& operator*() {
            if (actual == nullptr) throw std::out_of_range("Intento de quitar la referencia de un iterador nulo");
            return actual->dato;
        }

        const T& operator*() const {
             if (actual == nullptr) throw std::out_of_range("Intento de quitar la referencia de un iterador nulo");
            return actual->dato;
        }

        bool operator!=(const Iterador& other) const {
            return actual != other.actual;
        }
    };

    ListaEnlazada() : _inicio(nullptr), _fin(nullptr), _tam(0) {}

    ~ListaEnlazada() {
        libera();
    }

    ListaEnlazada(const ListaEnlazada& origen) : _inicio(nullptr), _fin(nullptr), _tam(0) {
        if (origen._inicio == nullptr) {
            return;
        }

        _inicio = new Nodo(origen._inicio->dato);
        _fin = _inicio;
        _tam = 1;

        Nodo* actual_origen = origen._inicio->sig;
        while (actual_origen != nullptr) {
            _fin->sig = new Nodo(actual_origen->dato);
            _fin = _fin->sig;
            actual_origen = actual_origen->sig;
            _tam++;
        }
    }


    ListaEnlazada& operator=(const ListaEnlazada& origen) {
        if (this != &origen) {
            libera();

            if (origen._inicio != nullptr) {
                _inicio = new Nodo(origen._inicio->dato);
                _fin = _inicio;
                _tam = 1;

                Nodo *actual_origen = origen._inicio->sig;
                while (actual_origen != nullptr) {
                    _fin->sig = new Nodo(actual_origen->dato);
                    _fin = _fin->sig;
                    actual_origen = actual_origen->sig;
                    _tam++;
                }
            }
        }
        return *this;
    }

    Iterador iterador() { return Iterador(_inicio, this); }
    Iterador end() { return Iterador(nullptr, this); }

    void insertarFinal(const T& dato) {
        Nodo* nuevo = new Nodo(dato);
        if (vacia()) {
            _inicio = _fin = nuevo;
        } else {
            _fin->sig = nuevo;
            _fin = nuevo;
        }
        _tam++;
    }

    Iterador borra(Iterador& i) {
        if (i.actual == nullptr) {
            throw std::invalid_argument("El iterador no es válido o ya fue borrado.");
        }

        Nodo* a_borrar = i.actual;
        Iterador siguiente_valido(a_borrar->sig, this);

        if (a_borrar == _inicio) {
            _inicio = _inicio->sig;
            if (_tam == 1) _fin = nullptr;
        } else {
            Nodo* anterior = _inicio;
            while (anterior->sig != a_borrar) {
                anterior = anterior->sig;
            }
            anterior->sig = a_borrar->sig;
            if (a_borrar == _fin) {
                _fin = anterior;
            }
        }

        delete a_borrar;
        _tam--;
        i.actual = nullptr; // Invalidamos el iterador pasado por referencia
        return siguiente_valido; // Devolvemos un iterador al siguiente nodo válido
    }

    int tam() const { return _tam; }
    bool vacia() const { return _inicio == nullptr; }
};

#endif //PRACTICA2_LISTAENLAZADA_H
