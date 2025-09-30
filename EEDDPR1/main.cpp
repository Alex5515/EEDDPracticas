#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "VDinamico.h"
using namespace std;
int main(int argc, const char * argv[]) {

    VDinamico<PaMedicamento> vMedicamentos;
    ifstream fichero("../pa_medicamentos.csv");

    string linea;
    while (getline(fichero, linea)) {
        stringstream ss(linea);
        string id_numero_string, id_alpha, nombre;

        getline(ss, id_numero_string, ';');
        getline(ss, id_alpha, ';');
        getline(ss, nombre, ';');

        PaMedicamento m(stoi(id_numero_string), id_alpha, nombre);
         vMedicamentos.insertar(m);
    }

    unsigned int MaximoElementos = 50;
    for (unsigned int i = 0; i < MaximoElementos; ++i) {
        auto& medaux = vMedicamentos[i];
        cout << "IdNum:" << medaux.getIdNum() << endl;
    }

    vMedicamentos.ordenar();

    for (unsigned int i = 0; i < MaximoElementos; ++i) {
        auto &medaux2 = vMedicamentos[i];
        cout << medaux2.getIdNum() << " " << medaux2.getIdAlpha()<< " " << medaux2.getNombre() << endl;
    }

    int id[]={350,409,820,9009,12370};
    for(int buscado : id){

        //inicializamos los elementos que vamos a usar para hacer la busqueda
        int inicio=0;
        int fin=vMedicamentos.tamlog() -1;
        int pos= -1;
        while (inicio <= fin){

            //definimos el punto medio
            int medio=(inicio + fin)/2;
            int actual=vMedicamentos[medio].getIdNum();
            
            //comprobamos si la posicion tiene el id que buscamos
            if(actual==buscado){
                pos=medio;
                break;
            }
            if(actual<buscado){
                inicio=medio+1;
            }else{
                fin=medio-1;
            }
        }
        if(pos != -1){
            cout<<buscado<<"-Se ha encontrado en la posicion:"<<pos<<endl;
        }else{
            cout<<buscado<<"-No se ha encontrado "<<endl;
        }
    }

        /**
            std::ifstream is;
            std::stringstream  columnas;
            std::string fila;
            int contador=0;

            std::string id_number = "";
            std::string id_alpha="";
            std::string nombre="";


            is.open("../pa_medicamentos.csv"); //carpeta de proyecto
            if ( is.good() ) {

                clock_t t_ini = clock();
                        while ( getline(is, fila ) ) {

                            //¿Se ha leído una nueva fila?
                            if (fila!="") {

                                columnas.str(fila);

                                //formato de fila: id_number;id_alpha;nombre;

                                getline(columnas, id_number, ';'); //leemos caracteres hasta encontrar y omitir ';'
                                getline(columnas, id_alpha,';');
                                getline(columnas, nombre,';');


                                fila="";
                                columnas.clear();


                    }

                            }
                        }

                is.close();

                std::cout << "Tiempo de lectura: " << ((clock() - t_ini) / (float) CLOCKS_PER_SEC) << " segs." << std::endl;
            } else {
                std::cout << "Error de apertura en archivo" << std::endl;
            }
            **/
    }