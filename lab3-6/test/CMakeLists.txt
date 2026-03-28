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

# Tests unitaires : Univers (construction, ajout de particules, VTK)
add_executable(testUnivers testUnivers.cxx)
target_link_libraries(testUnivers GTest::gtest_main particules)
gtest_discover_tests(testUnivers)
