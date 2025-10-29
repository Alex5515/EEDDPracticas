#include "MediExpress.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>

// ---------------- CONSTRUCTOR ----------------
MediExpress::MediExpress(const std::string& file_meds,
                         const std::string& file_labs,
                         const std::string& file_farms) {
    std::cout << "Inicializando MediExpress..." << std::endl;

    cargarMedicamentos(file_meds);
    cargarLaboratorios(file_labs);
    enlazarMedicamentosConLabs();

    cargarFarmaciasAVL(file_farms);
    cargarFarmaciasVector(file_farms);
    prepararCIF500(file_farms);

    // enlazar vínculo MediExpress a cada farmacia del AVL
    auto inord = pharmacy.recorreInorden();
    for (unsigned i = 0; i < inord.tamlog(); ++i)
        inord[i]->setLinkMedi(this);

    std::cout << "Inicializacion completada." << std::endl;
}

// --------------- CARGA MEDICAMENTOS ----------------
void MediExpress::cargarMedicamentos(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de medicamentos: " << filename << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        std::stringstream ss(line);
        std::string idnum_str, idalpha, nombre;
        getline(ss, idnum_str, ';');
        getline(ss, idalpha,   ';');
        getline(ss, nombre,    ';');

        // Convertimos idnum a int (según UML). Si falla, saltamos la línea.
        try {
            int idnum = std::stoi(idnum_str);
            medication.insertar(PaMedicamento(idnum, idalpha, nombre));
        } catch (...) {
            // Posible cabecera u otras líneas no válidas
            continue;
        }
    }
    std::cout << "Cargados " << medication.tamlog() << " medicamentos." << std::endl;
}

// --------------- CARGA LABORATORIOS ----------------
void MediExpress::cargarLaboratorios(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de laboratorios: " << filename << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        std::stringstream ss(line);
        std::string id_str, nombre, dir, cp, full_loc, ciudad, prov;

        getline(ss, id_str, ';');
        getline(ss, nombre, ';');
        getline(ss, dir,    ';');
        getline(ss, cp,     ';');
        getline(ss, full_loc);

        // full_loc puede venir con formato "Ciudad(Provincia)"
        size_t pos_open  = full_loc.find('(');
        size_t pos_close = full_loc.find(')');
        if (pos_open != std::string::npos && pos_close != std::string::npos && pos_close > pos_open) {
            ciudad = full_loc.substr(0, pos_open);
            prov   = full_loc.substr(pos_open + 1, pos_close - pos_open - 1);
        } else {
            // Si no viene así, lo usamos para ambos campos
            ciudad = prov = full_loc;
        }

        try {
            int id = std::stoi(id_str);
            // Nuestra clase Laboratorio = (id, nombre, direccion, CP, provincia)
            labs.insertarFinal(Laboratorio(id, nombre, dir, cp, prov));
        } catch (...) {
            continue;
        }
    }
    std::cout << "Cargados " << labs.tam() << " laboratorios." << std::endl;
}

// -------- ENLACE AUTOMÁTICO MEDICAMENTOS ↔ LABS --------
void MediExpress::enlazarMedicamentosConLabs() {
    ListaEnlazada<Laboratorio>::Iterador it = labs.iterador();
    int med_idx = 0;

    while (it != labs.end() && med_idx < static_cast<int>(medication.tamlog())) {
        Laboratorio* lab_actual = &(*it);

        // Asignamos dos medicamentos por laboratorio (como en tu versión previa)
        if (med_idx < static_cast<int>(medication.tamlog()))
            medication[med_idx++].setSuministrador(lab_actual); // alias: suministrarMed()

        if (med_idx < static_cast<int>(medication.tamlog()))
            medication[med_idx++].setSuministrador(lab_actual);

        ++it;
    }
    std::cout << "Enlace automatico completado." << std::endl;
}

// ----------------- UTILIDADES -----------------
PaMedicamento* MediExpress::buscarMedPorId(const std::string& idnum_str) {
    // Utilidad de compatibilidad si entra como string: convertimos a int y delegamos
    try {
        int id = std::stoi(idnum_str);
        for (unsigned i = 0; i < medication.tamlog(); ++i)
            if (medication[i].getIdNum() == id)   // UML: comparar por int
                return &medication[i];
    } catch (...) { /* ignore */ }
    return nullptr;
}

VDinamico<Laboratorio*> MediExpress::buscarLabsPorNombreMed(const std::string& nombreParcial) {
    VDinamico<Laboratorio*> res;
    VDinamico<PaMedicamento*> meds = buscarCompuesto(nombreParcial);

    for (unsigned i = 0; i < meds.tamlog(); ++i) {
        Laboratorio* l = meds[i]->getSuministrador();  // UML: servidoPor / getSuministrador()
        if (!l) continue;

        bool rep = false;
        for (unsigned j = 0; j < res.tamlog(); ++j) {
            if (res[j]->getId() == l->getId()) { rep = true; break; }
        }
        if (!rep) res.insertar(l);
    }
    return res;
}

// ----------------- API EXISTENTE + UML -----------------
void MediExpress::suministrarMed(PaMedicamento* pa, Laboratorio* l) {
    if (pa) pa->setSuministrador(l); // alias de suministrarMed(l)
}

Laboratorio* MediExpress::buscarLab(const std::string& nombreLab) {
    std::string needle = nombreLab;
    std::transform(needle.begin(), needle.end(), needle.begin(), ::tolower);

    for (auto it = labs.iterador(); it != labs.end(); ++it) {
        std::string cur = (*it).getNombre();
        std::transform(cur.begin(), cur.end(), cur.begin(), ::tolower);
        if (cur.find(needle) != std::string::npos) return &(*it);
    }
    return nullptr;
}

VDinamico<Laboratorio*> MediExpress::buscarLabCiudad(const std::string& nombreCiudad) {
    VDinamico<Laboratorio*> resultado;
    std::string needle = nombreCiudad;
    std::transform(needle.begin(), needle.end(), needle.begin(), ::tolower);

    for (auto it = labs.iterador(); it != labs.end(); ++it) {
        std::string prov = (*it).getProvincia();
        std::transform(prov.begin(), prov.end(), prov.begin(), ::tolower);
        if (prov.find(needle) != std::string::npos) resultado.insertar(&(*it));
    }
    return resultado;
}

// UML: buscarCompuesto(Integer id_num) → PaMedicamento*
PaMedicamento* MediExpress::buscarCompuesto(int id_num) {
    for (unsigned i = 0; i < medication.tamlog(); ++i)
        if (medication[i].getIdNum() == id_num)
            return &medication[i];
    return nullptr;
}

// UML: buscarCompuesto(String nombrePA) → PaMedicamento*[]
VDinamico<PaMedicamento*> MediExpress::buscarCompuesto(const std::string& nombrePA) {
    VDinamico<PaMedicamento*> r;

    std::string needle = nombrePA;
    std::transform(needle.begin(), needle.end(), needle.begin(), ::tolower);

    for (unsigned i = 0; i < medication.tamlog(); ++i) {
        std::string name = medication[i].getNombre();
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.find(needle) != std::string::npos) r.insertar(&medication[i]);
    }
    return r;
}

// UML: buscarFarmacia(String cif)
Farmacia* MediExpress::buscarFarmacia(const std::string& cif) {
    Farmacia clave(cif);
    // Puedes usar buscaRec o buscaIt indistintamente
    return pharmacy.buscaIt(clave);
}

// UML: buscarLabs(String nombrePA)
VDinamico<Laboratorio*> MediExpress::buscarLabs(const std::string& nombrePA) {
    return buscarLabsPorNombreMed(nombrePA);
}

// UML: suministrarFarmacia(Farmacia f, Integer id_num)
PaMedicamento* MediExpress::suministrarFarmacia(Farmacia& f, int id_num) {
    PaMedicamento* m = buscarCompuesto(id_num);
    if (!m) return nullptr;
    f.dispensaMedicam(m);        // enlaza sin copias
    return m;
}

// Overload práctica: por CIF + id (string)
PaMedicamento* MediExpress::suministrarFarmacia(const std::string& cif, const std::string& med_id_num) {
    Farmacia* f = buscarFarmacia(cif);
    if (!f) return nullptr;

    // Convertimos a int para el flujo principal
    try {
        int id = std::stoi(med_id_num);
        return suministrarFarmacia(*f, id);
    } catch (...) {
        return nullptr;
    }
}

// ----------------- Lectura de farmacias -----------------
static inline std::string trim(std::string s){
    while(!s.empty() && (s.back()=='\r' || s.back()==' ' || s.back()=='\t')) s.pop_back();
    return s;
}

void MediExpress::cargarFarmaciasAVL(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir farmacias: " << filename << std::endl;
        return;
    }

    std::string line;
    // Formato asumido: cif;nombre;direccion;localidad;provincia;cp
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        std::stringstream ss(line);
        std::string cif, nombre, direccion, localidad, provincia, cp;
        getline(ss, cif,       ';');
        getline(ss, nombre,    ';');
        getline(ss, direccion, ';');
        getline(ss, localidad, ';');
        getline(ss, provincia, ';');
        getline(ss, cp,        ';');

        Farmacia f(trim(cif), trim(nombre), trim(direccion), trim(localidad), trim(provincia), trim(cp));
        pharmacy.inserta(f);
    }
    std::cout << "Cargadas " << pharmacy.numElementos() << " farmacias en AVL." << std::endl;
}

void MediExpress::cargarFarmaciasVector(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        std::stringstream ss(line);
        std::string cif, nombre, direccion, localidad, provincia, cp;
        getline(ss, cif,       ';');
        getline(ss, nombre,    ';');
        getline(ss, direccion, ';');
        getline(ss, localidad, ';');
        getline(ss, provincia, ';');
        getline(ss, cp,        ';');

        farmVec.insertar(Farmacia(trim(cif), trim(nombre), trim(direccion), trim(localidad), trim(provincia), trim(cp)));
    }
}

void MediExpress::prepararCIF500(const std::string& /*filename*/) {
    // Tomamos los 500 primeros del vector ya cargado
    for (unsigned i = 0; i < 500 && i < farmVec.tamlog(); ++i)
        farmCIF500.insertar(farmVec[i].cif());
}

// ----------------- Programa I (benchmark) -----------------
void MediExpress::programaI_benchmarkAVLvsVector() {
    clock_t t1 = clock();
    for (unsigned i = 0; i < farmCIF500.tamlog(); ++i)
        (void)buscarFarmacia(farmCIF500[i]);
    double tAVL = (clock() - t1) / (double)CLOCKS_PER_SEC;

    t1 = clock();
    for (unsigned i = 0; i < farmCIF500.tamlog(); ++i) {
        const std::string& cif = farmCIF500[i];
        for (unsigned j = 0; j < farmVec.tamlog(); ++j)
            if (farmVec[j].cif() == cif) break;
    }
    double tVec = (clock() - t1) / (double)CLOCKS_PER_SEC;

    std::cout << "[Programa I] Tiempo 500 busquedas:" << std::endl;
    std::cout << "  AVL:   " << tAVL << " s" << std::endl;
    std::cout << "  Vector:" << tVec << " s" << std::endl;
    std::cout << "  ¿AVL mas rapido? " << (tAVL < tVec ? "SI" : "NO") << std::endl;
}

void MediExpress::mostrarAlturaAVL() const {
    std::cout << "Altura del AVL de farmacias: " << pharmacy.altura() << std::endl;
}

void MediExpress::imprimir100Inorden() const {
    auto inord = pharmacy.recorreInorden();
    std::cout << "Primeras 100 farmacias (inorden):" << std::endl;
    for (unsigned i = 0; i < inord.tamlog() && i < 100; ++i)
        std::cout << *inord[i] << std::endl;
}

// ----------------- Programa II -----------------
void MediExpress::asignarMedicamentosAFarmaciasCiclico() {
    auto inord = pharmacy.recorreInorden();
    unsigned totalMeds = medication.tamlog();
    if (totalMeds == 0) return;

    const unsigned bloque = 100;
    unsigned idxBase = 0;

    for (unsigned i = 0; i < inord.tamlog(); ++i) {
        Farmacia* f = inord[i];
        f->setLinkMedi(this);

        for (unsigned k = 0; k < bloque; ++k) {
            unsigned idx = (idxBase + k) % totalMeds;
            f->dispensaMedicam(&medication[idx]);
        }
        idxBase = (idxBase + bloque) % totalMeds;
    }
    std::cout << "Asignados 100 medicamentos a cada farmacia (ciclico)." << std::endl;
}
