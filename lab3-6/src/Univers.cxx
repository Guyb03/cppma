#include "Univers.hxx"

Univers::Univers(int dim) : dim(dim) {}

int Univers::getDim()         const { return dim; }
int Univers::getNbParticules() const { return (int)particules.size(); }

void Univers::addParticule(const Particule& p) {
    particules.push_back(p);
}

Vecteur<3> Univers::forceGrav(size_t i, size_t j) const {
    Vecteur<3> rij = particules[j].getPosition() - particules[i].getPosition();
    double d = rij.norm();
    if (d == 0.0) return Vecteur<3>{};
    double mi = particules[i].getMasse();
    double mj = particules[j].getMasse();
    return (mi * mj / (d * d * d)) * rij;
}

void Univers::calcForces(std::vector<Vecteur<3>>& F) const {
    int n = (int)particules.size();
    for (int i = 0; i < n; ++i) F[i] = Vecteur<3>{};

    // Newton 3 : F_ij = -F_ji → N(N-1)/2 calculs au lieu de N(N-1)
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            Vecteur<3> Fij = forceGrav(i, j);
            F[i] += Fij;
            F[j] -= Fij;
        }
    }
}

void Univers::StromerVerlet(double t_start, double t_end, double dt, bool afficher) {
    int n = (int)particules.size();
    std::vector<Vecteur<3>> F(n), F_old(n);

    calcForces(F);

    // En-tête CSV : noms des particules
    if (afficher) {
        std::cout << "t";
        for (const auto& p : particules) {
            if (dim >= 1) std::cout << "," << p.getCategorie() << "_x";
            if (dim >= 2) std::cout << "," << p.getCategorie() << "_y";
            if (dim >= 3) std::cout << "," << p.getCategorie() << "_z";
        }
        std::cout << "\n";
    }

    for (double t = t_start; t < t_end; t += dt) {

        // Mise à jour des positions
        for (int i = 0; i < n; ++i) {
            Vecteur<3> newPos = particules[i].getPosition()
                + dt * (particules[i].getVitesse() + (0.5 * dt / particules[i].getMasse()) * F[i]);
            particules[i].setPosition(newPos);
            F_old[i] = F[i];
        }

        calcForces(F);

        // Mise à jour des vitesses
        for (int i = 0; i < n; ++i) {
            Vecteur<3> newVit = particules[i].getVitesse()
                + (0.5 * dt / particules[i].getMasse()) * (F[i] + F_old[i]);
            particules[i].setVitesse(newVit);
        }

        // Affichage CSV : une ligne par pas de temps
        if (afficher) {
            std::cout << (t + dt);
            for (const auto& p : particules) {
                std::cout << "," << p.getPosition()[0];
                if (dim >= 2) std::cout << "," << p.getPosition()[1];
                if (dim >= 3) std::cout << "," << p.getPosition()[2];
            }
            std::cout << "\n";
        }
    }
}

// Affiche les positions courantes (snapshot), respecte dim
std::ostream& operator<<(std::ostream& os, const Univers& u) {
    for (const auto& p : u.particules) {
        os << p.getCategorie();
        for (int d = 0; d < u.dim; ++d)
            os << " " << p.getPosition()[d];
        os << "\n";
    }
    return os;
}
