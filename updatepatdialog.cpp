#include "updatepatdialog.h"
#include "ui_updatepatdialog.h"
#include <QMessageBox>

UpdatePatDialog::UpdatePatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdatePatDialog)
{
    ui->setupUi(this);
}
UpdatePatDialog::UpdatePatDialog(int idPat, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdatePatDialog)
{
    ui->setupUi(this);

    Patient oldPat = getPatientById(idPat);
    ui->numPat->setText("PAT-"+QString::number(idPat));
    ui->nomInput->setText(QString(oldPat.nom));
    ui->prenomInput->setText(QString(oldPat.prenom));
    ui->sexeCombo->setCurrentText(QString(oldPat.sexe));
    ui->adresseInput->setText(QString(oldPat.adresse));
}

UpdatePatDialog::~UpdatePatDialog()
{
    delete ui;
}

void UpdatePatDialog::on_savePatBtn_clicked()
{
    if(ui->nomInput->text()=="" || ui->prenomInput->text()=="" || ui->adresseInput->text()==""){
        QMessageBox::warning(this, "Atention", "Veuiller completer tous les champs s'il vous plait!");
        return;
    }
    QString nom = ui->nomInput->text(), prenom = ui->prenomInput->text();
    QString sexe = ui->sexeCombo->currentText(), adresse=ui->adresseInput->text();

    Patient pat;
    pat.id=ui->numPat->text().remove("PAT-").toInt();
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
    updatePatientById(pat.id, pat);
    this->close();
}

void UpdatePatDialog::on_cancelBtn_clicked()
{
    this->close();
}
