#include <gtest/gtest.h>
#include "Vecteur.hxx"

// ============================================================
// Constructeurs
// ============================================================

TEST(VecteurTest, ConstructeurZero) {
    Vecteur<3> v;
    EXPECT_EQ(v[0], 0.0);
    EXPECT_EQ(v[1], 0.0);
    EXPECT_EQ(v[2], 0.0);
}

TEST(VecteurTest, ConstructeurInitializerList) {
    Vecteur<3> v{1.0, 2.0, 3.0};
    EXPECT_EQ(v[0], 1.0);
    EXPECT_EQ(v[1], 2.0);
    EXPECT_EQ(v[2], 3.0);
}

TEST(VecteurTest, ConstructeurPointeur) {
    double tab[] = {4.0, 5.0, 6.0};
    Vecteur<3> v(tab);
    EXPECT_EQ(v[0], 4.0);
    EXPECT_EQ(v[1], 5.0);
    EXPECT_EQ(v[2], 6.0);
}

// ============================================================
// Accesseurs
// ============================================================

TEST(VecteurTest, GetDim) {
    Vecteur<3> v;
    EXPECT_EQ(v.getDim(), 3);
}

TEST(VecteurTest, DimStatique) {
    EXPECT_EQ(Vecteur<3>::dim, 3);
    EXPECT_EQ(Vecteur<2>::dim, 2);
}

TEST(VecteurTest, GetCoords) {
    Vecteur<3> v{1.0, 2.0, 3.0};
    const auto& c = v.getCoords();
    EXPECT_EQ(c[0], 1.0);
    EXPECT_EQ(c[1], 2.0);
    EXPECT_EQ(c[2], 3.0);
}

TEST(VecteurTest, OperatorBracketLecture) {
    Vecteur<3> v{1.0, 2.0, 3.0};
    EXPECT_EQ(v[1], 2.0);
}

TEST(VecteurTest, OperatorBracketEcriture) {
    Vecteur<3> v{1.0, 2.0, 3.0};
    v[1] = 9.0;
    EXPECT_EQ(v[1], 9.0);
}

// ============================================================
// Normes
// ============================================================

TEST(VecteurTest, Norm2) {
    Vecteur<3> v{3.0, 4.0, 0.0};
    EXPECT_DOUBLE_EQ(v.norm2(), 25.0);
}

TEST(VecteurTest, Norm) {
    Vecteur<3> v{3.0, 4.0, 0.0};
    EXPECT_NEAR(v.norm(), 5.0, 1e-12);
}

TEST(VecteurTest, NormVecteurNul) {
    Vecteur<3> v;
    EXPECT_DOUBLE_EQ(v.norm(), 0.0);
}

TEST(VecteurTest, Norm2D) {
    Vecteur<2> v{0.0, 1.0};
    EXPECT_DOUBLE_EQ(v.norm(), 1.0);
}

// ============================================================
// Produit scalaire
// ============================================================

TEST(VecteurTest, DotOrthogonal) {
    Vecteur<3> a{1.0, 0.0, 0.0};
    Vecteur<3> b{0.0, 1.0, 0.0};
    EXPECT_DOUBLE_EQ(a.dot(b), 0.0);
}

TEST(VecteurTest, Dot) {
    Vecteur<3> c{1.0, 2.0, 3.0};
    Vecteur<3> d{4.0, 5.0, 6.0};
    EXPECT_DOUBLE_EQ(c.dot(d), 32.0);
}

TEST(VecteurTest, DotAvecLuiMeme) {
    Vecteur<3> v{3.0, 4.0, 0.0};
    EXPECT_DOUBLE_EQ(v.dot(v), v.norm2());
}

// ============================================================
// Opérateurs composés
// ============================================================

TEST(VecteurTest, OperatorPlusEq) {
    Vecteur<3> u{1.0, 2.0, 3.0};
    u += Vecteur<3>{1.0, 1.0, 1.0};
    EXPECT_DOUBLE_EQ(u[0], 2.0);
    EXPECT_DOUBLE_EQ(u[1], 3.0);
    EXPECT_DOUBLE_EQ(u[2], 4.0);
}

TEST(VecteurTest, OperatorMoinsEq) {
    Vecteur<3> u{2.0, 3.0, 4.0};
    u -= Vecteur<3>{1.0, 1.0, 1.0};
    EXPECT_DOUBLE_EQ(u[0], 1.0);
    EXPECT_DOUBLE_EQ(u[1], 2.0);
    EXPECT_DOUBLE_EQ(u[2], 3.0);
}

TEST(VecteurTest, OperatorMulEq) {
    Vecteur<3> u{1.0, 2.0, 3.0};
    u *= 2.0;
    EXPECT_DOUBLE_EQ(u[0], 2.0);
    EXPECT_DOUBLE_EQ(u[1], 4.0);
    EXPECT_DOUBLE_EQ(u[2], 6.0);
}

TEST(VecteurTest, OperatorDivEq) {
    Vecteur<3> u{2.0, 4.0, 6.0};
    u /= 2.0;
    EXPECT_DOUBLE_EQ(u[0], 1.0);
    EXPECT_DOUBLE_EQ(u[1], 2.0);
    EXPECT_DOUBLE_EQ(u[2], 3.0);
}

// ============================================================
// Opérateurs binaires libres
// ============================================================

TEST(VecteurTest, OperatorPlus) {
    Vecteur<3> r = Vecteur<3>{1.0, 0.0, 0.0} + Vecteur<3>{0.0, 1.0, 0.0};
    EXPECT_DOUBLE_EQ(r[0], 1.0);
    EXPECT_DOUBLE_EQ(r[1], 1.0);
    EXPECT_DOUBLE_EQ(r[2], 0.0);
}

TEST(VecteurTest, OperatorMoins) {
    Vecteur<3> r = Vecteur<3>{1.0, 0.0, 0.0} - Vecteur<3>{0.0, 1.0, 0.0};
    EXPECT_DOUBLE_EQ(r[0],  1.0);
    EXPECT_DOUBLE_EQ(r[1], -1.0);
    EXPECT_DOUBLE_EQ(r[2],  0.0);
}

TEST(VecteurTest, OperatorMulScalairePost) {
    Vecteur<3> r = Vecteur<3>{1.0, 2.0, 3.0} * 3.0;
    EXPECT_DOUBLE_EQ(r[0], 3.0);
    EXPECT_DOUBLE_EQ(r[1], 6.0);
    EXPECT_DOUBLE_EQ(r[2], 9.0);
}

TEST(VecteurTest, OperatorMulScalairePre) {
    Vecteur<3> r = 3.0 * Vecteur<3>{1.0, 2.0, 3.0};
    EXPECT_DOUBLE_EQ(r[0], 3.0);
    EXPECT_DOUBLE_EQ(r[1], 6.0);
    EXPECT_DOUBLE_EQ(r[2], 9.0);
}

TEST(VecteurTest, OperatorDiv) {
    Vecteur<3> r = Vecteur<3>{2.0, 4.0, 6.0} / 2.0;
    EXPECT_DOUBLE_EQ(r[0], 1.0);
    EXPECT_DOUBLE_EQ(r[1], 2.0);
    EXPECT_DOUBLE_EQ(r[2], 3.0);
}

// ============================================================
// Comparaison exacte
// ============================================================

TEST(VecteurTest, EgaliteTrue) {
    Vecteur<3> v1{1.0, 2.0, 3.0};
    Vecteur<3> v2{1.0, 2.0, 3.0};
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
}

TEST(VecteurTest, EgaliteFalse) {
    Vecteur<3> v1{1.0, 2.0, 3.0};
    Vecteur<3> v2{1.0, 2.0, 4.0};
    EXPECT_FALSE(v1 == v2);
    EXPECT_TRUE(v1 != v2);
}

// ============================================================
// Comparaison approchée
// ============================================================

TEST(VecteurTest, ApproxEqualDansTolerance) {
    Vecteur<3> x{1.0, 2.0, 3.0};
    Vecteur<3> y{1.0, 2.0, 3.0 + 1e-11};
    EXPECT_TRUE(approxEqual(x, y));
}

TEST(VecteurTest, ApproxEqualHorsTolerance) {
    Vecteur<3> x{1.0, 2.0, 3.0};
    Vecteur<3> y{1.0, 2.0, 3.0 + 1e-11};
    EXPECT_FALSE(approxEqual(x, y, 1e-12));
}

TEST(VecteurTest, ApproxEqualIdentique) {
    Vecteur<3> v{1.0, 2.0, 3.0};
    EXPECT_TRUE(approxEqual(v, v));
}

// ============================================================
// Distance euclidienne
// ============================================================

TEST(VecteurTest, Distance) {
    Vecteur<3> p{0.0, 0.0, 0.0};
    Vecteur<3> q{3.0, 4.0, 0.0};
    EXPECT_NEAR(distance(p, q), 5.0, 1e-12);
}

TEST(VecteurTest, DistanceNulle) {
    Vecteur<3> p{1.0, 2.0, 3.0};
    EXPECT_DOUBLE_EQ(distance(p, p), 0.0);
}

TEST(VecteurTest, DistanceSymetrique) {
    Vecteur<3> p{1.0, 0.0, 0.0};
    Vecteur<3> q{4.0, 0.0, 0.0};
    EXPECT_DOUBLE_EQ(distance(p, q), distance(q, p));
}
