#ifndef CELLULE_H
#define CELLULE_H

/**
 * @file Cellule.hxx
 * @brief Déclaration de la classe Cellule
 *
 * Une Cellule représente une sous-région cubique de l'Univers utilisée pour
 * accélérer le calcul des interactions à courte portée (méthode de la linked-cell list).
 * Elle stocke les indices (dans Univers::particules) des particules qu'elle contient,
 * ainsi que la position de son centre.
 */

#include "Vecteur.hxx"
#include <vector>

/**
 * @brief Sous-région de l'Univers pour la linked-cell list
 *
 * Chaque Cellule conserve les indices des particules qui se trouvent dans sa région.
 * Elle ne possède pas les particules : seuls les indices sont stockés afin d'éviter
 * toute copie inutile.
 */
class Cellule {
private:
    std::vector<int> indices; ///< indices dans Univers::particules des particules présentes
    Vecteur<>        centre;  ///< position du centre géométrique de la cellule

public:

    /// @name Constructeur
    /// @{

    /**
     * @brief Construit une cellule avec un centre donné
     * @param centre position du centre de la cellule (défaut : origine)
     */
    explicit Cellule(const Vecteur<>& centre = Vecteur<>{});

    /// @}

    /// @name Accesseurs
    /// @{

    /**
     * @brief Retourne la position du centre de la cellule
     * @return référence constante vers le vecteur centre
     */
    const Vecteur<>&        getCentre()  const;

    /**
     * @brief Retourne la liste des indices des particules contenues
     * @return référence constante vers le vecteur d'indices
     */
    const std::vector<int>& getIndices() const;

    /// @}

    /// @name Gestion des particules
    /// @{

    /**
     * @brief Ajoute l'indice d'une particule dans la cellule
     * @param idx indice de la particule dans Univers::particules
     */
    void addParticule(int idx);

    /**
     * @brief Vide la liste des indices (la cellule ne contient plus aucune particule)
     *
     * Appelée à chaque pas de temps pour réinitialiser la grille avant de
     * réaffecter toutes les particules à leurs nouvelles cellules.
     */
    void vider();

    /// @}
};

#endif // CELLULE_H
