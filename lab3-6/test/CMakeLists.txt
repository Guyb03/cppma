include(GoogleTest)

# Tests unitaires : Vecteur (header-only template)
add_executable(testVecteur testVecteur.cxx)
target_link_libraries(testVecteur GTest::gtest_main particules)
gtest_discover_tests(testVecteur)

# Tests unitaires : Particule
add_executable(testParticule testParticule.cxx)
target_link_libraries(testParticule GTest::gtest_main particules)
gtest_discover_tests(testParticule)

# Tests unitaires : Cellule
add_executable(testCellule testCellule.cxx)
target_link_libraries(testCellule GTest::gtest_main particules)
gtest_discover_tests(testCellule)

# Tests unitaires : Univers (construction, ajout de particules)
add_executable(testUnivers testUnivers.cxx)
target_link_libraries(testUnivers GTest::gtest_main particules)
gtest_discover_tests(testUnivers)

# Tests d'intégration : système solaire (gravitation)
add_executable(testSysSol testSysSol.cxx)
target_link_libraries(testSysSol GTest::gtest_main particules)
gtest_discover_tests(testSysSol)

# Tests d'intégration : potentiel de Lennard-Jones
add_executable(testLJ testLJ.cxx)
target_link_libraries(testLJ GTest::gtest_main particules)
gtest_discover_tests(testLJ)
