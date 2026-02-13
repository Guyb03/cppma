/// Code élémentaire pour le calcul de la solution d'une équation différentielle
/// en utilisant la méthode d'Euler explicite et implicite.
#include<cmath>
#include<cstdlib>
#include<iostream>

double* euler_explicite(int, double, double (*phi)(double, double));
double* euler_implicite(int, double, double (*phi)(double, double));    
double* solution_exacte(Grid grid);
double phi(double x, double u);
void print_results(double*, int);

struct Grid {
    double* x;
    double h;
    int n;
};

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

double* euler_implicite(double u0, double (*phi)(double, double), Grid grid) {
  double *u = (double*)calloc(grid.n, sizeof(double));
  u[0] = u0;
  for (int i = 1; i < grid.n; i++) {
    double x_next = grid.x[i];
    double u_next_guess = u[i-1]; 
    for (int iter = 0; iter < 100; iter++) { // Simple fixed-point iteration
      double u_next_new = u[i-1] + grid.h * phi(x_next, u_next_guess);
      if (fabs(u_next_new - u_next_guess) < 1e-6) {
        break; // Convergence
      }
      u_next_guess = u_next_new;
    }
    u[i] = u_next_guess;
  }
  return u;
}


double phi(double x, double u) {
  return 2*x*u;
}

/*
double phi(double x, double u) {
    return -50*(u-math.cos(x));
}
*/

void print_results(double* u, int n) {
    Grid grid = discretize(0.0, 1.0, n);
    double* u_exact = solution_exacte(grid);
    for (int i = 0; i < n; i++) {
        std::cout << "u[" << i << "] = " << u[i] << " | exact = " << u_exact[i] << std::endl;
    }
    free(u_exact);
}

double* solution_exacte(Grid grid) {
  double *u = (double*)calloc(grid.n, sizeof(double));
  for (int i = 0; i < grid.n; i++) {
    u[i] = exp(pow(i*grid.h, 2));
  }
  return u;
}

int main() {
  int n = 10;
  double u0 = 1.0;
  Grid grid = discretize(0.0, 1.0, n);
  double *result = euler_explicite(u0, phi, grid);

  
  std::cout << "Euler explicite results:" << std::endl;
  print_results(result, n);
  free(result);

  return 0;
}