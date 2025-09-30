
#ifndef PRACTICA1_VDINAMICO_H
#define PRACTICA1_VDINAMICO_H
#include <stdexcept>
#include <algorithm>
using namespace std;

class PaMedicamento {
private:
    int IdNum;
    string IdAlpha;
    string nombre;

public:
    PaMedicamento() : IdNum(0), IdAlpha(""), nombre("") {}
    PaMedicamento(int id, const string& alpha, const string& nom)
        : IdNum(id), IdAlpha(alpha), nombre(nom) {}

    int getIdNum() const { return IdNum; }
    string getIdAlpha() const { return IdAlpha; }
    string getNombre() const { return nombre; }

    void setIdNum(int id) { IdNum = id; }
    void setIdAlpha(const string& alpha) { IdAlpha = alpha; }
    void setNombre(const string& nom) { nombre = nom; }

    bool operator<(const PaMedicamento& distinto) const { return IdNum < distinto.IdNum; }

};


template <typename T>
    class VDinamico {
    private:
        T* datos;
        unsigned int _tamfis; //Tamaño fisico del vector
        unsigned int _tamlog; //Tamaño lógico del vector (tamaño usado).

    public:
        /** @brief Constructor por defecto
         *  @param _tamfis = 1 ///> tamaño físico del vector
         *  @param _tamlog = 0 ///> tamaño lógico del vector
         */
    VDinamico() : _tamlog(0),_tamfis(1) {
        datos = new T[_tamfis];
    }

    /**
    *  @brief Constructor parametrizado dado un tam logico
    *  @param tamlog: tamaño logico que se le pasa al constructor
    *  */

    VDinamico(unsigned int tamlog) : _tamfis(1), _tamlog(tamlog) {
        while (_tamfis <= tamlog) {
            _tamfis *= 2;
        }
    }
    /**
    * @brief Constructor de copia
    * @param orig: objeto puntero utilizado para copiar los datos
    * */
    VDinamico(const VDinamico<T> &orig) : _tamlog(orig._tamlog), _tamfis(orig._tamfis) {
        datos = new T[orig._tamfis];
        for (int i=0; i < _tamlog; i++) {
            datos[i] = orig.datos[i];
        }
    }
    /**
    * Constructor de copia parcial
    * @param orig objeto para copiar datos
    * @param posInicial posición desde la que se quiere copiar un dato
    * @param numElem número de elementos que se quieren copiar
    * @throw excepción si el tamaño fisico no es potencia de 2
    * @throw excepción fuera de rango si se supera el tamaño fisico tamfis
    * */
    VDinamico(const VDinamico<T> &orig, unsigned int posInicial, unsigned int numElem) : _tamfis(orig._tamfis), _tamlog(orig._tamlog) {
        unsigned int n = 1;
        //Compruebo que tamaño fisico sea potencia de 2
        while (n < _tamfis) {
            n *= 2;
        }
        //lanza una excepcion si el tamaño físico no es potencia de 2
        if (_tamfis != n) {
            throw invalid_argument("[VDinamico<T>::VDinamico<T>]: El tam fisico de no es potencia de 2");
        }
        //Compruebo si se ha pasado el tamaño lógico
        if (numElem + posInicial > orig._tamlog) {
            throw out_of_range("[VDinamico<T>::VDinamico<T>]:Se ha superado el tam logico del vector");
        }
        //Una vez que se comprueba que el vector generado es potencia de 2, se puede copiar el tamaño
        _tamfis = orig._tamfis;
        //Creo un nuevo vector con el nuevo tamaño
        datos = new T[orig._tamfis];
        //Se copia el número de datos pasado
        for (int i=0; i < numElem && i < _tamlog; i++) {
            datos[i] = orig.datos[posInicial + i];
        }
    }
    /**
    * @brief Operador =
    * @param copia objeto que copia datos
    * @return devuelve un puntero al objeto
    * */

    VDinamico& operator= (const VDinamico<T> &copia)
    {
        if (this != &copia) {
            delete [] datos;
            _tamfis = copia._tamfis;
            datos = new T[_tamfis];
            for (int i=0; i < _tamfis; i++) {
                datos[i] = copia.datos[i];
            }
        }
        return *this;
    }
    /**
        * @brief Operador []
        * @param accedemos a traves de su indice
        * @return devuelve una referencia
        * */

    T& operator[](unsigned int pos){
        if (pos >= _tamlog) {
            throw out_of_range("Posicion no valida, ya que es >= que el tamlog");
        }
        return datos[pos];
    }
    /**
    * @brief Insertar
    * @param insertamos un dato en una posicion determinada
    * @return modifica el vector pero no devuelve un resultado
    * */

    void insertar(const T& dato, unsigned int pos=UINT_MAX) {
        if (_tamlog >= _tamfis) {
            unsigned int nuevoTamfisico = _tamfis * 2;
            T* NuevosDatos = new T[nuevoTamfisico];
            for (unsigned int i = 0; i < _tamlog; ++i) {
                NuevosDatos[i] = datos[i];
            }
            delete[] datos;
            datos = NuevosDatos;
            _tamfis = nuevoTamfisico;
        }
        if (pos == UINT_MAX) {     //inserta al final si se cumple
            datos[_tamlog++] = dato;
        } else {
            if (pos > _tamlog) {
                for (unsigned int i = _tamlog; i > pos; --i) {
                    datos[i] = datos[i - 1];
                }
                datos[pos] = dato;
                _tamlog++;
            }
        }
    }
            /**
             * @brief Borrar
             * @param eliminamor un dato en una posicion determinada
             * @return devuelve el elemento borrado
                * */

    T borrar(unsigned int pos=UINT_MAX) {
        if (pos == UINT_MAX) pos = _tamlog - 1;
        T aux = datos[pos];
        for (unsigned int i = pos; i < _tamlog - 1; ++i) {
            datos[i] = datos[i + 1];
        }
        _tamlog--;
        return aux;
    }
            /**
                 * @brief ordenar
                 * @param ordenamos los datos con la funcion sort de algorithm
                    * */
    void ordenar() {
        sort(datos, datos + _tamlog);
    }

/**
 * @brief tamlog
* @return devuelve el tamaño logico
* */
    unsigned int tamlog() {
        return _tamlog;
    }


    virtual ~VDinamico() {
        //otra manera -> if (datos != NULL)
        if (datos) {
            delete [] datos;
        }
    }


};
#endif //PRACTICA1_VDINAMICO_H