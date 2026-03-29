/**
 * @file testOcean.cxx
 * @brief Test de stabilité : ~1000 particules sous gravité uniforme (« océan »)
 *
 * Compare le comportement des deux modes de réflexion de paroi sous gravité :
 *  - REFLEXION     : inversion instantanée de la vitesse perpendiculaire
 *  - REFLEXION_LJ  : potentiel de Lennard-Jones de paroi (continu)
 *
 * Dans les deux cas les particules forment un océan au fond du domaine
 * sous l'effet de la gravité G=-12.
 *
 * Usage :
 *   mkdir -p vtk/ocean_refl vtk/ocean_reflLJ
 *   ./build/demo/testOcean
 */

#include "Univers.hxx"
#include "Particule.hxx"
#include <cmath>
#include <iostream>

static Univers makeOcean(ConditionLimite mode) {
    const double Lx      = 50.0;
    const double Ly      = 70.0;
    const double epsilon = 1.0;
    const double sigma   = 1.0;
    const double rcut    = 2.5 * sigma;
    const double G       = -12.0;
    const double d       = std::pow(2.0, 1.0 / 6.0) * sigma;  // ≈ 1.122

    Univers u(2, {Lx, Ly, 0.0}, epsilon, sigma, rcut, G, false, true, true);
    u.setConditionsLimites({mode, mode, mode, mode,
                            ConditionLimite::LIBRE, ConditionLimite::LIBRE});

    // Grille de nx × 25 particules, rangée basse à y = d/2
    const int    nx = (int)std::floor((Lx - 2.0 * rcut) / d) + 1;
    const int    ny = 25;
    const double x0 = (Lx - (nx - 1) * d) / 2.0;
    const double y0 = d / 2.0;

    long id = 0;
    for (int ix = 0; ix < nx; ++ix)
        for (int iy = 0; iy < ny; ++iy)
            u.addParticule(Particule(
                Vecteur<3>{x0 + ix * d, y0 + iy * d, 0.0},
                Vecteur<3>{0.0, 0.0, 0.0},
                Vecteur<3>{},
                1.0, "P", id++
            ));

    return u;
}

int main() {
    const double dt      = 0.0005;
    const double t_end   = 5.0;
    const int    vtkFreq = 50;

    std::cout << "=== Test océan : REFLEXION vs REFLEXION_LJ ===\n\n";

    std::cout << "1/2  REFLEXION  →  vtk/ocean_refl/\n";
    {
        Univers u = makeOcean(ConditionLimite::REFLEXION);
        u.StromerVerlet(0.0, t_end, dt, false, vtkFreq, "vtk/ocean_refl/ocean",
                        0.0, 1000, true);
    }
    std::cout << "\n     done.\n\n";

    std::cout << "2/2  REFLEXION_LJ  →  vtk/ocean_reflLJ/\n";
    {
        Univers u = makeOcean(ConditionLimite::REFLEXION_LJ);
        u.StromerVerlet(0.0, t_end, dt, false, vtkFreq, "vtk/ocean_reflLJ/ocean",
                        0.0, 1000, true);
    }
    std::cout << "\n     done.\n";

    return 0;
}
