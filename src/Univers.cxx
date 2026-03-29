#include "Univers.hxx"
#include "Cellule.hxx"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <utility>
#include <map>
#include <cmath>

Univers::Univers(int dim, std::array<double, 3> L, double epsilon,
                 double sigma, double rcut,
                 double G, bool useGrav, bool useLJ, bool usePotG)
    : dim(dim), L(L), epsilon(epsilon),
      sigma(sigma), rcut(rcut), G(G),
      useGrav(useGrav), useLF(useLJ), usePotG(usePotG),
      condLimites{ConditionLimite::LIBRE, ConditionLimite::LIBRE,
                  ConditionLimite::LIBRE, ConditionLimite::LIBRE,
                  ConditionLimite::LIBRE, ConditionLimite::LIBRE}
{
    for(int d = 0; d < 3; ++d) {
        nc[d] = (d < dim) ? std::max(1, (int)(L[d] / rcut)) : 1;
    }
    int total = nc[0] * nc[1] * nc[2];
    cellules.reserve(total);
    for(int iz = 0; iz < nc[2]; ++iz) {
        for(int iy = 0; iy < nc[1]; ++iy) {
            for(int ix = 0; ix < nc[0]; ++ix) {
                cellules.emplace_back(Vecteur<>{
                    (ix + 0.5) * rcut,
                    (iy + 0.5) * rcut,
                    (iz + 0.5) * rcut});
            }
        }
    }
}

int Univers::getDim() const { return dim; }
int Univers::getNbParticules() const { return (int)particules.size(); }

void Univers::addParticule(const Particule& p) {
    int idx = (int)particules.size();
    particules.push_back(p);
    cellules[celluleIdx(p.getPosition())].addParticule(idx);
}

void Univers::setConditionsLimites(const std::array<ConditionLimite, 6>& conds) {
    condLimites = conds;
}

bool Univers::appliquerConditionParticule(int i) {
    Vecteur<3> pos = particules[i].getPosition();
    Vecteur<3> vit = particules[i].getVitesse();

    for (int d = 0; d < dim; ++d) {
        if (pos[d] < 0.0) {
            switch (condLimites[2*d]) {
                case ConditionLimite::REFLEXION:
                    pos[d] = -pos[d];
                    vit[d] = -vit[d];
                    break;
                case ConditionLimite::ABSORPTION:
                    return false;
                case ConditionLimite::PERIODIQUE:
                    pos[d] += L[d];
                    break;
                default: break;
            }
        } else if (pos[d] >= L[d]) {
            switch (condLimites[2*d + 1]) {
                case ConditionLimite::REFLEXION:
                    pos[d] = 2.0 * L[d] - pos[d];
                    vit[d] = -vit[d];
                    break;
                case ConditionLimite::ABSORPTION:
                    return false;
                case ConditionLimite::PERIODIQUE:
                    pos[d] -= L[d];
                    break;
                default: break;
            }
        }
    }

    particules[i].setPosition(pos);
    particules[i].setVitesse(vit);
    return true;
}

int Univers::celluleIdx(const Vecteur<3>& pos) const {
    int ix = std::max(0, std::min((int)(pos[0] / rcut), nc[0] - 1));
    int iy = std::max(0, std::min((int)(pos[1] / rcut), nc[1] - 1));
    int iz = std::max(0, std::min((int)(pos[2] / rcut), nc[2] - 1));
    return ix + nc[0] * iy + nc[0] * nc[1] * iz;
}

std::vector<int> Univers::voisinesIdx(int idx) const {
    int iz =  idx / (nc[0] * nc[1]);
    int iy = (idx % (nc[0] * nc[1])) / nc[0];
    int ix =  idx % nc[0];

    std::vector<int> result;
    result.reserve(27);

    for (int dz = -1; dz <= 1; ++dz) {
        int jz = iz + dz; if (jz < 0 || jz >= nc[2]) continue;
        for (int dy = -1; dy <= 1; ++dy) {
            int jy = iy + dy; if (jy < 0 || jy >= nc[1]) continue;
            for (int dx = -1; dx <= 1; ++dx) {
                int jx = ix + dx; if (jx < 0 || jx >= nc[0]) continue;
                result.push_back(jx + nc[0]*jy + nc[0]*nc[1]*jz);
            }
        }
    }
    return result;
}

Vecteur<3> Univers::forceGrav(size_t i, size_t j) const {
    Vecteur<3> rij = particules[j].getPosition() - particules[i].getPosition();
    double d = rij.norm();
    if (d == 0.0) return Vecteur<3>{};
    double mi = particules[i].getMasse();
    double mj = particules[j].getMasse();
    return (mi * mj / (d * d * d)) * rij;
}

Vecteur<3> Univers::forceLJ(size_t i, size_t j) const {
    Vecteur<3> rij = particules[j].getPosition() - particules[i].getPosition();
    double d = rij.norm();
    if (d == 0.0) return Vecteur<3>{};
    return 24.0 * epsilon / (d * d) * std::pow(sigma / d, 6) * (1.0 - 2.0 * std::pow(sigma / d, 6)) * rij;
}

Vecteur<3> Univers::potGrav(int i) const {
    double mi = particules[i].getMasse();
    return Vecteur<3>{0.0, mi * G, 0.0};
}

void Univers::calcForcesParoi(std::vector<Vecteur<3>>& F) const {
    // r_cut du potentiel de paroi : 2^{1/6}·σ (englobe la zone répulsive + le puits)
    const double rcut_wall = std::pow(2.0, 1.0 / 6.0) * sigma;
    int n = (int)particules.size();

    for (int i = 0; i < n; ++i) {
        const Vecteur<3> pos = particules[i].getPosition();
        for (int d = 0; d < dim; ++d) {
            // --- Face min (paroi à pos[d] = 0) : force dans la direction +d ---
            if (condLimites[2*d] == ConditionLimite::REFLEXION_LJ) {
                double r = pos[d];
                if (r > 0.0 && r < rcut_wall) {
                    double d2r    = 2.0 * r;
                    double ratio6 = std::pow(sigma / d2r, 6);
                    F[i][d] += -24.0 * epsilon / d2r * ratio6 * (1.0 - 2.0 * ratio6);
                }
            }
            // --- Face max (paroi à pos[d] = L[d]) : force dans la direction −d ---
            if (condLimites[2*d + 1] == ConditionLimite::REFLEXION_LJ) {
                double r = L[d] - pos[d];
                if (r > 0.0 && r < rcut_wall) {
                    double d2r    = 2.0 * r;
                    double ratio6 = std::pow(sigma / d2r, 6);
                    F[i][d] -= -24.0 * epsilon / d2r * ratio6 * (1.0 - 2.0 * ratio6);
                }
            }
        }
    }
}

void Univers::calcForces(std::vector<Vecteur<3>>& F) const {
    int n = (int)particules.size();
    for (int i = 0; i < n; ++i) F[i] = Vecteur<3>{};

    // Gravitation newtonienne entre particules (O(N²))
    if (useGrav) {
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                Vecteur<3> Fij = forceGrav(i, j);
                F[i] += Fij;
                F[j] -= Fij;
            }
        }
    }

    // Lennard-Jones via linked-cell list
    if (useLF) {
        for (int i = 0; i < n; ++i) {
            int cellIdx = celluleIdx(particules[i].getPosition());
            for (int c : voisinesIdx(cellIdx)) {
                for (int j : cellules[c].getIndices()) {
                    if (j <= i) continue;  // Newton : chaque paire une seule fois
                    double d = distance(particules[i].getPosition(), particules[j].getPosition());
                    if (d < rcut) {
                        Vecteur<3> Fij = forceLJ(i, j);
                        F[i] += Fij;
                        F[j] -= Fij;
                    }
                }
            }
        }
    }

    // Champ gravitationnel uniforme
    if (usePotG) {
        for (int i = 0; i < n; ++i)
            F[i] += potGrav(i);
    }

    // Forces de paroi LJ (REFLEXION_LJ)
    calcForcesParoi(F);
}

void Univers::sauvegarderVTK(const std::string& filename) const {
    std::ofstream f(filename);
    int n = (int)particules.size();

    f << "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n"
      << "  <UnstructuredGrid>\n"
      << "    <Piece NumberOfPoints=\"" << n << "\" NumberOfCells=\"0\">\n"
      << "      <Points>\n"
      << "        <DataArray name=\"Position\" type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n"
      << "          ";
    for (const auto& p : particules)
        f << p.getPosition()[0] << " " << p.getPosition()[1] << " " << p.getPosition()[2] << " ";
    f << "\n        </DataArray>\n"
      << "      </Points>\n"
      << "      <PointData Vectors=\"Velocity\">\n"
      << "        <DataArray type=\"Float32\" Name=\"Velocity\" NumberOfComponents=\"3\" format=\"ascii\">\n"
      << "          ";
    for (const auto& p : particules)
        f << p.getVitesse()[0] << " " << p.getVitesse()[1] << " " << p.getVitesse()[2] << " ";
    f << "\n        </DataArray>\n"
      << "        <DataArray type=\"Float32\" Name=\"Masse\" format=\"ascii\">\n"
      << "          ";
    for (const auto& p : particules)
        f << p.getMasse() << " ";
    f << "\n        </DataArray>\n"
      << "      </PointData>\n"
      << "      <Cells>\n"
      << "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\"/>\n"
      << "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\"/>\n"
      << "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\"/>\n"
      << "      </Cells>\n"
      << "    </Piece>\n"
      << "  </UnstructuredGrid>\n"
      << "</VTKFile>\n";
}

void Univers::StromerVerlet(double t_start, double t_end, double dt,
                             bool afficher, int vtkFreq, const std::string& vtkPrefix,
                             double targetEcin, int ecFreq, bool showProgress) {
    int n = (int)particules.size();
    std::vector<Vecteur<3>> F(n), F_old(n);

    const int totalSteps   = (int)std::round((t_end - t_start) / dt);
    const int progressFreq = std::max(1, totalSteps / 200);

    calcForces(F);

    // En-tête CSV
    if (afficher) {
        std::cout << "t";
        for (const auto& p : particules) {
            if (dim >= 1) std::cout << "," << p.getCategorie() << "_x";
            if (dim >= 2) std::cout << "," << p.getCategorie() << "_y";
            if (dim >= 3) std::cout << "," << p.getCategorie() << "_z";
        }
        std::cout << "\n";
    }

    int step = 0;
    if (vtkFreq > 0) {
        std::ostringstream oss;
        oss << vtkPrefix << "_" << std::setw(6) << std::setfill('0') << step << ".vtu";
        sauvegarderVTK(oss.str());
    }

    for (double t = t_start; t < t_end; t += dt) {

        // Mise à jour des positions
        for (int i = 0; i < n; ++i) {
            Vecteur<3> newPos = particules[i].getPosition()
                + dt * (particules[i].getVitesse() + (0.5 * dt / particules[i].getMasse()) * F[i]);
            particules[i].setPosition(newPos);
            F_old[i] = F[i];
        }

        // Application des conditions aux limites
        {
            std::vector<int> toRemove;
            for (int i = 0; i < n; ++i) {
                if (!appliquerConditionParticule(i))
                    toRemove.push_back(i);
            }
            for (int k = (int)toRemove.size() - 1; k >= 0; --k) {
                int idx = toRemove[k];
                particules.erase(particules.begin() + idx);
                F.erase(F.begin() + idx);
                F_old.erase(F_old.begin() + idx);
                --n;
            }
        }

        // Mise à jour de la grille
        for (auto& c : cellules) c.vider();
        for (int i = 0; i < n; ++i) {
            cellules[celluleIdx(particules[i].getPosition())].addParticule(i);
        }

        calcForces(F);

        // Mise à jour des vitesses
        for (int i = 0; i < n; ++i) {
            Vecteur<3> newVit = particules[i].getVitesse()
                + (0.5 * dt / particules[i].getMasse()) * (F[i] + F_old[i]);
            particules[i].setVitesse(newVit);
        }

        // Rescaling d'énergie cinétique (thermostat simple)
        if (targetEcin > 0.0 && step % ecFreq == 0) {
            double Ec = 0.0;
            for (int i = 0; i < n; ++i)
                Ec += 0.5 * particules[i].getMasse() * particules[i].getVitesse().norm2();
            if (Ec > 0.0) {
                double beta = std::sqrt(targetEcin / Ec);
                for (int i = 0; i < n; ++i)
                    particules[i].setVitesse(beta * particules[i].getVitesse());
            }
        }

        ++step;

        if (vtkFreq > 0 && step % vtkFreq == 0) {
            std::ostringstream oss;
            oss << vtkPrefix << "_" << std::setw(6) << std::setfill('0') << step << ".vtu";
            sauvegarderVTK(oss.str());
        }

        // Barre de progression (stderr)
        if (showProgress && step % progressFreq == 0) {
            constexpr int W = 40;
            double pct = (totalSteps > 0) ? (double)step / totalSteps : 1.0;
            int filled = (int)(pct * W);
            std::cerr << "\r[";
            for (int k = 0; k < W; ++k) std::cerr << (k < filled ? '#' : ' ');
            std::cerr << "] " << (int)(pct * 100) << "% t=" << (t + dt) << "/" << t_end;
            std::cerr.flush();
        }

        // Affichage CSV
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

    if (showProgress) std::cerr << "\n";
}

std::ostream& operator<<(std::ostream& os, const Univers& u) {
    for (const auto& p : u.particules) {
        os << p.getCategorie();
        for (int d = 0; d < u.dim; ++d)
            os << " " << p.getPosition()[d];
        os << "\n";
    }
    return os;
}
