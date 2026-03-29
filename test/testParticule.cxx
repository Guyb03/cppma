#include <gtest/gtest.h>
#include "Particule.hxx"

namespace {

Particule makeParticule() {
    return Particule(
        Vecteur<3>{1.0, 2.0, 3.0},
        Vecteur<3>{0.1, 0.2, 0.3},
        Vecteur<3>{0.5, 0.6, 0.7},
        1.5, "test", 42
    );
}

} // namespace

// ============================================================
// Constructeur / accesseurs en lecture
// ============================================================

TEST(ParticuleTest, GetPosition) {
    auto p = makeParticule();
    EXPECT_DOUBLE_EQ(p.getPosition()[0], 1.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[1], 2.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[2], 3.0);
}

TEST(ParticuleTest, GetVitesse) {
    auto p = makeParticule();
    EXPECT_DOUBLE_EQ(p.getVitesse()[0], 0.1);
    EXPECT_DOUBLE_EQ(p.getVitesse()[1], 0.2);
    EXPECT_DOUBLE_EQ(p.getVitesse()[2], 0.3);
}

TEST(ParticuleTest, GetForce) {
    auto p = makeParticule();
    EXPECT_DOUBLE_EQ(p.getForce()[0], 0.5);
    EXPECT_DOUBLE_EQ(p.getForce()[1], 0.6);
    EXPECT_DOUBLE_EQ(p.getForce()[2], 0.7);
}

TEST(ParticuleTest, GetMasse) {
    auto p = makeParticule();
    EXPECT_DOUBLE_EQ(p.getMasse(), 1.5);
}

TEST(ParticuleTest, GetCategorie) {
    auto p = makeParticule();
    EXPECT_EQ(p.getCategorie(), "test");
}

TEST(ParticuleTest, GetIdentifiant) {
    auto p = makeParticule();
    EXPECT_EQ(p.getIdentifiant(), 42);
}

// ============================================================
// Mutateurs
// ============================================================

TEST(ParticuleTest, SetPosition) {
    auto p = makeParticule();
    p.setPosition(Vecteur<3>{9.0, 8.0, 7.0});
    EXPECT_DOUBLE_EQ(p.getPosition()[0], 9.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[1], 8.0);
    EXPECT_DOUBLE_EQ(p.getPosition()[2], 7.0);
}

TEST(ParticuleTest, SetVitesse) {
    auto p = makeParticule();
    p.setVitesse(Vecteur<3>{5.0, 6.0, 7.0});
    EXPECT_DOUBLE_EQ(p.getVitesse()[0], 5.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[1], 6.0);
    EXPECT_DOUBLE_EQ(p.getVitesse()[2], 7.0);
}

TEST(ParticuleTest, SetForce) {
    auto p = makeParticule();
    p.setForce(Vecteur<3>{1.0, 2.0, 3.0});
    EXPECT_DOUBLE_EQ(p.getForce()[0], 1.0);
    EXPECT_DOUBLE_EQ(p.getForce()[1], 2.0);
    EXPECT_DOUBLE_EQ(p.getForce()[2], 3.0);
}

TEST(ParticuleTest, SetMasse) {
    auto p = makeParticule();
    p.setMasse(3.14);
    EXPECT_DOUBLE_EQ(p.getMasse(), 3.14);
}

// ============================================================
// Opérateur de comparaison (ordre sur les identifiants)
// ============================================================

TEST(ParticuleTest, OperatorLessTrue) {
    Particule p1(Vecteur<3>{}, Vecteur<3>{}, Vecteur<3>{}, 1.0, "a", 0);
    Particule p2(Vecteur<3>{}, Vecteur<3>{}, Vecteur<3>{}, 1.0, "b", 1);
    EXPECT_TRUE(p1 < p2);
    EXPECT_FALSE(p2 < p1);
}

TEST(ParticuleTest, OperatorLessEgal) {
    Particule p1(Vecteur<3>{}, Vecteur<3>{}, Vecteur<3>{}, 1.0, "a", 5);
    Particule p2(Vecteur<3>{}, Vecteur<3>{}, Vecteur<3>{}, 1.0, "b", 5);
    EXPECT_FALSE(p1 < p2);
    EXPECT_FALSE(p2 < p1);
}

TEST(ParticuleTest, OperatorLessIndependantDeMasse) {
    // La comparaison ne dépend que de l'identifiant, pas de la masse
    Particule lourd(Vecteur<3>{}, Vecteur<3>{}, Vecteur<3>{}, 1000.0, "lourd", 0);
    Particule leger(Vecteur<3>{}, Vecteur<3>{}, Vecteur<3>{},    1.0, "leger", 1);
    EXPECT_TRUE(lourd < leger);
}
