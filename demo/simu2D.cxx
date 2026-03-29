#include "Univers.hxx"
#include "Particule.hxx"
#include <cmath>

int main() {
    // Paramètres physiques
    const double epsilon = 5.0;
    const double sigma   = 1.0;
    const double m       = 1.0;
    const double rcut    = 2.5 * sigma;
    const double dt      = 5e-5;
    const double d       = std::pow(2.0, 1.0/6.0) * sigma; // ≈ 1.122

    // Dimensions des objets en nombre de particules
    const int nx_rect = 160, ny_rect = 40; // rectangle bleu  (N2 = 6400)
    const int nx_carr = 40,  ny_carr = 40; // carré rouge     (N1 = 1600)

    // Domaine : Lx=250 (sujet), Ly calculé pour contenir les deux objets + marges
    const double Lx  = 250.0;
    const double gap = 3.0 * rcut;
    const double Ly  = (ny_rect - 1)*d + (ny_carr - 1)*d + gap + 4.0*rcut;

    Univers u(2, {Lx, Ly, 0.0}, epsilon, sigma, rcut, 0.0, false, true);

    long id = 0;

    // Rectangle bleu : centré en x, positionné en bas du domaine, vitesse nulle
    const double rect_x0 = (Lx - (nx_rect - 1)*d) / 2.0;
    const double rect_y0 = rcut;

    for (int i = 0; i < nx_rect; ++i) {
        for (int j = 0; j < ny_rect; ++j) {
            u.addParticule(Particule(
                Vecteur<3>{rect_x0 + i*d, rect_y0 + j*d, 0.0},
                Vecteur<3>{0.0, 0.0, 0.0},
                Vecteur<3>{},
                m, "bleu", id++
            ));
        }
    }

    // Carré rouge : centré horizontalement sur le rectangle, au-dessus, vitesse vers le bas
    const double carr_x0 = rect_x0 + ((nx_rect - 1) - (nx_carr - 1)) * d / 2.0;
    const double carr_y0 = rect_y0 + (ny_rect - 1)*d + gap;

    for (int i = 0; i < nx_carr; ++i) {
        for (int j = 0; j < ny_carr; ++j) {
            u.addParticule(Particule(
                Vecteur<3>{carr_x0 + i*d, carr_y0 + j*d, 0.0},
                Vecteur<3>{0.0, -10.0, 0.0},
                Vecteur<3>{},
                m, "rouge", id++
            ));
        }
    }

    // Sauvegarde VTK toutes les 1000 itérations dans le répertoire courant
    // Créer le dossier avant de lancer : mkdir -p vtk
    u.StromerVerlet(0.0, 19.5, dt, false, 1000, "vtk/simu2D");
    return 0;
}
