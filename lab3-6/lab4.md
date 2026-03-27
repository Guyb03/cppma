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
