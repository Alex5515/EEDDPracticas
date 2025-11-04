#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <unordered_set>

#include "MediExpress.h"
#include "Farmacia.h"
#include "PaMedicamento.h"
#include "Laboratorio.h"
#include "VDinamico.h"

// --------------------- Utilidades ---------------------
// Split por ';' respetando campos entrecomillados
static std::vector<std::string> split_semicolon_quoted(const std::string &line) {
    std::vector<std::string> out;
    std::string cur;
    bool in_quotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '\"') {
            in_quotes = !in_quotes;
            // No añadimos la comilla al contenido final
        } else if (c == ';' && !in_quotes) {
            out.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    // último campo
    out.push_back(cur);

    // trim básico de espacios en extremos
    auto trim = [](std::string &s){
        while (!s.empty() && (s.back() == '\r' || s.back() == '\n' || s.back()==' ' || s.back()=='\t')) s.pop_back();
        size_t j = 0; while (j < s.size() && (s[j]==' ' || s[j]=='\t')) ++j;
        if (j>0) s.erase(0, j);
    };
    for (auto &s : out) trim(s);
    return out;
}

// --------------------- Carga de datos ---------------------
// Estructuras reales (sin cabecera):
// farmacias.csv: CIF;Provincia;Localidad;Nombre;Dirección;CP
// pa_medicamentos.csv: id_num;id_alpha;nombre;
// laboratorios.csv: id;nombre;direccion;cp;localidad
bool cargarMedicamentos(const std::string &file, MediExpress &medi) {
    std::ifstream in(file);
    if (!in.is_open()) {
        std::cerr << "No se pudo abrir " << file << "\n";
        return false;
    }
    std::string line;
    unsigned count = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        auto cols = split_semicolon_quoted(line);
        // hay líneas con punto y coma al final -> puede salir col extra vacía: aceptamos >=3
        if (cols.size() >= 3) {
            // cols[0]=id_num, cols[1]=id_alpha, cols[2]=nombre
            try {
                int id = std::stoi(cols[0]);
                medi.insertarMedicamento(PaMedicamento(id, cols[1], cols[2]));
                count++;
            } catch (...) {
                // línea no válida -> ignorar
            }
        }
    }
    std::cout << "[OK] Cargados " << count << " medicamentos.\n";
    return count > 0;
}

bool cargarLaboratorios(const std::string &file, MediExpress &medi) {
    std::ifstream in(file);
    if (!in.is_open()) {
        std::cerr << "No se pudo abrir " << file << "\n";
        return false;
    }
    std::string line;
    unsigned count = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        auto cols = split_semicolon_quoted(line);
        if (cols.size() >= 5) {
            // cols[0]=id, cols[1]=nombre, cols[2]=direccion, cols[3]=cp, cols[4]=localidad
            try {
                int id = std::stoi(cols[0]);
                Laboratorio lab(id, cols[1], cols[2], cols[3], cols[4]);
                medi.insertarLaboratorio(lab);
                count++;
            } catch (...) {
                // Si el id no es numérico, podemos asignar un id incremental opcionalmente
                // (pero según sus datos reales, el id es numérico)
            }
        }
    }
    std::cout << "[OK] Cargados " << count << " laboratorios.\n";
    return count > 0;
}

bool cargarFarmacias(const std::string &file, MediExpress &medi, VDinamico<Farmacia> &vec) {
    std::ifstream in(file);
    if (!in.is_open()) {
        std::cerr << "No se pudo abrir " << file << "\n";
        return false;
    }
    std::string line;
    unsigned count = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        auto cols = split_semicolon_quoted(line);
        if (cols.size() >= 6) {
            // CIF;Provincia;Localidad;Nombre;Dirección;CP
            Farmacia f(cols[0], cols[1], cols[2], cols[3], cols[4], cols[5]);
            medi.insertarFarmacia(f);
            vec.insertar(f);
            count++;
        }
    }
    std::cout << "[OK] Cargadas " << count << " farmacias.\n";
    return count > 0;
}

void cargar500CIF(const std::string &file, VDinamico<std::string> &buf) {
    std::ifstream in(file);
    std::string line;
    unsigned count = 0;
    while (count < 500 && std::getline(in, line)) {
        if (line.empty()) continue;
        auto cols = split_semicolon_quoted(line);
        if (!cols.empty()) {
            buf.insertar(cols[0]);
            count++;
        }
    }
}

// --------------------- Benchmark ---------------------
void benchmarkAVLvsVector(MediExpress &medi, const VDinamico<Farmacia> &vec, const VDinamico<std::string> &cifs) {
    using namespace std::chrono;
    auto t1 = high_resolution_clock::now();
    unsigned ok1 = 0;
    for (unsigned i = 0; i < cifs.tamlog(); ++i) {
        if (medi.buscarFarmacia(cifs[i])) ok1++;
    }
    auto t2 = high_resolution_clock::now();
    long avl_ms = duration_cast<milliseconds>(t2 - t1).count();

    t1 = high_resolution_clock::now();
    unsigned ok2 = 0;
    for (unsigned i = 0; i < cifs.tamlog(); ++i) {
        for (unsigned j = 0; j < vec.tamlog(); ++j) {
            if (vec[j] == cifs[i]) { ok2++; break; }
        }
    }
    t2 = high_resolution_clock::now();
    long vec_ms = duration_cast<milliseconds>(t2 - t1).count();

    std::cout << "\n--- Benchmark busqueda de " << cifs.tamlog() << " CIFs ---\n";
    std::cout << "AVL: " << avl_ms << " ms (" << ok1 << " aciertos)\n";
    std::cout << "Vector: " << vec_ms << " ms (" << ok2 << " aciertos)\n";
    std::cout << "Diferencia (Vector - AVL): " << (vec_ms - avl_ms) << " ms\n";
}

// --------------------- Programa II ---------------------
void enlazarMedLab(MediExpress &medi) {
    // Convertimos la lista enlazada de labs en un vector de punteros para round-robin
    VDinamico<Laboratorio*> labsPtr;
    {
        auto it = medi.getLabs().iterador();
        while (it != medi.getLabs().end()) {
            Laboratorio &ref = *it;
            labsPtr.insertar(&ref);
            ++it;
        }
    }

    VDinamico<PaMedicamento> &meds = medi.getMedications();
    if (meds.tamlog() == 0) {
        std::cerr << "[AVISO] No hay medicamentos.\n";
        return;
    }
    if (labsPtr.tamlog() == 0) {
        std::cerr << "[AVISO] No hay laboratorios. Se omite el enlace Med-Lab.\n";
        return;
    }

    for (unsigned i = 0; i < meds.tamlog(); ++i) {
        Laboratorio *lab = labsPtr[i % labsPtr.tamlog()];
        medi.suministrarMed(&meds[i], lab);
    }
    std::cout << "[OK] Enlazados " << meds.tamlog() << " medicamentos con " << labsPtr.tamlog() << " laboratorios (round-robin).\n";
}

void asignarMedicamentos(MediExpress &medi, const VDinamico<std::string> &cifs) {
    VDinamico<PaMedicamento> &meds = medi.getMedications();
    if (meds.tamlog() == 0) {
        std::cerr << "[AVISO] No hay medicamentos para asignar a farmacias.\n";
        return;
    }
    if (cifs.tamlog() == 0) {
        std::cerr << "[AVISO] No hay CIFs para asignar medicamentos.\n";
        return;
    }

    unsigned base = 0;
    unsigned asignadas = 0;
    for (unsigned i = 0; i < cifs.tamlog(); ++i) {
        Farmacia *f = medi.buscarFarmacia(cifs[i]);
        if (!f) continue;
        for (unsigned j = 0; j < 100; ++j) {
            int id = meds[(base + j) % meds.tamlog()].getIdNum();
            medi.suministrarFarmacia(*f, id);
        }
        base = (base + 100) % meds.tamlog();
        asignadas++;
    }
    std::cout << "[OK] Asignados 100 medicamentos a cada una de " << asignadas << " farmacias (ciclico).\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "      PRACTICA 3 AVL - MediExpress\n";
    std::cout << "========================================\n";

    MediExpress medi;
    VDinamico<Farmacia> vecFarm;
    VDinamico<std::string> bufCIF;

    // --- CARGA DE DATOS (sin cabeceras) ---
    bool okM = cargarMedicamentos("pa_medicamentos.csv", medi);
    bool okL = cargarLaboratorios("laboratorios.csv", medi);
    bool okF = cargarFarmacias("farmacias.csv", medi, vecFarm);
    cargar500CIF("farmacias.csv", bufCIF);

    if (!okF) {
        std::cerr << "[ERROR] No se cargaron farmacias. Abortando.\n";
        return 1;
    }

    // --- PROGRAMA I ---
    std::cout << "\n=== PROGRAMA I: Prueba de AVL ===\n";
    if (bufCIF.tamlog() == 0) std::cerr << "[AVISO] No se obtuvieron 500 CIFs; se usara 0.\n";
    benchmarkAVLvsVector(medi, vecFarm, bufCIF);
    std::cout << "Altura del AVL: " << medi.avlFarmacias().altura() << "\n";

    // Mostrar primeras 100 farmacias con todos sus datos
    VDinamico<Farmacia*> inorden = medi.avlFarmacias().recorreInorden();
    std::cout << "\nPrimeras 100 farmacias (inorden):\n";
    std::cout << "----------------------------------------------------------------------------------------------------------\n";
    std::cout << "CIF\t\tProvincia\tLocalidad\t\tNombre\t\tDireccion\t\tCP\n";
    std::cout << "----------------------------------------------------------------------------------------------------------\n";

    for (unsigned i = 0; i < std::min(100u, inorden.tamlog()); ++i) {
        if (inorden[i]) {
            std::cout << inorden[i]->getCIF() << "\t"
                      << inorden[i]->getProvincia() << "\t"
                      << inorden[i]->getLocalidad() << "\t"
                      << inorden[i]->getNombre() << "\t"
                      << inorden[i]->getDireccion() << "\t"
                      << inorden[i]->getCodPostal() << "\n";
        }
    }

    // --- PROGRAMA II ---
    std::cout << "\n=== PROGRAMA II: MediExpress ===\n";
    enlazarMedLab(medi);                 // 1) enlazar meds con labs (si hay)
    asignarMedicamentos(medi, bufCIF);   // 2) 100 meds por farmacia (cíclico, según CIFs)

    // 3) Buscar “OXIDO DE MAGNESIO” (ID = 3640); pedir si falta
    int oxidoID = 3640;
    unsigned pedidos = 0;
    for (unsigned i = 0; i < bufCIF.tamlog(); ++i) {
        Farmacia *f = medi.buscarFarmacia(bufCIF[i]);
        if (!f) continue;
        if (!f->buscaMedicam(oxidoID)) {
            f->pedidoMedicam(oxidoID);
            pedidos++;
        }
    }
    std::cout << "\nPedidos de 'OXIDO DE MAGNESIO' (ID=3640): " << pedidos << "\n";

    // 4) Buscar laboratorios que trabajan con “MAGNESIO”
    VDinamico<Laboratorio*> labsMag = medi.buscarLabs("MAGNESIO");
    std::unordered_set<std::string> unicos;
    std::cout << "\n=== Laboratorios que trabajan con 'MAGNESIO' ===\n";
    std::cout << "--------------------------------------------------------------------------------------------------------\n";
    std::cout << "ID\tNombre\t\t\tDireccion\t\tCP\tLocalidad\n";
    std::cout << "--------------------------------------------------------------------------------------------------------\n";
    for (unsigned i = 0; i < labsMag.tamlog(); ++i) {
        Laboratorio* L = labsMag[i];
        if (L && unicos.insert(L->getNombre()).second) { // evita duplicados por nombre
            std::cout << L->getId() << "\t"
                      << L->getNombre() << "\t"
                      << L->getDireccion() << "\t"
                      << L->getCodPostal() << "\t"
                      << L->getLocalidad() << "\n";
        }
    }
    std::cout << "---------------------------------------------------------------------------------------------------------\n";
    std::cout << "Total laboratorios con 'MAGNESIO': " << unicos.size() << "\n";

    // 5) (Parejas) Buscar laboratorios con “VIRUS”
    VDinamico<Laboratorio*> labsVirus = medi.buscarLabs("VIRUS");
    unicos.clear();
    std::cout << "\n=== Laboratorios que trabajan con 'VIRUS' ===\n";
    std::cout << "---------------------------------------------------------------------------------------------------------\n";
    std::cout << "ID\tNombre\t\t\tDireccion\t\tCP\tLocalidad\n";
    std::cout << "---------------------------------------------------------------------------------------------------------\n";
    for (unsigned i = 0; i < labsVirus.tamlog(); ++i) {
        Laboratorio* L = labsVirus[i];
        if (L && unicos.insert(L->getNombre()).second) {
            std::cout << L->getId() << "\t"
                      << L->getNombre() << "\t"
                      << L->getDireccion() << "\t"
                      << L->getCodPostal() << "\t"
                      << L->getLocalidad() << "\n";
        }
    }
    std::cout << "----------------------------------------------------------------------------------------------------------\n";
    std::cout << "Total laboratorios con 'VIRUS': " << unicos.size() << "\n";


    std::cout << "\n=== Fin de ejecucion ===\n";
    return 0;
}
