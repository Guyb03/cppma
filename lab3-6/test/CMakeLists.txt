# Tests simulation système solaire
add_executable(testSysSol testSysSol.cxx)
target_link_libraries(testSysSol particules)

# Tests unitaires de la classe Vecteur
add_executable(testVecteur testVecteur.cxx)
target_link_libraries(testVecteur particules)

# Tests de la classe Univers
add_executable(testUnivers testUnivers.cxx)
target_link_libraries(testUnivers particules)
