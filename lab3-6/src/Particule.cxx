#include "Particule.hxx"

Particule::Particule(const Vecteur<>& position,
                     const Vecteur<>& vitesse,
                     const Vecteur<>& force,
                     double masse, const std::string& categorie, long identifiant)
    : position(position), vitesse(vitesse), force(force),
      masse(masse), categorie(categorie), identifiant(identifiant)
{}

// Accesseurs

Vecteur<> Particule::getPosition() const { return position; }

Vecteur<> Particule::getVitesse() const { return vitesse; }

Vecteur<> Particule::getForce() const { return force; }

double Particule::getMasse() const { return masse; }

const std::string& Particule::getCategorie() const { return categorie; }

long Particule::getIdentifiant() const { return identifiant; }

// Mutateurs

void Particule::setPosition(const Vecteur<>& position) { this->position = position; }

void Particule::setVitesse(const Vecteur<>& vitesse) { this->vitesse = vitesse; }

void Particule::setForce(const Vecteur<>& force) { this->force = force; }

void Particule::setMasse(double masse) { this->masse = masse; }