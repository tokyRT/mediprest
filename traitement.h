#ifndef TRAITEMENT_H_INCLUDED
#define TRAITEMENT_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

#define TRAIT_FILE "traitement.table"
#define WRITE_IN_TRAIT_FILE FILE *fd=fopen(TRAIT_FILE, "w")
#define READ_IN_TRAIT_FILE FILE *fd=fopen(TRAIT_FILE, "r")
#define APP_IN_TRAIT_FILE FILE *fd=fopen(TRAIT_FILE, "a")
#define SCAN_TRAIT_FORMAT "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%[^\n]\n"
#define SCAN_STRUCT_TRAIT &trait.id, &trait.idMed, &trait.idPat, &trait.nbJours, &trait.dateConsu.day, &trait.dateConsu.mounth, &trait.dateConsu.year, &trait.taux, trait.description
#define PRINT_TRAIT_FORMAT "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n"
#define PRINT_STRUCT_TRAIT trait.id, trait.idMed, trait.idPat, trait.nbJours, trait.dateConsu.day, trait.dateConsu.mounth, trait.dateConsu.year, trait.taux, trait.description

typedef struct Date Date;
struct Date{
    int day;
    int mounth;
    int year;
};

typedef struct Traitement Traitement;
struct Traitement{
    int id; //autoincrement
    int idMed;
    int idPat;
    int nbJours;
    Date dateConsu;
    int taux;
    char description[300]; //facultatif
};
void createFileTraitement();
void addTraitement(Traitement trait);
void removeTraitementById(int idTrait);
void updateTraitementById(int idTrait, Traitement newTrait);
Traitement getTraitementById(int idTrait);
Traitement* getAllTraitement(int *nb);
int getLastTraitementId();
int getTraitementNumber();
Traitement* getPatTraitement(int idPat, int *nb);
Traitement* getMedTraitement(int idMed, int *nb);
int getPrest(int idMed, int year);
int getPrestMounth(int idMed, int mounth, int year);
int getPatNb(int idMed, int mounth, int year);


//ces fonctions sont juste pour les tests
void showTraitTableContent();
void showTrait(Traitement trait);



#endif // TRAITEMENT_H_INCLUDED
