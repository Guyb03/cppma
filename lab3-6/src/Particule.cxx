#include "Particule.hxx"

Particule::Particule(const Vecteur<3>& pos, const Vecteur<3>& vit, const Vecteur<3>& frc,
                     double m, const std::string& cat, long id)
    : position(pos), vitesse(vit), force(frc), force_old{},
      masse(m), categorie(cat), identifiant(id)
{}

bool Particule::operator<(const Particule& other) const {
    return identifiant < other.identifiant;
}

const Vecteur<3>& Particule::getPosition()    const { return position;  }
const Vecteur<3>& Particule::getVitesse()     const { return vitesse;   }
const Vecteur<3>& Particule::getForce()       const { return force;     }
const Vecteur<3>& Particule::getForceOld()    const { return force_old; }
double           Particule::getMasse()       const { return masse;     }
const std::string& Particule::getCategorie() const { return categorie; }
long             Particule::getIdentifiant() const { return identifiant; }

void Particule::setPosition(const Vecteur<3>& pos) { position  = pos;                }
void Particule::setVitesse (const Vecteur<3>& vit) { vitesse   = vit;                }
void Particule::setForce   (const Vecteur<3>& frc) { force = frc; }
void Particule::setMasse   (double m)             { masse     = m;             }

std::ostream& operator<<(std::ostream& os, const Particule& p) {
    os << "[" << p.getCategorie() << " id=" << p.getIdentifiant()
       << " pos=" << p.getPosition()
       << " vit=" << p.getVitesse()
       << " m="   << p.getMasse() << "]";
    return os;
}
