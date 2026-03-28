#ifndef VECTEUR_H
#define VECTEUR_H

/**
 * @file Vecteur.hxx
 * @brief Déclaration de la classe template Vecteur<N>
 *
 * Vecteur<N> représente un vecteur de dimension N à composantes réelles (double).
 * La dimension est un paramètre template entier (défaut N=3).
 * Toutes les opérations arithmétiques usuelles sont définies comme opérateurs membres
 * ou libres, ainsi que des fonctions utilitaires (norme, produit scalaire, distance).
 */

#include <array>
#include <algorithm>
#include <cmath>
#include <iostream>

/**
 * @brief Vecteur de dimension N à composantes doubles
 * @tparam N dimension du vecteur (défaut : 3)
 */
template<int N = 3>
class Vecteur {
private:
    std::array<double, N> coords; ///< tableau interne des coordonnées

public:
    /// Dimension du vecteur, accessible statiquement
    static constexpr int dim = N;

    /// @name Constructeurs
    /// @{

    /** @brief Construit le vecteur nul (toutes composantes à 0) */
    Vecteur() : coords{} {}

    /**
     * @brief Construit un vecteur à partir d'une liste d'initialisation
     * @param coord liste de coordonnées (longueur ≤ N ; composantes non fournies restent à 0)
     */
    Vecteur(std::initializer_list<double> coord) {
        std::copy(coord.begin(), coord.end(), coords.begin());
    }

    /**
     * @brief Construit un vecteur à partir d'un tableau C
     * @param v pointeur vers un tableau d'au moins N doubles
     */
    Vecteur(const double* v) {
        std::copy(v, v + N, coords.begin());
    }

    /// @}

    /// @name Accesseurs
    /// @{

    /**
     * @brief Retourne le tableau interne des coordonnées (lecture seule)
     * @return référence constante vers std::array<double, N>
     */
    const std::array<double, N>& getCoords() const { return coords; }

    /**
     * @brief Retourne la dimension du vecteur
     * @return N
     */
    int getDim() const { return N; }

    /// @}

    /// @name Opérations vectorielles
    /// @{

    /**
     * @brief Calcule la norme au carré
     * @return somme des carrés des composantes
     */
    double norm2() const {
        double s = 0.0;
        for (int i = 0; i < N; ++i) s += coords[i] * coords[i];
        return s;
    }

    /**
     * @brief Calcule la norme euclidienne
     * @return sqrt(norm2())
     */
    double norm() const { return std::sqrt(norm2()); }

    /**
     * @brief Calcule le produit scalaire avec un autre vecteur
     * @param v vecteur avec lequel effectuer le produit scalaire
     * @return somme des produits composante par composante
     */
    double dot(const Vecteur& v) const {
        double s = 0.0;
        for (int i = 0; i < N; ++i) s += coords[i] * v[i];
        return s;
    }

    /// @}

    /// @name Opérateurs d'accès
    /// @{

    /**
     * @brief Accès en lecture/écriture à la i-ème composante
     * @param i indice (0-indexé)
     * @return référence vers la composante
     */
    double& operator[](int i)       { return coords[i]; }

    /**
     * @brief Accès en lecture seule à la i-ème composante
     * @param i indice (0-indexé)
     * @return valeur de la composante
     */
    double  operator[](int i) const { return coords[i]; }

    /// @}

    /// @name Opérateurs arithmétiques composés
    /// @{

    /**
     * @brief Addition avec affectation
     * @param v vecteur à ajouter
     * @return référence vers *this
     */
    Vecteur& operator+=(const Vecteur& v) {
        for (int i = 0; i < N; ++i) coords[i] += v[i];
        return *this;
    }

    /**
     * @brief Soustraction avec affectation
     * @param v vecteur à soustraire
     * @return référence vers *this
     */
    Vecteur& operator-=(const Vecteur& v) {
        for (int i = 0; i < N; ++i) coords[i] -= v[i];
        return *this;
    }

    /**
     * @brief Multiplication par un scalaire avec affectation
     * @param a scalaire multiplicateur
     * @return référence vers *this
     */
    Vecteur& operator*=(double a) {
        for (int i = 0; i < N; ++i) coords[i] *= a;
        return *this;
    }

    /**
     * @brief Division par un scalaire avec affectation
     * @param a scalaire diviseur (non nul)
     * @return référence vers *this
     */
    Vecteur& operator/=(double a) {
        for (int i = 0; i < N; ++i) coords[i] /= a;
        return *this;
    }

    /**
     * @brief Égalité exacte composante par composante
     * @param v vecteur à comparer
     * @return true si tous les composantes sont identiques
     */
    bool operator==(const Vecteur& v) const { return coords == v.coords; }

    /**
     * @brief Inégalité composante par composante
     * @param v vecteur à comparer
     * @return true si au moins une composante diffère
     */
    bool operator!=(const Vecteur& v) const { return coords != v.coords; }

    /// @}

    /// @name Affichage
    /// @{

    /**
     * @brief Affiche le vecteur au format (x, y, z)
     * @param os flux de sortie
     * @param v  vecteur à afficher
     * @return référence vers os
     */
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

/**
 * @brief Addition de deux vecteurs
 * @param v1 premier vecteur
 * @param v2 second vecteur
 * @return v1 + v2
 */
template<int N>
Vecteur<N> operator+(Vecteur<N> v1, const Vecteur<N>& v2) { return v1 += v2; }

/**
 * @brief Soustraction de deux vecteurs
 * @param v1 premier vecteur
 * @param v2 second vecteur
 * @return v1 - v2
 */
template<int N>
Vecteur<N> operator-(Vecteur<N> v1, const Vecteur<N>& v2) { return v1 -= v2; }

/**
 * @brief Multiplication d'un vecteur par un scalaire (v * a)
 * @param v vecteur
 * @param a scalaire
 * @return v * a
 */
template<int N>
Vecteur<N> operator*(Vecteur<N> v, double a) { return v *= a; }

/**
 * @brief Multiplication d'un scalaire par un vecteur (a * v)
 * @param a scalaire
 * @param v vecteur
 * @return a * v
 */
template<int N>
Vecteur<N> operator*(double a, Vecteur<N> v) { return v *= a; }

/**
 * @brief Division d'un vecteur par un scalaire
 * @param v vecteur
 * @param a scalaire diviseur (non nul)
 * @return v / a
 */
template<int N>
Vecteur<N> operator/(Vecteur<N> v, double a) { return v /= a; }

/**
 * @brief Distance euclidienne entre deux vecteurs
 * @param v1 premier vecteur
 * @param v2 second vecteur
 * @return ||v1 - v2||
 */
template<int N>
double distance(const Vecteur<N>& v1, const Vecteur<N>& v2) {
    return (v1 - v2).norm();
}

/**
 * @brief Égalité approchée composante par composante
 * @param v1  premier vecteur
 * @param v2  second vecteur
 * @param eps tolérance absolue (défaut : 1e-10)
 * @return true si |v1[i] - v2[i]| < eps pour tout i
 */
template<int N>
bool approxEqual(const Vecteur<N>& v1, const Vecteur<N>& v2, double eps = 1e-10) {
    for (int i = 0; i < N; ++i)
        if (std::abs(v1[i] - v2[i]) > eps) return false;
    return true;
}

/// @}

#endif // VECTEUR_H
