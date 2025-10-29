#include "AVL.h"
#include <algorithm>
#include <stdexcept>

// **************************Implementación plantilla**************************

template <class T>
int AVL<T>::h(Nodo* n) { return n ? n->altura : 0; }

template <class T>
int AVL<T>::balance(Nodo* n) { return n ? h(n->izq) - h(n->der) : 0; }

template <class T>
void AVL<T>::actAltura(Nodo* n) {
    if (n) n->altura = 1 + std::max(h(n->izq), h(n->der));
}

template <class T>
void AVL<T>::rotDer(Nodo*& n) {
    if (!n || !n->izq) throw std::invalid_argument("rotDer sobre nodo invalido");
    Nodo* x = n->izq;
    n->izq = x->der;
    x->der = n;
    actAltura(n);
    actAltura(x);
    n = x;
}

template <class T>
void AVL<T>::rotIzq(Nodo*& n) {
    if (!n || !n->der) throw std::invalid_argument("rotIzq sobre nodo invalido");
    Nodo* y = n->der;
    n->der = y->izq;
    y->izq = n;
    actAltura(n);
    actAltura(y);
    n = y;
}

template <class T>
bool AVL<T>::insertaRec(Nodo*& n, const T& dato) {
    if (!n) { n = new Nodo(dato); return true; }

    if (dato < n->dato) {
        if (!insertaRec(n->izq, dato)) return false;
    } else if (n->dato < dato) {
        if (!insertaRec(n->der, dato)) return false;
    } else {
        // duplicado -> no insertamos
        return false;
    }
    equilibrar(n);
    return true;
}

template <class T>
void AVL<T>::equilibrar(Nodo*& n) {
    actAltura(n);
    int bf = balance(n);
    if (bf > 1) {
        if (balance(n->izq) < 0) rotIzq(n->izq); // IZQ-DER
        rotDer(n);
    } else if (bf < -1) {
        if (balance(n->der) > 0) rotDer(n->der); // DER-IZQ
        rotIzq(n);
    }
}

template <class T>
T* AVL<T>::buscaRecImpl(Nodo* n, const T& dato) {
    if (!n) return nullptr;
    if (dato < n->dato)       return buscaRecImpl(n->izq, dato);
    if (n->dato < dato)       return buscaRecImpl(n->der, dato);
    return &(n->dato);
}

template <class T>
T* AVL<T>::buscaItImpl(Nodo* n, const T& dato) {
    while (n) {
        if (dato < n->dato)       n = n->izq;
        else if (n->dato < dato)  n = n->der;
        else return &(n->dato);
    }
    return nullptr;
}

// Nodo es const; devolvemos T* con const_cast para mantener la firma
template <class T>
void AVL<T>::inorden(const Nodo* n, VDinamico<T*>& acc) {
    if (!n) return;
    inorden(n->izq, acc);
    acc.insertar(const_cast<T*>(&(n->dato)));
    inorden(n->der, acc);
}

template <class T>
typename AVL<T>::Nodo* AVL<T>::copiaPreorden(Nodo* o) {
    if (!o) return nullptr;
    Nodo* n = new Nodo(o->dato);
    n->izq = copiaPreorden(o->izq);
    n->der = copiaPreorden(o->der);
    actAltura(n);
    return n;
}

template <class T>
void AVL<T>::liberaPostorden(Nodo* n) {
    if (!n) return;
    liberaPostorden(n->izq);
    liberaPostorden(n->der);
    delete n;
}

// ************ API pública ************
template <class T>
AVL<T>::AVL(const AVL<T>& origen) {
    raiz   = copiaPreorden(origen.raiz);
    nElems = origen.nElems;
}

template <class T>
AVL<T>& AVL<T>::operator=(const AVL<T>& origen) {
    if (this != &origen) {
        liberaPostorden(raiz);
        raiz   = copiaPreorden(origen.raiz);
        nElems = origen.nElems;
    }
    return *this;
}

template <class T>
AVL<T>::~AVL() {
    liberaPostorden(raiz);
}

template <class T>
bool AVL<T>::inserta(T& dato) {
    bool ok = insertaRec(raiz, dato);
    if (ok) ++nElems;
    return ok;
}

template <class T>
T* AVL<T>::buscaRec(T& dato) {
    return buscaRecImpl(raiz, dato);
}

template <class T>
T* AVL<T>::buscaIt(T& dato) {
    return buscaItImpl(raiz, dato);
}

// ahora es const
template <class T>
VDinamico<T*> AVL<T>::recorreInorden() const {
    VDinamico<T*> v;
    inorden(raiz, v);
    return v;
}

// =================== Instanciación explícita necesaria ===================
// debemos instanciar los tipos usados.
#include "Farmacia.h"
template class AVL<Farmacia>;
