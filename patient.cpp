#include "patient.h"



void createFilePatient()
{
    Patient pat={0, "Nom", "Prenom", "sexe", "Adresse"};
    WRITE_IN_PAT_FILE;
    fprintf(fd, PRINT_PAT_FORMAT, PRINT_STRUCT_PAT);
    fclose(fd);
}
int getLastPatientId()
{
    READ_IN_PAT_FILE;
    if(fd==nullptr) return -1;
    Patient pat;
    while(!feof(fd)) fscanf(fd, SCAN_PAT_FORMAT, SCAN_STRUCT_PAT);
    fclose(fd);
    return pat.id;
}
void addPatient(Patient pat)
{
    READ_IN_PAT_FILE;
    if(fd==nullptr){
        createFilePatient();
        fclose(fd);
    }
    fclose(fd);
    fd=fopen(PAT_FILE, "a");
    pat.id=getLastPatientId()+1;
    fprintf(fd, PRINT_PAT_FORMAT, PRINT_STRUCT_PAT);
    fclose(fd);
}
void showPat(Patient pat)
{
    printf(PRINT_PAT_FORMAT, PRINT_STRUCT_PAT);
}
void showPatTableContent()
{
    Patient pat;
    READ_IN_PAT_FILE;
    if(fd==nullptr){printf("Table not found!"); return;}
    int entry=-1;
    while(!feof(fd)){
        fscanf(fd, SCAN_PAT_FORMAT, SCAN_STRUCT_PAT);
        printf(PRINT_PAT_FORMAT, PRINT_STRUCT_PAT);
        entry++;
    }
    printf("\n\nTotal Entries: %d\n\n", entry);
}
void removePatientById(int idPat)
{
    if(idPat==0)  return;
    Patient pat;
    READ_IN_PAT_FILE;
    if(fd==nullptr) return;
    FILE* tmpFile=fopen("patient.table.tmp", "w");
    while(!feof(fd)){
        fscanf(fd, SCAN_PAT_FORMAT, SCAN_STRUCT_PAT);
        if(pat.id!=idPat) fprintf(tmpFile, PRINT_PAT_FORMAT, PRINT_STRUCT_PAT);
    }
    fclose(fd); fclose(tmpFile);
    remove(PAT_FILE);
    rename("patient.table.tmp", PAT_FILE);
}
void updatePatientById(int idPat, Patient newPat)
{
    newPat.id=idPat;
    Patient pat;
    READ_IN_PAT_FILE;
    if(fd==nullptr) return;
    FILE* tmpFile=fopen("patient.table.tmp", "w");
    while(!feof(fd)){
        fscanf(fd, SCAN_PAT_FORMAT, SCAN_STRUCT_PAT);
        if(pat.id==idPat) fprintf(tmpFile, PRINT_PAT_FORMAT, newPat.id, newPat.nom, newPat.prenom, newPat.sexe, newPat.adresse);
        else fprintf(tmpFile, PRINT_PAT_FORMAT, PRINT_STRUCT_PAT);
    }
    fclose(fd); fclose(tmpFile);
    remove(PAT_FILE);
    rename("patient.table.tmp", PAT_FILE);
}
Patient getPatientById(int idPat)
{
    Patient pat;
    READ_IN_PAT_FILE;
    if(fd==nullptr) return pat;
    while(!feof(fd)){
        fscanf(fd, SCAN_PAT_FORMAT, SCAN_STRUCT_PAT);
        if(pat.id==idPat) break;
    }
    fclose(fd);
    return pat;
}
Patient* getAllPatient(int *nb)
{
    Patient *pat=(Patient*)malloc(sizeof(Patient));
    READ_IN_PAT_FILE;
    if(fd==nullptr){
        createFilePatient();
        fscanf(fd, SCAN_PAT_FORMAT,  &pat[0].id, pat[0].nom, pat[0].prenom, pat[0].sexe, pat[0].adresse);
        *nb=0;
        return pat;
    }
    int i=0, positionCurseur=0;
    while(!feof(fd)){
        fscanf(fd, SCAN_PAT_FORMAT, &pat[i].id, pat[i].nom, pat[i].prenom, pat[i].sexe, pat[i].adresse);
        if(positionCurseur>0){
            i++;
            pat=(Patient*)realloc(pat, sizeof(Patient)*(i+1));
        }
        positionCurseur++;
    }
    fclose(fd);
    *nb=i;
    return pat;
}
int getPatientNumber()
{
    int nb;
    Patient *p=getAllPatient(&nb);
    delete p;
    return nb;
}






