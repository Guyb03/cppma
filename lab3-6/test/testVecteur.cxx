#include "Vecteur.hxx"
#include <iostream>
#include <cmath>

static int passed = 0, failed = 0;

void check(const std::string& name, bool result) {
    if (result) { std::cout << "[PASS] " << name << "\n"; ++passed; }
    else        { std::cout << "[FAIL] " << name << "\n"; ++failed; }
}

int main() {

    // --- Constructeurs ---
    Vecteur<3> v0;
    check("constructeur zero", v0[0] == 0.0 && v0[1] == 0.0 && v0[2] == 0.0);

    Vecteur<3> v1{1.0, 2.0, 3.0};
    check("constructeur initializer_list", v1[0] == 1.0 && v1[1] == 2.0 && v1[2] == 3.0);

    double tab[] = {4.0, 5.0, 6.0};
    Vecteur<3> v2(tab);
    check("constructeur pointeur", v2[0] == 4.0 && v2[1] == 5.0 && v2[2] == 6.0);

    // --- Accesseurs ---
    check("getDim", v1.getDim() == 3);
    check("dim statique", Vecteur<3>::dim == 3);
    check("operator[] lecture", v1[1] == 2.0);
    v1[1] = 9.0;
    check("operator[] écriture", v1[1] == 9.0);
    v1[1] = 2.0; // restauration

    // --- Normes ---
    Vecteur<3> v3{3.0, 4.0, 0.0};
    check("norm2", v3.norm2() == 25.0);
    check("norm",  std::abs(v3.norm() - 5.0) < 1e-12);

    // --- Produit scalaire ---
    Vecteur<3> a{1.0, 0.0, 0.0};
    Vecteur<3> b{0.0, 1.0, 0.0};
    check("dot orthogonal", a.dot(b) == 0.0);
    Vecteur<3> c{1.0, 2.0, 3.0};
    Vecteur<3> d{4.0, 5.0, 6.0};
    check("dot", c.dot(d) == 32.0);

    // --- Opérateurs composés ---
    Vecteur<3> u{1.0, 2.0, 3.0};
    Vecteur<3> w{1.0, 1.0, 1.0};
    u += w;
    check("operator+=", u[0] == 2.0 && u[1] == 3.0 && u[2] == 4.0);

    u -= w;
    check("operator-=", u[0] == 1.0 && u[1] == 2.0 && u[2] == 3.0);

    u *= 2.0;
    check("operator*=", u[0] == 2.0 && u[1] == 4.0 && u[2] == 6.0);

    u /= 2.0;
    check("operator/=", u[0] == 1.0 && u[1] == 2.0 && u[2] == 3.0);

    // --- Opérateurs binaires libres ---
    Vecteur<3> p{1.0, 0.0, 0.0};
    Vecteur<3> q{0.0, 1.0, 0.0};

    Vecteur<3> pq = p + q;
    check("operator+", pq[0] == 1.0 && pq[1] == 1.0 && pq[2] == 0.0);

    Vecteur<3> pq2 = p - q;
    check("operator-", pq2[0] == 1.0 && pq2[1] == -1.0 && pq2[2] == 0.0);

    Vecteur<3> ps = p * 3.0;
    check("operator* (v*a)", ps[0] == 3.0 && ps[1] == 0.0);

    Vecteur<3> sp = 3.0 * p;
    check("operator* (a*v)", sp[0] == 3.0 && sp[1] == 0.0);

    Vecteur<3> pd = p / 2.0;
    check("operator/", pd[0] == 0.5);

    // --- Comparaison avec tolérance ---
    Vecteur<3> x{1.0, 2.0, 3.0};
    Vecteur<3> y{1.0, 2.0, 3.0 + 1e-11};
    check("approxEqual dans tolerance",  approxEqual(x, y));
    check("approxEqual hors tolerance", !approxEqual(x, y, 1e-12));

    // --- Affichage ---
    std::cout << "\nAffichage : " << Vecteur<3>{1.0, 2.0, 3.0} << "\n\n";

    // --- Bilan ---
    std::cout << passed << "/" << (passed + failed) << " tests passés\n";
    return failed;
}
