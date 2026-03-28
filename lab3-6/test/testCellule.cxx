#include <gtest/gtest.h>
#include "Cellule.hxx"

// ============================================================
// Constructeurs
// ============================================================

TEST(CelluleTest, ConstructeurDefaut) {
    Cellule c;
    EXPECT_TRUE(c.getIndices().empty());
    EXPECT_DOUBLE_EQ(c.getCentre()[0], 0.0);
    EXPECT_DOUBLE_EQ(c.getCentre()[1], 0.0);
    EXPECT_DOUBLE_EQ(c.getCentre()[2], 0.0);
}

TEST(CelluleTest, ConstructeurAvecCentre) {
    Cellule c(Vecteur<>{1.0, 2.0, 3.0});
    EXPECT_DOUBLE_EQ(c.getCentre()[0], 1.0);
    EXPECT_DOUBLE_EQ(c.getCentre()[1], 2.0);
    EXPECT_DOUBLE_EQ(c.getCentre()[2], 3.0);
}

// ============================================================
// Ajout de particules
// ============================================================

TEST(CelluleTest, AddParticule) {
    Cellule c;
    c.addParticule(0);
    ASSERT_EQ(c.getIndices().size(), 1u);
    EXPECT_EQ(c.getIndices()[0], 0);
}

TEST(CelluleTest, AddMultiplesParticules) {
    Cellule c;
    c.addParticule(0);
    c.addParticule(3);
    c.addParticule(7);
    const auto& idx = c.getIndices();
    ASSERT_EQ(idx.size(), 3u);
    EXPECT_EQ(idx[0], 0);
    EXPECT_EQ(idx[1], 3);
    EXPECT_EQ(idx[2], 7);
}

TEST(CelluleTest, AddPreserveOrdre) {
    Cellule c;
    for (int i = 9; i >= 0; --i) c.addParticule(i);
    const auto& idx = c.getIndices();
    ASSERT_EQ(idx.size(), 10u);
    for (int i = 0; i < 10; ++i)
        EXPECT_EQ(idx[i], 9 - i) << "à l'indice " << i;
}

// ============================================================
// Vidage
// ============================================================

TEST(CelluleTest, Vider) {
    Cellule c;
    c.addParticule(0);
    c.addParticule(1);
    c.vider();
    EXPECT_TRUE(c.getIndices().empty());
}

TEST(CelluleTest, ViderPuisAjouter) {
    Cellule c;
    c.addParticule(0);
    c.vider();
    c.addParticule(5);
    ASSERT_EQ(c.getIndices().size(), 1u);
    EXPECT_EQ(c.getIndices()[0], 5);
}

TEST(CelluleTest, ViderCelluleDejaVide) {
    Cellule c;
    EXPECT_NO_FATAL_FAILURE(c.vider());
    EXPECT_TRUE(c.getIndices().empty());
}

// ============================================================
// Centre inchangé après modifications
// ============================================================

TEST(CelluleTest, CentreInchangeApresAdd) {
    Cellule c(Vecteur<>{2.0, 3.0, 4.0});
    c.addParticule(0);
    c.addParticule(1);
    EXPECT_DOUBLE_EQ(c.getCentre()[0], 2.0);
    EXPECT_DOUBLE_EQ(c.getCentre()[1], 3.0);
    EXPECT_DOUBLE_EQ(c.getCentre()[2], 4.0);
}

TEST(CelluleTest, CentreInchangeApresVider) {
    Cellule c(Vecteur<>{5.0, 6.0, 7.0});
    c.addParticule(0);
    c.vider();
    EXPECT_DOUBLE_EQ(c.getCentre()[0], 5.0);
    EXPECT_DOUBLE_EQ(c.getCentre()[1], 6.0);
    EXPECT_DOUBLE_EQ(c.getCentre()[2], 7.0);
}
