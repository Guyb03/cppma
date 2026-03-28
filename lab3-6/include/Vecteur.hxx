#ifndef VECTEUR_H
#define VECTEUR_H

/**
 * @file Vecteur.hxx
 * @brief Déclaration de la classe Vecteur
 *
 * Elle utilise un template pour sauvegarder la dimension du vecteur et implémente toutes
 * les méthodes utiles à la classe
 */

#include <array>
#include <algorithm>
#include <cmath>
#include <iostream>

template<int N = 3>
class Vecteur {
private:
    std::array<double, N> coords;

public:
    static constexpr int dim = N;

    /// @name Constructeurs
    /// @{

    /** @brief Constructeur zéro */
    Vecteur() : coords{} {}

    /** @brief Constructeur par liste d'initialisation
     *  @param coord liste de coordonnées
     */
    Vecteur(std::initializer_list<double> coord) {
        std::copy(coord.begin(), coord.end(), coords.begin());
    }

    /** @brief Constructeur à partir d'un pointeur
     *  @param v pointeur vers un tableau d'au moins N doubles
     */
    Vecteur(const double* v) {
        std::copy(v, v + N, coords.begin());
    }

    /// @}

    /// @name Accesseurs
    /// @{

    /** @brief Retourne les coordonnées du vecteur */
    const std::array<double, N>& getCoords() const { return coords; }

    /** @brief Retourne la dimension du vecteur */
    int getDim() const { return N; }

    /// @}

    /// @name Opérations vectorielles
    /// @{

    /** @brief Norme au carré */
    double norm2() const {
        double s = 0.0;
        for (int i = 0; i < N; ++i) s += coords[i] * coords[i];
        return s;
    }

    /** @brief Norme euclidienne */
    double norm() const { return std::sqrt(norm2()); }

    /** @brief Produit scalaire
     *  @param v vecteur avec lequel calculer le produit scalaire
     */
    double dot(const Vecteur& v) const {
        double s = 0.0;
        for (int i = 0; i < N; ++i) s += coords[i] * v[i];
        return s;
    }

    /// @}

    /// @name Opérateurs
    /// @{

    /** @brief Accès à la i-ème composante */
    double& operator[](int i)       { return coords[i]; }
    double  operator[](int i) const { return coords[i]; }

    /** @brief Somme interne avec vecteur */
    Vecteur& operator+=(const Vecteur& v) {
        for (int i = 0; i < N; ++i) coords[i] += v[i];
        return *this;
    }

    /** @brief Soustraction interne avec vecteur */
    Vecteur& operator-=(const Vecteur& v) {
        for (int i = 0; i < N; ++i) coords[i] -= v[i];
        return *this;
    }

    /** @brief Multiplication scalaire interne */
    Vecteur& operator*=(double a) {
        for (int i = 0; i < N; ++i) coords[i] *= a;
        return *this;
    }

    /** @brief Division scalaire interne */
    Vecteur& operator/=(double a) {
        for (int i = 0; i < N; ++i) coords[i] /= a;
        return *this;
    }

    bool operator==(const Vecteur& v) const { return coords == v.coords; }
    bool operator!=(const Vecteur& v) const { return coords != v.coords; }

    /// @}

    /// @name Affichage
    /// @{

    friend std::ostream& operator<<(std::ostream& os, const Vecteur& v) {
        os << "(";
        for (int i = 0; i < N; ++i) {
            os << v[i];
            if (i < N - 1) os << ", ";
        }
        os << ")";
        return os;
    }

    /// @}
};

/// @name Opérateurs binaires libres
/// @{

/** @brief Addition de deux vecteurs */
template<int N>
Vecteur<N> operator+(Vecteur<N> v1, const Vecteur<N>& v2) { return v1 += v2; }

/** @brief Soustraction de deux vecteurs */
template<int N>
Vecteur<N> operator-(Vecteur<N> v1, const Vecteur<N>& v2) { return v1 -= v2; }

/** @brief Multiplication d'un vecteur par un scalaire (v * a) */
template<int N>
Vecteur<N> operator*(Vecteur<N> v, double a) { return v *= a; }

/** @brief Multiplication d'un scalaire par un vecteur (a * v) */
template<int N>
Vecteur<N> operator*(double a, Vecteur<N> v) { return v *= a; }

/** @brief Division d'un vecteur par un scalaire */
template<int N>
Vecteur<N> operator/(Vecteur<N> v, double a) { return v /= a; }

/** @brief Distance euclidienne entre deux vecteurs
 *  @param v1 premier vecteur
 *  @param v2 second vecteur
 */
template<int N>
double distance(const Vecteur<N>& v1, const Vecteur<N>& v2) {
    return (v1 - v2).norm();
}

/** @brief Comparaison avec tolérance
 *  @param v1  premier vecteur
 *  @param v2  second vecteur
 *  @param eps tolérance absolue (défaut : 1e-10)
 *  @return true si toutes les composantes diffèrent de moins de eps
 */
template<int N>
bool approxEqual(const Vecteur<N>& v1, const Vecteur<N>& v2, double eps = 1e-10) {
    for (int i = 0; i < N; ++i)
        if (std::abs(v1[i] - v2[i]) > eps) return false;
    return true;
}

/// @}

#endif // VECTEUR_H
