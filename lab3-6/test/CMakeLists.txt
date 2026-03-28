# Tests simulation système solaire
add_executable(testSysSol testSysSol.cxx)
target_link_libraries(testSysSol particules)

# Tests unitaires de la classe Vecteur
add_executable(testVecteur testVecteur.cxx)
target_link_libraries(testVecteur particules)

# Tests de la classe Univers
add_executable(testUnivers testUnivers.cxx)
target_link_libraries(testUnivers particules)

# Test du potentiel de Lennard-Jones (2 particules en oscillation)
add_executable(testLJ testLJ.cxx)
target_link_libraries(testLJ particules)
