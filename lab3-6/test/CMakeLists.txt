# Tests unitaires de la classe Vecteur (Q3)
add_executable(testVecteur testVecteur.cxx)
target_link_libraries(testVecteur particules)

# Tests de la classe Univers (Q5-Q10)
add_executable(testUnivers testUnivers.cxx)
target_link_libraries(testUnivers particules)
