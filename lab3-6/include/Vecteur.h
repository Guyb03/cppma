#ifndef VECTEUR_H
#define VECTEUR_H

/**
 * @file Vecteur.h
 * @brief Déclaration de la classe Vecteur 
 * 
 * Elle utilise un template pour sauvegarder la dimension du vecteur et implémente toutes
 * les méthodes utiles à la classe
 *  
 */
#include<array>
#include<algorithm>
#include<iostream>

template<int N = 3>
class Vecteur {
private:
    std::array<double, N> coords;

public:
    /// @name Constructeurs
    /// @{

    /** @brief Costructeur zéro */
    Vecteur() : coords{} {}

    /** @brief Constructeur par liste d'initialisation
     *  @param list liste de coordonnées
     */
    Vecteur(std::initializer_list<double> coord) {
        std::copy(list.begin(), list.end(), coords.begin());
    }

    /**
     * @brief Constructeur à partir d'un pointeur
     * @param v pointeur vers un tableau d'au moins N doubles
     */
    Vecteur(const double* v) {
        std::copy(v, v + N, coords.begin());
    }

    /// @}

    /// @name Accesseurs
    /// @{

    /** @brief Retourne les coordonnées du vecteur
     *  @return référence constante sur le tableau de coordonnées
     */
    const std::array<double, N>& getCoords() const { return coords; }

    /**
     * @brief Retourne la taille du vecteur
     * @return entier naturel renseigné lors de l'initialisation du template
     */
    const int getDim() const { return N;}

    /// @}

    /// @name Opérateurs
    /// @{

    /** @brief Accès à la i-ème composante
     *  @param i indice de la composante (0 à N-1)
     *  @return référence sur la composante
     */
    double& operator[](int i) { return coords[i]; }

    /// @brief Version constante de operator[]
    double operator[](int i) const { return coords[i]; }

    /// @brief Affectation
    Vecteur& operator=(const Vecteur &v) const {
        for (int i = 0; i < N; ++i) {
            coords[i] = v[i];
        }
    }

    /**
     * @brief Multiplication scalaire
     * @param 
     */
    Vecteur& operator*=(double a) {
        for (int i=0; i<N; ++i){
            coords[i] *= a;
        }
        return *this
    }

    /**
     * @brief 
     * 
     */
    

    /**
     * @brief Somme
     * 
     */

    /// @}

};



#endif//VECTEUR_H