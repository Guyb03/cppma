#ifndef CELLULE_H
#define CELLULE_H

/**
 * @file Cellule.hxx
 * @brief Déclaration de la classe Cellule
 * 
 * Représente une division de l'univers caractérisé par les indices des particules,
 * son centre
 */

#include "Vecteur.hxx"
#include <vector>

class Cellule {
private:
    std::vector<int> indices; ///< indices dans Univers::particules (pas de copies)
    Vecteur<>        centre;  ///< position du centre de la cellule

public:
    explicit Cellule(const Vecteur<>& centre = Vecteur<>{});

    const Vecteur<>&        getCentre()  const;
    const std::vector<int>& getIndices() const;
    void addParticule(int idx);
    void vider();
};

#endif // CELLULE_H