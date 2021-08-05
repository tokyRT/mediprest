#ifndef PATIENT_H_INCLUDED
#define PATIENT_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

#define PAT_FILE "patient.table"
#define WRITE_IN_PAT_FILE FILE *fd=fopen(PAT_FILE, "w")
#define READ_IN_PAT_FILE FILE *fd=fopen(PAT_FILE, "r")
#define APP_IN_PAT_FILE FILE *fd=fopen(PAT_FILE, "a")
#define SCAN_PAT_FORMAT "%d\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\n]\n"
#define SCAN_STRUCT_PAT &pat.id, pat.nom, pat.prenom, pat.sexe, pat.adresse
#define PRINT_PAT_FORMAT "%d\t%s\t%s\t%s\t%s\n"
#define PRINT_STRUCT_PAT pat.id, pat.nom, pat.prenom, pat.sexe, pat.adresse

typedef struct Patient Patient;
struct Patient{
    int id; //auto-increment
    //char matricule[40]; //generé automatiquement
    char nom[40];
    char prenom[40];
    char sexe[10];
    char adresse[200];
};
void createFilePatient();
void addPatient(Patient pat);
void removePatientById(int idPat);
void updatePatientById(int oldPatId, Patient newPat);
Patient getPatientById(int idPat);
Patient* getAllPatient(int *nb);
int getLastPatientId();
int getPatientNumber();

//ces fonctions sont juste pour les tests
void showPatTableContent();
void showPat(Patient pat);

#endif // PATIENT_H_INCLUDED
