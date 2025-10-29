#include <iostream>
#include <string>
#include "MediExpress.h"
#include "Farmacia.h"
#include "PaMedicamento.h"
#include "Laboratorio.h"
#include "VDinamico.h"

int main() {
    std::cout << "==============================" << std::endl;
    std::cout << "   Proyecto MediExpress 2025" << std::endl;
    std::cout << "==============================" << std::endl;

    // --- Rutas de archivos ---
    std::string meds_file = "pa_medicamentos.csv";
    std::string labs_file = "laboratorios.csv";
    std::string farms_file = "farmacias.csv";

    // --- Crear sistema MediExpress ---
    MediExpress medi(meds_file, labs_file, farms_file);

    std::cout << "\n=== PROGRAMA I: Prueba de AVL ===" << std::endl;
    medi.programaI_benchmarkAVLvsVector();
    medi.mostrarAlturaAVL();
    medi.imprimir100Inorden();

    std::cout << "\n=== PROGRAMA II: Prueba de MediExpress ===" << std::endl;

    // --- 1) Enlazar medicamentos con farmacias cíclicamente ---
    medi.asignarMedicamentosAFarmaciasCiclico();

    // --- 2) Lista de CIFs a comprobar (según enunciado) ---
    const std::string cif_array[] = {
        "37656422V","46316032N","77092934Q","33961602D","B62351861","B62351861",
        "B65828113","46138599R","35069965W","37579913Y","37682300C","37643742X",
        "46112335A","47980171D","38116138D","46315600V","37640233C","37931842N",
        "33964303L","35022080A","B66046640","E66748344","47640201W","B66621954",
        "46121385Z","X6806622W","46046390E"
    };

    VDinamico<std::string> cifbuf;
    for (const auto& c : cif_array) cifbuf.insertar(c);

    // --- 3) Buscar “ÓXIDO DE MAGNESIO” (id=3640) en cada farmacia ---
    std::cout << "\nBuscando medicamento 'OXIDO DE MAGNESIO' (ID=3640)..." << std::endl;
    int oxidoId = 3640;
    int pedidos = 0;

    for (unsigned i = 0; i < cifbuf.tamlog(); ++i) {
        Farmacia* f = medi.buscarFarmacia(cifbuf[i]);
        if (!f) continue;
        PaMedicamento* med = f->buscaMedicam(oxidoId);
        if (!med) {
            f->pedidoMedicam(oxidoId);
            pedidos++;
        }
    }
    std::cout << "Pedidos realizados de 'OXIDO DE MAGNESIO': " << pedidos << std::endl;

    // --- 4) Buscar laboratorios que trabajan con “MAGNESIO” ---
    std::cout << "\nBuscando laboratorios que trabajan con 'MAGNESIO'..." << std::endl;
    VDinamico<Laboratorio*> labsMag = medi.buscarLabs("MAGNESIO");
    std::cout << "Laboratorios encontrados: " << labsMag.tamlog() << std::endl;

    // Mostrar algunos
    for (unsigned i = 0; i < std::min(10u, labsMag.tamlog()); ++i) {
        std::cout << " - " << labsMag[i]->getNombre() << std::endl;
    }

    // --- 5) (Opcional) Buscar laboratorios que suministran medicamentos con “VIRUS” ---
    std::cout << "\nBuscando laboratorios que trabajan con 'VIRUS'..." << std::endl;
    VDinamico<Laboratorio*> labsVirus = medi.buscarLabs("VIRUS");
    std::cout << "Laboratorios con medicamentos relacionados con VIRUS: "
              << labsVirus.tamlog() << std::endl;

    std::cout << "\n=== Fin del programa ===" << std::endl;
    return 0;
}
