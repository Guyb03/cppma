/**
 * @file compareReflexion.cxx
 * @brief Comparaison des deux implémentations de réflexion aux limites
 *
 * Deux simulations identiques (même état initial, même durée) sont lancées :
 *
 *  - REFLEXION     : inversion instantanée de la composante de vitesse perpendiculaire
 *                    lorsque la particule franchit le bord.
 *
 *  - REFLEXION_LJ  : potentiel de Lennard-Jones de paroi intégré en continu dans Verlet.
 *                    F = -24ε/(2r) · (σ/(2r))^6 · (1 − 2·(σ/(2r))^6), r_cut = 2^{1/6}·σ.
 *                    La réflexion est progressive : la particule ralentit avant de rebondir.
 *
 * Setup : un disque de 5×5 particules LJ se déplaçant vers le bord gauche.
 *
 * Sorties VTK :
 *   vtk/refl/refl_XXXXXX.vtu    → mode inversion de vitesse
 *   vtk/reflLJ/reflLJ_XXXXXX.vtu → mode potentiel LJ
 *
 * Visualisation (Paraview) :
 *   Ouvrir les deux séries côte à côte pour observer la différence de trajectoire
 *   au voisinage de la paroi.
 *
 * Usage :
 *   mkdir -p vtk/refl vtk/reflLJ
 *   ./build/demo/compareReflexion
 */

#include "Univers.hxx"
#include "Particule.hxx"
#include <cmath>
#include <iostream>

// ---------------------------------------------------------------------------
// Construit un univers 2D peuplé d'un disque de nx×ny particules LJ.
// modeX est appliqué sur les quatre parois.
// ---------------------------------------------------------------------------
static Univers makeSim(ConditionLimite mode) {
    const double Lx      = 30.0, Ly = 20.0;
    const double epsilon = 1.0,  sigma = 1.0;
    const double rcut    = 2.5 * sigma;
    const double d_eq    = std::pow(2.0, 1.0 / 6.0) * sigma;  // ≈ 1.122

    Univers u(2, {Lx, Ly, 0.0}, epsilon, sigma, rcut, false, true);
    u.setConditionsLimites({mode, mode, mode, mode,
                            ConditionLimite::LIBRE, ConditionLimite::LIBRE});

    // Grille 5×5 centrée dans le domaine, vitesse initiale vers x=0
    const int nx = 5, ny = 5;
    const double x0 = (Lx - (nx - 1) * d_eq) / 2.0;
    const double y0 = (Ly - (ny - 1) * d_eq) / 2.0;

    int id = 0;
    for (int ix = 0; ix < nx; ++ix)
        for (int iy = 0; iy < ny; ++iy)
            u.addParticule(Particule(
                Vecteur<3>{x0 + ix * d_eq, y0 + iy * d_eq, 0.0},
                Vecteur<3>{-4.0, 0.0, 0.0},
                Vecteur<3>{},
                1.0, "P", id++
            ));

    return u;
}

int main() {
    const double dt      = 0.0005;
    const double t_end   = 4.0;
    const int    vtkFreq = 200;   // ≈ 40 fichiers par simulation

    std::cout << "=== Comparaison REFLEXION vs REFLEXION_LJ ===\n\n";

    // --- Simulation 1 : inversion instantanée de vitesse ---
    std::cout << "1/2  REFLEXION (inversion de vitesse)  →  vtk/refl/\n";
    {
        Univers u = makeSim(ConditionLimite::REFLEXION);
        u.StromerVerlet(0.0, t_end, dt, false, vtkFreq, "vtk/refl/refl");
    }
    std::cout << "     done.\n\n";

    // --- Simulation 2 : potentiel LJ de paroi ---
    std::cout << "2/2  REFLEXION_LJ (potentiel de paroi)  →  vtk/reflLJ/\n";
    {
        Univers u = makeSim(ConditionLimite::REFLEXION_LJ);
        u.StromerVerlet(0.0, t_end, dt, false, vtkFreq, "vtk/reflLJ/reflLJ");
    }
    std::cout << "     done.\n\n";

    std::cout << "Ouvrir les deux séries dans Paraview pour comparer :\n"
              << "  vtk/refl/refl_*.vtu     (rebond abrupt)\n"
              << "  vtk/reflLJ/reflLJ_*.vtu (rebond progressif)\n";

    return 0;
}
