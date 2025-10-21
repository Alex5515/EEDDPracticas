#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>  // Para std::lower_bound, std::sort
#include <vector>     // Para el método burbuja
#include <map>        // Para contar palabras repetidas
#include <ctime>      // Para clock_t

#include "VDinamico.h"
#include "PaMedicamento.h"
#include "MediExpress.h"
/**
 * @author Alejandro Leyva San Juan als00075@red.ujaen.es
 * @author Fernando  fsc00016@red.ujaen.es
 */
// Función para búsqueda binaria en VDinamico
template <class T>
int busquedaBinaria(const VDinamico<T>& v, const T& dato) {
    int inf = 0;
    int sup = v.tamlog() - 1;
    while (inf <= sup) {
        int curIn = inf + (sup - inf) / 2; // Evita desbordamiento
        if (v[curIn] == dato) {
            return curIn;
        } else if (v[curIn] < dato) {
            inf = curIn + 1;
        } else {
            sup = curIn - 1;
        }
    }
    return -1; // No encontrado
}

// Función para búsqueda secuencial de compuesto
VDinamico<Pamedicamento*> buscarCompuesto(const std::string& comp, const VDinamico<Pamedicamento>& vMedicamentos) {
    VDinamico<Pamedicamento*> resultados;
    for (unsigned int i = 0; i < vMedicamentos.tamlog(); ++i) {
        // Convertir a minúsculas para búsqueda insensible a mayúsculas
        std::string nombre_lower = vMedicamentos[i].get_nombre();
        std::transform(nombre_lower.begin(), nombre_lower.end(), nombre_lower.begin(), ::tolower);
        std::string comp_lower = comp;
        std::transform(comp_lower.begin(), comp_lower.end(), comp_lower.begin(), ::tolower);

        if (nombre_lower.find(comp_lower) != std::string::npos) {
            // Se añade la dirección de memoria (puntero)
            resultados.insertar(new Pamedicamento(vMedicamentos[i]));
        }
    }
    return resultados;
}

// Función para ordenar por nombre con método burbuja
void ordenarBurbujaPorNombre(VDinamico<Pamedicamento>& v) {
    unsigned int n = v.tamlog();
    for (unsigned int i = 0; i < n - 1; ++i) {
        for (unsigned int j = 0; j < n - i - 1; ++j) {
            if (v[j].get_nombre() > v[j + 1].get_nombre()) {
                // Intercambiar elementos
                Pamedicamento temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }
}

int main(int argc, const char * argv[]) {
/**
    VDinamico<Pamedicamento> m;

    std::ifstream is;
    std::stringstream  columnas;
    std::string fila;
    int contador=0;

    std::string id_number_str;
    std::string id_alpha;
    std::string nombre;


    is.open("../pa_medicamentos.csv"); //carpeta de proyecto
    if ( is.good() ) {

        clock_t t_ini = clock();

        while ( getline(is, fila ) ) {

            //¿Se ha leído una nueva fila?
            if (fila!="") {

                columnas.str(fila);

                //formato de fila: id_number;id_alpha;nombre;

                getline(columnas, id_number_str, ';'); //leemos caracteres hasta encontrar y omitir ';'
                getline(columnas, id_alpha,';');
                getline(columnas, nombre,';');


                fila="";
                columnas.clear();

                // std::cout << ++contador
                //           << " Medicamento: ( Id_number=" << id_number_str
                //           << " id_alpha=" << id_alpha << " Nombre=" << nombre
                //           << ")" << std::endl;

                // Agregar medicamento al vector dinámico
                Pamedicamento medicamento(id_number_str, id_alpha, nombre);
                m.insertar(medicamento);
                contador++;
            }
        }

        is.close();

        std::cout << "Tiempo de lectura: " << ((clock() - t_ini) / (float) CLOCKS_PER_SEC) << " segs." << std::endl;
    } else {
        std::cout << "Error de apertura en archivo pa_medicamentos.csv" << std::endl;
        return 1; // Salir con error
    }

    // Mostrar los identificadores de los primeros 50 elementos del vector (tal como se leyeron)
    std::cout << "\n--- Identificadores de los primeros 50 medicamentos (orden de lectura) ---" << std::endl;
    try {
        for (unsigned int i = 0; i < std::min((unsigned int)50, m.tamlog()); i++) {
            std::cout << i << ": " << m[i].getIdnum() << std::endl;
        }
    }catch(std::out_of_range &o) {
        std::cout << "Error: " << o.what() << ". Al mostrar los datos de los 50 primeros medicamentos." << std::endl;
    }


    // Ordenar el vector de menor a mayor (por el id_num)
    m.ordenar();

    // Mostrar los ID de los primeros 50 medicamentos ordenados de menor a mayor
    std::cout << "\n--- ID de los primeros 50 medicamentos (ordenados de menor a mayor por id_num) ---" << std::endl;
    try {
        for (unsigned int i = 0; i < std::min((unsigned int)50, m.tamlog()); i++) {
            std::cout << i << ": " << m[i].getIdnum() << std::endl;
        }
    } catch (std::out_of_range &o) {
        std::cout << "Error: " << o.what() << ". Al mostrar los datos de los 50 primeros medicamentos ordenados de menor a Mayor." << std::endl;
    }

    // Búsqueda en O(log n) de medicamentos
    std::cout << "\n--- Busqueda de medicamentos por ID (O(log n)) ---" << std::endl;
    std::vector<std::string> ids_a_buscar = {"350", "409", "820", "9009", "12370"};
    for (const std::string& id_buscado : ids_a_buscar) {
        PaMedicamento temp_med;
        temp_med.setIdnum(id_buscado); // Solo necesitamos el id_num para la comparación

        int pos = busquedaBinaria(m, temp_med);
        if (pos != -1) {
            std::cout << "Medicamento con ID " << id_buscado << " encontrado en la posicion: " << pos << " " << m[pos] << std::endl;
        } else {
            std::cout << "Medicamento con ID " << id_buscado << " NO encontrado." << std::endl;
        }
    }

    // Búsqueda secuencial de medicamentos con "aceite"
    std::cout << "\n--- Busqueda secuencial de medicamentos con 'aceite' ---" << std::endl;
    std::string compuesto_buscado = "aceite";
    VDinamico<PaMedicamento*> medicamentos_con_aceite = buscarCompuesto(compuesto_buscado, m);

    if (medicamentos_con_aceite.tamlog() > 0) {
        std::cout << "Medicamentos que contienen '" << compuesto_buscado << "' en su nombre:" << std::endl;
        for (unsigned int i = 0; i < medicamentos_con_aceite.tamlog(); ++i) {
            std::cout << *medicamentos_con_aceite[i] << std::endl;
        }
    } else {
        std::cout << "No se encontraron medicamentos con '" << compuesto_buscado << "' en su nombre." << std::endl;
    }
    // Liberar memoria de los punteros en medicamentos_con_aceite
    for (unsigned int i = 0; i < medicamentos_con_aceite.tamlog(); ++i) {
        delete medicamentos_con_aceite[i];
    }
    // --- Funcionalidades para parejas ---
    // Ordenar el vector por el string "nombre" directamente en el código de prueba con el método burbuja
    std::cout << "\n--- Ordenando por nombre con metodo burbuja ---" << std::endl;
    ordenarBurbujaPorNombre(m);

    // Mostrar los primeros 50 elementos después de ordenar por nombre
    std::cout << "\n--- Primeros 50 medicamentos (ordenados por nombre con burbuja) ---" << std::endl;
    try {
        for (unsigned int i = 0; i < std::min((unsigned int)50, m.tamlog()); i++) {
            std::cout << i << ": " << m[i] << std::endl;
        }
    } catch (std::out_of_range &o) {
        std::cout << "Error: " << o.what() << ". Al mostrar los datos de los 50 primeros medicamentos ordenados por nombre." << std::endl;
    }
    // Averiguar cuántos medicamentos tienen la primera palabra de su nombre repetida al menos una vez.
    std::cout << "\n--- Conteo de primeras palabras de nombres repetidas ---" << std::endl;
    std::map<std::string, int> primera_palabra_conteo;
    for (unsigned int i = 0; i < m.tamlog(); ++i) {
        std::string nombre_completo = m[i].get_nombre();
        std::istringstream iss(nombre_completo);
        std::string primera_palabra;
        if (iss >> primera_palabra) {
            // Convertir a minúsculas para un conteo insensible a mayúsculas
            std::transform(primera_palabra.begin(), primera_palabra.end(), primera_palabra.begin(), ::tolower);
            primera_palabra_conteo[primera_palabra]++;
        }
    }
    int medicamentos_con_primera_palabra_repetida = 0;
    for (const auto& pair : primera_palabra_conteo) {
        if (pair.second > 1) {
            medicamentos_con_primera_palabra_repetida++;
            // Opcional: mostrar las palabras repetidas y su conteo
            // std::cout << "Palabra: '" << pair.first << "' - Repeticiones: " << pair.second << std::endl;
        }
    }
    std::cout << "Numero de medicamentos con la primera palabra de su nombre repetida al menos una vez: "
              << medicamentos_con_primera_palabra_repetida << std::endl;
**/

    std::string meds_file = "pa_medicamentos.csv";
    std::string labs_file = "lab2.csv";

    MediExpress MediEXp(meds_file, labs_file);

    MediEXp.BuscarGranada();
    MediEXp.BuscarJaen();
    MediEXp.ByMostrarMadrid();
    MediEXp.Aceites();
    MediEXp.AsignarMadrid();

    
    MediEXp.EliminarBruselas();

    return 0;
}