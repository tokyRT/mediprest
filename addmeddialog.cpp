#include "addmeddialog.h"
#include "ui_addmeddialog.h"
#include "medecin.h"
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>

AddMedDialog::AddMedDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMedDialog)
{
    ui->setupUi(this);
    ui->pathPdp->setText(pdpPath);
    ui->tauxInput->setRange(1000, 100000);
    ui->numMed->setText("MED-"+QString::number(getLastIdMedecin()+1));
}

AddMedDialog::~AddMedDialog()
{
    delete ui;
}

void AddMedDialog::on_cancelBtn_clicked()
{
    this->close();
}

void AddMedDialog::on_addMedBtn_clicked()
{
    if(ui->nomInput->text()=="" || ui->prenomInput->text()=="" || ui->specialiteInput->text()=="" || ui->tauxInput->value()==0){
        QMessageBox::warning(this, "Atention", "Veuiller completer tous les champs s'il vous plait!");
        return;
    }


    QString nom = ui->nomInput->text(), prenom = ui->prenomInput->text();
    QString sexe = ui->sexeCombo->currentText(), spec = ui->specialiteInput->text();
    int taux = ui->tauxInput->value();

    Medecin med;
    med.id=0;
    strcpy(med.nom, nom.toLatin1());
    strcpy(med.prenom, prenom.toLatin1());
    strcpy(med.specialite, spec.toLatin1());
    strcpy(med.sexe, sexe.toLatin1());
    if(pdpPath!="Aucune photo choisie") uploadPdp(pdpPath, getLastIdMedecin()+1);
    //if(pdpPath=="Aucune photo choisie"){}
    //else pdpPath="pdp/MED-"+QString::number(getLastIdMedecin()+1)+".jpg";
    strcpy(med.photo, pdpPath.toLatin1());
    med.taux=taux;
    int nb;
    Medecin *all = getAllMedecin(&nb);
    bool exist=false;
    for(int i=0; i<nb; i++){
        if(strcmp(all[i].nom, med.nom)==0 && strcmp(all[i].prenom, med.prenom)==0) {
            exist=true;
            break;
        }
    }
    if(exist){
        QMessageBox::warning(this, "Doublon", "ce medecin existe deja");
        return;
    }
    addMedecin(med);
    qDebug()<<pdpPath;
    this->close();

}

void AddMedDialog::on_pdpBtn_clicked()
{
    pdpPath = QFileDialog::getOpenFileName(this, "Choisissez une photo", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    QPixmap pdp(pdpPath);
    qDebug()<<pdpPath;
    ui->pathPdp->setPixmap(pdp.scaled(200, 200, Qt::KeepAspectRatioByExpanding));
    ui->pathPdp->setStyleSheet("border: 1px solid black;");
    ui->pathPdp->setFixedSize(200, 200);

}
void AddMedDialog::uploadPdp(QString pathOrigin, int idMed)
{
    QDir *pdpDir = new QDir(QDir::currentPath()+"/pdp/");
    if(!pdpDir->exists()) pdpDir->mkpath(QDir::currentPath()+"/pdp/");
    QString ext;
    if(pathOrigin.contains(".png")) ext=".png";
    else if(pathOrigin.contains(".PNG")) ext=".PNG";
    else if(pathOrigin.contains(".jpg")) ext=".jpg";
    else if(pathOrigin.contains(".JPG")) ext=".JPG";
    else if(pathOrigin.contains(".JPEG")) ext=".JPEG";
    else if(pathOrigin.contains(".jpeg")) ext=".jpeg";
    else if(pathOrigin.contains(".gif")) ext=".gif";
    else if(pathOrigin.contains(".GIF")) ext=".GIF";
    else ext=".jpg";
    QFile::copy(pathOrigin, QDir::currentPath()+"/pdp/"+"MED-"+QString::number(idMed)+ext);
    pdpPath="pdp/MED-"+QString::number(idMed)+ext;
    return;

}









