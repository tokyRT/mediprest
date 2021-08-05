#include "traitement.h"
#include <cstring>

void createFileTraitement()
{
    Traitement trait={0,0,0,0,31, 12, 2000, 10000, "description"};
    WRITE_IN_TRAIT_FILE;
    fprintf(fd, PRINT_TRAIT_FORMAT, PRINT_STRUCT_TRAIT);
    fclose(fd);
}
int getLastTraitementId()
{
    READ_IN_TRAIT_FILE;
    if(fd==NULL) return -1;
    Traitement trait;
    while(!feof(fd)) fscanf(fd, SCAN_TRAIT_FORMAT, SCAN_STRUCT_TRAIT);
    fclose(fd);
    return trait.id;
}
void addTraitement(Traitement trait)
{
    READ_IN_TRAIT_FILE;
    if(fd==NULL){
        createFileTraitement();
        fclose(fd);
    }
    fclose(fd);
    fd=fopen(TRAIT_FILE, "a");
    trait.id=getLastTraitementId()+1;
    fprintf(fd, PRINT_TRAIT_FORMAT, PRINT_STRUCT_TRAIT);
    fclose(fd);
}
void showTrait(Traitement trait)
{
    printf(PRINT_TRAIT_FORMAT, PRINT_STRUCT_TRAIT);
}
void showTraitTableContent()
{
    Traitement trait;
    READ_IN_TRAIT_FILE;
    if(fd==NULL) {printf("table not found!"); return;}
    int entry=-1;
    while(!feof(fd)){
        fscanf(fd, SCAN_TRAIT_FORMAT, SCAN_STRUCT_TRAIT);
        printf(PRINT_TRAIT_FORMAT, PRINT_STRUCT_TRAIT);
        entry++;
    }
    printf("\n\nTotal Entries: %d\n\n", entry);
}
void removeTraitementById(int idTrait)
{
    if(idTrait==0) return;
    Traitement trait;
    READ_IN_TRAIT_FILE;
    if(fd==NULL) return;
    FILE *tmpFile=fopen("traitement.table.tmp", "w");
    while(!feof(fd)){
        fscanf(fd, SCAN_TRAIT_FORMAT, SCAN_STRUCT_TRAIT);
        if(trait.id!=idTrait) fprintf(tmpFile, PRINT_TRAIT_FORMAT, PRINT_STRUCT_TRAIT);
    }
    fclose(fd); fclose(tmpFile);
    remove(TRAIT_FILE);
    rename("traitement.table.tmp", TRAIT_FILE);
}
void updateTraitementById(int idTrait, Traitement newTrait)
{
    newTrait.id=idTrait;
    Traitement trait;
    READ_IN_TRAIT_FILE;
    if(fd==NULL) return;
    FILE* tmpFile=fopen("traitement.table.tmp", "w");
    while(!feof(fd)){
        fscanf(fd, SCAN_TRAIT_FORMAT, SCAN_STRUCT_TRAIT);
        if(trait.id==idTrait) fprintf(tmpFile, PRINT_TRAIT_FORMAT, newTrait.id, newTrait.idMed, newTrait.idPat, newTrait.nbJours, newTrait.dateConsu.day, newTrait.dateConsu.mounth, newTrait.dateConsu.year, newTrait.taux ,newTrait.description);
        else fprintf(tmpFile, PRINT_TRAIT_FORMAT, PRINT_STRUCT_TRAIT);
    }
    fclose(fd); fclose(tmpFile);
    remove(TRAIT_FILE);
    rename("traitement.table.tmp", TRAIT_FILE);
}
Traitement getTraitementById(int idTrait)
{
    Traitement trait;
    READ_IN_TRAIT_FILE;
    if(fd==NULL) return trait;
    while(!feof(fd)){
        fscanf(fd, SCAN_TRAIT_FORMAT, SCAN_STRUCT_TRAIT);
        if(trait.id==idTrait) break;
    }
    fclose(fd);
    return trait;
}
Traitement* getAllTraitement(int *nb)
{
    Traitement *trait=(Traitement*)malloc(sizeof(Traitement));
    READ_IN_TRAIT_FILE;
    int i=0, positionCurseur=0;
    while(!feof(fd)){
        fscanf(fd, SCAN_TRAIT_FORMAT, &trait[i].id, &trait[i].idMed, &trait[i].idPat, &trait[i].nbJours, &trait[i].dateConsu.day, &trait[i].dateConsu.mounth, &trait[i].dateConsu.year, &trait[i].taux ,trait[i].description);
        if(positionCurseur>0){
            i++;
            trait=(Traitement*)realloc(trait, sizeof(Traitement)*(i+1));
        }
        positionCurseur++;
    }
    fclose(fd);
    *nb=i;
    return trait;
}
int getTraitementNumber()
{
    int nb;
    Traitement *t=getAllTraitement(&nb);
    return nb;
}

Traitement* getPatTraitement(int idPat, int *nb)
{
    Traitement *trait = (Traitement*)malloc(sizeof(Traitement));
    int eff,i;
    Traitement *all=getAllTraitement(&eff);
    int j=0;
    for(i=0; i<eff; i++){
        if(all[i].idPat==idPat){
            trait[j].id=all[i].id;
            trait[j].idMed=all[i].idMed;
            trait[j].idPat=all[i].idPat;
            trait[j].nbJours=all[i].nbJours;
            trait[j].dateConsu.day=all[i].dateConsu.day;
            trait[j].dateConsu.mounth=all[i].dateConsu.mounth;
            trait[j].dateConsu.year=all[i].dateConsu.year;
            trait[j].taux=all[i].taux;
            strcpy(trait[j].description, all[i].description);
            j++;
            trait=(Traitement*)realloc(trait, sizeof(Traitement)*(j+1));
        }
    }
    *nb=j;
    return trait;
}
Traitement* getMedTraitement(int idMed, int* nb)
{
     Traitement *trait = (Traitement*)malloc(sizeof(Traitement));
    int eff,i;
    Traitement *all=getAllTraitement(&eff);
    int j=0;
    for(i=0; i<eff; i++){
        if(all[i].idMed==idMed){
            trait[j].id=all[i].id;
            trait[j].idMed=all[i].idMed;
            trait[j].idPat=all[i].idPat;
            trait[j].nbJours=all[i].nbJours;
            trait[j].dateConsu.day=all[i].dateConsu.day;
            trait[j].dateConsu.mounth=all[i].dateConsu.mounth;
            trait[j].dateConsu.year=all[i].dateConsu.year;
            trait[j].taux=all[i].taux;
            strcpy(trait[j].description, all[i].description);
            j++;
            trait=(Traitement*)realloc(trait, sizeof(Traitement)*(j+1));
        }
    }
    *nb=j;
    return trait;
}
int getPrest(int idMed, int year)
{
    int nb;
    Traitement *trait=getMedTraitement(idMed, &nb);
    int i, prest=0;
    for(i=0; i<nb; i++){
        if(trait[i].dateConsu.year==year) prest+=trait[i].taux*trait[i].nbJours;
    }
    return prest;
}
int getPrestMounth(int idMed, int mounth, int year)
{
    int nb;
    Traitement *trait=getMedTraitement(idMed, &nb);
    int i, prest=0;
    for(i=0; i<nb; i++){
        if(trait[i].dateConsu.mounth==mounth && trait[i].dateConsu.year==year) prest+=trait[i].taux*trait[i].nbJours;
    }
    return prest;
}
int getPatNb(int idMed, int mounth, int year)
{
    int nb;
    Traitement *trait=getMedTraitement(idMed, &nb);
    int i, patNb=0;
    for(i=0; i<nb; i++){
        if(trait[i].dateConsu.mounth==mounth && trait[i].dateConsu.year==year) patNb++;;
    }
    return patNb;
}






