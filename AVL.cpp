/**
 * @file AVL.cpp
 * @brief Implementación de la clase plantilla AVL<T>.
 */

#ifndef AVL_CPP
#define AVL_CPP

#include "AVL.h"
#include <stdexcept>

/* --------------------------------------------------------- */
/* ------------------- Constructores ----------------------- */
/* --------------------------------------------------------- */

template <typename T>
AVL<T>::AVL() : raiz(nullptr), numNodos(0) {}

template <typename T>
AVL<T>::AVL(const AVL<T>& origen) : raiz(nullptr), numNodos(0) {
    raiz = copiarPreorden(origen.raiz);
    numNodos = origen.numNodos;
}

template <typename T>
AVL<T>::~AVL() {
    destruir(raiz);
    raiz = nullptr;
    numNodos = 0;
}

/* --------------------------------------------------------- */
/* -------------------- Operador = -------------------------- */
/* --------------------------------------------------------- */

template <typename T>
AVL<T>& AVL<T>::operator=(const AVL<T>& origen) {
    if (this != &origen) {
        destruir(raiz);
        raiz = copiarPreorden(origen.raiz);
        numNodos = origen.numNodos;
    }
    return *this;
}

/* --------------------------------------------------------- */
/* -------------------- Inserción AVL ----------------------- */
/* --------------------------------------------------------- */

template <typename T>
bool AVL<T>::inserta(const T& dato) {
    int antes = numNodos;
    insertaRec(raiz, dato);
    return numNodos > antes;
}

/**
 * @brief Inserta un dato recursivamente y equilibra el árbol.
 * @param c Nodo actual (por referencia).
 * @param dato Elemento a insertar.
 * @return Variación de altura (1 si sube, 0 si no cambia).
 */
template <typename T>
int AVL<T>::insertaRec(Nodo<T>*& c, const T& dato) {
    int deltaH = 0;
    if (!c) {
        c = new Nodo<T>(dato);
        numNodos++;
        deltaH = 1;
    } else if (dato < c->dato) {
        if (insertaRec(c->izq, dato)) {
            c->bal++;
            if (c->bal == 1) deltaH = 1;
            else if (c->bal == 2) {
                if (c->izq->bal == -1) rotIzqda(c->izq); // Rotación doble (caso 2)
                rotDecha(c); // Caso 1
            }
        }
    } else if (dato > c->dato) {
        if (insertaRec(c->der, dato)) {
            c->bal--;
            if (c->bal == -1) deltaH = 1;
            else if (c->bal == -2) {
                if (c->der->bal == 1) rotDecha(c->der); // Rotación doble (caso 3)
                rotIzqda(c); // Caso 4
            }
        }
    } else {
        // Dato duplicado, no se inserta
        deltaH = 0;
    }
    return deltaH;
}

/* --------------------------------------------------------- */
/* --------------------- Rotaciones ------------------------- */
/* --------------------------------------------------------- */

template <typename T>
void AVL<T>::rotDecha(Nodo<T>*& p) {
    Nodo<T>* q = p;
    Nodo<T>* l = q->izq;
    p = l;
    q->izq = l->der;
    l->der = q;

    q->bal--;
    if (l->bal > 0) q->bal -= l->bal;
    l->bal--;
    if (q->bal < 0) l->bal -= -q->bal;
}

template <typename T>
void AVL<T>::rotIzqda(Nodo<T>*& p) {
    Nodo<T>* q = p;
    Nodo<T>* r = q->der;
    p = r;
    q->der = r->izq;
    r->izq = q;

    q->bal++;
    if (r->bal < 0) q->bal += -r->bal;
    r->bal++;
    if (q->bal > 0) r->bal += q->bal;
}

/* --------------------------------------------------------- */
/* -------------------- Búsquedas --------------------------- */
/* --------------------------------------------------------- */

template <typename T>
T* AVL<T>::buscaRec(const T& dato) const {
    return buscaRecAux(raiz, dato);
}

template <typename T>
T* AVL<T>::buscaRecAux(Nodo<T>* nodo, const T& dato) const {
    if (!nodo) return nullptr;
    if (dato == nodo->dato) return &(nodo->dato);
    if (dato < nodo->dato) return buscaRecAux(nodo->izq, dato);
    return buscaRecAux(nodo->der, dato);
}

template <typename T>
T* AVL<T>::buscaIt(const T& dato) const {
    return buscaItAux(raiz, dato);
}

template <typename T>
T* AVL<T>::buscaItAux(Nodo<T>* nodo, const T& dato) const {
    while (nodo) {
        if (dato == nodo->dato) return &(nodo->dato);
        nodo = (dato < nodo->dato) ? nodo->izq : nodo->der;
    }
    return nullptr;
}

/* --------------------------------------------------------- */
/* ------------------- Recorrido Inorden -------------------- */
/* --------------------------------------------------------- */

template <typename T>
VDinamico<T*> AVL<T>::recorreInorden() const {
    VDinamico<T*> resultado;
    recorreInordenAux(raiz, resultado);
    return resultado;
}

template <typename T>
void AVL<T>::recorreInordenAux(Nodo<T>* nodo, VDinamico<T*>& v) const {
    if (!nodo) return;
    recorreInordenAux(nodo->izq, v);
    v.insertar(&(nodo->dato));
    recorreInordenAux(nodo->der, v);
}

/* --------------------------------------------------------- */
/* -------------------- Métricas AVL ------------------------ */
/* --------------------------------------------------------- */

template <typename T>
unsigned int AVL<T>::numElementos() const {
    return numNodos;
}

template <typename T>
unsigned int AVL<T>::altura() const {
    return alturaRec(raiz);
}

template <typename T>
unsigned int AVL<T>::alturaRec(Nodo<T>* nodo) const {
    if (!nodo) return 0;
    unsigned int izqH = alturaRec(nodo->izq);
    unsigned int derH = alturaRec(nodo->der);
    return 1 + (izqH > derH ? izqH : derH);
}

/* --------------------------------------------------------- */
/* ------------------ Utilidades internas ------------------- */
/* --------------------------------------------------------- */

template <typename T>
void AVL<T>::destruir(Nodo<T>* nodo) {
    if (nodo) {
        destruir(nodo->izq);
        destruir(nodo->der);
        delete nodo;
    }
}

template <typename T>
Nodo<T>* AVL<T>::copiarPreorden(const Nodo<T>* nodoOrigen) {
    if (!nodoOrigen) return nullptr;
    Nodo<T>* nuevo = new Nodo<T>(nodoOrigen->dato);
    nuevo->bal = nodoOrigen->bal;
    nuevo->izq = copiarPreorden(nodoOrigen->izq);
    nuevo->der = copiarPreorden(nodoOrigen->der);
    return nuevo;
}

#endif
