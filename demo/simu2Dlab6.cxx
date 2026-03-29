/**
 * @file simu2Dlab6.cxx
 * @brief Simulation finale lab 6 : disque tombant sur un rectangle (potentiel LJ + gravité uniforme)
 *
 * Deux phases :
 *  1. Pré-relaxation du rectangle seul avec thermostat serré (ecFreq=100) pendant 2 s
 *     pour obtenir un état initial stable au contact des parois REFLEXION_LJ.
 *  2. Simulation complète avec le disque qui tombe sur le rectangle.
 *
 * Paramètres du sujet :
 *  - Rectangle : ~17 000 particules, vitesse initiale thermique T=0.005
 *  - Disque     : ~395 particules, vitesse initiale vers le bas (vy=-10)
 *  - Gravité uniforme G=-12 selon y
 *  - Conditions aux limites : REFLEXION_LJ sur toutes les parois actives
 *
 * Usage :
 *   mkdir -p vtk/lab6
 *   ./build/demo/simu2Dlab6
 */

#include "Univers.hxx"
#include "Particule.hxx"
#include <cmath>
#include <iostream>
#include <random>

int main() {
    const double Lx      = 250.0;
    const double Ly      = 180.0;
    const double epsilon = 1.0;
    const double sigma   = 1.0;
    const double rcut    = 2.5 * sigma;
    const double dt      = 0.0005;
    const double G_field = -12.0;
    const double t_end   = 29.5;
    const double m       = 1.0;
    const double d       = std::pow(2.0, 1.0 / 6.0) * sigma;  // ≈ 1.122

    Univers u(2, {Lx, Ly, 0.0}, epsilon, sigma, rcut, G_field, false, true, true);
    u.setConditionsLimites({
        ConditionLimite::REFLEXION_LJ, ConditionLimite::REFLEXION_LJ,
        ConditionLimite::REFLEXION_LJ, ConditionLimite::REFLEXION_LJ,
        ConditionLimite::LIBRE,        ConditionLimite::LIBRE
    });

    // -----------------------------------------------------------------------
    // Rectangle (N2 ≈ 17 227) : rangée basse à y = d/2 (équilibre paroi LJ)
    // Vitesses initiales thermiques T = 0.005
    // -----------------------------------------------------------------------
    const int    nx_rect  = (int)std::floor((Lx - 2.0 * rcut) / d) + 1;
    const int    ny_rect  = (int)std::ceil(17227.0 / nx_rect);
    const double rect_x0  = (Lx - (nx_rect - 1) * d) / 2.0;
    const double rect_y0  = d / 2.0;

    const double v_init = std::sqrt(3.0 * 0.005);
    std::mt19937 rng(42);
    std::uniform_real_distribution<double> dist_v(-v_init, v_init);

    long id = 0;
    for (int ix = 0; ix < nx_rect; ++ix)
        for (int iy = 0; iy < ny_rect; ++iy)
            u.addParticule(Particule(
                Vecteur<3>{rect_x0 + ix * d, rect_y0 + iy * d, 0.0},
                Vecteur<3>{dist_v(rng), dist_v(rng), 0.0},
                Vecteur<3>{},
                m, "rect", id++
            ));

    const int    N2_reel  = u.getNbParticules();
    const double EcD_rect = 0.005 * N2_reel;

    // -----------------------------------------------------------------------
    // Phase 1 : pré-relaxation du rectangle (thermostat serré, pas de VTK)
    // -----------------------------------------------------------------------
    std::cout << "Phase 1 : stabilisation du rectangle (" << N2_reel << " particules)...\n";
    u.StromerVerlet(0.0, 2.0, dt, false, 0, "", EcD_rect, 100, true);
    std::cout << "\n";

    // -----------------------------------------------------------------------
    // Disque (N1 ≈ 395) : au-dessus du rectangle, vitesse initiale vers le bas
    // -----------------------------------------------------------------------
    const double r_disk  = std::sqrt(395.0 / M_PI) * d;
    const double rect_top = rect_y0 + (ny_rect - 1) * d;
    const double disk_cx = Lx / 2.0;
    const double disk_cy = rect_top + 3.0 * rcut + r_disk;

    const int R_steps = (int)std::ceil(r_disk / d) + 1;
    for (int ix = -R_steps; ix <= R_steps; ++ix)
        for (int iy = -R_steps; iy <= R_steps; ++iy)
            if (std::hypot(ix * d, iy * d) <= r_disk)
                u.addParticule(Particule(
                    Vecteur<3>{disk_cx + ix * d, disk_cy + iy * d, 0.0},
                    Vecteur<3>{0.0, -10.0, 0.0},
                    Vecteur<3>{},
                    m, "disk", id++
                ));

    const double EcD = 0.005 * u.getNbParticules();
    std::cout << "Phase 2 : simulation collision (" << u.getNbParticules() << " particules)...\n";

    // -----------------------------------------------------------------------
    // Phase 2 : simulation complète avec sauvegarde VTK
    // -----------------------------------------------------------------------
    u.StromerVerlet(0.0, t_end, dt, false, 200, "vtk/lab6/collision", EcD, 1000, true);
    std::cout << "\nTerminé. Fichiers VTK dans vtk/lab6/\n";

    return 0;
}
