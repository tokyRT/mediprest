#include "voirpatdumed.h"
#include "ui_voirpatdumed.h"
#include "addtraitdialog.h"
#include <QDebug>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QDir>

VoirPatDuMed::VoirPatDuMed(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoirPatDuMed)
{
    ui->setupUi(this);
}
VoirPatDuMed::VoirPatDuMed(int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoirPatDuMed)
{
    ui->setupUi(this);
    idMed=id;
    Medecin med=getMedecinById(idMed);
    setWindowTitle("Liste des patients traités par le Dr "+QString(med.prenom)+" "+QString(med.nom));
    ui->numMed->setText("MED-"+QString::number(idMed));
    ui->nomMed->setText("Dr "+QString(med.prenom)+" "+QString(med.nom));
    ui->critereLbl->setText("[Tous les patients]");
    ui->medTable->setColumnWidth(1, 200);
    ui->medTable->setColumnWidth(4, 130);
    displayTable();
    dialog = new EditCritereDialog(this);
}

VoirPatDuMed::~VoirPatDuMed()
{
    delete ui;
}
void VoirPatDuMed::displayTable()
{
    Patient pat;
    int nbTrait;
    Traitement *trait=getMedTraitement(idMed, &nbTrait);
    QDate dateConsu;
    ui->medTable->setRowCount(0);
    int totalJours=0, coupTotal=0;
    for (int i=0;i<nbTrait;i++) {
        ui->medTable->insertRow(i);
        pat=getPatientById(trait[i].idPat);
        dateConsu.setDate(trait[i].dateConsu.year, trait[i].dateConsu.mounth, trait[i].dateConsu.day);
        ui->medTable->setItem(i, 0, new QTableWidgetItem("PAT-"+QString::number(trait[i].idPat)));
        ui->medTable->setItem(i, 1, new QTableWidgetItem(QString(pat.nom)+" "+QString(pat.prenom)));
        ui->medTable->setItem(i, 2, new QTableWidgetItem(QString::number(trait[i].taux)+" Ar"));
        ui->medTable->setItem(i, 3, new QTableWidgetItem(QString::number(trait[i].nbJours)+" jours"));
        ui->medTable->setItem(i, 4, new QTableWidgetItem(dateConsu.toString("d MMM yyyy")));
        totalJours+=trait[i].nbJours;
        coupTotal+=trait[i].nbJours*trait[i].taux;
    }
    ui->effectif->setNum(ui->medTable->rowCount());
    ui->totalJours->setText(QString::number(totalJours)+" jours");
    ui->coupTotal->setText(QString::number(coupTotal)+" Ar");
    selectRow(ui->medTable->rowCount()-1);
    showPatDetails(getPatientById(trait[nbTrait-1].idPat));
}
void VoirPatDuMed::displayTable(QString mounth, int mounthIndex, int yearMounth)
{
    int nb;
    Traitement *trait=getMedTraitement(idMed, &nb);
    Patient pat;
    ui->medTable->setRowCount(0);
    QDate dateConsu;
    int row=0;
    int lastPatId=1;
    int totalJours=0, coupTotal=0;
    for (int i=0;i<nb;i++) {
        if(trait[i].dateConsu.mounth==mounthIndex && trait[i].dateConsu.year==yearMounth){
            qDebug()<<mounthIndex;
            ui->medTable->insertRow(row);
            pat=getPatientById(trait[i].idPat);
            dateConsu.setDate(trait[i].dateConsu.year, trait[i].dateConsu.mounth, trait[i].dateConsu.day);
            ui->medTable->setItem(row, 0, new QTableWidgetItem("PAT-"+QString::number(pat.id)));
            ui->medTable->setItem(row, 1, new QTableWidgetItem(QString(pat.nom)+" "+QString(pat.prenom)));
            ui->medTable->setItem(row, 2, new QTableWidgetItem(QString::number(trait[i].taux)+" Ar"));
            ui->medTable->setItem(row, 3, new QTableWidgetItem(QString::number(trait[i].nbJours)+" jours"));
            ui->medTable->setItem(row, 4, new QTableWidgetItem(dateConsu.toString("d MMM yyyy")));
            row++;
            lastPatId=pat.id;
            totalJours+=trait[i].nbJours;
            coupTotal=coupTotal+(trait[i].nbJours*trait[i].taux);
        }
    }
    ui->effectif->setText(QString::number(ui->medTable->rowCount()));
    ui->totalJours->setText(QString::number(totalJours)+" Jours");
    ui->coupTotal->setText(QString::number(coupTotal)+" Ar");
    selectRow(ui->medTable->rowCount()-1);
    showPatDetails(getPatientById(lastPatId));
}
void VoirPatDuMed::displayTable(int year)
{
    int nb;
    Traitement *trait=getMedTraitement(idMed, &nb);
    Patient pat;
    ui->medTable->setRowCount(0);
    QDate dateConsu;
    int row=0;
    int lastPatId=1;
    int totalJours=0, coupTotal=0;
    for (int i=0;i<nb;i++) {
        if(trait[i].dateConsu.year==year){
            ui->medTable->insertRow(row);
            pat=getPatientById(trait[i].idPat);
            dateConsu.setDate(trait[i].dateConsu.year, trait[i].dateConsu.mounth, trait[i].dateConsu.day);
            ui->medTable->setItem(row, 0, new QTableWidgetItem("PAT-"+QString::number(pat.id)));
            ui->medTable->setItem(row, 1, new QTableWidgetItem(QString(pat.nom)+" "+QString(pat.prenom)));
            ui->medTable->setItem(row, 2, new QTableWidgetItem(QString::number(trait[i].taux)+" Ar"));
            ui->medTable->setItem(row, 3, new QTableWidgetItem(QString::number(trait[i].nbJours)+" jours"));
            ui->medTable->setItem(row, 4, new QTableWidgetItem(dateConsu.toString("d MMM yyyy")));
            row++;
            lastPatId=pat.id;
            totalJours+=trait[i].nbJours;
            coupTotal=coupTotal+(trait[i].nbJours*trait[i].taux);
        }
    }
    ui->effectif->setText(QString::number(ui->medTable->rowCount()));
    ui->totalJours->setText(QString::number(totalJours)+" Jours");
    ui->coupTotal->setText(QString::number(coupTotal)+" Ar");
    selectRow(ui->medTable->rowCount()-1);
    showPatDetails(getPatientById(lastPatId));
}
void VoirPatDuMed::displayTable(QDate firstDate, QDate secondDate)
{
    int nb;
    Traitement *trait=getMedTraitement(idMed, &nb);
    Patient pat;
    ui->medTable->setRowCount(0);
    QDate dateConsu;
    int row=0;
    int lastPatId=1;
    int totalJours=0, coupTotal=0;
    for (int i=0;i<nb;i++) {
        dateConsu.setDate(trait[i].dateConsu.year, trait[i].dateConsu.mounth, trait[i].dateConsu.day);
        if(dateConsu>=firstDate && dateConsu<=secondDate){
            ui->medTable->insertRow(row);
            pat=getPatientById(trait[i].idPat);
            ui->medTable->setItem(row, 0, new QTableWidgetItem("PAT-"+QString::number(pat.id)));
            ui->medTable->setItem(row, 1, new QTableWidgetItem(QString(pat.nom)+" "+QString(pat.prenom)));
            ui->medTable->setItem(row, 2, new QTableWidgetItem(QString::number(trait[i].taux)+" Ar"));
            ui->medTable->setItem(row, 3, new QTableWidgetItem(QString::number(trait[i].nbJours)+" jours"));
            ui->medTable->setItem(row, 4, new QTableWidgetItem(dateConsu.toString("d MMM yyyy")));
        }
        row++;
        lastPatId=pat.id;
        totalJours+=trait[i].nbJours;
        coupTotal=coupTotal+(trait[i].nbJours*trait[i].taux);
    }
    ui->effectif->setText(QString::number(ui->medTable->rowCount()));
    ui->totalJours->setText(QString::number(totalJours)+" Jours");
    ui->coupTotal->setText(QString::number(coupTotal)+" Ar");
    selectRow(ui->medTable->rowCount()-1);
    showPatDetails(getPatientById(lastPatId));
}
void VoirPatDuMed::selectRow(int row)
{
    QTableWidgetSelectionRange rangeMed(row, 0, row, 4);
    ui->medTable->setRangeSelected(rangeMed, true);
}
void VoirPatDuMed::showPatDetails(Patient pat)
{
    ui->numPatLbl->setText("PAT-"+QString::number(pat.id));
    ui->nomPatLbl->setText(QString(pat.nom));
    ui->prenomPatLbl->setText(QString(pat.prenom));
    ui->adressePatLbl->setText(QString(pat.adresse));
    ui->sexePatLbl->setText(QString(pat.sexe));
    int nb;
    Traitement *trait=getPatTraitement(pat.id, &nb);
    QDate last(trait[nb-1].dateConsu.year, trait[nb-1].dateConsu.mounth, trait[nb-1].dateConsu.day);
    ui->lastConsu->setText(last.toString("d MMM yyyy"));
}
void VoirPatDuMed::on_medTable_cellPressed(int row, int column)
{
    selectRow(row);
    int idPat = ui->medTable->item(row, 0)->text().remove("PAT-").toInt();
    showPatDetails(getPatientById(idPat));
}

void VoirPatDuMed::on_medTable_cellClicked(int row, int column)
{
    selectRow(row);
    int idPat = ui->medTable->item(row, 0)->text().remove("PAT-").toInt();
    showPatDetails(getPatientById(idPat));
}

void VoirPatDuMed::on_critereBtn_clicked()
{
//    EditCritereDialog dialog(this);
    dialog->exec();
    if(!dialog->isEdited()) return;
    switch (dialog->getChoice()) {
    case ALL:
        displayTable();
        ui->critereLbl->setText("[Tous les patients]");
        break;
    case MOUNTH:
        displayTable(dialog->getMounth(), dialog->getMounthIndex(), dialog->getYearMounth());
        ui->critereLbl->setText("Durant le mois de <b>"+dialog->getMounth()+" "+QString::number(dialog->getYearMounth())+"</b>");
        break;
    case YEAR:
        displayTable(dialog->getYear());
        ui->critereLbl->setText("Durant l'année <b>"+QString::number(dialog->getYear())+"</b>");
        break;
    default:
        displayTable(dialog->getFirstDate(), dialog->getSecondDate());
        ui->critereLbl->setText("Entre <b>"+dialog->getFirstDate().toString("d MMM yyyy")+" et "+dialog->getSecondDate().toString("d MMM yyyy")+"</b>");
        break;
    }
}

void VoirPatDuMed::on_traitPat_clicked()
{
  AddTraitDialog dialogAdd(idMed, MEDECIN, dialog, this);
  dialogAdd.exec();
  displayTable();
}

void VoirPatDuMed::on_printMedtable_clicked()
{
    QString pdfName=QFileDialog::getSaveFileName(this, "Enregistrer le pdf sous...", "/home/patients traités.pdf", "Pdf (*.pdf)");
    if(pdfName=="") return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::NativeFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setOutputFileName(pdfName);
    QString title, effectif, headerColor;
    headerColor="#0d6f7a";
    title="Liste des patients traités";
    QTableWidget *table=ui->medTable;

    Medecin med=getMedecinById(idMed);

    QTextDocument doc;
    QString trWidthLg="width: 280px;";
    QString text("<html><head/><body><p><span style=\" font-size:11pt; font-weight:600; color:#0d6f7a; border: 1px solid red;\">"+title+"</span></p></body></html>");
    text.append("<html><head/><body><p><span style=\" font-size:9pt; font-weight:600; color:#0d6f7a;\">Medecin: </span><span style=\" font-size:9pt; font-weight:600; color:#03e185;\">MED-"+QString::number(med.id)+" "+QString(med.nom)+" "+QString(med.prenom)+" </span></p></body></html>");
    text.append("<span>Période: <b>"+ui->critereLbl->text()+"</b></span><br>");
    text.append("<span>Patients traités: <b>"+ui->effectif->text()+"</b></span><br>");
    text.append("<span>Nombre total de jours: <b>"+ui->totalJours->text()+"</b></span><br>");
    text.append("<span>Taux journalier: <b>"+ui->taux->text()+"</b></span><br>");
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

void VoirPatDuMed::on_cancelBtn_clicked()
{
    this->close();
}
