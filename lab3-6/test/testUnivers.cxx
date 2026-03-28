#include <gtest/gtest.h>
#include "Univers.hxx"
#include <random>

namespace {

Univers makeUnivers2D() {
    return Univers(2, {10.0, 10.0, 0.0}, 1.0, 1.0, 2.5, false, false);
}

Particule makeParticule(double x, double y, int id) {
    return Particule(
        Vecteur<3>{x, y, 0.0},
        Vecteur<3>{},
        Vecteur<3>{},
        1.0, "P", id
    );
}

} // namespace

// ============================================================
// Construction
// ============================================================

TEST(UniversTest, ConstructeurDim2) {
    Univers u = makeUnivers2D();
    EXPECT_EQ(u.getDim(), 2);
}

TEST(UniversTest, ConstructeurDim3) {
    Univers u(3, {1.0, 1.0, 1.0}, 1.0, 1.0, 0.5, false, false);
    EXPECT_EQ(u.getDim(), 3);
}

TEST(UniversTest, ConstructeurVide) {
    Univers u = makeUnivers2D();
    EXPECT_EQ(u.getNbParticules(), 0);
}

// ============================================================
// Ajout de particules
// ============================================================

TEST(UniversTest, AddUneParticule) {
    Univers u = makeUnivers2D();
    u.addParticule(makeParticule(1.0, 1.0, 0));
    EXPECT_EQ(u.getNbParticules(), 1);
}

TEST(UniversTest, AddMultiplesParticules) {
    Univers u = makeUnivers2D();
    for (int i = 0; i < 5; ++i)
        u.addParticule(makeParticule((double)i + 0.5, 5.0, i));
    EXPECT_EQ(u.getNbParticules(), 5);
}

TEST(UniversTest, AddParticuleSurBord) {
    // Une particule placée exactement à la limite doit être acceptée
    Univers u = makeUnivers2D();
    u.addParticule(makeParticule(0.0, 0.0, 0));
    EXPECT_EQ(u.getNbParticules(), 1);
}

// ============================================================
// Störmer-Verlet : invariants observables
// ============================================================

TEST(UniversTest, StromerVerletConserveNombreParticules) {
    Univers u(2, {10.0, 10.0, 0.0}, 1.0, 1.0, 2.5, false, true);
    u.addParticule(makeParticule(3.0, 5.0, 0));
    u.addParticule(makeParticule(4.5, 5.0, 1));
    int n_avant = u.getNbParticules();
    u.StromerVerlet(0.0, 0.01, 0.001, false);
    EXPECT_EQ(u.getNbParticules(), n_avant);
}

TEST(UniversTest, StromerVerletGraviteConserveNombreParticules) {
    Univers u(2, {100.0, 100.0, 0.0}, 1.0, 1.0, 50.0, true, false);
    u.addParticule(Particule(Vecteur<3>{0.0, 0.0, 0.0}, Vecteur<3>{},       Vecteur<3>{}, 1.0,   "A", 0));
    u.addParticule(Particule(Vecteur<3>{1.0, 0.0, 0.0}, Vecteur<3>{0.0, 1.0, 0.0}, Vecteur<3>{}, 1e-6, "B", 1));
    int n_avant = u.getNbParticules();
    u.StromerVerlet(0.0, 0.01, 0.005, false);
    EXPECT_EQ(u.getNbParticules(), n_avant);
}

TEST(UniversTest, StromerVerletSansParticules) {
    Univers u = makeUnivers2D();
    EXPECT_NO_FATAL_FAILURE(u.StromerVerlet(0.0, 0.01, 0.001, false));
    EXPECT_EQ(u.getNbParticules(), 0);
}

// ============================================================
// Performance insertion (vérification scalabilité)
// ============================================================

TEST(UniversTest, InsertionGrandNombre) {
    std::mt19937 mt(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    Univers u(3, {1.0, 1.0, 1.0}, 1.0, 1.0, 0.3, false, false);
    const int N = 1000;
    for (int i = 0; i < N; ++i) {
        Vecteur<3> pos{dist(mt), dist(mt), dist(mt)};
        u.addParticule(Particule(pos, Vecteur<3>{}, Vecteur<3>{}, 1.0e-6, "P", i));
    }
    EXPECT_EQ(u.getNbParticules(), N);
}
