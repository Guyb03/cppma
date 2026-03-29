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
 * @brief Types de conditions aux limites applicables sur chaque face du domaine
 *
 * Les six faces sont indexées [xmin, xmax, ymin, ymax, zmin, zmax].
 * Les conditions PERIODIQUE doivent être appliquées par paire (xmin/xmax ensemble).
 */
enum class ConditionLimite {
    LIBRE,        ///< aucune contrainte : la particule peut sortir du domaine
    REFLEXION,    ///< réflexion spéculaire : inversion instantanée de la composante de vitesse perpendiculaire
    ABSORPTION,   ///< absorption : la particule est supprimée dès qu'elle touche le bord
    PERIODIQUE,   ///< transport vers le bord opposé sans modification de trajectoire
    REFLEXION_LJ  ///< réflexion par potentiel LJ de paroi (r_cut = 2^{1/6}·σ), force continue intégrée dans Verlet
};

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
    double                   G;         ///< constante du champ gravitationnel uniforme (négatif = vers le bas)
    bool                     useGrav;   ///< active la gravitation newtonienne entre particules
    bool                     useLF;     ///< active le potentiel de Lennard-Jones
    bool                     usePotG;   ///< active le champ gravitationnel uniforme (F = m*G selon y)
    std::array<int, 3>       nc;        ///< nombre de cellules par axe (nc[d] = floor(L[d]/rcut))
    std::vector<Cellule>     cellules;  ///< grille de cellules (linked-cell list)
    std::vector<Particule>   particules;///< collection de toutes les particules

    /// Conditions aux limites par face : [xmin, xmax, ymin, ymax, zmin, zmax]
    std::array<ConditionLimite, 6> condLimites;

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
     * @brief Calcule la force gravitationnelle newtonienne exercée sur i par j
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
     * @brief Calcule la force du champ gravitationnel uniforme sur la particule i
     *
     * Applique un champ uniforme selon l'axe y : F = (0, m_i * G, 0).
     * G négatif correspond à une gravité vers le bas.
     * @param i indice de la particule
     * @return vecteur force (N)
     */
    Vecteur<3> potGrav(int i) const;

    /**
     * @brief Calcule la force totale sur chaque particule et la stocke dans F
     * @param F vecteur de forces (taille nbParticules), modifié en sortie
     */
    void calcForces(std::vector<Vecteur<3>>& F) const;

    /**
     * @brief Applique la condition aux limites à la particule i après mise à jour de sa position
     *
     * Corrige la position et la vitesse selon la condition de chaque face franchie.
     * Non utilisée pour REFLEXION_LJ (gérée via calcForcesParoi).
     * @param i indice de la particule
     * @return false si la particule est absorbée (doit être supprimée), true sinon
     */
    bool appliquerConditionParticule(int i);

    /**
     * @brief Ajoute les forces exercées par les parois en mode REFLEXION_LJ
     *
     * Pour chaque face configurée en REFLEXION_LJ, applique le potentiel :
     *   F_i = -24ε/(2r) · (σ/(2r))^6 · (1 - 2·(σ/(2r))^6)
     * avec r la distance à la paroi et r_cut = 2^{1/6}·σ.
     * La force est orientée perpendiculairement à la paroi, en s'en éloignant.
     * @param F vecteur de forces (taille nbParticules), incrémenté en sortie
     */
    void calcForcesParoi(std::vector<Vecteur<3>>& F) const;

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
     * @param G       constante du champ gravitationnel uniforme (négatif = vers le bas)
     * @param useGrav active la gravitation newtonienne entre particules
     * @param useLJ   active le potentiel de Lennard-Jones
     * @param usePotG active le champ gravitationnel uniforme (F = m*G selon y)
     */
    explicit Univers(int dim, std::array<double, 3> L,
                     double epsilon, double sigma, double rcut,
                     double G = 0.0, bool useGrav = false,
                     bool useLJ = false, bool usePotG = false);

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

    /// @name Conditions aux limites
    /// @{

    /**
     * @brief Définit les conditions aux limites sur chaque face du domaine
     *
     * @param conds tableau de 6 conditions dans l'ordre [xmin, xmax, ymin, ymax, zmin, zmax]
     */
    void setConditionsLimites(const std::array<ConditionLimite, 6>& conds);

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
     * Les conditions aux limites sont appliquées après la mise à jour des positions.
     * La grille de cellules est reconstruite après chaque mise à jour des positions.
     *
     * @param t_start      temps initial de la simulation
     * @param t_end        temps final de la simulation
     * @param dt           pas de temps
     * @param afficher     si true, écrit une ligne CSV sur stdout à chaque pas
     * @param vtkFreq      fréquence de sauvegarde VTK en nombre de pas (0 = désactivé)
     * @param vtkPrefix    préfixe des fichiers VTK générés (ex. "output" → output_000001.vtu)
     * @param targetEcin   énergie cinétique cible pour le thermostat (0 = désactivé)
     * @param ecFreq       fréquence de rescaling de l'énergie cinétique (en pas)
     * @param showProgress affiche une barre de progression sur stderr
     */
    void StromerVerlet(double t_start, double t_end, double dt,
                       bool afficher = true,
                       int vtkFreq = 0,
                       const std::string& vtkPrefix = "output",
                       double targetEcin = 0.0,
                       int ecFreq = 1000,
                       bool showProgress = false);

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
