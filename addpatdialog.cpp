#include "addpatdialog.h"
#include "ui_addpatdialog.h"
#include "patient.h"
#include <QMessageBox>
#include <QSystemTrayIcon>

AddPatDialog::AddPatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPatDialog)
{
    ui->setupUi(this);
    ui->numPat->setText("PAT-"+QString::number(getLastPatientId()+1));
}

AddPatDialog::~AddPatDialog()
{
    delete ui;
}

void AddPatDialog::on_addMedBtn_clicked()
{
    if(ui->nomInput->text()=="" || ui->prenomInput->text()=="" || ui->adresseInput->text()==""){
        QMessageBox::warning(this, "Atention", "Veuiller completer tous les champs s'il vous plait!");
        return;
    }
    QString nom = ui->nomInput->text(), prenom = ui->prenomInput->text();
    QString sexe = ui->sexeCombo->currentText(), adresse=ui->adresseInput->text();

    Patient pat;
    pat.id=0;
    strcpy(pat.nom, nom.toLatin1());
    strcpy(pat.prenom, prenom.toLatin1());
    strcpy(pat.adresse, adresse.toLatin1());
    strcpy(pat.sexe, sexe.toLatin1());
    int nb;
    Patient *all=getAllPatient(&nb);
    for (int i=0;i<nb;i++) {
        if(QString(all[i].nom)==nom && QString(all[i].prenom)==prenom && QString(all[i].adresse)==adresse){
            QMessageBox::warning(this, "Doublon", "ce patient existe deja </br>PAT-"+QString::number(all[i].id)+" "+QString(all[i].nom)+" "+QString(all[i].prenom));
            return;
        }
    }
    addPatient(pat);
    this->close();
}

void AddPatDialog::on_cancelBtn_clicked()
{
    this->close();
}
