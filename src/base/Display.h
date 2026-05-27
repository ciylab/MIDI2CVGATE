/**
 * @file Display.h
 * @brief Gestion de l'affichage.
 *
 * Utilisation d'un tampon pour n'afficher que les caractères 
 * qui changent. 
 *
 * À chaque boucle du fichier ino, on place un nouveau 
 * caractère de la position begin jusqu'à la position end.
 *
 * On limite la latence au maximum pour mieux gérer
 * le temps réel.
 */
#ifndef DISPLAY_H
#define DISPLAY_H
#define CURSOR '>'

class Display
{
    private:
        char screen[65];
        char buffer[65];
        int begin;
        int end;
    public:
        /**
         * Initialisation avec un écran vide.
         */
        Display();
        /**
         * Choix de la police (modifiée).
         */
        void init();
        /**
         * Afficher un message de bienvenue.
         */
        void welcome();
        /**
         * Vidage de l'écran.
         */
        void clear();
        /**
         * Placement sans tampon d'un caractère.
         */
        void writeChar(int position, char c);
        /**
         * Retourne la position du prochain caractère à placer.
         */
        void nextBegin();
        /**
         * Mettre dans le tampon la page à afficher.
         */
        void setBuffer(char str[65]);
        void clearCursor();
        void showCursor();
        /**
         * Effacement du curseur dans sa position courante
         * et écriture dans sa nouvelle position sans tampon.
         */
        void moveCursor();
        /**
         * Fonction d'affichage d'une barre de progression.
         */
        void progressBar(char bar[8], int level);
        /**
         * Fonction d'affichage d'une barre de progression centrée.
         */
        void progressCenteredBar(char bar[8], int level);
        /**
         * Mise en tampon de la nouvelle page.
         */        
        void loadPage();
        /**
         * Afficher la valeur sur une plage de 7 caractères.
         */
        void showValue();
        /**
         * Afficher le nom du paramètre sur une plage de 7 caractères.
         */
        void showName();
        /**
         * Algorithme de l'affichage caractère par caractère.
         */
        void display();
};

#endif
