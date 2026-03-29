# SimUnivers — Simulation moléculaire C++

Projet de simulation de dynamique moléculaire (MD) en 1D/2D/3D avec potentiel de Lennard-Jones,
gravitation newtonienne et conditions aux limites configurables.
Développé dans le cadre du cours **C++ pour les mathématiques appliquées** (MMIS, Grenoble INP).

---

## Structure du projet

```
.
├── include/          # En-têtes : Vecteur, Particule, Cellule, Univers
├── src/              # Implémentations (.cxx) + CMakeLists de la bibliothèque
├── test/             # Tests Google Test (testVecteur, testParticule, testCellule, testUnivers)
├── demo/             # Programmes de démonstration (simu2D, ...)
├── doc/              # Documentation générée par Doxygen (html/)
├── Doxyfile          # Configuration Doxygen
├── afficheSimuCsv.py # Visualisation CSV par animation matplotlib
├── LennardJones.py   # Tracé du potentiel LJ
├── CMakeLists.txt    # Racine CMake
└── labs/             # Exercices préliminaires du cours
    ├── lab1/         #   Intégration numérique, tracé de trajectoires
    └── lab2/         #   Profiling et optimisation
```

---

## Prérequis

| Outil | Version minimale |
|-------|-----------------|
| CMake | 3.20 |
| Compilateur C++ | C++17 (GCC ≥ 9, Clang ≥ 10) |
| Doxygen | 1.9 (documentation) |
| Python 3 + matplotlib + pandas | (visualisation CSV) |
| Paraview | (visualisation VTK) |

Google Test est téléchargé automatiquement par CMake via `FetchContent`.

---

## Compilation

```bash
cmake -S . -B build
cmake --build build -j$(nproc)
```

Les artefacts produits dans `build/` :
- `build/src/libparticules.a` — bibliothèque statique
- `build/test/testVecteur`, `testParticule`, `testCellule`, `testUnivers` — exécutables de tests
- `build/demo/simu2D` — démonstration collision 2D

---

## Exécuter les tests

```bash
# Tous les tests
cd build && ctest --output-on-failure

# Une suite précise
cd build && ctest --output-on-failure -R UniversTest

# Ou directement l'exécutable pour voir le détail Google Test
./build/test/testUnivers
./build/test/testVecteur
./build/test/testParticule
./build/test/testCellule
```

---

## Démonstrations

### Collision 2D (Lennard-Jones)

Simule la collision entre un rectangle de particules au repos et un carré en chute.
Les fichiers VTK sont écrits dans `vtk/` toutes les 1000 itérations.

```bash
mkdir -p vtk
./build/demo/simu2D
```

### Oscillation LJ (2 particules)

Produit une sortie CSV sur stdout qu'on redirige vers un fichier.

```bash
./build/demo/simuLJ > simuLJ.csv
```

---

## Conditions aux limites

Après construction d'un `Univers`, appeler `setConditionsLimites` avec un tableau
de 6 valeurs de type `ConditionLimite` dans l'ordre `[xmin, xmax, ymin, ymax, zmin, zmax]` :

| Valeur | Comportement |
|--------|-------------|
| `ConditionLimite::LIBRE` | aucune contrainte (défaut) |
| `ConditionLimite::REFLEXION` | réflexion spéculaire — la composante de vitesse perpendiculaire est inversée |
| `ConditionLimite::ABSORPTION` | la particule est supprimée au contact du bord |
| `ConditionLimite::PERIODIQUE` | transport vers le bord opposé (appliquer en paire xmin/xmax) |

Exemple :
```cpp
Univers u(2, {100.0, 50.0, 0.0}, 1.0, 1.0, 2.5, false, true);
u.setConditionsLimites({
    ConditionLimite::PERIODIQUE, ConditionLimite::PERIODIQUE,  // x : périodique
    ConditionLimite::REFLEXION,  ConditionLimite::REFLEXION,   // y : réflexif
    ConditionLimite::LIBRE,      ConditionLimite::LIBRE        // z : inactif
});
```

---

## Générer la documentation

```bash
doxygen Doxyfile
xdg-open doc/html/index.html   # Linux
```

---

## Visualiser les résultats

### Fichiers VTK dans Paraview

1. Ouvrir Paraview.
2. **File › Open** → sélectionner `vtk/simu2D_*.vtu` (Paraview les regroupe automatiquement en série).
3. Cliquer **Apply**, puis **Play** pour l'animation.
4. Colorer les particules par le champ `Masse` ou la norme de `Velocity` via le menu de rendu.

### Trajectoires CSV avec matplotlib

```bash
./build/demo/simuLJ > simuLJ.csv
python3 afficheSimuCsv.py simuLJ.csv
```

---

## Résumé des commandes

```bash
# 1. Compiler
cmake -S . -B build && cmake --build build -j$(nproc)

# 2. Tests
cd build && ctest --output-on-failure && cd ..

# 3. Démo VTK
mkdir -p vtk && ./build/demo/simu2D

# 4. Démo CSV
./build/demo/simuLJ > simuLJ.csv && python3 afficheSimuCsv.py simuLJ.csv

# 5. Documentation
doxygen Doxyfile && xdg-open doc/html/index.html
```
