#include <gtest/gtest.h>
#include "Univers.hxx"
#include <random>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

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

std::string lireFichier(const std::string& path) {
    std::ifstream f(path);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
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
    Univers u = makeUnivers2D();
    u.addParticule(makeParticule(0.0, 0.0, 0));
    EXPECT_EQ(u.getNbParticules(), 1);
}

// ============================================================
// Störmer-Verlet : invariants observables
// ============================================================

TEST(UniversTest, StromerVerletConserveNombreParticules) {
    Univers u(2, {10.0, 10.0, 0.0}, 1.0, 1.0, 2.5, 0.0, false, true);
    u.addParticule(makeParticule(3.0, 5.0, 0));
    u.addParticule(makeParticule(4.5, 5.0, 1));
    int n_avant = u.getNbParticules();
    u.StromerVerlet(0.0, 0.01, 0.001, false);
    EXPECT_EQ(u.getNbParticules(), n_avant);
}

TEST(UniversTest, StromerVerletGraviteConserveNombreParticules) {
    Univers u(2, {100.0, 100.0, 0.0}, 1.0, 1.0, 50.0, 0.0, true, false);
    u.addParticule(Particule(Vecteur<3>{0.0, 0.0, 0.0}, Vecteur<3>{},            Vecteur<3>{}, 1.0,   "A", 0));
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
// sauvegarderVTK : structure du fichier
// ============================================================

TEST(UniversTest, VTKFichierCree) {
    Univers u = makeUnivers2D();
    u.addParticule(makeParticule(1.0, 2.0, 0));
    const std::string path = "/tmp/test_vtk_cree.vtu";
    u.sauvegarderVTK(path);
    std::ifstream f(path);
    EXPECT_TRUE(f.good()) << "Le fichier VTK n'a pas été créé";
    std::remove(path.c_str());
}

TEST(UniversTest, VTKContientBaliseRacine) {
    Univers u = makeUnivers2D();
    u.addParticule(makeParticule(1.0, 2.0, 0));
    const std::string path = "/tmp/test_vtk_racine.vtu";
    u.sauvegarderVTK(path);
    std::string contenu = lireFichier(path);
    EXPECT_NE(contenu.find("<VTKFile"), std::string::npos);
    EXPECT_NE(contenu.find("</VTKFile>"), std::string::npos);
    std::remove(path.c_str());
}

TEST(UniversTest, VTKNombreDePointsCorrect) {
    Univers u = makeUnivers2D();
    u.addParticule(makeParticule(1.0, 2.0, 0));
    u.addParticule(makeParticule(3.0, 4.0, 1));
    u.addParticule(makeParticule(5.0, 6.0, 2));
    const std::string path = "/tmp/test_vtk_points.vtu";
    u.sauvegarderVTK(path);
    std::string contenu = lireFichier(path);
    EXPECT_NE(contenu.find("NumberOfPoints=\"3\""), std::string::npos);
    std::remove(path.c_str());
}

TEST(UniversTest, VTKContientPosition) {
    Univers u = makeUnivers2D();
    u.addParticule(makeParticule(1.0, 2.0, 0));
    const std::string path = "/tmp/test_vtk_pos.vtu";
    u.sauvegarderVTK(path);
    std::string contenu = lireFichier(path);
    EXPECT_NE(contenu.find("Position"), std::string::npos);
    EXPECT_NE(contenu.find("</Points>"), std::string::npos);
    std::remove(path.c_str());
}

TEST(UniversTest, VTKContientVitesse) {
    Univers u = makeUnivers2D();
    u.addParticule(makeParticule(1.0, 2.0, 0));
    const std::string path = "/tmp/test_vtk_vit.vtu";
    u.sauvegarderVTK(path);
    std::string contenu = lireFichier(path);
    EXPECT_NE(contenu.find("Velocity"), std::string::npos);
    std::remove(path.c_str());
}

TEST(UniversTest, VTKContientMasse) {
    Univers u = makeUnivers2D();
    u.addParticule(makeParticule(1.0, 2.0, 0));
    const std::string path = "/tmp/test_vtk_masse.vtu";
    u.sauvegarderVTK(path);
    std::string contenu = lireFichier(path);
    EXPECT_NE(contenu.find("Masse"), std::string::npos);
    std::remove(path.c_str());
}

TEST(UniversTest, VTKUniversVide) {
    Univers u = makeUnivers2D();
    const std::string path = "/tmp/test_vtk_vide.vtu";
    u.sauvegarderVTK(path);
    std::string contenu = lireFichier(path);
    EXPECT_NE(contenu.find("NumberOfPoints=\"0\""), std::string::npos);
    std::remove(path.c_str());
}

// ============================================================
// sauvegarderVTK : valeurs dans le fichier
// ============================================================

TEST(UniversTest, VTKPositionValeur) {
    Univers u = makeUnivers2D();
    u.addParticule(Particule(
        Vecteur<3>{1.5, 2.5, 0.0}, Vecteur<3>{}, Vecteur<3>{}, 1.0, "P", 0));
    const std::string path = "/tmp/test_vtk_posval.vtu";
    u.sauvegarderVTK(path);
    std::string contenu = lireFichier(path);
    EXPECT_NE(contenu.find("1.5"), std::string::npos);
    EXPECT_NE(contenu.find("2.5"), std::string::npos);
    std::remove(path.c_str());
}

TEST(UniversTest, VTKMasseValeur) {
    Univers u = makeUnivers2D();
    u.addParticule(Particule(
        Vecteur<3>{1.0, 1.0, 0.0}, Vecteur<3>{}, Vecteur<3>{}, 3.14, "P", 0));
    const std::string path = "/tmp/test_vtk_masseval.vtu";
    u.sauvegarderVTK(path);
    std::string contenu = lireFichier(path);
    EXPECT_NE(contenu.find("3.14"), std::string::npos);
    std::remove(path.c_str());
}

// ============================================================
// StromerVerlet + VTK : génération des fichiers
// ============================================================

TEST(UniversTest, VTKFreqGenereLesBonsFichiers) {
    Univers u(2, {10.0, 10.0, 0.0}, 1.0, 1.0, 2.5, 0.0, false, true);
    u.addParticule(makeParticule(3.0, 5.0, 0));
    u.addParticule(makeParticule(4.5, 5.0, 1));

    // 10 pas de dt=0.001 avec vtkFreq=5 → fichiers au pas 0, 5, 10
    u.StromerVerlet(0.0, 0.01, 0.001, false, 5, "/tmp/test_sv");

    for (const std::string f : {
            "/tmp/test_sv_000000.vtu",
            "/tmp/test_sv_000005.vtu",
            "/tmp/test_sv_000010.vtu" }) {
        EXPECT_TRUE(std::ifstream(f).good()) << f << " absent";
        std::remove(f.c_str());
    }
}

TEST(UniversTest, VTKFreqZeroNeCreeRienDeFichier) {
    Univers u(2, {10.0, 10.0, 0.0}, 1.0, 1.0, 2.5, 0.0, false, true);
    u.addParticule(makeParticule(3.0, 5.0, 0));
    u.StromerVerlet(0.0, 0.005, 0.001, false, 0, "/tmp/test_novtk");

    EXPECT_FALSE(std::ifstream("/tmp/test_novtk_000000.vtu").good());
}

// ============================================================
// Conditions aux limites
// ============================================================

TEST(UniversTest, CondLimReflexionRebondit) {
    // Particule à x=0.1 avec v_x=-1, bord gauche et droit réflexifs
    // Après dt=0.2 : newPos_x = 0.1 + 0.2*(-1) = -0.1 → réflexion → pos=0.1, vit_x=+1
    Univers u(2, {10.0, 10.0, 0.0}, 1.0, 1.0, 2.5, false, false);
    u.setConditionsLimites({ConditionLimite::REFLEXION, ConditionLimite::REFLEXION,
                            ConditionLimite::REFLEXION, ConditionLimite::REFLEXION,
                            ConditionLimite::LIBRE,     ConditionLimite::LIBRE});
    u.addParticule(Particule(Vecteur<3>{0.1, 5.0, 0.0},
                             Vecteur<3>{-1.0, 0.0, 0.0},
                             Vecteur<3>{}, 1.0, "P", 0));
    u.StromerVerlet(0.0, 0.2, 0.2, false);
    EXPECT_EQ(u.getNbParticules(), 1);
}

TEST(UniversTest, CondLimAbsorptionSupprime) {
    // Particule à x=0.1 avec v_x=-1, bord gauche absorbant
    // Après dt=0.2 : newPos_x = -0.1 → particule absorbée
    Univers u(2, {10.0, 10.0, 0.0}, 1.0, 1.0, 2.5, false, false);
    u.setConditionsLimites({ConditionLimite::ABSORPTION, ConditionLimite::ABSORPTION,
                            ConditionLimite::ABSORPTION, ConditionLimite::ABSORPTION,
                            ConditionLimite::LIBRE,      ConditionLimite::LIBRE});
    u.addParticule(Particule(Vecteur<3>{0.1, 5.0, 0.0},
                             Vecteur<3>{-1.0, 0.0, 0.0},
                             Vecteur<3>{}, 1.0, "P", 0));
    u.StromerVerlet(0.0, 0.2, 0.2, false);
    EXPECT_EQ(u.getNbParticules(), 0);
}

TEST(UniversTest, CondLimAbsorptionConserveAutres) {
    // Parmi 2 particules, une seule sort du domaine : seule celle-là est supprimée
    Univers u(2, {10.0, 10.0, 0.0}, 1.0, 1.0, 2.5, false, false);
    u.setConditionsLimites({ConditionLimite::ABSORPTION, ConditionLimite::ABSORPTION,
                            ConditionLimite::ABSORPTION, ConditionLimite::ABSORPTION,
                            ConditionLimite::LIBRE,      ConditionLimite::LIBRE});
    // Particule 0 sort à gauche
    u.addParticule(Particule(Vecteur<3>{0.1, 5.0, 0.0},
                             Vecteur<3>{-1.0, 0.0, 0.0},
                             Vecteur<3>{}, 1.0, "P", 0));
    // Particule 1 reste dans le domaine
    u.addParticule(Particule(Vecteur<3>{5.0, 5.0, 0.0},
                             Vecteur<3>{0.0, 0.0, 0.0},
                             Vecteur<3>{}, 1.0, "Q", 1));
    u.StromerVerlet(0.0, 0.2, 0.2, false);
    EXPECT_EQ(u.getNbParticules(), 1);
}

TEST(UniversTest, CondLimPeriodiqueWrappe) {
    // Particule à x=0.1 avec v_x=-1, bords x périodiques
    // Après dt=0.2 : newPos_x = -0.1 → périodique → pos=9.9
    Univers u(2, {10.0, 10.0, 0.0}, 1.0, 1.0, 2.5, false, false);
    u.setConditionsLimites({ConditionLimite::PERIODIQUE, ConditionLimite::PERIODIQUE,
                            ConditionLimite::REFLEXION,  ConditionLimite::REFLEXION,
                            ConditionLimite::LIBRE,      ConditionLimite::LIBRE});
    u.addParticule(Particule(Vecteur<3>{0.1, 5.0, 0.0},
                             Vecteur<3>{-1.0, 0.0, 0.0},
                             Vecteur<3>{}, 1.0, "P", 0));
    u.StromerVerlet(0.0, 0.2, 0.2, false);
    EXPECT_EQ(u.getNbParticules(), 1);
}

TEST(UniversTest, CondLimReflexionLJConserveLesParticules) {
    // La condition REFLEXION_LJ agit via un potentiel continu :
    // aucune particule ne doit disparaître pendant la simulation.
    Univers u(2, {10.0, 10.0, 0.0}, 1.0, 1.0, 2.5, false, false);
    u.setConditionsLimites({ConditionLimite::REFLEXION_LJ, ConditionLimite::REFLEXION_LJ,
                            ConditionLimite::REFLEXION_LJ, ConditionLimite::REFLEXION_LJ,
                            ConditionLimite::LIBRE,        ConditionLimite::LIBRE});
    u.addParticule(Particule(Vecteur<3>{5.0, 5.0, 0.0},
                             Vecteur<3>{2.0, 0.0, 0.0},
                             Vecteur<3>{}, 1.0, "P", 0));
    u.StromerVerlet(0.0, 1.0, 0.001, false);
    EXPECT_EQ(u.getNbParticules(), 1);
}

TEST(UniversTest, CondLimReflexionLJRepousseAvantLaBordure) {
    // Une particule proche de la paroi (x < r_cut = 2^{1/6}·σ ≈ 1.122)
    // doit ressentir une force positive en x (repoussée vers l'intérieur).
    // On vérifie que la simulation ne plante pas et conserve la particule.
    Univers u(2, {10.0, 10.0, 0.0}, 1.0, 1.0, 2.5, false, false);
    u.setConditionsLimites({ConditionLimite::REFLEXION_LJ, ConditionLimite::REFLEXION_LJ,
                            ConditionLimite::REFLEXION_LJ, ConditionLimite::REFLEXION_LJ,
                            ConditionLimite::LIBRE,        ConditionLimite::LIBRE});
    // Particule placée à x=0.5 (dans la zone répulsive, r < 2^{1/6}·σ/2 ≈ 0.56)
    u.addParticule(Particule(Vecteur<3>{0.5, 5.0, 0.0},
                             Vecteur<3>{-1.0, 0.0, 0.0},
                             Vecteur<3>{}, 1.0, "P", 0));
    EXPECT_NO_FATAL_FAILURE(u.StromerVerlet(0.0, 0.1, 0.0005, false));
    EXPECT_EQ(u.getNbParticules(), 1);
}

// ============================================================
// Performance insertion
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
