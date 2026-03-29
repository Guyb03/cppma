#include "Univers.hxx"

// ================================================================
// Simulation LJ : 2 particules placées à 1.5σ (> équilibre ≈ 1.122σ)
// Elles s'attirent, dépassent l'équilibre et oscillent.
// Lancer avec : ./simuLJ > simuLJ.csv
// Visualiser  : python3 afficheSimuCsv.py simuLJ.csv
// ================================================================
int main() {
    const double epsilon = 1.0;
    const double sigma   = 1.0;
    const double rcut    = 2.5 * sigma;

    Univers u(2, {10.0, 10.0, 0.0}, epsilon, sigma, rcut, 0.0, false, true);

    u.addParticule(Particule(
        Vecteur<3>{3.0,             5.0, 0.0},
        Vecteur<3>{}, Vecteur<3>{},
        1.0, "P1", 0
    ));
    u.addParticule(Particule(
        Vecteur<3>{3.0 + 1.5*sigma, 5.0, 0.0},
        Vecteur<3>{}, Vecteur<3>{},
        1.0, "P2", 1
    ));

    u.StromerVerlet(0.0, 20.0, 0.001, true);
    return 0;
}
