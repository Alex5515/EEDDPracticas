#ifndef PRACTICA1_AVL_H
#define PRACTICA1_AVL_H
/**
 * @file AVL.h
 * @brief Declaración de la clase plantilla AVL<T> implementando un árbol equilibrado AVL.
 *
 * Implementa la funcionalidad completa descrita en la Lección 11 y la Práctica 3:
 *  - Inserción con balanceo (rotaciones simples y dobles)
 *  - Búsqueda recursiva e iterativa
 *  - Recorrido inorden que devuelve VDinamico<T*>
 *  - Constructor copia (preorden), destructor (postorden)
 *  - Operador de asignación
 *  - Cálculo de altura y número de elementos
 *
 * @author Fernando Salas
 * @date 2025
 */



#include <iostream>
#include "VDinamico.h"

/// @brief Nodo de un árbol AVL.
template <typename T>
class Nodo {
public:
    T dato;                  ///< Dato almacenado en el nodo
    Nodo<T>* izq;            ///< Puntero al hijo izquierdo
    Nodo<T>* der;            ///< Puntero al hijo derecho
    char bal;                ///< Factor de equilibrio (-1, 0, 1)

    /**
     * @brief Constructor del nodo.
     * @param ele Elemento a insertar en el nodo.
     */
    explicit Nodo(const T& ele) : dato(ele), izq(nullptr), der(nullptr), bal(0) {}
};

/// @brief Clase plantilla que representa un árbol equilibrado AVL.
template <typename T>
class AVL {
private:
    Nodo<T>* raiz; ///< Puntero a la raíz del árbol
    unsigned int numNodos; ///< Número de elementos del árbol

    // Métodos auxiliares privados
    void destruir(Nodo<T>* nodo);
    Nodo<T>* copiarPreorden(const Nodo<T>* nodoOrigen);
    unsigned int contarNodos(Nodo<T>* nodo) const;
    unsigned int alturaRec(Nodo<T>* nodo) const;
    void recorreInordenAux(Nodo<T>* nodo, VDinamico<T*>& v) const;

    // Operaciones básicas AVL
    void rotDecha(Nodo<T>*& p);
    void rotIzqda(Nodo<T>*& p);
    int insertaRec(Nodo<T>*& c, const T& dato);

    // Búsquedas auxiliares
    T* buscaRecAux(Nodo<T>* nodo, const T& dato) const;
    T* buscaItAux(Nodo<T>* nodo, const T& dato) const;

public:
    // Constructores y destructor
    AVL();
    AVL(const AVL<T>& origen);
    ~AVL();

    // Operador de asignación
    AVL<T>& operator=(const AVL<T>& origen);

    // Operaciones públicas del árbol
    bool inserta(const T& dato);
    T* buscaRec(const T& dato) const;
    T* buscaIt(const T& dato) const;
    VDinamico<T*> recorreInorden() const;
    unsigned int numElementos() const;
    unsigned int altura() const;

    /// @brief Comprueba si el árbol está vacío.
    bool vacio() const { return raiz == nullptr; }
};

#include "AVL.cpp" // Inclusión del cpp para plantillas


#endif //PRACTICA1_AVL_H
