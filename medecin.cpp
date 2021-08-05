#include "medecin.h"
#include <stdio.h>


void createFileMedecin()
{
    Medecin med={0, "Nom", "Prenom", "Sexe", "Specialite", "Photo de Profil", 0};
    WRITE_IN_MED_FILE;
    fprintf(fd, PRINT_MED_FORMAT, PRINT_STRUCT_MED);

    fclose(fd);
}
int getLastIdMedecin()
{
    READ_IN_MED_FILE;
    if(fd==nullptr) return -1;
    Medecin med;
    while(!feof(fd))
        fscanf(fd, SCAN_MED_FORMAT, SCAN_STRUCT_MED);
    fclose(fd);
    return med.id;
}
void addMedecin(Medecin med)
{
    READ_IN_MED_FILE;
    if(fd==nullptr){
       createFileMedecin();
       fclose(fd);
    }
    fclose(fd);
    fd=fopen(MED_FILE, "a");
    med.id=getLastIdMedecin()+1;
    fprintf(fd, PRINT_MED_FORMAT, PRINT_STRUCT_MED);
    fclose(fd);
}
void showMed(Medecin med)
{
    printf(PRINT_MED_FORMAT, PRINT_STRUCT_MED);
}
void showMedTableContent()
{
    Medecin med;
    READ_IN_MED_FILE;
    if(fd==nullptr){
        printf("Table not found!");
        return;
    }
    int entry=-1;
    while(!feof(fd)){
        fscanf(fd, SCAN_MED_FORMAT, SCAN_STRUCT_MED);
        printf(PRINT_MED_FORMAT, PRINT_STRUCT_MED);
        entry++;
    }
    printf("\n\nTotal Entries: %d\n\n", entry);

}
void removeMedecinById(int idMed)
{
    if(idMed==0) return;
        Medecin med;
        READ_IN_MED_FILE;
        if(fd==nullptr) return;
        FILE* tmpFile=fopen("medecin.table.tmp", "a");
        while(!feof(fd)){
            fscanf(fd, SCAN_MED_FORMAT, SCAN_STRUCT_MED);
            if(med.id!=idMed) fprintf(tmpFile, PRINT_MED_FORMAT, PRINT_STRUCT_MED);
        }
        fclose(fd);
        fclose(tmpFile);
        remove(MED_FILE);
        rename("medecin.table.tmp", MED_FILE);
}
void updateMedecinById(int oldMedId, Medecin newMed)
{
    newMed.id=oldMedId;
    Medecin med;
    READ_IN_MED_FILE;
    if(fd==nullptr) return;
    FILE* tmpFile=fopen("medecin.table.tmp", "a");
    while(!feof(fd)){
        fscanf(fd, SCAN_MED_FORMAT, SCAN_STRUCT_MED);
        if(med.id==oldMedId) fprintf(tmpFile, PRINT_MED_FORMAT, newMed.id, newMed.nom, newMed.prenom, newMed.sexe, newMed.specialite, newMed.photo, newMed.taux);
        else fprintf(tmpFile, PRINT_MED_FORMAT, PRINT_STRUCT_MED);
    }
    fclose(fd);
    fclose(tmpFile);
    remove(MED_FILE);
    rename("medecin.table.tmp", MED_FILE);
}
Medecin getMedecinById(int idMed)
{
    Medecin med;
    READ_IN_MED_FILE;
    if(fd==nullptr) return med;
    while(!feof(fd)){
        fscanf(fd, SCAN_MED_FORMAT, SCAN_STRUCT_MED);
        if(med.id==idMed) break;
    }
    fclose(fd);
    return med;
}
Medecin* getAllMedecin(int *nb)
{
    Medecin *med=(Medecin *)malloc(sizeof(Medecin));
    READ_IN_MED_FILE;
    if(fd==nullptr){
      createFileMedecin();
      fscanf(fd, SCAN_MED_FORMAT, &med[0].id, med[0].nom, med[0].prenom, med[0].sexe, med[0].specialite, med[0].photo, &med[0].taux);
      *nb=0;
      return  med;
    }
    int i=0, positionCurseur=0;
    while(!feof(fd)){
        fscanf(fd, SCAN_MED_FORMAT, &med[i].id, med[i].nom, med[i].prenom, med[i].sexe, med[i].specialite, med[i].photo, &med[i].taux);
        if(positionCurseur>0){
            i++;
            med=(Medecin *)realloc(med, sizeof(Medecin)*(i+1));
        }
        positionCurseur++;
    }
    fclose(fd);
    *nb=i;
    return med;
}
int getMedNumber()
{
    int nb;
    Medecin *m=getAllMedecin(&nb);
    return nb;
}









