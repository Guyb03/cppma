#ifndef PARTICULE_H
#define PARTICULE_H

/**
 * @file Particule.hxx
 * @brief Déclaration de la classe Particule
 *
 * Représente une particule physique caractérisée par sa position, vitesse,
 * force, masse, catégorie et identifiant.
 * Utilise Vecteur<3> pour les grandeurs vectorielles.
 */

#include "Vecteur.hxx"
#include <string>
#include <iostream>

class Particule {
private:
    Vecteur<> position;        ///< position de la particule
    Vecteur<> vitesse;         ///< vitesse de la particule
    Vecteur<> force;           ///< force appliquée à la particule
    double masse;              ///< masse de la particule
    std::string categorie;     ///< catégorie de la particule
    long identifiant;          ///< identifiant unique de la particule

public:

    /// @name Constructeur
    /// @{

    /** @brief Constructeur
     *  @param pos  position initiale
     *  @param vit  vitesse initiale
     *  @param frc  force initiale
     *  @param m    masse
     *  @param cat  catégorie
     *  @param id   identifiant unique
     */
    Particule(const Vecteur<>& pos, const Vecteur<>& vit, const Vecteur<>& frc,
              double m, const std::string& cat, long id);

    /// @}

    /// @name Opérateurs
    /// @{

    /** @brief Comparaison par identifiant (nécessaire pour std::set<Particule>) */
    bool operator<(const Particule& other) const;

    /// @}

    /// @name Accesseurs
    /// @{

    /// @brief Retourne la position
    const Vecteur<>& getPosition()  const;

    /// @brief Retourne la vitesse
    const Vecteur<>& getVitesse()   const;

    /// @brief Retourne la force courante
    const Vecteur<>& getForce()     const;

    /// @brief Retourne la masse
    double getMasse()               const;

    /// @brief Retourne la catégorie
    const std::string& getCategorie() const;

    /// @brief Retourne l'identifiant
    long getIdentifiant()           const;

    /// @}

    /// @name Mutateurs
    /// @{

    /** @brief Modifie la position */
    void setPosition(const Vecteur<>& pos);

    /** @brief Modifie la vitesse */
    void setVitesse(const Vecteur<>& vit);

    /** @brief Modifie la force */
    void setForce(const Vecteur<>& frc);

    /** @brief Modifie la masse */
    void setMasse(double m);

    /// @}
};

/// @brief Affichage d'une particule
std::ostream& operator<<(std::ostream& os, const Particule& p);

#endif // PARTICULE_H
