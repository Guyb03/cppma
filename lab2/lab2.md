# Lab 2

## Comparaison des différents types de collection

Pour pouvoir comparer au mieux les différentes performances des structures possibles en C++, on a choisi de créer un nombre croisant de collection de différentes particules dont les coordonnées ont été générée selon la loi normale comme suggéré dans le squelette de code fourni.
Pour se faire, 4 fonctions de remplissage pour les différentes structures de données testées (list, deque, set, vector) on était mises au point. Elles sont appelées dans une fonction compareStructures qui, grâce à la bibliothèque chrono, mesure le temps écoulé entre le début et la fin de la création puis l'affiche dans la sortie standard. Les résultats d'une génération ont été sauvegardé dans le fichier compare.txt. 
Ce test n'est pas très représentatif des performances que nous pourront espérer observé au fil du projet étant donné que la création de la collection de particules n'est pas l'étape la plus coûteuse de la simulation finale. Elle permet tout de même, outre de tester le fonctionnement de chrono, d'avoir une idée de la collection traitant le plus rapidement les très grands nombres de particules.

## Strömer-Verlet

Le choix d’implémentation est resté fidèle à l’algorithme de Strömer-Verlet proposé. On a choisi ici le type de collection deque. Les vecteurs de position, vitesse et force sont des array de double de dimension 3. Afin de clarifier les calculs nous avons choisi de surcharger les opérateurs arithmétiques de calcul vectoriel comme le produit d’un array par un scalaire, l’incrément d’un array par un autre array et la somme entre 2 arrays. Cela permet de faciliter la lecture des calculs des vecteurs et d’alléger le code. On a défini 3 fonctions outils : distance, vecteurDistance et calculForce qui calculer respectivement la distance Euclidienne entre deux éléments du système, le vecteur distance qui les sépare et indique ainsi le sens dans lequel s’exerce la gravitation et la force gravitationnelle exercé par l’ensemble du système sur l’élément donné en paramètre.
