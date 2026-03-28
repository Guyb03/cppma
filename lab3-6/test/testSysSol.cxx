#include "Univers.hxx"
#include <iostream>

// ================================================================
// Création d'un Système Solaire et simulation avec Störmer-Verlet
// ================================================================
void testSysSol() {
    // Domaine large pour couvrir les orbites, gravitation uniquement
    Univers u(2, {100.0, 100.0, 0.0}, 1.0, 1.0, 50.0, true, false);

    u.addParticule(Particule(Vecteur<3>{0.0,   0.0,  0.0}, Vecteur<3>{0.0,    0.0,    0.0}, Vecteur<3>{}, 1.0,     "Soleil",  0));
    u.addParticule(Particule(Vecteur<3>{0.0,   1.0,  0.0}, Vecteur<3>{-1.0,   0.0,    0.0}, Vecteur<3>{}, 3.0e-6,  "Terre",   1));
    u.addParticule(Particule(Vecteur<3>{0.0,   5.36, 0.0}, Vecteur<3>{-0.425, 0.0,    0.0}, Vecteur<3>{}, 9.55e-4, "Jupiter", 2));
    u.addParticule(Particule(Vecteur<3>{34.75, 0.0,  0.0}, Vecteur<3>{0.0,    0.0296, 0.0}, Vecteur<3>{}, 1.0e-14, "Halley",  3));

    u.StromerVerlet(0.0, 468.5, 0.015, true);
}

int main() {
    testSysSol();
    return 0;
}
