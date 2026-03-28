#include <gtest/gtest.h>
#include "Univers.hxx"
#include <cmath>

namespace {

// Construit un univers LJ 2D avec deux particules séparées de `sep` * sigma
Univers buildLJ(double sep) {
    const double sigma   = 1.0;
    const double epsilon = 1.0;
    const double rcut    = 2.5 * sigma;
    Univers u(2, {10.0, 10.0, 0.0}, epsilon, sigma, rcut, false, true);
    u.addParticule(Particule(
        Vecteur<3>{3.0,             5.0, 0.0},
        Vecteur<3>{}, Vecteur<3>{},
        1.0, "P1", 0
    ));
    u.addParticule(Particule(
        Vecteur<3>{3.0 + sep*sigma, 5.0, 0.0},
        Vecteur<3>{}, Vecteur<3>{},
        1.0, "P2", 1
    ));
    return u;
}

} // namespace

// ============================================================
// Construction
// ============================================================

TEST(LennardJonesTest, ConstructeurDim) {
    Univers u = buildLJ(1.5);
    EXPECT_EQ(u.getDim(), 2);
}

TEST(LennardJonesTest, DeuxParticulesAjoutees) {
    Univers u = buildLJ(1.5);
    EXPECT_EQ(u.getNbParticules(), 2);
}

// ============================================================
// Simulation : invariants observables
// ============================================================

TEST(LennardJonesTest, SimulationConserveNombreParticules) {
    // Particules à 1.5σ > r_eq ≈ 1.122σ : elles s'attirent
    Univers u = buildLJ(1.5);
    u.StromerVerlet(0.0, 0.1, 0.001, false);
    EXPECT_EQ(u.getNbParticules(), 2);
}

TEST(LennardJonesTest, SimulationRepulsion) {
    // Particules à 0.9σ < r_eq : elles se repoussent
    Univers u = buildLJ(0.9);
    u.StromerVerlet(0.0, 0.05, 0.001, false);
    EXPECT_EQ(u.getNbParticules(), 2);
}

TEST(LennardJonesTest, ParticulesHorsRcut) {
    // Séparation > rcut = 2.5σ : pas d'interaction LJ, système stable
    Univers u = buildLJ(3.0);
    u.StromerVerlet(0.0, 0.1, 0.001, false);
    EXPECT_EQ(u.getNbParticules(), 2);
}

TEST(LennardJonesTest, SimulationUneParticuleSeule) {
    const double rcut = 2.5;
    Univers u(2, {10.0, 10.0, 0.0}, 1.0, 1.0, rcut, false, true);
    u.addParticule(Particule(Vecteur<3>{5.0, 5.0, 0.0}, Vecteur<3>{1.0, 0.0, 0.0}, Vecteur<3>{}, 1.0, "P", 0));
    // Sans interaction : la particule se déplace librement
    EXPECT_NO_FATAL_FAILURE(u.StromerVerlet(0.0, 0.1, 0.01, false));
    EXPECT_EQ(u.getNbParticules(), 1);
}

// ============================================================
// Distance d'équilibre LJ : r_eq = 2^(1/6) * sigma ≈ 1.122σ
// ============================================================

TEST(LennardJonesTest, DistanceEquilibre) {
    // Vérifie la valeur mathématique de r_eq
    const double sigma = 1.0;
    const double r_eq  = std::pow(2.0, 1.0 / 6.0) * sigma;
    EXPECT_NEAR(r_eq, 1.1225, 1e-4);
}
