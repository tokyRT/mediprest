#include "voirtrait.h"
#include "ui_voirtrait.h"
#include "medecin.h"
#include "traitement.h"
#include "patient.h"
#include "editcriteredialog.h"
#include "addtraitdialog.h"
#include <QPrinter>
#include <QTextDocument>
#include <QMessageBox>
#include <QDir>
#include <QDate>
#include <QFileDialog>


VoirTrait::VoirTrait(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoirTrait)
{
    ui->setupUi(this);
}
VoirTrait::VoirTrait(int patId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoirTrait)
{
    ui->setupUi(this);
    idPat=patId;
    Patient pat=getPatientById(idPat);
    ui->numPat->setText("PAT-"+QString::number(idPat));
    ui->nomPat->setText(QString(pat.nom)+" "+QString(pat.prenom));
    displayTraitTable();

    setWindowTitle("Liste des medecins consultés par le patient PAT-"+QString::number(idPat)+" "+QString(pat.nom)+" "+QString(pat.prenom));

    pdpLbl->setFixedSize(200, 200);
    QVBoxLayout *det=new QVBoxLayout(this);
    det->addWidget(pdpLbl);
    det->addStretch();
    det->addLayout(ui->infoMedLayout);
    ui->medDetailsGroup->setLayout(det);


    ui->medTable->setAlternatingRowColors(true);
    ui->medTable->setColumnWidth(1, 80);
    ui->medTable->setColumnWidth(0, 260);
    ui->medTable->setColumnWidth(3, 120);
    ui->medTable->setColumnWidth(4, 130);
    ui->critereLbl->setText("[Tous les medecins]");

    dialog = new EditCritereDialog(this);
}


VoirTrait::~VoirTrait()
{
    delete ui;
}
void VoirTrait::display(Traitement *trait, int nb)
{
    Medecin med;
    ui->medTable->setRowCount(0);
    for (int i=0;i<nb;i++) {
        ui->medTable->insertRow(i);
        med=getMedecinById(trait[i].idMed);
        ui->medTable->setItem(i, 1, new QTableWidgetItem("MED-"+QString::number(med.id)));
        ui->medTable->setItem(i, 0, new QTableWidgetItem("Dr "+QString(med.nom)+" "+QString(med.prenom)));
        ui->medTable->setItem(i, 2, new QTableWidgetItem(QString::number(med.taux)+" Ar"));
        ui->medTable->setItem(i, 3, new QTableWidgetItem(QString::number(trait[i].nbJours)));
//        ui->medTable->setItem(i, 4, new QTableWidgetItem(QString(trait[i].dateConsultation)));
    }
    selectMedRow(ui->medTable->rowCount()-1);
}
void VoirTrait::displayTraitTable()
{
    ui->critereLbl->setText("[Tous les medecins]");
    int nbTrait;
    Traitement *trait=getPatTraitement(idPat, &nbTrait);
    Medecin med;
    QDate dateConsu;
    ui->medTable->setRowCount(0);
    int totalJours=0, coupTotal=0;
    for (int i=0;i<nbTrait;i++) {
        ui->medTable->insertRow(i);
        med=getMedecinById(trait[i].idMed);
        dateConsu.setDate(trait[i].dateConsu.year, trait[i].dateConsu.mounth, trait[i].dateConsu.day);
        ui->medTable->setItem(i, 1, new QTableWidgetItem("MED-"+QString::number(med.id)));
        ui->medTable->setItem(i, 0, new QTableWidgetItem("Dr "+QString(med.nom)+" "+QString(med.prenom)));
        ui->medTable->setItem(i, 2, new QTableWidgetItem(QString::number(trait[i].taux)+" Ar"));
        ui->medTable->setItem(i, 3, new QTableWidgetItem(QString::number(trait[i].nbJours)+" jours"));
        ui->medTable->setItem(i, 4, new QTableWidgetItem(dateConsu.toString("d MMM yyyy")));
        totalJours+=trait[i].nbJours;
        coupTotal=coupTotal+(trait[i].nbJours*trait[i].taux);
    }
    ui->effectif->setText(QString::number(ui->medTable->rowCount()));
    ui->totalJours->setText(QString::number(totalJours));
    ui->coupTotal->setText(QString::number(coupTotal)+" Ar");
    selectMedRow(ui->medTable->rowCount()-1);
    showMedDetails(getMedecinById(trait[nbTrait-1].idMed));

}
void VoirTrait::displayTraitTable(QString mounth, int mounthIndex, int yearMounth)
{
    ui->critereLbl->setText("Durant le mois de <b>"+mounth+" "+QString::number(yearMounth)+"</b>");
    int nb;
    Traitement *trait=getPatTraitement(idPat, &nb);
    Medecin med;
    ui->medTable->setRowCount(0);
    QDate mois(2000, mounthIndex, 1);
    QDate dateConsu;
//    qDebug()<<mois.toString();
    int row=0;
    int lastMedId=1;
    int totalJours=0, coupTotal=0;
    for (int i=0;i<nb;i++) {
        if(trait[i].dateConsu.mounth==mounthIndex && trait[i].dateConsu.year==yearMounth)
        {
            qDebug()<<"Index="<<mounthIndex;
            ui->medTable->insertRow(row);
            med=getMedecinById(trait[i].idMed);
            dateConsu.setDate(trait[i].dateConsu.year, trait[i].dateConsu.mounth, trait[i].dateConsu.day);
            ui->medTable->setItem(row, 1, new QTableWidgetItem("MED-"+QString::number(med.id)));
            ui->medTable->setItem(row, 0, new QTableWidgetItem("Dr "+QString(med.nom)+" "+QString(med.prenom)));
            ui->medTable->setItem(row, 2, new QTableWidgetItem(QString::number(trait[i].taux)+" Ar"));
            ui->medTable->setItem(row, 3, new QTableWidgetItem(QString::number(trait[i].nbJours)+" jours"));
            ui->medTable->setItem(row, 4, new QTableWidgetItem(dateConsu.toString("d MMM yyyy")));
            row++;
            lastMedId=med.id;
            totalJours+=trait[i].nbJours;
            coupTotal=coupTotal+(trait[i].nbJours*trait[i].taux);
        }
    }
    ui->effectif->setText(QString::number(ui->medTable->rowCount()));
    ui->totalJours->setText(QString::number(totalJours));
    ui->coupTotal->setText(QString::number(coupTotal)+" Ar");
    selectMedRow(ui->medTable->rowCount()-1);
    showMedDetails(getMedecinById(lastMedId));
}
void VoirTrait::displayTraitTable(int year)
{
    ui->critereLbl->setText("Durant l'année <b>"+QString::number(year)+"</b>");
    int nb;
    Traitement *trait=getPatTraitement(idPat, &nb);
    Medecin med;
    ui->medTable->setRowCount(0);
    QDate dateConsu;
    int row=0;
    int lastMedId=0;
    int totalJours=0, coupTotal=0;
    for (int i=0;i<nb;i++) {
        if(trait[i].dateConsu.year==year)
        {
            ui->medTable->insertRow(row);
            med=getMedecinById(trait[i].idMed);
            dateConsu.setDate(trait[i].dateConsu.year, trait[i].dateConsu.mounth, trait[i].dateConsu.day);
            ui->medTable->setItem(row, 1, new QTableWidgetItem("MED-"+QString::number(med.id)));
            ui->medTable->setItem(row, 0, new QTableWidgetItem("Dr "+QString(med.nom)+" "+QString(med.prenom)));
            ui->medTable->setItem(row, 2, new QTableWidgetItem(QString::number(trait[i].taux)+" Ar"));
            ui->medTable->setItem(row, 3, new QTableWidgetItem(QString::number(trait[i].nbJours)+" jours"));
            ui->medTable->setItem(row, 4, new QTableWidgetItem(dateConsu.toString("d MMM yyyy")));
            row++;
            lastMedId=med.id;
            totalJours+=trait[i].nbJours;
            coupTotal=coupTotal+(trait[i].nbJours*trait[i].taux);
        }
    }
    ui->effectif->setText(QString::number(ui->medTable->rowCount()));
    ui->totalJours->setText(QString::number(totalJours));
    ui->coupTotal->setText(QString::number(coupTotal)+" Ar");
    selectMedRow(ui->medTable->rowCount()-1);
    showMedDetails(getMedecinById(lastMedId));
}
void VoirTrait::displayTraitTable(QDate firstDate, QDate secondDate)
{
    ui->critereLbl->setText("Entre <b>"+firstDate.toString("d MMM yyyy")+" et "+secondDate.toString("d MMM yyyy")+"</b>");
    int nb;
    Traitement *trait=getPatTraitement(idPat, &nb);
    Medecin med;
    ui->medTable->setRowCount(0);
    QDate dateConsu;
    int row=0;
    int lastMedId = 0;
    int totalJours=0, coupTotal=0;
    for (int i=0;i<nb;i++) {
        dateConsu.setDate(trait[i].dateConsu.year, trait[i].dateConsu.mounth, trait[i].dateConsu.day);
        if(dateConsu>=firstDate && dateConsu<=secondDate)
        {
            ui->medTable->insertRow(row);
            med=getMedecinById(trait[i].idMed);
            ui->medTable->setItem(row, 1, new QTableWidgetItem("MED-"+QString::number(med.id)));
            ui->medTable->setItem(row, 0, new QTableWidgetItem("Dr "+QString(med.nom)+" "+QString(med.prenom)));
            ui->medTable->setItem(row, 2, new QTableWidgetItem(QString::number(trait[i].taux)+" Ar"));
            ui->medTable->setItem(row, 3, new QTableWidgetItem(QString::number(trait[i].nbJours)+" jours"));
            ui->medTable->setItem(row, 4, new QTableWidgetItem(dateConsu.toString("d MMM yyyy")));
            row++;
            lastMedId=med.id;
            totalJours+=trait[i].nbJours;
            coupTotal=coupTotal+(trait[i].nbJours*trait[i].taux);
        }
    }
    ui->effectif->setText(QString::number(ui->medTable->rowCount()));
    ui->totalJours->setText(QString::number(totalJours));
    ui->coupTotal->setText(QString::number(coupTotal)+" Ar");
    selectMedRow(ui->medTable->rowCount()-1);
    showMedDetails(getMedecinById(lastMedId));
}

void VoirTrait::on_cancelBtn_clicked()
{
    this->close();
}

void VoirTrait::on_medTable_cellClicked(int row, int column)
{
    QTableWidgetSelectionRange rangeMed(row, 0, row, 4);
    ui->medTable->setRangeSelected(rangeMed, true);
    int idMed = ui->medTable->item(row, 1)->text().remove("MED-").toInt();
    showMedDetails(getMedecinById(idMed));
}
void VoirTrait::showMedDetails(Medecin med)
{
    ui->pseudoLbl->setText("<strong>Dr "+QString(med.prenom)+"</strong>");
    ui->nomLbl->setText(QString(med.nom));
    ui->prenomLbl->setText(QString(med.prenom));
    ui->matriculeLbl->setText("MED-"+QString::number(med.id));
    ui->specLbl->setText(QString(med.specialite));
    ui->tauxLbl->setText(QString::number(med.taux)+" Ar");
    QString pdpPath;
    if(QString(med.photo)=="Aucune photo choisie"){
        if(QString(med.sexe)=="Feminin") pdpPath=":/img/img/Feminin.ico";
        else if (QString(med.sexe)=="Masculin") pdpPath=":/img/img/Masculin.png";
    }
    else pdpPath=QDir::currentPath()+"/"+QString(med.photo);
    QPixmap pdp(pdpPath);


    pdpLbl->setPixmap(pdp.scaled(200, 200, Qt::KeepAspectRatioByExpanding));


}

void VoirTrait::on_critereBtn_clicked()
{
//    EditCritereDialog dialog(this);
    dialog->exec();
    if(!dialog->isEdited()) return;
    switch (dialog->getChoice()) {
    case ALL:
        displayTraitTable();
        break;
    case MOUNTH:
        displayTraitTable(dialog->getMounth(), dialog->getMounthIndex(), dialog->getYearMounth());
        break;
    case YEAR:
        displayTraitTable(dialog->getYear());
        break;
    default:
        displayTraitTable(dialog->getFirstDate(), dialog->getSecondDate());
        break;
    }
}
void VoirTrait::allMed()
{
    qDebug()<<"all med";
}
void VoirTrait::selectMedRow(int row)
{
    QTableWidgetSelectionRange rangeMed(row, 0, row, 4);
    ui->medTable->setRangeSelected(rangeMed, true);
}

void VoirTrait::on_consuMedBtn_clicked()
{
    AddTraitDialog dialogAdd(idPat, dialog, this);
    dialogAdd.exec();
    displayTraitTable();
}

void VoirTrait::on_printMedtable_clicked()
{
    QString pdfName=QFileDialog::getSaveFileName(this, "Enregistrer le pdf sous...", "/home/medecins consultés.pdf", "Pdf (*.pdf)");
    if(pdfName=="") return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::NativeFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setOutputFileName(pdfName);
    QString title, effectif, headerColor;
    headerColor="#0d6f7a";
    title="Liste des medecins consultés";
    QTableWidget *table=ui->medTable;

    Patient pat=getPatientById(idPat);

    QTextDocument doc;
    QString trWidthLg="width: 280px;";
    QString text("<html><head/><body><p><span style=\" font-size:11pt; font-weight:600; color:#0d6f7a; border: 1px solid red;\">"+title+"</span></p></body></html>");
    text.append("<html><head/><body><p><span style=\" font-size:9pt; font-weight:600; color:#0d6f7a;\">Patient: </span><span style=\" font-size:9pt; font-weight:600; color:#03e185;\">PAT-"+QString::number(pat.id)+" "+QString(pat.nom)+" "+QString(pat.prenom)+" </span></p></body></html>");
    text.append("<span>Période: <b>"+ui->critereLbl->text()+"</b></span><br>");
    text.append("<span>Patients traités: <b>"+ui->effectif->text()+"</b></span><br>");
    text.append("<span>Nombre total de jours: <b>"+ui->totalJours->text()+"</b></span><br>");
    text.append("<span>Coup  total: <b>"+ui->coupTotal->text()+"</b></span><br>");


    text.append("<table style:\"color: lightGrey;\"><thead>");
    text.append("<tr style=\"border:1px solid black; background-color: "+headerColor+";color: white; height: 40px;\">");
    for (int i=0;i<table->columnCount();i++) {
        text.append("<th  style=\"border: 1px solid black;\">&nbsp;&nbsp;").append(table->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString()).append("&nbsp;&nbsp;</th>");
    }
    text.append("</tr>");
    text.append("</thead>");
    text.append("<tbody style=\"color: rgb(99, 99, 99);\">");
    for (int i=0;i<table->rowCount();i++) {
        if(i%2==1) text.append("<tr style=\"background-color: rgb(196, 230, 223);\">");
        else text.append("<tr>");
        for (int j=0;j<table->columnCount();j++) {
             text.append("<td>").append(table->item(i, j)->text()).append("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        }
        text.append("</tr>");
    }
    text.append("</tbody></table>");
    QString dateTime=QDateTime::currentDateTime().toString();
    text.append("</br></br></br></br></br><hr><em style=\"font-size: 0.5em;\">Pdf généré le "+dateTime+"</em>");
    doc.setHtml(text);
    doc.setPageSize(printer.pageRect().size());
    doc.setDocumentMargin(50);
    doc.print(&printer);
    QMessageBox::information(this, this->windowTitle(), "Le pdf a bien ete enregistre sous: "+pdfName);
}
