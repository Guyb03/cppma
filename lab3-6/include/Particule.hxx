#ifndef PARTICULE_H
#define PARTICULE_H

/**
 * @file Particule.hxx
 * @brief Déclaration de la classe Particule
 *
 * Représente une particule physique caractérisée par sa position, vitesse,
 * force, masse, catégorie et identifiant unique.
 * Les grandeurs vectorielles utilisent Vecteur<3>.
 */

#include "Vecteur.hxx"
#include <string>
#include <iostream>

/**
 * @brief Particule physique dans un espace 3D
 *
 * Encapsule l'ensemble des attributs physiques d'une particule : position,
 * vitesse, force appliquée, masse, catégorie (label) et identifiant entier unique.
 * L'identifiant sert de clé de tri (operator<) pour permettre l'usage dans std::set.
 */
class Particule {
private:
    Vecteur<> position;        ///< position courante de la particule (m)
    Vecteur<> vitesse;         ///< vitesse courante de la particule (m/s)
    Vecteur<> force;           ///< force totale appliquée à la particule (N)
    double masse;              ///< masse de la particule (kg)
    std::string categorie;     ///< label de la particule (ex. "bleu", "Soleil")
    long identifiant;          ///< identifiant unique (entier positif)

public:

    /// @name Constructeur
    /// @{

    /**
     * @brief Construit une particule avec tous ses attributs
     * @param pos position initiale
     * @param vit vitesse initiale
     * @param frc force initiale
     * @param m   masse (doit être > 0)
     * @param cat catégorie / label
     * @param id  identifiant unique
     */
    Particule(const Vecteur<>& pos, const Vecteur<>& vit, const Vecteur<>& frc,
              double m, const std::string& cat, long id);

    /// @}

    /// @name Opérateurs
    /// @{

    /**
     * @brief Ordre strict sur l'identifiant (requis pour std::set<Particule>)
     * @param other particule à comparer
     * @return true si this->identifiant < other.identifiant
     */
    bool operator<(const Particule& other) const;

    /// @}

    /// @name Accesseurs
    /// @{

    /**
     * @brief Retourne la position courante
     * @return référence constante vers le vecteur position
     */
    const Vecteur<>& getPosition()    const;

    /**
     * @brief Retourne la vitesse courante
     * @return référence constante vers le vecteur vitesse
     */
    const Vecteur<>& getVitesse()     const;

    /**
     * @brief Retourne la force totale courante
     * @return référence constante vers le vecteur force
     */
    const Vecteur<>& getForce()       const;

    /**
     * @brief Retourne la masse
     * @return masse en kg
     */
    double getMasse()                 const;

    /**
     * @brief Retourne la catégorie / label
     * @return référence constante vers la chaîne catégorie
     */
    const std::string& getCategorie() const;

    /**
     * @brief Retourne l'identifiant unique
     * @return identifiant entier
     */
    long getIdentifiant()             const;

    /// @}

    /// @name Mutateurs
    /// @{

    /**
     * @brief Modifie la position
     * @param pos nouvelle position
     */
    void setPosition(const Vecteur<>& pos);

    /**
     * @brief Modifie la vitesse
     * @param vit nouvelle vitesse
     */
    void setVitesse(const Vecteur<>& vit);

    /**
     * @brief Modifie la force
     * @param frc nouvelle force
     */
    void setForce(const Vecteur<>& frc);

    /**
     * @brief Modifie la masse
     * @param m nouvelle masse (doit être > 0)
     */
    void setMasse(double m);

    /// @}
};

/**
 * @brief Affiche une particule sur un flux (format lisible)
 * @param os flux de sortie
 * @param p  particule à afficher
 * @return référence vers os
 */
std::ostream& operator<<(std::ostream& os, const Particule& p);

#endif // PARTICULE_H
