#include "updatemeddialog.h"
#include "ui_updatemeddialog.h"
#include "medecin.h"
#include "QMessageBox"
#include <QFileDialog>
#include <QDir>
#include <QDebug>

UpdateMedDialog::UpdateMedDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateMedDialog)
{
    ui->setupUi(this);
}
UpdateMedDialog::UpdateMedDialog(QWidget *parent, int idMed) :
    QDialog(parent),
    ui(new Ui::UpdateMedDialog)
{
    ui->setupUi(this);
    Medecin oldMed = getMedecinById(idMed);
    ui->numMed->setText("MED-"+QString::number(oldMed.id));
    ui->nomInput->setText(QString(oldMed.nom));
    ui->prenomInput->setText(QString(oldMed.prenom));
    ui->sexeCombo->setCurrentText(QString(oldMed.sexe));
    ui->specialiteInput->setText(QString(oldMed.specialite));
    ui->tauxInput->setRange(1000, 100000);
    ui->tauxInput->setValue(oldMed.taux);
    QString photo;
    if(QString(oldMed.photo)=="Aucune photo choisie"){
        if(QString(oldMed.sexe)=="Masculin") photo=":/img/img/Masculin.png";
        else photo=":/img/img/Feminin.ico";
    }
    else photo=QDir::currentPath()+"/"+QString(oldMed.photo);
    QPixmap pdp(photo);
    ui->pathPdp->setPixmap(pdp.scaled(150, 150, Qt::KeepAspectRatioByExpanding));
    ui->pathPdp->setFixedSize(150, 150);
    qDebug()<<QDir::currentPath()+"/"+QString(oldMed.photo);
    newMed.id = idMed;
}

UpdateMedDialog::~UpdateMedDialog()
{
    delete ui;
}

void UpdateMedDialog::on_updateMedBtn_clicked()
{
    if(ui->nomInput->text()=="" || ui->prenomInput->text()=="" || ui->specialiteInput->text()=="" || ui->tauxInput->value()==0){
        QMessageBox::warning(this, "Atention", "Veuiller completer tous les champs s'il vous plait!");
        return;
    }


    QString nom = ui->nomInput->text(), prenom = ui->prenomInput->text();
    QString sexe = ui->sexeCombo->currentText(), spec = ui->specialiteInput->text();
    int taux = ui->tauxInput->value();

    strcpy(newMed.nom, nom.toLatin1());
    strcpy(newMed.prenom, prenom.toLatin1());
    strcpy(newMed.specialite, spec.toLatin1());
    strcpy(newMed.sexe, sexe.toLatin1());
    if(pdpPath==""){
        pdpPath=QString(getMedecinById(newMed.id).photo);
    }
    else uploadPdp(pdpPath, newMed.id);
    strcpy(newMed.photo, pdpPath.toLatin1());
    newMed.taux=taux;
    updateMedecinById(newMed.id, newMed);
    this->close();
}



void UpdateMedDialog::on_pdpBtn_clicked()
{
    pdpPath = QFileDialog::getOpenFileName(this, "Choisissez une photo", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    if(pdpPath=="") return;
    QPixmap pdp(pdpPath);
    ui->pathPdp->setPixmap(pdp.scaled(150, 150, Qt::KeepAspectRatioByExpanding));
    ui->pathPdp->setFixedSize(150, 150);
}
void UpdateMedDialog::on_cancelBtn_clicked()
{
    this->close();
}

void UpdateMedDialog::uploadPdp(QString pathOrigin, int idMed)
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
    Medecin med=getMedecinById(idMed);
    QFile::remove(QString(med.photo));
    QFile::copy(pathOrigin, QDir::currentPath()+"/pdp/"+"MED-"+QString::number(idMed)+ext);
    pdpPath="pdp/MED-"+QString::number(idMed)+ext;
    return;
}




















