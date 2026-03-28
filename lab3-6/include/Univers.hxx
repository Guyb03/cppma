#ifndef UNIVERS_H
#define UNIVERS_H

/**
 * @file Univers.hxx
 * @brief Déclaration de la classe Univers
 *
 * Univers gère une collection de particules en interaction (gravitationnelle
 * et/ou Lennard-Jones) dans un domaine rectangulaire de dimension 1, 2 ou 3.
 *
 * L'intégration temporelle est assurée par l'algorithme de Störmer-Verlet.
 * Les interactions à courte portée sont accélérées par une linked-cell list
 * (grille de cellules de taille rcut).
 *
 * La sauvegarde de l'état peut se faire en format CSV (stdout) ou en série de
 * fichiers VTK XML UnstructuredGrid (.vtu) exploitables avec Paraview.
 */

#include "Cellule.hxx"
#include "Particule.hxx"
#include <vector>
#include <array>
#include <iostream>
#include <string>

/**
 * @brief Système de particules en interaction dans un domaine borné
 *
 * Gère la grille de cellules, les forces (gravité et/ou Lennard-Jones),
 * l'intégration de Störmer-Verlet et la sauvegarde VTK.
 */
class Univers {
private:
    int                      dim;       ///< dimension active du domaine (1, 2 ou 3)
    std::array<double, 3>    L;         ///< longueurs caractéristiques Ld selon chaque axe
    double                   epsilon;   ///< profondeur du puits de potentiel Lennard-Jones
    double                   sigma;     ///< distance à laquelle le potentiel LJ est nul
    double                   rcut;      ///< rayon de coupure des interactions LJ
    bool                     useGrav;   ///< active la gravitation newtonienne
    bool                     useLF;     ///< active le potentiel de Lennard-Jones
    std::array<int, 3>       nc;        ///< nombre de cellules par axe (nc[d] = floor(L[d]/rcut))
    std::vector<Cellule>     cellules;  ///< grille de cellules (linked-cell list)
    std::vector<Particule>   particules;///< collection de toutes les particules

    /**
     * @brief Retourne l'indice plat de la cellule contenant une position donnée
     * @param pos position à localiser
     * @return indice dans le vecteur cellules
     */
    int celluleIdx(const Vecteur<3>& pos) const;

    /**
     * @brief Retourne les indices des cellules voisines d'une cellule (y compris elle-même)
     * @param idx indice de la cellule centrale
     * @return vecteur d'indices des cellules dans le voisinage 3×3×3
     */
    std::vector<int> voisinesIdx(int idx) const;

    /**
     * @brief Calcule la force gravitationnelle exercée sur la particule i par j
     * @param i indice de la particule subissant la force
     * @param j indice de la particule source
     * @return vecteur force (N)
     */
    Vecteur<3> forceGrav(size_t i, size_t j) const;

    /**
     * @brief Calcule la force de Lennard-Jones exercée sur la particule i par j
     * @param i indice de la particule subissant la force
     * @param j indice de la particule source
     * @return vecteur force (N)
     */
    Vecteur<3> forceLJ(size_t i, size_t j) const;

    /**
     * @brief Calcule la force totale sur chaque particule et la stocke dans F
     * @param F vecteur de forces (taille nbParticules), modifié en sortie
     */
    void calcForces(std::vector<Vecteur<3>>& F) const;

public:

    /// @name Constructeur
    /// @{

    /**
     * @brief Construit un univers vide avec les paramètres physiques donnés
     * @param dim     dimension active du domaine (1, 2 ou 3)
     * @param L       longueurs caractéristiques selon chaque axe
     * @param epsilon profondeur du puits Lennard-Jones (énergie)
     * @param sigma   distance d'équilibre du potentiel LJ (longueur)
     * @param rcut    rayon de coupure des interactions LJ
     * @param useGrav active la gravitation newtonienne entre particules
     * @param useLJ   active le potentiel de Lennard-Jones
     */
    explicit Univers(int dim, std::array<double, 3> L,
                     double epsilon, double sigma, double rcut,
                     bool useGrav, bool useLJ);

    /// @}

    /// @name Accesseurs
    /// @{

    /**
     * @brief Retourne la dimension active du domaine
     * @return entier 1, 2 ou 3
     */
    int getDim()          const;

    /**
     * @brief Retourne le nombre de particules dans l'univers
     * @return nombre de particules
     */
    int getNbParticules()  const;

    /// @}

    /// @name Gestion des particules
    /// @{

    /**
     * @brief Ajoute une particule à l'univers et l'inscrit dans la cellule correspondante
     * @param p particule à ajouter
     */
    void addParticule(const Particule& p);

    /// @}

    /// @name Simulation
    /// @{

    /**
     * @brief Intègre le mouvement des particules par l'algorithme de Störmer-Verlet
     *
     * À chaque pas de temps dt, les positions et vitesses sont mises à jour selon :
     * - x(t+dt) = x(t) + dt * v(t) + dt²/(2m) * F(t)
     * - v(t+dt) = v(t) + dt/(2m) * (F(t) + F(t+dt))
     *
     * La grille de cellules est reconstruite après chaque mise à jour des positions.
     *
     * @param t_start   temps initial de la simulation
     * @param t_end     temps final de la simulation
     * @param dt        pas de temps
     * @param afficher  si true, écrit une ligne CSV sur stdout à chaque pas
     * @param vtkFreq   fréquence de sauvegarde VTK en nombre de pas (0 = désactivé)
     * @param vtkPrefix préfixe des fichiers VTK générés (ex. "output" → output_000001.vtu)
     */
    void StromerVerlet(double t_start, double t_end, double dt,
                       bool afficher = true,
                       int vtkFreq = 0,
                       const std::string& vtkPrefix = "output");

    /**
     * @brief Sauvegarde l'état courant des particules dans un fichier VTK XML
     *
     * Le fichier généré est au format UnstructuredGrid (.vtu) et contient :
     * - les positions 3D de chaque particule (champ Position)
     * - les vitesses 3D (champ Velocity)
     * - les masses scalaires (champ Masse)
     *
     * Les fichiers peuvent être ouverts directement dans Paraview.
     *
     * @param filename chemin du fichier à créer (ex. "output_000001.vtu")
     */
    void sauvegarderVTK(const std::string& filename) const;

    /// @}

    /**
     * @brief Affiche les positions courantes de toutes les particules
     * @param os flux de sortie
     * @param u  univers à afficher
     * @return référence vers os
     */
    friend std::ostream& operator<<(std::ostream& os, const Univers& u);
};

#endif // UNIVERS_H
