#ifndef UNIVERS_H
#define UNIVERS_H

/**
 * @file Univers.hxx
 * @brief Déclaration de la classe Univers
 *
 * Gère une collection de particules en interaction gravitationnelle.
 * Implémente l'intégration temporelle par l'algorithme de Störmer-Verlet.
 * La dimension (1, 2 ou 3) est un paramètre runtime qui restreint les axes actifs.
 */

#include "Particule.hxx"
#include <vector>
#include <iostream>

class Univers {
private:
    int dim;                             ///< Dimension active (1, 2 ou 3)
    std::vector<Particule> particules;   ///< Collection de particules

    /// @brief Force gravitationnelle exercée sur la particule i par la particule j
    Vecteur<3> forceGrav(size_t i, size_t j) const;

    /// @brief Calcule la force totale sur chaque particule, résultat dans F
    void calcForces(std::vector<Vecteur<3>>& F) const;

public:

    /// @name Constructeur
    /// @{

    /** @brief Constructeur
     *  @param dim dimension de l'univers (1, 2 ou 3)
     */
    explicit Univers(int dim);

    /// @}

    /// @name Accesseurs
    /// @{

    int getDim()         const;
    int getNbParticules() const;

    /// @}

    /// @name Gestion des particules
    /// @{

    /** @brief Ajoute une particule à l'univers */
    void addParticule(const Particule& p);

    /// @}

    /// @name Simulation
    /// @{

    /** @brief Intègre le mouvement par l'algorithme de Störmer-Verlet
     *  @param t_start  temps initial
     *  @param t_end    temps final
     *  @param dt       pas de temps
     *  @param afficher affiche l'état à chaque pas si true
     */
    void StromerVerlet(double t_start, double t_end, double dt, bool afficher = true);

    /// @}

    /// @brief Affichage de l'état de l'univers (positions de chaque particule)
    friend std::ostream& operator<<(std::ostream& os, const Univers& u);
};

#endif // UNIVERS_H
