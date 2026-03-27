#include "Univers.hxx"
#include <iostream>

// ================================================================
// Création d'un Système Solaire et simulation avec Stromër-Verlet
// ================================================================
void testSysSol() {
    Univers u(2);
    u.addParticule(Particule({0.0,0.0}, {0.0,0.0}, {0.0,0.0}, 1.0,      "Soleil",  0));
    u.addParticule(Particule({0.0,1.0}, {-1.0,0.0},{0.0,0.0}, 3.0e-6,   "Terre",   1));
    u.addParticule(Particule({0.0,5.36},{-0.425,0.0},{0.0,0.0},9.55e-4,  "Jupiter", 2));
    u.addParticule(Particule({34.75,0.0},{0.0,0.0296},{0.0,0.0},1.0e-14, "Halley",  3));

    u.StromerVerlet(0.0, 468.5, 0.015, true);
}

int main() {
    testSysSol();
    return 0;
}