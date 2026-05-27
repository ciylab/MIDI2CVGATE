/**
 * @file Modules.h
 * @brief Classe pour l'ensemble des modules. 
 *
 * Contient le texte pour l'affichage et le numéro du module affiché.
 */
#ifndef MODULES_H
#define MODULES_H
#include "Module.h"

class Modules {
    private:
        int size;
        
    public:
        /** Le nom des modules utilisés.*/
        char page[65];
        /** Numéro du module affiché. */
        int moduleNum;
        /** Indique un changement de page.*/
        bool newPage;
        /** Tableau des modules.*/
        Module *modules[8];
        /** Constructeur avec initialisation d'une page blanche.*/
        Modules();
        /** Mise à jour du menu principal.*/
        void updatePage(Module m);
        /** Fonction d'ajout des modules.*/
        void add(Module *m);
        /** Retourne le module affiché.*/
        Module *getCurrentModule();
        void setCurrentModule(int number);
        /** Retourne le nombre de modules.*/
        int getSize() {
            return this->size;
        }
};

#endif
