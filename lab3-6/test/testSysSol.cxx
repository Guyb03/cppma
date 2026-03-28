#include <gtest/gtest.h>
#include "Univers.hxx"

namespace {

// Construit le système solaire à 4 corps (Soleil, Terre, Jupiter, Halley)
Univers buildSysSol() {
    Univers u(2, {100.0, 100.0, 0.0}, 1.0, 1.0, 50.0, true, false);
    u.addParticule(Particule(Vecteur<3>{0.0,   0.0,  0.0}, Vecteur<3>{0.0,    0.0,    0.0}, Vecteur<3>{}, 1.0,     "Soleil",  0));
    u.addParticule(Particule(Vecteur<3>{0.0,   1.0,  0.0}, Vecteur<3>{-1.0,   0.0,    0.0}, Vecteur<3>{}, 3.0e-6,  "Terre",   1));
    u.addParticule(Particule(Vecteur<3>{0.0,   5.36, 0.0}, Vecteur<3>{-0.425, 0.0,    0.0}, Vecteur<3>{}, 9.55e-4, "Jupiter", 2));
    u.addParticule(Particule(Vecteur<3>{34.75, 0.0,  0.0}, Vecteur<3>{0.0,    0.0296, 0.0}, Vecteur<3>{}, 1.0e-14, "Halley",  3));
    return u;
}

} // namespace

// ============================================================
// Construction
// ============================================================

TEST(SysSolTest, Dimension) {
    Univers u = buildSysSol();
    EXPECT_EQ(u.getDim(), 2);
}

TEST(SysSolTest, NombreDeCorps) {
    Univers u = buildSysSol();
    EXPECT_EQ(u.getNbParticules(), 4);
}

// ============================================================
// Simulation courte : invariants observables
// ============================================================

TEST(SysSolTest, SimulationCourteConserveNombreCorps) {
    Univers u = buildSysSol();
    u.StromerVerlet(0.0, 0.015, 0.015, false);
    EXPECT_EQ(u.getNbParticules(), 4);
}

TEST(SysSolTest, SimulationPlusieursPas) {
    Univers u = buildSysSol();
    // 10 pas de dt = 0.015
    u.StromerVerlet(0.0, 0.15, 0.015, false);
    EXPECT_EQ(u.getNbParticules(), 4);
}

// ============================================================
// Système à 2 corps : vérification gravitationnelle
// ============================================================

TEST(SysSolTest, DeuxCorpsGravite) {
    // Soleil + Terre uniquement : simulation sans crash
    Univers u(2, {100.0, 100.0, 0.0}, 1.0, 1.0, 50.0, true, false);
    u.addParticule(Particule(Vecteur<3>{0.0, 0.0, 0.0}, Vecteur<3>{},           Vecteur<3>{}, 1.0,   "Soleil", 0));
    u.addParticule(Particule(Vecteur<3>{0.0, 1.0, 0.0}, Vecteur<3>{-1.0, 0.0, 0.0}, Vecteur<3>{}, 3e-6, "Terre",  1));
    EXPECT_NO_FATAL_FAILURE(u.StromerVerlet(0.0, 0.03, 0.015, false));
    EXPECT_EQ(u.getNbParticules(), 2);
}
