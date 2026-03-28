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
// (2^5)^3 = 32768 particules uniformes sur [0,1]^3
// ============================================================
Univers buildUniforme(int k, std::mt19937& mt) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    int n = (int)std::pow(2, k);
    int total = n * n * n;
    Univers u(3, {1.0, 1.0, 1.0}, 1.0, 1.0, 0.3, true, false);
    u.addParticule(Particule(Vecteur<3>{0.0,0.0,0.0},Vecteur<3>{0.0,0.0,0.0},Vecteur<3>{},1.0,"seed",0));
    for (int i = 1; i < total; ++i) {
        Vecteur<3> pos{dist(mt), dist(mt), dist(mt)};
        Vecteur<3> zero{};
        u.addParticule(Particule(pos, zero, zero, 1.0e-6, "P", i));
    }
    return u;
}

void testCreation() {
    std::cout << "=== (2^5)^3 = 32768 particules ===\n";
    std::mt19937 mt(42);
    Univers u = buildUniforme(5, mt);
    std::cout << "Nb particules créées : " << u.getNbParticules() << "\n\n";
}

// ============================================================
// Performance en insertion
// ============================================================
void testInsertion() {
    std::cout << "=== Performance insertion ===\n";
    std::mt19937 mt(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int k = 3; k <= 6; ++k) {
        int n = (int)std::pow(2, k);
        int total = n * n * n;
        std::cout << "N = " << total << " particules :\n";
        mesure("insertion", [&]() {
            Univers u(3, {1.0, 1.0, 1.0}, 1.0, 1.0, 0.3, true, false);
            for (int i = 0; i < total; ++i) {
                Vecteur<3> pos{dist(mt), dist(mt), dist(mt)};
                Vecteur<3> zero{};
                u.addParticule(Particule(pos, zero, zero, 1.0e-6, "P", i));
            }
        });
    }
    std::cout << "\n";
}

// ============================================================
// Performance calcul des interactions
// ============================================================
void testInteraction() {
    std::cout << "=== Performance calcul des interactions ===\n";
    std::mt19937 mt(42);

    for (int k = 3; k < 7; ++k) {
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

int main() {
    testCreation();
    testInsertion();
    testInteraction();
    return 0;
}
