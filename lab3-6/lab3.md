# Lab 3

En reprenant la structure fournie, nous avons fait le choix de séparer les définitions des opérateurs, méthodes, constructeurs et destructeurs. Ainsi toutes les définitions des méthodes, opérateurs et attributs seront dans le fichier d'en-tête et les implémentations réelles seront dans le fichier C++.
Ce choix relève d'une implémentation classique d'un tel projet en C++ offrant ainsi une modularité appréciable en évitant les répétitions. Il convient cependant de s'assurer des différents attributs et méthodes définies dans le fichier h, en respectant à l'identique leur signature pour éviter les erreurs de compilation.

Pour ce qui est du choix de la structure pour les coordonnées de la classe vecteur, on a choisi le `array`, et pour gérer la dimension N du vecteur, on crée un template qui l'initie par défaut à 3. Ce choix impose la définition des méthodes de la classe vecteur directement dans le fichier header.
Pour créer le vecteur on utilise la méthode standarde copy. `std::copy` prend le pointeur de début et de fin de la source, puis le pointeur de destination. Ceci impose que la destination ait une capacité suffisante, d'où l'intérêt du template qui garantit la correspondance des dimensions à la compilation.
