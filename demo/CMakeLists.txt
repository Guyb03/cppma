# add_executable(demo_univers demo_univers.cxx)
# target_link_libraries(demo_univers particules)

add_executable(simu2D simu2D.cxx)
target_link_libraries(simu2D particules)

add_executable(compareReflexion compareReflexion.cxx)
target_link_libraries(compareReflexion particules)

# add_executable(simu2DRelaxe simu2DRelaxe.cxx)
# target_link_libraries(simu2DRelaxe particules)

add_executable(simu2Dlab6 simu2Dlab6.cxx)
target_link_libraries(simu2Dlab6 particules)

add_executable(testOcean testOcean.cxx)
target_link_libraries(testOcean particules)