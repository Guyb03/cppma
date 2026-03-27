#ifndef PARTICULE_H
#define PARTICULE_H

/**
 * @file Particule.h
 * @brief Déclaration de la classe Particule
 * 
 * La classe Particule représente une particule physique dans l'univers.
 * Elle contient sa position, sa vitesse, sa force appliquée, sa masse et ses propriétés
 * pour les interactions (Lennard-Jones, gravité).
 */

#include <vector>
#include <iostream>
#include "Vecteur.hxx"
#include <string>

/**
 * @class Particule
 * @brief Représente une particule dans un espace de dimension 1, 2 ou 3.
 * 
 * Une particule possède une position, une vitesse, une masse, une force actuelle et précédente,
 * un identifiant unique et des constantes liées aux interactions (epsilon, sigma, gravité G).
 */
class Particule{
public:
    /**
     * @brief Constructeur
     * 
     * @param pos Position initiale
     * @param v Vitesse initiale
     * @param m Masse
     * @param i Identifiant unique
     * @param f Force initiale
     * @param epsilon Constante de profondeur du potentiel de Lennard-Jones
     * @param sigma Distance à laquelle le potentiel est nul
     * @param G Constante gravitationnelle (si applicable)
     * @param n Nom/catégorie de la particule
     */
    Particule(const Vecteur &pos, const Vecteur &v, double m, int i, const Vecteur &f,
              double epsilon = 1, double sigma = 1, double G = -12, const std::string& n = "particule");
    /// @brief Retourne la position actuelle
    const Vecteur& getPosition() const;

    /// @brief Retourne la vitesse actuelle
    const Vecteur& getVitesse() const;

    /// @brief Retourne la masse de la particule
    const double& getMasse() const;

    /// @brief Retourne la force actuelle
    const Vecteur& getForce() const;

    /// @brief Retourne la force de l'étape précédente
    const Vecteur& getFold() const;

    /// @brief Retourne l'identifiant de la particule
    int getId() const;

    /// @brief Retourne l'indice de cellule auquel appartient la particule
    int getCell() const;

    /// @brief Retourne la force gravitationnelle potentielle
    const Vecteur& getPotGrav() const;

    /// @brief Pointeur vers la position
    const Vecteur* getPointPos() const;

    /// @brief Pointeur vers la force
    const Vecteur* getPointForce() const;

    /// @brief Pointeur vers l'ancienne force
    const Vecteur* getPointFold() const;

    /// @brief Pointeur vers la vitesse
    const Vecteur* getPointVit() const;

    // === Setters ===

    void setForce(const Vecteur &f);
    void setVitesse(const Vecteur &v);
    void setPosition(const Vecteur &p);
    void setFold(const Vecteur &f);
    void setCell(int);

    /// @brief Inverse la vitesse dans une direction donnée
    void opposeVit(int ind);

    // === Opérations sur les vecteurs dynamiques ===

    void addForce(const Vecteur &f);
    void addPos(const Vecteur &f);
    void addVit(const Vecteur &v);
    void multVit(double beta);
    void resetForce();

    // === Calculs physiques ===

    /**
     * @brief Calcule la force gravitationnelle entre deux particules
     */
    Vecteur calcForce(const Particule &other) const;

    /**
     * @brief Calcule l'énergie cinétique de la particule
     */
    Vecteur calcEc() const;

    /**
     * @brief Calcule la valeur scalaire du potentiel de Lennard-Jones
     */
    double calcLennard(const Particule &other) const;

    /**
     * @brief Calcule la force de Lennard-Jones exercée par une autre particule
     */
    Vecteur calcForceLennard(const Particule &other) const;

    /**
     * @brief Retourne la valeur critique de r pour appliquer la réflexion
     */
    double rCutReflexive() const;

    /**
     * @brief Calcule la force de réflexion appliquée à une particule proche d'une frontière
     */
    Vecteur calcReflexion(double r, int dim, int coord, bool sens) const;

    /// @brief Comparaison de particules (utilisé pour les containers ordonnés)
    bool operator<(const Particule &other) const;

    /// @brief Compteur de copies de particules (utile pour les statistiques ou débogage)
    static int nbPartCopie;

private:
    Vecteur position;   ///< Position de la particule
    Vecteur vitesse;    ///< Vitesse actuelle
    double masse;       ///< Masse
    int id;             ///< Identifiant
    Vecteur force;      ///< Force actuelle
    std::string categorie; ///< Type ou catégorie
    Vecteur fold;       ///< Force de l'étape précédente
    double epsilon;     ///< Constante de Lennard-Jones
    double sigma;       ///< Constante de Lennard-Jones
    int cell;           ///< Index de cellule
    double G;           ///< Constante gravitationnelle
    Vecteur potGrav;    ///< Potentiel gravitationnel (pré-calculé ou constant)
};

/**
 * @brief Affichage d'une particule
 */
std::ostream & operator<<(std::ostream &out, const Particule &p);

#endif // PARTICULE_H
