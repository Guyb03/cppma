# Lab 3

En reprenant la structure fournie, nous avons fait le choix de séparer les définitions des opérateurs, méthodes, constructeurs et destructeurs. Ainsi toutes les définitions des méthodes, opérateurs et attributs seront dans le fichier d'en-tête et les implémentations réelles seront dans le fichier C++.
Ce choix relève d'une implémentation classique d'un tel projet en C++ offrant ainsi une modularité appréciable en évitant les répétitions. Il convient cependant de s'assurer des différents attributs et méthodes définies dans le fichier h, en respectant à l'identique leur signature pour éviter les erreurs de compilation.

Pour ce qui est du choix de la structure pour les coordonnées de la classe vecteur, on a choisi le `array`, et pour gérer la dimension N du vecteur, on crée un template qui l'initie par défaut à 3. Ce choix impose la définition des méthodes de la classe vecteur directement dans le fichier header.
Pour créer le vecteur on utilise la méthode standarde copy. `std::copy` prend le pointeur de début et de fin de la source, puis le pointeur de destination. Ceci impose que la destination ait une capacité suffisante, d'où l'intérêt du template qui garantit la correspondance des dimensions à la compilation.

## Q5 — Classe Univers

La classe `Univers` encapsule une collection de particules dans un `std::vector<Particule>` et expose la méthode `StromerVerlet()` qui implémente l'algorithme d'intégration. La dimension (1, 2 ou 3) est un paramètre runtime : elle ne change pas les types mais permet de restreindre les axes actifs lors des affichages ou des conditions aux bords. Le choix de `std::vector` pour le conteneur interne est justifié par l'accès aléatoire O(1) nécessaire dans la double boucle de calcul des forces, et par la localité mémoire qui favorise le prefetch cache.

## Q6 — \((2^5)^3\) = 32 768 particules uniformes sur \([0,1]^3\)

Les positions sont tirées uniformément avec `std::uniform_real_distribution<double>(0.0, 1.0)` sur chaque axe, indépendamment. Toutes les particules ont la même vitesse initiale nulle (particules uniformes). La masse est fixée à 10⁻⁶ pour éviter des forces gravitationnelles dominantes entre particules de même ordre. Une particule centrale de masse 1 joue le rôle d'attracteur.

## Q7 — Performance en insertion

L'insertion dans `std::vector` avec `push_back` est amortie O(1) grâce au doublement de capacité. Pour N = (2ᵏ)³ particules, le temps d'insertion croît linéairement en N. Les allocations sont rares car le vecteur sous-alloue dès qu'il double. Pour un test comparatif plus rigoureux, on pourrait pré-réserver avec `reserve(N)` et comparer avec `std::list` (insertion O(1) mais mauvaise localité mémoire) et `std::deque`.

## Q8 — Performance du calcul des interactions

Le calcul naïf des forces est O(N²) : pour N particules, chaque paire (i,j) est évaluée. Les mesures montrent une croissance quadratique du temps de calcul quand N double, le temps est multiplié par ~4. C'est le goulot d'étranglement principal de la simulation.

## Q9 — Diviser le temps de calcul par 2

La modification s'appuie sur la **3ème loi de Newton** : F_ji = −F_ij. Il suffit de ne calculer chaque paire qu'une fois et d'accumuler simultanément sur les deux particules :

```cpp
for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {  // j > i seulement
        Vecteur<3> Fij = forceGrav(i, j);
        F[i] += Fij;
        F[j] -= Fij;    // F_ji = -F_ij
    }
}
```

On passe de N(N−1) à N(N−1)/2 évaluations de `forceGrav`, soit exactement **×2** en temps de calcul. Cette optimisation est déjà intégrée dans `Univers::calcForces`.

## Q10 — Autres simplifications possibles

1. **Distance de coupure (r_cut)** : ignorer les interactions entre particules distantes de plus de r_cut. Réduit la complexité à O(N) pour des distributions uniformes denses, au prix d'une légère erreur physique contrôlée.

2. **Méthode de Barnes-Hut** : regrouper les particules lointaines en super-particules via un octree. Complexité O(N log N), précision paramétrable via le critère θ.

3. **Méthode particule-maillage (PM)** : projeter les masses sur une grille, résoudre Poisson par FFT (O(N log N)), interpoler les forces. Adapté aux grandes simulations cosmologiques.

4. **Parallélisation** : la double boucle de forces est embarrassingly parallel (chaque paire est indépendante). OpenMP permet de paralléliser avec `#pragma omp parallel for` sur la boucle externe.
