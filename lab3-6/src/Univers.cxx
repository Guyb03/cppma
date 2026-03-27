#include "Univers.hxx"

Univers::Univers(int dim) : dim(dim) {}

int Univers::getDim()         const { return dim; }
int Univers::getNbParticles() const { return (int)particules.size(); }

void Univers::addParticle(const Particule& p) {
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

        if (afficher) std::cout << "t=" << t + dt << "\n" << *this;
    }
}

std::ostream& operator<<(std::ostream& os, const Univers& u) {
    for (const auto& p : u.particules)
        os << p << "\n";
    return os;
}
