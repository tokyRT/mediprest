#include "addtraitdialog.h"
#include "ui_addtraitdialog.h"


AddTraitDialog::AddTraitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTraitDialog)
{
    ui->setupUi(this);
    setWindowTitle("Ajout de traitement");
    setWindowIcon(QIcon(":/img/img/logo.png"));
    basicSetup();
}
AddTraitDialog::AddTraitDialog(int idPat, EditCritereDialog *critere, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTraitDialog)
{
    ui->setupUi(this);
    setWindowTitle("Consulter un medecin");
    setWindowIcon(QIcon(":/img/img/logo.png"));
    basicSetup();
    Patient pat=getPatientById(idPat);
    ui->listePat->setCurrentText("PAT-"+QString::number(pat.id)+" "+QString(pat.nom)+" "+QString(pat.prenom));
    ui->listePat->setEnabled(false);

    if(critere==nullptr) return;
    setInUpCritere(critere);

}

AddTraitDialog::AddTraitDialog(int idMed, int type, EditCritereDialog *critere, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTraitDialog)
{
    ui->setupUi(this);
    setWindowTitle("Traiter un patient");
    setWindowIcon(QIcon(":/img/img/logo.png"));
    basicSetup();
    ui->critereLbl->show();
    Medecin med = getMedecinById(idMed);
    for (int i=0;i<ui->listeMed->count();i++) {
        if(ui->listeMed->itemText(i).contains("MED-"+QString::number(idMed)+" ")){
            ui->listeMed->setCurrentIndex(i);
            break;
        }
    }
//    ui->listeMed->setCurrentText("MED-"+QString::number(med.id)+" "+QString(med.nom)+" "+QString(med.prenom));
    ui->listeMed->setEnabled(false);
    ui->tauxLbl->setText(QString::number(med.taux)+" Ar");
    ui->coupTotal->setText(QString::number(med.taux)+" Ar");


    if(critere==nullptr) return;
    setInUpCritere(critere);
}
AddTraitDialog::AddTraitDialog(Traitement trait, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTraitDialog)
{
    ui->setupUi(this);
    setWindowTitle("Modification d'un traitement");
    setWindowIcon(QIcon(":/img/img/logo.png"));
    QDate date(trait.dateConsu.year, trait.dateConsu.mounth, trait.dateConsu.day);

    basicSetup();
    editing=true;
    for (int i=0;i<ui->listePat->count();i++) {
        if(ui->listePat->itemText(i).contains("PAT-"+QString::number(trait.idPat)+" ")){
            ui->listePat->setCurrentIndex(i);
            break;
        }
    }

    for (int i=0;i<ui->listeMed->count();i++) {
        if(ui->listeMed->itemText(i).contains("MED-"+QString::number(trait.idMed)+" ")){
            ui->listeMed->setCurrentIndex(i);
            break;
        }
    }
    ui->nbJours->setValue(trait.nbJours);
    ui->tauxLbl->setText(QString::number(trait.taux)+" Ar");
    ui->description->setPlainText(QString(trait.description).replace(";", "\n"));
    ui->coupTotal->setText(QString::number(trait.taux*trait.nbJours));
    ui->addMedBtn->setText("Enregistrer");
    ui->numTrait->setText("TRAIT-"+QString::number(trait.id));
    ui->dateConsu->setMinimumDate(QDate(2000, 1, 1));
    ui->dateConsu->setDate(QDate(trait.dateConsu.year, trait.dateConsu.mounth, trait.dateConsu.day));
    ui->newTraitLbl->hide();
    ui->editTraitLbl->show();

}

AddTraitDialog::~AddTraitDialog()
{
    delete ui;
}
void AddTraitDialog::setInUpCritere(EditCritereDialog *critere)
{
    ui->critereLbl->show();
    if(critere->getChoice()==MOUNTH){
        QDate m(critere->getYearMounth(), critere->getMounthIndex(), 1);
        ui->dateConsu->setMinimumDate(m);
        ui->dateConsu->setMaximumDate(m.addDays(29));
        ui->critereLbl->setText("(durant le mois de <b>"+critere->getMounth()+" "+QString::number(critere->getYearMounth())+"</b>)");
        setWindowTitle("Ajout de patient à traiter pour le mois de "+critere->getMounth()+" "+QString::number(critere->getYearMounth()));
    }
    else if (critere->getChoice()==YEAR) {
        ui->dateConsu->setMinimumDate(QDate(critere->getYear(),  1, 1));
        ui->dateConsu->setMaximumDate(QDate(critere->getYear(), 12, 31));
        ui->critereLbl->setText("(Durant l'année <b>"+QString::number(critere->getYear())+"</b>(");
        setWindowTitle("Ajout de patient à traiter pour l'année "+QString::number(critere->getYear()));
    }
    else if (critere->getChoice()==TWO_DATES) {
        ui->dateConsu->setMinimumDate(critere->getFirstDate());
        ui->dateConsu->setMaximumDate(critere->getSecondDate());
        ui->critereLbl->setText("(Entre le <b>"+critere->getFirstDate().toString("d MMM yyyy")+" et "+critere->getSecondDate().toString("d MMM yyyy")+"</b>)");
        setWindowTitle("Ajout de patient à traiter: entre "+critere->getFirstDate().toString("d MMM yyyy")+" et "+critere->getSecondDate().toString("d MMM yyyy"));
    }
    else {
        ui->critereLbl->setText("");
    }
}
void AddTraitDialog::basicSetup()
{
    ui->editTraitLbl->hide();
    ui->critereLbl->hide();
    int nbMed=0;
    Medecin *allMed=getAllMedecin(&nbMed);
    for (int i=0;i<nbMed;i++) {
        ui->listeMed->addItem("MED-"+QString::number(allMed[i].id)+" Dr "+QString(allMed[i].prenom)+" ("+QString(allMed[i].specialite)+")");
    }

    int nbPat=0;
    Patient *allPat=getAllPatient(&nbPat);
    for (int i=nbPat-1;i>=0;i--) {
        ui->listePat->addItem("PAT-"+QString::number(allPat[i].id)+" "+QString(allPat[i].nom)+" "+QString(allPat[i].prenom));
    }

    Medecin med=getMedecinById(ui->listeMed->currentText().remove("MED-").section(" ", 0, 0).toInt());
    ui->numTrait->setText("TRAIT-"+QString::number(getLastTraitementId()+1));
    ui->dateConsu->setDate(QDate::currentDate());
    ui->dateConsu->setMinimumDate(QDate::currentDate());
    ui->dateConsu->setCalendarPopup(true);
    ui->nbJours->setMinimum(1);
    ui->tauxLbl->setText(QString::number(med.taux)+" Ar");
    ui->coupTotal->setText(QString::number(med.taux)+" Ar");
}

void AddTraitDialog::on_addMedBtn_clicked()
{
    Traitement trait;
    trait.idPat=ui->listePat->currentText().remove("PAT-").section(" ", 0, 0).toInt();
    trait.idMed=ui->listeMed->currentText().remove("MED-").section(" ", 0, 0).toInt();
    trait.nbJours=ui->nbJours->value();
    QDate dateConsu=ui->dateConsu->date();
    trait.dateConsu.day=dateConsu.day();
    trait.dateConsu.mounth=dateConsu.month();
    trait.dateConsu.year=dateConsu.year();
    QString descri=ui->description->toPlainText().replace("\n", "; ");
    if(descri=="") descri="Aucune description";
    strcpy(trait.description, descri.toLatin1());
    trait.taux=ui->tauxLbl->text().remove(" Ar").toInt();
    if(editing){
        updateTraitementById(ui->numTrait->text().remove("TRAIT-").toInt(), trait);
        qDebug()<<"trait-ID: "<<ui->numTrait->text().remove("TRAIT-").toInt();
    }
    else addTraitement(trait);
    saved=true;
    this->close();

}
bool AddTraitDialog::isSaved(){return saved;}

void AddTraitDialog::on_listeMed_activated(const QString &arg1)
{
    QString m=arg1;
    Medecin med=getMedecinById(m.remove("MED-").section(" ", 0, 0).toInt());
    ui->tauxLbl->setText(QString::number(med.taux)+" Ar");
    ui->coupTotal->setText(QString::number(med.taux*ui->nbJours->value())+" Ar");
}

void AddTraitDialog::on_nbJours_valueChanged(int arg1)
{
    ui->coupTotal->setText(QString::number(ui->tauxLbl->text().remove(" Ar").toInt()*arg1)+" Ar");
}

void AddTraitDialog::on_cancelBtn_clicked()
{
    this->close();
}
