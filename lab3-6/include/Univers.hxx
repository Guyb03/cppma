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

#include "Cellule.hxx"
#include "Particule.hxx"
#include <vector>
#include <array>
#include <iostream>
#include <string>

class Univers {
private:
    // domaine
    int dim;                             ///< Dimension active (1, 2 ou 3)
    std::array<double, 3> L;             ///< Longueurs caractéristiques Ld
    double epsilon;                      ///< Profondeur du potentiel de Lennard-Jones
    double sigma;                        ///< Interaction avec l'axe nul du potentiel
    double rcut;                         ///< Rayon de coupe
    bool useGrav;                        ///< Choix force gravitationnelle
    bool useLF;                          ///< Choix force Lennard Jones
    std::array<int, 3> nc;               ///< ncd = floor(Ld / rcut); nombre de cubes
    std::vector<Cellule> cellules;       ///< Collection de cellules
    std::vector<Particule> particules;   ///< Collection de particules

    /// @brief Retourne l'indice plat de la cellule contenant la position @param pos
    int celluleIdx(const Vecteur<3>& pos) const;

    /// @brief Retourne les indices des cellules voisines (y compris elle-même) de la cellule idx
    std::vector<int> voisinesIdx(int idx) const;

    /// @brief Force gravitationnelle exercée sur la particule i par la particule j
    Vecteur<3> forceGrav(size_t i, size_t j) const;

    /// @brief Force issue du potentiel de Lennard-Jones exercée sur la particule i par la particule j
    Vecteur<3> forceLJ(size_t i, size_t j) const;

    /// @brief Calcule la force totale sur chaque particule, résultat dans F
    void calcForces(std::vector<Vecteur<3>>& F) const;

public:

    /// @name Constructeur
    /// @{

    /** @brief Constructeur
     *  @param dim dimension de l'univers (1, 2 ou 3)
     *  @param L longueurs caractéristiques
     *  @param epsilon constante Lennard Jones
     *  @param sigma constante Lennard Jones
     *  @param rcut rayon de coupe pour interaction Lennard Jones
     *  @param useGrav choix d'utilisation de la gravité
     *  @param useLJ choix d'utilisation du potentiel de Lennard Jones
     */
    explicit Univers(int dim, std::array<double, 3> L, double epsilon, double sigma, double rcut, bool useGrav, bool useLJ);

    /// @}

    /// @name Accesseurs
    /// @{

    int getDim()         const;
    int getNbParticules() const;

    /// @}

    /// @name Gestion des particules
    /// @{

    /** @brief Ajoute une particule à l'univers et ajoute son indice dans la cellule la contenant */
    void addParticule(const Particule& p);

    /// @name Simulation
    /// @{

    /** @brief Intègre le mouvement par l'algorithme de Störmer-Verlet
     *  @param t_start  temps initial
     *  @param t_end    temps final
     *  @param dt       pas de temps
     *  @param afficher affiche l'état à chaque pas si true
     */
    void StromerVerlet(double t_start, double t_end, double dt,
                       bool afficher = true,
                       int vtkFreq = 0,
                       const std::string& vtkPrefix = "output");

    /** @brief Sauvegarde l'état courant dans un fichier VTK XML UnstructuredGrid (.vtu) */
    void sauvegarderVTK(const std::string& filename) const;

    /// @}

    /// @brief Affichage de l'état de l'univers (positions de chaque particule)
    friend std::ostream& operator<<(std::ostream& os, const Univers& u);
};

#endif // UNIVERS_H
