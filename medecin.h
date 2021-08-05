#ifndef MEDECIN_H
#define MEDECIN_H


#include <stdio.h>
#include <stdlib.h>

#define SCAN_MED_FORMAT "%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%d\n"
#define SCAN_STRUCT_MED &med.id, med.nom, med.prenom, med.sexe, med.specialite, med.photo, &med.taux
#define PRINT_MED_FORMAT "%d\t%s\t%s\t%s\t%s\t%s\t%d\n"
#define PRINT_STRUCT_MED med.id, med.nom, med.prenom, med.sexe, med.specialite, med.photo, med.taux
#define MED_FILE "medecin.table"
#define READ_IN_MED_FILE FILE *fd=fopen(MED_FILE, "r")
#define APP_IN_MED_FILE FILE *fd=fopen(MED_FILE, "a")
#define WRITE_IN_MED_FILE FILE *fd=fopen(MED_FILE, "w")

typedef struct Medecin Medecin;
struct Medecin{
    int id; // automatiquement completé dans les fichiers
    char nom[50];
    char prenom[50];
    char sexe[10];
    char specialite[50];
    char photo[100]; // chemin de la photo de profil
    int taux;
};
void createFileMedecin(); //cree le fichier medecin
void addMedecin(Medecin med); //ajoute un medecin
void removeMedecinById(int idMed); //supprime un medecin dont l'id est envoye en parametre (int idMed)
void removeMedecinByName(const char *prenomMed); //Mbola tsy vita ity fonction ity
void updateMedecinById(int oldMedId, Medecin newMed); //modifie le medecin dont l'id (int idMed) est envoyé en parametre
Medecin getMedecinById(int idMed); //retourne le medecin correspondant a son id
Medecin* getAllMedecin(int *nb);
int getLastIdMedecin(); //retourne le dernier numero de medecin dans le fichier
int getMedNumber(); //renvoie l'effectif des medecins

// ces fonctions sont juste pour les tests
void showMedTableContent();
void showMed(Medecin med);

#endif // MEDECIN_H
