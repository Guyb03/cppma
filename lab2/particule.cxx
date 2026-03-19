#include<iostream>
#include<cmath>
#include<set>
#include<list>
#include<deque>
#include<vector>
#include<array>
#include<random>
#include<chrono>

// Surcharge des opérateurs de bases sur un array pour les calculs des force

// Surcharge de += pour std::array<double, 3>& a,
std::array<double, 3>& operator+=(std::array<double, 3>& a, const std::array<double, 3>& b) {
    for (int i = 0; i < 3; ++i) {
        a[i] += b[i];
    }
    return a;
}

// Surcharge de * pour multiplier un array par un scalaire
std::array<double, 3> operator*(const std::array<double, 3>& a, double scalar) {
    std::array<double, 3> result;
    for (int i = 0; i < 3; ++i) {
        result[i] = a[i] * scalar;
    }
    return result;
}

// Surcharge de / pour diviser un array par un scalaire
std::array<double, 3> operator/(const std::array<double, 3>& a, double scalar) {
    std::array<double, 3> result;
    for (int i = 0; i < 3; ++i) {
        result[i] = a[i] / scalar;
    }
    return result;
}

// Surcharge de + pour la somme entre 2 arrays
std::array<double, 3> operator+(const std::array<double, 3>& a, const std::array<double, 3>& b) {
    std::array<double, 3> res;
    for (int i = 0; i < 3; ++i) {
        res[i] = a[i] + b[i];
    }
    return res;
}

class Particule
{
private:
    std::array<double, 3> position, vitesse, force;
    double masse;
    const char* categorie;
    long identifiant;
    
public:
    Particule(const std::array<double,3>& position,
              const std::array<double,3>& vitesse,
              const std::array<double,3>& force,
              double masse, const char* categorie, long identifiant);
    ~Particule() = default;
    bool operator<(const Particule& other) const {return this->identifiant < other.identifiant;} // Nécessaire pour définir un set
    std::array<double, 3> getPosition() const {return this->position;}
    std::array<double, 3> getVitesse() const {return this->vitesse;}
    std::array<double, 3> getForce() const {return this->force;}
    double getMasse() const {return this->masse;}
    const char* getCategorie() const {return this->categorie;}

    void setPosition(const std::array<double, 3>& position) { this->position = position; }
    void setVitesse(const std::array<double, 3>& vitesse) { this->vitesse = vitesse; }
    void setForce(const std::array<double, 3>& force) { this->force = force; }
    void setMasse(double masse) { this->masse = masse; }

};


Particule::Particule(const std::array<double,3>& position,
                     const std::array<double,3>& vitesse,
                     const std::array<double,3>& force,
                     double masse, const char* categorie, long identifiant)
    : position(position), vitesse(vitesse), force(force), masse(masse), categorie(categorie), identifiant(identifiant)
{
}

std::list<Particule> createList(int size, double rd) {
    std::list<Particule> list;
    for (int i=0; i<size; ++i) {
        std::array<double,3> position{rd, rd, rd};
        std::array<double,3> vitesse{0.0, 0.0, 0.0};
        std::array<double,3> force{0.0, 0.0, 0.0};
        double masse = 0.0;
        
        Particule p(position, vitesse, force, masse, "P", i);
        list.push_back(p);
    }
    return list;
}

std::set<Particule> createSet(int size, double rd) {
    std::set<Particule> set;
    for (int i=0; i<size; ++i) {
        std::array<double,3> position{rd, rd, rd};
        std::array<double,3> vitesse{0.0, 0.0, 0.0};
        std::array<double,3> force{0.0, 0.0, 0.0};
        double masse = 0.0;
        
        Particule p(position, vitesse, force, masse, "P", i);
        set.insert(p);
    }
    return set;
}

std::deque<Particule> createDeque(int size, double rd) {
    std::deque<Particule> deque;
    for (int i=0; i<size; ++i) {
        std::array<double,3> position{rd, rd, rd};
        std::array<double,3> vitesse{0.0, 0.0, 0.0};
        std::array<double,3> force{0.0, 0.0, 0.0};
        double masse = 0.0;
        
        Particule p(position, vitesse, force, masse, "P", i);
        deque.push_back(p);
    }
    return deque;
}

std::vector<Particule> createVector(int size, double rd) {
    std::vector<Particule> vector;
    for (int i=0; i<size; ++i) {
        std::array<double,3> position{rd, rd, rd};
        std::array<double,3> vitesse{0.0, 0.0, 0.0};
        std::array<double,3> force{0.0, 0.0, 0.0};
        double masse = 0.0;
        
        Particule p(position, vitesse, force, masse, "P", i);
        vector.push_back(p);
    }
    return vector;
}

/// @brief Mesure le temps d'exécution d'une fonction et l'affiche
/// @param label le nom du test affiché
/// @param func la fonction à mesurer
template<typename Func>
double mesureTemps(const std::string& label, Func func) {
    auto start = std::chrono::steady_clock::now();
    func();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "  " << label << ": " << elapsed.count() << "s\n";
    return elapsed.count();
}

/// @brief Génère une particule avec des valeurs aléatoires
/// @param mt le générateur de nombres aléatoires
/// @param dist la distribution uniforme
/// @param id l'identifiant de la particule
Particule makeRandomParticule(std::mt19937& mt, std::uniform_real_distribution<double>& dist, long id) {
    return Particule(
        {dist(mt), dist(mt), dist(mt)},  // position aléatoire
        {dist(mt), dist(mt), dist(mt)},  // vitesse aléatoire
        {0.0, 0.0, 0.0},
        dist(mt) + 0.1,                  // masse > 0
        "P", id
    );
}

void compareStructures(std::mt19937& mt, std::uniform_real_distribution<double>& dist) {

    for (int n = 64; n < 200000; n *= 2) {
        std::cout << "\n=== N = " << n << " particules ===\n";

        // Pré-génération des particules (hors mesure)
        std::vector<Particule> source;
        source.reserve(n);
        for (int i = 0; i < n; ++i)
            source.push_back(makeRandomParticule(mt, dist, i));

        // Indice et identifiant aléatoire pour les tests de recherche/suppression
        size_t idx_milieu = n / 2;

        // INSERTION
        std::cout << "[Insertion]\n";
        std::list<Particule>   lst;
        std::deque<Particule>  dq;
        std::vector<Particule> vec;
        std::set<Particule>    st;
        vec.reserve(n); // réservation pour comparaison équitable

        mesureTemps("list  ", [&]{ for (auto& p : source) lst.push_back(p); });
        mesureTemps("deque ", [&]{ for (auto& p : source) dq.push_back(p); });
        mesureTemps("vector", [&]{ for (auto& p : source) vec.push_back(p); });
        mesureTemps("set   ", [&]{ for (auto& p : source) st.insert(p); });

        // ITÉRATION COMPLÈTE
        std::cout << "[Iteration complete]\n";
        volatile double sink = 0.0; // empêche l'optimisation du compilateur

        mesureTemps("list  ", [&]{ for (auto& p : lst) sink += p.getMasse(); });
        mesureTemps("deque ", [&]{ for (auto& p : dq)  sink += p.getMasse(); });
        mesureTemps("vector", [&]{ for (auto& p : vec) sink += p.getMasse(); });
        mesureTemps("set   ", [&]{ for (auto& p : st)  sink += p.getMasse(); });

        // ACCÈS ALÉATOIRE (index n/2)
        // set n'est pas testé : il ne supporte pas l'accès par indice
        std::cout << "[Acces aleatoire index n/2] (set non applicable)\n";

        mesureTemps("deque ", [&]{ sink += dq[idx_milieu].getMasse(); });
        mesureTemps("vector", [&]{ sink += vec[idx_milieu].getMasse(); });

        // INSERTION EN MILIEU
        // vector et list seulement (deque similaire à vector ici)
        std::cout << "[Insertion en milieu]\n";
        Particule extra = makeRandomParticule(mt, dist, n + 1);

        mesureTemps("list  ", [&]{
            auto it = lst.begin(); std::advance(it, n/2);
            lst.insert(it, extra);
        });
        mesureTemps("vector", [&]{
            vec.insert(vec.begin() + n/2, extra);
        });
        mesureTemps("deque ", [&]{
            dq.insert(dq.begin() + n/2, extra);
        });

        // SUPPRESSION EN MILIEU
        std::cout << "[Suppression en milieu]\n";

        mesureTemps("list  ", [&]{
            auto it = lst.begin(); std::advance(it, n/2);
            lst.erase(it);
        });
        mesureTemps("vector", [&]{
            vec.erase(vec.begin() + n/2);
        });
        mesureTemps("deque ", [&]{
            dq.erase(dq.begin() + n/2);
        });
    }
}

/// @brief Calcule la distance entre 2 particules
/// @param p1
/// @param p2 
/// @return la distance euclidienne entre les 2 particules
double distance(const Particule& p1, const Particule& p2) {
    const std::array<double, 3> position1 = p1.getPosition();
    const std::array<double, 3> position2 = p2.getPosition();
    double dist2 = 0.0;
    for (int i=0; i<3; i++) {
        double d = position2[i] - position1[i];
        dist2 += d * d;
    }
    return std::sqrt(dist2);
}

/// @brief Calcule le vecteur distance entre 2 particules (p2 - p1)
/// @param p1 
/// @param p2 
/// @return le vecteur distance entre les 2 particules
std::array<double, 3> vecteurDistance(const Particule& p1, const Particule& p2) {
    const auto position1 = p1.getPosition();
    const auto position2 = p2.getPosition();
    std::array<double, 3> vec;
    for (int i=0; i<3; i++) {
        vec[i] = position2[i] - position1[i];
    }
    return vec;
}

/// @brief Calcule la force gravitationnelle exercée sur la particule d'indice idx par les autres particules du système
/// @param p la liste des particules du système
/// @param idx l'indice de la particule pour laquelle on veut calculer la force
/// @return le vecteur force gravitationnelle exercée sur la particule d'indice idx
std::array<double, 3> calculForce(const std::deque<Particule>& p, size_t idx) {
    std::array<double, 3> F = {0.0, 0.0, 0.0};
    const Particule& pi = p[idx];
    for(size_t j = 0; j < p.size(); ++j) {
        if (j == idx) continue;            // skip self
        double d = distance(pi, p[j]);
        if (d == 0.0) continue;            // avoid division by zero
        F += (vecteurDistance(pi, p[j]) / d) * (pi.getMasse()*p[j].getMasse());
    }
    return F;
}

/// @brief  Calcule l'énergie cinétique d'une particule
/// @param p la particule pour laquelle on veut calculer l'énergie cinétique
/// @return l'énergie cinétique de la particule p, calculée à partir de sa masse et de sa vitesse
double calculEnergieCin(const Particule& p) {
    double v2 = 0.0;
    const auto& v = p.getVitesse();
    for (int i=0; i<3; i++) {
        v2 += v[i] * v[i];
    }
    return 0.5 * p.getMasse() * v2;
}


/// @brief Calcule l'énergie potentielle d'une particule par rapport aux autres particules du système
/// @param p la particule pour laquelle on veut calculer l'énergie potentielle
/// @param pList la liste des particules du système
/// @return l'énergie potentielle de la particule p, calculée à partir de sa masse et de la distance qui la sépare des autres particules du système
double calculEnergiePot(const Particule& p, const std::deque<Particule>& pList) {
    double Ep = 0.0;
    for(size_t j = 0; j < pList.size(); ++j) {
        double d = distance(p, pList[j]);
        if (d != 0.0) {
            Ep += (p.getMasse() * pList[j].getMasse()) / d;
        }
    }
    return Ep;
}

/// @brief Implemente l'algorithme de Stromer-Verlet pour faire évoluer le système de particules dans le temps
/// @param p la liste des particules du système
/// @param tend la durée d'intégration
/// @param dt le pas de temps
void StromerVerlet(std::deque<Particule>& p, double tend, double dt) {
    double t = 0.0;
    std::deque<std::array<double, 3>> F, F_old;
    F.resize(p.size());
    F_old.resize(p.size());
    for(size_t i=0; i<p.size(); ++i) {
        F_old[i] = p[i].getForce();
        F[i] = calculForce(p, i);
    }
    while (t < tend) {
        t += dt;

        for (size_t i=0; i<p.size(); ++i) { 
            p[i].setPosition((p[i].getPosition() + (p[i].getVitesse() + (F[i]*(0.5/p[i].getMasse()*dt))*dt)));
            F_old[i] = F[i];
        }

        for(size_t i=0; i<p.size(); ++i) {
            F[i] = calculForce(p, i);
        }
        
        for (size_t i=0; i<p.size(); ++i) {
            p[i].setVitesse((p[i].getVitesse() + (F[i] + F_old[i])* 0.5 * dt/ p[i].getMasse()));
            
        }
        
        double Ecin = 0.0, Epot = 0.0;
        for(size_t i=0; i<p.size(); ++i) {
            Ecin += calculEnergieCin(p[i]);
            Epot += calculEnergiePot(p[i], p);
        }
        std::cout << " t = " << t << " :" << std::endl;
        for(size_t i=0; i<p.size(); ++i) {
            std::cout << "  " << p[i].getCategorie() << ": x =" << p[i].getPosition()[0] << "; y = " << p[i].getPosition()[1] << ";" << std::endl;
        }
        std::cout << "\n";
    }
}

int main() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    compareStructures(mt, dist);

    Particule Soleil({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 1, "Soleil", 0);
    Particule Terre({0.0, 1.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, 3.0e-6, "Terre", 1);
    Particule Jupiter({0.0, 5.36, 0.0}, {-0.425, 0.0, 0.0}, {0.0, 0.0, 0.0}, 9.55e-4, "Jupiter", 2);
    Particule Hayley({34.75, 0.0, 0.0}, {0.0, 0.0296, 0.0}, {0.0, 0.0, 0.0}, 1e-14, "Hayley", 3);

        std::deque<Particule> systemeSolaire = {Soleil, Terre, Jupiter, Hayley};
        StromerVerlet(systemeSolaire,  468.5, 0.015);

    return 0;
}