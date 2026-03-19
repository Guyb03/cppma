#ifndef PARTICULE_H
#define PARTICULE_H

/**
 * @file Particule.h
 * @brief Déclaration de la classe Particule
 * 
 * Représente une particule physique avec sa position, vitesse, force, masse et catégorie.
 */
#include "Vecteur.hxx"
#include <array>
#include <string>

class Particule {
private:
    Vecteur<> position;        ///< position de la particule
    Vecteur<> vitesse;         ///< vitesse de la particule
    Vecteur<> force;           ///< force appliquée à la particule
    double masse;              ///< masse de la particule
    std::string categorie;     ///< catégorie de la particule
    long identifiant;          ///< identifiant unique de la particule

public:

    /// @name Constructeur / Destructeur
    /// @{

    /** @brief Constructeur
     *  @param position position initiale de la particule
     *  @param vitesse vitesse initiale de la particule
     *  @param force force initiale appliquée à la particule
     *  @param masse masse de la particule
     *  @param categorie catégorie de la particule
     *  @param identifiant identifiant unique de la particule
     */
    Particule(const Vecteur<>& position,
              const Vecteur<>& vitesse,
              const Vecteur<>& force,
              double masse, const std::string& categorie, long identifiant);

    /// @}

    /// @name Accesseurs
    /// @{

    /// @brief Retourne la position de la particule
    Vecteur<> getPosition() const;

    /// @brief Retourne la vitesse de la particule
    Vecteur<> getVitesse() const;

    /// @brief Retourne la force appliquée à la particule
    Vecteur<> getForce() const;

    /// @brief Retourne la masse de la particule
    double getMasse() const;

    /// @brief Retourne la catégorie de la particule
    const std::string& getCategorie() const;

    /// @brief Retourne l'identifiant de la particule
    long getIdentifiant() const;

    /// @}

    /// @name Mutateurs
    /// @{

    /** @brief Modifie la position de la particule
     *  @param position nouvelle position
     */
    void setPosition(const Vecteur<>& position);

    /** @brief Modifie la vitesse de la particule
     *  @param vitesse nouvelle vitesse
     */
    void setVitesse(const Vecteur<>& vitesse);

    /** @brief Modifie la force appliquée à la particule
     *  @param force nouvelle force
     */
    void setForce(const Vecteur<>& force);

    /** @brief Modifie la masse de la particule
     *  @param masse nouvelle masse
     */
    void setMasse(double masse);

    /// @}
};

#endif // PARTICULE_H