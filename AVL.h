#ifndef PRACTICA1_AVL_H
#define PRACTICA1_AVL_H

#include <stdexcept>
#include "VDinamico.h"

// Árbol AVL genérico con excepciones y utilidades pedidas en el enunciado.
// NOTA: La implementación está en AVL.cpp y se instancia explícitamente para Farmacia.

template <class T>
class AVL {
private:
    struct Nodo {
        T dato;
        Nodo* izq;
        Nodo* der;
        int altura;
        explicit Nodo(const T& d) : dato(d), izq(nullptr), der(nullptr), altura(1) {}
    };

    Nodo* raiz = nullptr;
    unsigned int nElems = 0;

    // Utilidades internas
    static int  h(Nodo* n);
    static int  balance(Nodo* n);
    static void actAltura(Nodo* n);

    static void rotDer(Nodo*& n);
    static void rotIzq(Nodo*& n);

    static bool insertaRec(Nodo*& n, const T& dato);
    static void equilibrar(Nodo*& n);

    static T*  buscaRecImpl(Nodo* n, const T& dato);
    static T*  buscaItImpl(Nodo* n, const T& dato);

    // *** const-correctness: acepta puntero a Nodo const ***
    static void inorden(const Nodo* n, VDinamico<T*>& acc);

    static Nodo* copiaPreorden(Nodo* o);
    static void  liberaPostorden(Nodo* n);

public:
    // Constructores / asignación / destructor
    AVL() = default;
    AVL(const AVL<T>& origen);
    AVL<T>& operator=(const AVL<T>& origen);
    ~AVL();

    // Rotaciones públicas (si se quisieran probar externamente)
    void rotDerPublic(Nodo*& nodo) { rotDer(nodo); }
    void rotIzqPublic(Nodo*& nodo) { rotIzq(nodo); }

    // Operaciones del enunciado
    bool inserta(T& dato);
    T*   buscaRec(T& dato);
    T*   buscaIt(T& dato);

    // *** const-correctness: ahora es const ***
    VDinamico<T*> recorreInorden() const;

    unsigned int numElementos() const { return nElems; }
    unsigned int altura() const { return h(raiz); }
};


#endif //PRACTICA1_AVL_H
