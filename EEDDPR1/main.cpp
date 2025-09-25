#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "VDinamico.h"
using namespace std;
int main(int argc, const char * argv[]) {

    VDinamico<PaMedicamento> vMedicamentos;
    std::ifstream fichero("pa_medicamentos.csv");

    std::string linea;
    while (std::getline(fichero, linea)) {
        std::stringstream ss(linea);
        std::string id_numero_string, id_alpha, nombre;

        getline(ss, id_numero_string, ';');
        getline(ss, id_alpha, ';');
        getline(ss, nombre, ';');

        PaMedicamento m(stoi(id_numero_string), id_alpha, nombre);
        vMedicamentos.insertar(m);
    }

    unsigned int MaximoElementos = 50;
    for (unsigned int i = 0; i < MaximoElementos; ++i) {
        auto& medaux = vMedicamentos[i];
        std::cout << "IdNumber: " << medaux.getIdNum() << std::endl;
    }

    vMedicamentos.ordenar();
    for (unsigned int i = 0; i < MaximoElementos; ++i) {
        auto &medaux2 = vMedicamentos[i];
        std::cout << medaux2.getIdNum() << " " << medaux2.getIdAlpha()<< " " << medaux2.getNombre() << std::endl;
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