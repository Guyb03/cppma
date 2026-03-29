/// Code élémentaire pour le calcul de la solution d'une équation différentielle
/// en utilisant la méthode d'Euler explicite et implicite.
#include<cmath>
#include<cstdlib>
#include<iostream>
#include <iomanip>
#include <chrono>

double* euler_explicite(int, double, double (*phi)(double, double));
double* euler_implicite(int, double, double (*phi)(double, double)); 
double newton(double u_prev, double x, double h, double (*phi)(double, double));
double phi(double x, double u);
void print_results(double*, int);

struct Grid {
    double* x;
    double h;
    int n;
};

double* solution_exacte(Grid grid, double u0); 

/// @brief Routine de discrétisation qui permet de créer une grille
/// @param a est la borne inférieure du domaine de définition
/// @param b est la borne supérieure du domaine de définition
/// @param n est le nombre de points de la grille
/// @return la structure Grid contenant les informations sur la grille
Grid discretize(double a, double b, int n) {
    Grid grid;
    grid.n = n;
    grid.h = (b - a) / (n - 1);
    grid.x = (double*)calloc(n, sizeof(double));
    for (int i = 0; i < n; i++) {
        grid.x[i] = a + i * grid.h;
    }
    return grid;
}

/// @brief Implémentation de la méthode d'Euler explicite pour résoudre une équation différentielle du premier ordre
/// @param u0 est la condition initiale
/// @param phi est la fonction qui définit l'équation différentielle
/// @param grid est la structure qui contient les informations sur la grille de discrétisation
/// @return    Renvoie un tableau contenant les valeurs de la solution approximative à chaque point de la grille
double* euler_explicite(double u0, double (*phi)(double, double), Grid grid) {
  double *u = (double*)calloc(grid.n, sizeof(double));
  u[0] = u0;
  for (int i = 1; i < grid.n; i++) {
    u[i] = u[i-1] + grid.h * phi(grid.x[i-1], u[i-1]);
  }
  return u;
}

/*
double* euler_explicite_simple(int n, double u0, double (*phi)(double, double)) {
  double *u = (double*)calloc(n, sizeof(double));
  double h = 1.0 / (n - 1);
  u[0] = u0;
  for (int i = 1; i < n; i++) {
    u[i] = u[i-1] + h * phi(i*h, u[i-1]);
  }
  return u;
}
*/

/// @brief Implémentation de la méthode de Newton pour résoudre l'équation non linéaire dans la méthode d'Euler implicite
/// @param[in] u_prev est la valeur précédente de la solution
/// @param[in] x est la position actuelle sur la grille
/// @param[in] h est le pas de discrétisation
/// @param[in] phi est la fonction qui définit l'équation différentielle
/// @return    Renvoie la valeur de la solution à la position actuelle après convergence de la méthode de Newton
double newton(double u_prev, double x, double h, double (*phi)(double, double)) {
    double u_next = u_prev; // Initial guess
    double tol = 1e-6;
    int max_iter = 1000;
    for (int iter = 0; iter < max_iter; iter++) {
        double f = u_next - u_prev - h * phi(x, u_next);
        double df = 1 - h * (phi(x, u_next + tol) - phi(x, u_next - tol)) / (2 * tol); // Numerical derivative
        double delta = f / df;
        u_next -= delta; // Update guess
        if (fabs(f) < tol) break;
        if (iter == max_iter -1) {
            std::cerr << "Warning: Newton's method did not converge after " << max_iter << " iterations." << std::endl;
        }
    }
    return u_next;
}

/// @brief Implémentation de la méthode de résolution d'Euler implicite
/// @param[in] u0 est la condition initialee
/// @param[in] phi est la fonction qui définit l'équation différentielle
/// @param[in] grid est la structure qui contient les informations sur la grille de discrétisation
/// @return    Renvoie un tableau contenant les valeurs de la solution approximative à chaque point de la grille
double* euler_implicite(double u0, double (*phi)(double, double), Grid grid) {
  double *u = (double*)calloc(grid.n, sizeof(double));
  u[0] = u0;
  for (int i = 1; i < grid.n; i++) {
    u[i] = newton(u[i-1], grid.x[i], grid.h, phi);
  }
  return u;
}


/*
double phi(double x, double u) {
  return 2*x*u;
}
*/

/// @brief Fonction qui définit l'équation différentielle à résoudre
/// @param x est la position actuelle sur la grille
/// @param u est la valeur actuelle de la solution
/// @return la valeur de la fonction phi à la position x et pour la solution u
double phi(double x, double u) {
    return -50*(u-std::cos(x));
}


/// @brief Fonction qui permet d'afficher les résultats de la méthode d'Euler explicite et implicite
/// @param u_exp est le tableau contenant les valeurs de la solution approximative avec la méthode explicite
/// @param u_imp est le tableau contenant les valeurs de la solution approximative avec la méthode implicite
/// @param u_exact est le tableau contenant les valeurs exactes de la solution
/// @param grid est la structure qui contient les informations sur la grille de discrétisation
void print_results(double* u_exp, double* u_imp, double* u_exact, Grid grid) {
    std::cout << std::setw(10) << "x"
              << std::setw(15) << "u_explicit"
              << std::setw(15) << "u_implicit"
              << std::setw(15) << "u_exact"
              << std::setw(15) << "|err_exp|"
              << std::setw(15) << "|err_imp|" << std::endl;
    std::cout << std::string(85, '-') << std::endl;
    for (int i = 0; i < grid.n; i++) {
        std::cout << std::scientific << std::setprecision(4)
                  << std::setw(10) << grid.x[i]
                  << std::setw(15) << u_exp[i]
                  << std::setw(15) << u_imp[i]
                  << std::setw(15) << u_exact[i]
                  << std::setw(15) << fabs(u_exp[i] - u_exact[i])
                  << std::setw(15) << fabs(u_imp[i] - u_exact[i]) << std::endl;
    }
}

/// @brief Fonction qui calcule la solution exacte de l'équation différentielle
/// @param grid est la structure qui contient les informations sur la grille de discrétisation
/// @param u0 est la condition initiale
/// @return Renvoie un tableau contenant les valeurs exactes de la solution à chaque point de la grille
double* solution_exacte(Grid grid, double u0) {
    double* u = (double*)calloc(grid.n, sizeof(double));
      for (int i = 0; i < grid.n; i++) {
        u[i] = std::cos(grid.x[i]) + (u0 - 1.0) * std::exp(-50 * grid.x[i]);
    }
    return u;
}


int main() {
  int n;
  double u0, a , b;
  std::cout << "Enter a number of iteration: " << std::flush;
  std::cin >> n;
  std::cout << "Enter a u0 value: " << std::flush;
  std::cin >> u0;
  std::cout << "Enter lower bound and upper bound: " << std::flush;
  std::cin >> a;
  std::cin >> b;
  Grid grid = discretize(a, b, n);
  double *result_explicit = euler_explicite(u0, phi, grid);
  double *result_implicit = euler_implicite(u0, phi, grid);
  
  std::cout << "Euler explicite results:" << std::endl;
  double *exact = solution_exacte(grid, u0);
  print_results(result_explicit, result_implicit, exact, grid);
  free(result_explicit);
  free(result_implicit);
  free(exact);

  return 0;
}