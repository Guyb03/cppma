#include "Cellule.hxx"

Cellule::Cellule(const Vecteur<>& c) : centre(c) {}

const Vecteur<>&        Cellule::getCentre()  const { return centre;  }
const std::vector<int>& Cellule::getIndices() const { return indices; }

void Cellule::addParticule(int idx) { indices.push_back(idx); }
void Cellule::vider() { indices.clear(); }
