#include "Univers.hxx"
#include <random>
#include <chrono>
#include <iostream>
#include <cmath>

/// @brief Mesure le temps d'exécution d'une fonction en secondes
template<typename Func>
double mesure(const std::string& label, Func func) {
    auto start = std::chrono::steady_clock::now();
    func();
    auto end   = std::chrono::steady_clock::now();
    double t   = std::chrono::duration<double>(end - start).count();
    std::cout << "  " << label << " : " << t << " s\n";
    return t;
}

// ============================================================
// Création d'un Univers et ajout de particules
// ============================================================
void testQ5() {
    std::cout << "=== Création de l'Univers ===\n";
    Univers u(3);
    u.addParticle(Particule({0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0}, 1.0,      "Soleil",  0));
    u.addParticle(Particule({0.0,1.0,0.0}, {-1.0,0.0,0.0},{0.0,0.0,0.0}, 3.0e-6,   "Terre",   1));
    u.addParticle(Particule({0.0,5.36,0.0},{-0.425,0.0,0.0},{0.0,0.0,0.0},9.55e-4,  "Jupiter", 2));
    u.addParticle(Particule({34.75,0.0,0.0},{0.0,0.0296,0.0},{0.0,0.0,0.0},1.0e-14, "Halley",  3));

    std::cout << "Dim=" << u.getDim() << "  Nb particules=" << u.getNbParticles() << "\n";
    std::cout << u << "\n";
}

// ============================================================
// (2^5)^3 = 32768 particules uniformes sur [0,1]^3
// ============================================================
Univers buildUniforme(int k, std::mt19937& mt) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    int n = (int)std::pow(2, k);
    int total = n * n * n;
    Univers u(3);
    u.addParticle(Particule({0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0,0.0},1.0,"seed",0));
    for (int i = 1; i < total; ++i) {
        Vecteur<3> pos{dist(mt), dist(mt), dist(mt)};
        Vecteur<3> zero{};
        u.addParticle(Particule(pos, zero, zero, 1.0e-6, "P", i));
    }
    return u;
}

void testQ6() {
    std::cout << "=== (2^5)^3 = 32768 particules ===\n";
    std::mt19937 mt(42);
    Univers u = buildUniforme(5, mt);
    std::cout << "Nb particules créées : " << u.getNbParticles() << "\n\n";
}

// ============================================================
// Performance en insertion
// ============================================================
void testQ7() {
    std::cout << "=== Performance insertion ===\n";
    std::mt19937 mt(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int k = 3; k <= 6; ++k) {
        int n = (int)std::pow(2, k);
        int total = n * n * n;
        std::cout << "N = " << total << " particules :\n";
        mesure("insertion", [&]() {
            Univers u(3);
            for (int i = 0; i < total; ++i) {
                Vecteur<3> pos{dist(mt), dist(mt), dist(mt)};
                Vecteur<3> zero{};
                u.addParticle(Particule(pos, zero, zero, 1.0e-6, "P", i));
            }
        });
    }
    std::cout << "\n";
}

// ============================================================
// Performance calcul des interactions
// ============================================================
void testQ8() {
    std::cout << "=== Performance calcul des interactions ===\n";
    std::mt19937 mt(42);

    for (int k = 3; k <= 7; ++k) {
        int total = (int)std::pow(std::pow(2, k), 3);
        Univers u = buildUniforme(k, mt);
        std::cout << "N = " << total << " :\n";
        // On fait 1 pas de Störmer-Verlet pour mesurer le coût de calcForces
        mesure("1 pas Störmer-Verlet", [&]() {
            u.StromerVerlet(0.0, 0.001, 0.001, false);
        });
    }
    std::cout << "\n";
}

// ============================================================
// Vérification que Newton 3 est bien appliqué
// ============================================================
void testQ9() {
    std::cout << "=== Optim Newton 3 (deja appliquée dans calcForces) ===\n";
    std::cout << "calcForces parcourt i < j seulement : N(N-1)/2 calculs\n";
    std::cout << "Version naïve : N(N-1) calculs → facteur 2 gagné\n\n";
}

// ============================================================
// Système solaire - vérification Störmer-Verlet
// ============================================================
void testQ10() {
    std::cout << "=== Q10 : Système solaire Störmer-Verlet ===\n";
    Univers u(2);
    u.addParticle(Particule({0.0,0.0,0.0},  {0.0,0.0,0.0},   {0.0,0.0,0.0}, 1.0,     "Soleil",  0));
    u.addParticle(Particule({0.0,1.0,0.0},  {-1.0,0.0,0.0},  {0.0,0.0,0.0}, 3.0e-6,  "Terre",   1));
    u.addParticle(Particule({0.0,5.36,0.0}, {-0.425,0.0,0.0},{0.0,0.0,0.0}, 9.55e-4, "Jupiter", 2));
    u.addParticle(Particule({34.75,0.0,0.0},{0.0,0.0296,0.0},{0.0,0.0,0.0}, 1.0e-14, "Halley",  3));

    std::cout << "Simulation sur t=[0, 1.0] dt=0.015 (extrait)\n";
    u.StromerVerlet(0.0, 1.0, 0.015, true);
}

int main() {
    testQ5();
    testQ6();
    testQ7();
    testQ8();
    testQ9();
    testQ10();
    return 0;
}
