# Lab 4 — Découpage de l'espace

## Q1 — Potentiel de Lennard-Jones

Pour un système à deux particules, le potentiel de Lennard-Jones vaut :

$$U(r) = 4\varepsilon \left[ \left(\frac{\sigma}{r}\right)^{12} - \left(\frac{\sigma}{r}\right)^{6} \right]$$

Avec $\varepsilon = 1$ et $\sigma = 1$, on trace $U(r)$ pour $r \in [0.9, 4]$ :

```python
import numpy as np
import matplotlib.pyplot as plt

r = np.linspace(0.9, 4.0, 500)
U = 4 * ((1/r)**12 - (1/r)**6)

plt.figure()
plt.plot(r, U)
plt.axhline(0, color='k', lw=0.5)
plt.axvline(2**(1/6), color='r', lw=0.7, linestyle='--', label=r'$r^* = 2^{1/6}\sigma$')
plt.axvline(2.5, color='g', lw=0.7, linestyle='--', label=r'$r_{cut} = 2.5\sigma$')
plt.ylim(-2, 2)
plt.xlabel('r')
plt.ylabel('U(r)')
plt.title('Potentiel de Lennard-Jones')
plt.legend()
plt.savefig('lennard_jones.png')
plt.show()
```

**Analyse de la courbe :**

| Région | Comportement | Terme dominant |
|---|---|---|
| $r < \sigma$ | Répulsion forte (divergence en $r^{-12}$) | terme $\sigma^{12}/r^{12}$ |
| $r = 2^{1/6}\sigma \approx 1.122$ | Minimum $U = -\varepsilon$ (équilibre) | les deux |
| $r > 2^{1/6}\sigma$ | Attraction faible décroissante | terme $-\sigma^6/r^6$ |
| $r \to \infty$ | $U \to 0$ | — |

**Justification du rayon de coupure $r_{cut} = 2.5\sigma$ :**
Au-delà de $r_{cut}$, $U(r_{cut}) \approx -0.016\,\varepsilon$, soit moins de 2 % du puits de potentiel. Le terme $r^{-6}$ décroît suffisamment vite pour que la contribution soit négligeable. Tronquer le potentiel à cette distance réduit la complexité du calcul de O(N²) vers O(N) pour des densités uniformes (chaque particule n'interagit qu'avec ses voisines dans un rayon fixe).

## Q2 — Classe Univers

La classe `Univers` encapsule les paramètres physiques de la simulation (dimension, longueurs caractéristiques $L_d$, $\varepsilon$, $\sigma$, $r_{cut}$) ainsi que le choix des forces actives (`useGrav`, `useLF`). Elle expose `addParticule` et `StromerVerlet`.

## Q3 — Grille de cellules

Pour limiter le parcours des particules, l'espace est découpé en une grille tensorielle de cubes. Le nombre de cellules dans chaque direction $d$ est :

$$n_{cd} = \left\lfloor \frac{L_d}{r_{cut}} \right\rfloor$$

Chaque cellule de taille $r_{cut}$ stocke les indices des particules qu'elle contient (pas de copies). L'indice plat d'une cellule est calculé par :

$$\text{idx} = i_x + n_{c0} \cdot i_y + n_{c0} \cdot n_{c1} \cdot i_z$$

avec $i_d = \text{clamp}\!\left(\lfloor x_d / r_{cut} \rfloor,\, 0,\, n_{cd}-1\right)$.

Ce schéma est valide en 1D, 2D et 3D sans branchement : les dimensions inactives ont $n_{cd} = 1$, ce qui annule naturellement les termes correspondants.

## Q4 — Calcul des forces avec découpage

Au lieu d'une double boucle O(N²) sur toutes les paires, on exploite la grille :

1. Pour chaque particule $i$ on identifie sa cellule $c_i$.
2. On itère sur les cellules voisines $c_j$ de $c_i$ (au plus $3^d$ voisines, $d \in \{1,2,3\}$).
3. **Filtre coarse** : si $\|x_i - \text{centre}(c_j)\| \geq r_{cut}$, on ignore $c_j$.
4. Pour chaque particule $j > i$ dans $c_j$ : si $\|x_i - x_j\| < r_{cut}$, on calcule $F_{ij}^{LJ}$ et on accumule sur $F_i$ et $F_j$ (3ème loi de Newton).

La force élémentaire de Lennard-Jones est :

$$F_{ij} = \frac{24\,\varepsilon}{r_{ij}^2} \left(\frac{\sigma}{r_{ij}}\right)^6 \left[1 - 2\left(\frac{\sigma}{r_{ij}}\right)^6\right] \mathbf{r}_{ij}$$

La force est réflexive ($F_{ji} = -F_{ij}$) car $\mathbf{r}_{ji} = -\mathbf{r}_{ij}$ et la distance scalaire est identique. On exploite cette symétrie pour ne calculer chaque paire qu'une seule fois (condition $j > i$).

La force gravitationnelle reste disponible via un chemin séparé (double boucle O(N²) sur toutes les paires, sans $r_{cut}$), activable indépendamment.

## Q5 — Mise à jour des cellules

À la fin de chaque itération temporelle, après la mise à jour des positions, la grille est reconstruite :

```cpp
for (auto& c : cellules) c.vider();
for (int i = 0; i < n; ++i)
    cellules[celluleIdx(particules[i].getPosition())].addParticule(i);
```

`vider()` remet le vecteur d'indices à vide (O(total_cellules)), puis chaque particule est replacée dans sa cellule courante (O(N)). Le coût total est négligeable devant celui du calcul des forces.

## Q6 — Simulation collision de deux objets

**Paramètres :** $L_x = 250$, $\varepsilon = 5$, $\sigma = 1$, $m = 1$, $r_{cut} = 2.5\sigma$, $\delta t = 5 \times 10^{-5}$, $t_{fin} = 19.5$.

**Disposition initiale :** les particules sont espacées de $d = 2^{1/6}\sigma$ (distance d'équilibre LJ). Le rectangle bleu (160×40, vitesse nulle) est centré en $x$ dans le domaine et positionné en bas. Le carré rouge (40×40, vitesse $(0, -10)$) est centré horizontalement au-dessus du rectangle avec un écart de $3\,r_{cut}$.

```
Domaine : Lx = 250,  Ly = (39+39)·d + gap + 4·rcut ≈ 115
Carré   : x0 = rect_x0 + 60·d,   y0 = rect_y0 + 39·d + gap
Rectangle : x0 = (250 - 159·d) / 2,  y0 = rcut
```

La simulation est implémentée dans `demo/simu2D.cxx` et lancée avec :

```bash
./simu2D > simu2D.csv
python3 afficheSimuCsv.py simu2D.csv
```
