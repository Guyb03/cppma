## Indique qu'elle version de cmake minimale est utilisée.
cmake_minimum_required (VERSION 3.20)

## Specifie le nom du projet et le langage de programmation du projet
project(SimUnivers CXX)

set(CMAKE_BUILD_TYPE Release)

## Force le standard
set(CMAKE_CXX_STANDARD 17)

## Indique les options de compilations pour le projet
## -pg : donne la possibilité de faire du profiling
## -Wall : affiche les warnings
set(CMAKE_CXX_FLAGS "-Wall")

## Définir la localisation des entêtes.
## include_directories sera propager à l'ensemble du projet
include_directories(${CMAKE_SOURCE_DIR}/include)

## Parcours les sous répertoires contenant les définitions (.cxx)
## On commence par créer une bibliothèque
add_subdirectory(src)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)
enable_testing()

## On créée les tests pour la bibliothèque.
## Ces tests sont unitaires ou fonctionnels
add_subdirectory(test)

## On implémente une démo, un programme de vérification.
add_subdirectory(demo)