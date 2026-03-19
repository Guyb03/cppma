# Lab 1

Q1. Il manque la spécification des pour l’utilisation de endl, cin et de flush. Le compilateur remarque l’absence de celle-ci et demande son ajoût. Deux solutions sont possibles soit préciser qu’on utilise les références à la bibliothèque standard de iostream en faisant using endl, soit préciser l’origine à chaque emploi de cette manière std :: endl.
Le compilateur remarque que la variable j dans la fonction main est non utilisée. Comme elle n’est pas nécessaire dans le parcours de la matrice, remplissage ligne après ligne de vecteurs en utilisant fill_vector. D’ailleurs, il y a une autre erreur pas repérer par le compilateur mais qui génère une erreur lors de l’exécution, en entête du fichier on annonce une fonction fill_vectors qui peut être utilisé donc dans main mais en réalité on a crée fill_vector.
Dans la fonction initialization, le compilateur soulève un comportement permissif sur l’allocation de l’espace mémoire. On instancie matrix comme un pointeur de pointeur vers un entier double, mais on alloue n vecteurs de pointeur de pointeur dans calloc ce qui correspond donc à n matrices de double. De plus le cast est manquant donc calloc renvoie un pointeur vers du void. Il convient d’ajouter le cast est de bien allouer n vecteurs d’abord (pointeur vers entier double) puis n entiers double dans la boucle.
Dans la fonction fill_vector, on demande le retour d’un pointeur vers des entiers double (double*) mais on renvoie en réalité l’adresse de vec qui est de type double**. Au vu de l’emploi qui est fait de la fonction il convient de remplacer le &vec par juste vec.
Dans la fonction trace, on renvoie l’adresse de sum plutôt que l’entier sum directement ce qui est demandé dans la définition de trace dans l’en-tête. On peut soit choisir de travailler avec le pointeur vers sum ou bien de modifier la définition de trace en haut du fichier.
Dans le calcul de la trace et l’initialisation les vecteurs sont parcourus à partir de l’indice 1 qui n’est pas le début de la liste.

Q4. Afin d’éviter la surcharge côté mémoire, on peut faire une désallocation mémoire à la fin de main.


2. Résolution d’EDO

Euler explicite

	On a commencé par répondre eu problème en suivant les remarques pour se limiter à une fonction n’utilisant que 3 paramètres (nombres d’itérations, condition initiale et fonction phi) pour l’algorithme de la méthode d’Euler explicite. Comme le but de cette partie est la comparaison des deux algorithmes et dans une recherche de factorisation maximale du code, on a ensuite construit une structure rassemblant les données du problème indépendantes de la méthode de résolution. Cette structure Grid réunit l’ensemble (list) des nuds de la discrétisation du segment étudié, le nombre de nuds et le pas h.

Euler implicite

	On a implémenté la résolution d’une équation raide par la méthode d’Euler implicite en utilisant l’algorithme de Newton pour calculer u_n+1 à chaque itération. On calcule numériquement la dérivée centrée pour estimer la prochaine valeur, on définit un seuil de tolérance tol (1e-6) à partir duquel on peut sortir de cette boucle.
	Pour faciliter la comparaison entre les 2 méthodes de résolution, on se sert de la bibliothèque iomanip et chrono pour faire un affichage plus complet et explicite.