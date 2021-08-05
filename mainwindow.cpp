#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "medecin.h"
#include "patient.h"
#include "addmeddialog.h"
#include "updatemeddialog.h"
#include "addpatdialog.h"
#include "updatepatdialog.h"
#include "voirtrait.h"
#include "voirpatdumed.h"
#include "prestdialog.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QCompleter>
#include <QRegExp>
#include <QDate>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Medically");
    setWindowIcon(QIcon(":/img/img/logo.png"));

    setUpNavBtn();
    allAboutMed();
    allAboutPat();
    allAboutTrait();
    allAboutPrest();
    connect(ui->actionSe_deconnecter_et_quitter, SIGNAL(triggered()), qApp, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::allAboutPrest()
{
    yearList();
    listeMedPrest();
    ui->prestTable->setColumnHidden(3, true);
    ui->prestTable->setColumnHidden(4, true);
    displayAllPrest(ui->yearPrest->currentText().toInt());
    drawAllPrestHisto(ui->yearPrest->currentText().toInt());
    drawPiePrest(ui->yearPrest->currentText().toInt());
}
void MainWindow::displayAllPrest(int year)
{
    ui->prestTable->setAlternatingRowColors(true);
    ui->prestTable->setColumnWidth(1, 200);
    int nbMed;
    Medecin *med=getAllMedecin(&nbMed);
    int nbTrait;
    Traitement *trait;
    int prestTotal=0;
    ui->prestTable->setRowCount(0);
    for (int i=0;i<nbMed;i++) {
        ui->prestTable->insertRow(i);
        trait=getMedTraitement(med[i].id, &nbTrait);

        ui->prestTable->setItem(i, 0, new QTableWidgetItem("MED-"+QString::number(med[i].id)));
        ui->prestTable->setItem(i, 1, new QTableWidgetItem(QString(med[i].nom)+" "+QString(med[i].prenom)));
        ui->prestTable->setItem(i, 2, new QTableWidgetItem(QString::number(getPrest(med[i].id, year))+" Ar"));
        prestTotal+=getPrest(med[i].id, year);

        QTableWidgetItem *id=new QTableWidgetItem;
        id->setData(Qt::EditRole, med[i].id);
        ui->prestTable->setItem(i, 3, id);
        QTableWidgetItem *prest=new QTableWidgetItem;
        prest->setData(Qt::EditRole, getPrest(med[i].id, year));
        ui->prestTable->setItem(i, 4, prest);


    }
    ui->totalPrest->setText(QString::number(prestTotal)+" Ar");

}
void MainWindow::drawAllPrestHisto(int year)
{
    ui->yearPrest->setCurrentIndex(2);
    int nbMed;
    Medecin *med=getAllMedecin(&nbMed);
    series = new QBarSeries();
    QBarSet *set;
    QVector<QBarSet*> allSet;
    for (int i=0;i<nbMed;i++) {
        set = new QBarSet("MED-"+QString::number(med[i].id));
        allSet.append(set);
        allSet[i]->append(getPrest(med[i].id, year));
//        set->replace(0, getPrest(med[i].id, ui->yearPrest->currentText().toInt()));
        series->append(allSet[i]);
    }
    QChart *histo = new QChart();
    histo->addSeries(series);
    histo->setTitle("Etat des prestations par medecin durant l'année "+QString::number(year));
    histo->setAnimationOptions(QChart::SeriesAnimations);
    QStringList medList;
    for(int i=0; i<nbMed; i++){
//        medList.append("MED-"+QString::number(med[i].id));
    }
    medList.append("Medecins");
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(medList);
    histo->createDefaultAxes();
    histo->setAxisX(axis, series);
    chartView=new QChartView(histo);
//    chartView->setChart(histo);
    chartView->repaint();
    chartView->setParent(ui->allPrestHisto);
    chartView->setMaximumSize(700, 400);
}
void MainWindow::updateHisto(int newYear)
{
    chartView->repaint();
    int nbMed;
    Medecin *med=getAllMedecin(&nbMed);
    series=new QBarSeries();
    QBarSet *set;
    QVector<QBarSet*> allSet;
    for (int i=0;i<nbMed;i++) {
        set = new QBarSet("MED-"+QString::number(med[i].id));
        allSet.append(set);
        allSet[i]->append(getPrest(med[i].id, newYear));
//        set->replace(0, getPrest(med[i].id, ui->yearPrest->currentText().toInt()));
        series->append(allSet[i]);

    }

    QChart *histo = new QChart();
    histo->addSeries(series);
    histo->setTitle("Etat des prestations par medecin durant l'année "+QString::number(newYear));
    histo->setAnimationOptions(QChart::SeriesAnimations);
    QStringList medList;
    for(int i=0; i<nbMed; i++){
        medList.append("MED-"+QString::number(med[i].id));
    }
    QBarCategoryAxis *axis = new QBarCategoryAxis();
//    axis->append(medList);
    histo->createDefaultAxes();
    histo->setAxisX(axis, series);
    chartView->setChart(histo);
//    chartView->repaint();
    chartView->setParent(ui->allPrestHisto);
    chartView->setMaximumSize(700, 400);
}
void MainWindow::drawPiePrest(int year)
{
//    ui->grapheStack->setCurrentIndex(1);
    pieSeries=new QPieSeries;
    int nbMed;
    Medecin *med=getAllMedecin(&nbMed);
    for (int i=0;i<nbMed;i++) {
        pieSeries->append("MED-"+QString::number(med[i].id), getPrest(med[i].id, year));
    }

    QPieSlice *slice = pieSeries->slices().at(1);
    slice->setExploded();
    slice->setLabelVisible();
    slice->setBrush(Qt::lightGray);

    QChart *chart=new QChart();
    chart->addSeries(pieSeries);
    chart->setTitle("Etat des prestations par medecin durant l'année "+QString::number(year));

    chartPieView = new QChartView(chart);
    chartPieView->setParent(ui->allPrestPie);
    chartPieView->setMaximumSize(700, 400);

}
void MainWindow::updatePie(int newYear)
{
    qDebug()<<"new year: "<<newYear;
    chartPieView->repaint();
    pieSeries=new QPieSeries;
    int nbMed;
    Medecin *med=getAllMedecin(&nbMed);
    for (int i=0;i<nbMed;i++) {
        pieSeries->append("MED-"+QString::number(med[i].id), getPrest(med[i].id, newYear));
    }
    QPieSlice *slice = pieSeries->slices().at(1);
    slice->setExploded();
    slice->setLabelVisible();
    slice->setBrush(Qt::lightGray);

    QChart *chart=new QChart();
    chart->addSeries(pieSeries);
    chart->setTitle("Etat des prestations par medecin durant l'année "+QString::number(newYear));

    chartPieView = new QChartView(chart);
    chartPieView->setParent(ui->allPrestPie);
    chartPieView->setMaximumSize(700, 400);
}



void MainWindow::yearList()
{
    int nb;
    Traitement *all=getAllTraitement(&nb);
    ui->yearPrest->clear();
    for (int i=0;i<nb;i++) {
        if(allYear.contains(all[i].dateConsu.year)) continue;
        else{
            allYear.append(all[i].dateConsu.year);

        }
    }
    for (int i=0;i<allYear.length();i++) {
        ui->yearPrest->addItem(QString::number(allYear[i]));
    }
    int max=allYear[0];
    for (int i=0;i<allYear.length();i++) {
        if(allYear[i]>max) max=allYear[i];
    }
    ui->yearPrest->setCurrentText(QString::number(QDate::currentDate().year()));
}
void MainWindow::updateYearList()
{
    int nb;
    Traitement *all=getAllTraitement(&nb);
    QVector<int> allYear;
    allYear.append(all[0].dateConsu.year);


}
void MainWindow::listeMedPrest()
{
    int nb;
    Medecin *med=getAllMedecin(&nb);
    for (int i=0;i<nb;i++) {
        ui->listeMed->addItem("MED-"+QString::number(med[i].id)+" "+QString(med[i].nom)+" "+QString(med[i].prenom));
    }
}
void MainWindow::permut(int *a, int *b)
{
    int c=*a;
    *a=*b;
    *b=c;
    return;
}
void MainWindow::allAboutTrait()
{
    ui->removeTraitBtn->setDisabled(true);
    ui->editTraitBtn->setDisabled(true);
    displayTraitTable();
    ui->traitTable->setColumnWidth(0, 80);
    ui->traitTable->setColumnWidth(1, 200);
    ui->traitTable->setColumnWidth(2, 200);
    ui->traitTable->setColumnWidth(3, 200);
    ui->traitTable->setColumnWidth(5, 30);
}
void MainWindow::displayTraitTable()
{
    ui->traitTable->setAlternatingRowColors(true);
    QAction *test = new QAction;
    ui->traitTable->addAction(test);
    int nb;
    Patient pat;
    Medecin med;
    QDate dateConsu;
    Traitement *trait = getAllTraitement(&nb);
    ui->traitTable->setRowCount(0);
    for (int i=0;i<nb;i++) {
        pat=getPatientById(trait[i].idPat);
        med=getMedecinById(trait[i].idMed);
        dateConsu.setDate(trait[i].dateConsu.year, trait[i].dateConsu.mounth, trait[i].dateConsu.day);
        ui->traitTable->insertRow(i);
        ui->traitTable->setItem(i, 0, new QTableWidgetItem("TRAIT-"+QString::number(trait[i].id)));
        ui->traitTable->setItem(i, 1, new QTableWidgetItem("PAT-"+QString::number(trait[i].idPat)+" "+QString(pat.nom)+" "+QString(pat.prenom)));
        ui->traitTable->setItem(i, 2, new QTableWidgetItem("MED-"+QString::number(trait[i].idMed)+" "+QString(med.nom)+" "+QString(med.prenom)));
        ui->traitTable->setItem(i, 3, new QTableWidgetItem(QString(trait[i].description)));
        ui->traitTable->setItem(i, 4, new QTableWidgetItem(dateConsu.toString("d MMM yyyy")));
        ui->traitTable->setItem(i, 5, new QTableWidgetItem(QString::number(trait[i].nbJours)));
        ui->traitTable->setItem(i, 6, new QTableWidgetItem(QString::number(trait[i].taux)+" Ar"));
    }
    ui->traitNumber->setText(QString::number(nb));
//    selectTraitRow(0);
}
void MainWindow::selectTraitRow(int row)
{
    QTableWidgetSelectionRange rangeMed(row, 0, row, 6);
    ui->traitTable->setRangeSelected(rangeMed, true);
}



void MainWindow::allAboutPat()
{
    ui->searchBarPat->setClearButtonEnabled(true);
    ui->searchBarPat->setPlaceholderText("Numero, nom, adresse");
    ui->patTable->setColumnHidden(4, true);
    displayPatTable();
}
void MainWindow::displayPatTable()
{
    ui->patTable->setAlternatingRowColors(true);
    ui->patTable->setColumnWidth(0, 80);
    ui->patTable->setColumnWidth(1, 300);
    ui->patTable->setColumnWidth(2, 100);
    ui->patTable->setColumnWidth(3, 270);
    ui->patNumber->setText(QString::number(getPatientNumber()));
    if(getPatientNumber()<=0){
        ui->patTable->setRowCount(0);
        return;
    }
    int nb;
    Patient *pat = getAllPatient(&nb);
    ui->patTable->setRowCount(0);
    for (int i=0;i<nb;i++) {
        ui->patTable->insertRow(i);
        ui->patTable->setItem(i, 0, new QTableWidgetItem("PAT-"+QString::number(pat[i].id)));
        ui->patTable->setItem(i, 1, new QTableWidgetItem(QString(pat[i].nom)+" "+QString(pat[i].prenom)));
        ui->patTable->setItem(i, 2, new QTableWidgetItem(QString(pat[i].sexe)));
        ui->patTable->setItem(i, 3, new QTableWidgetItem(QString(pat[i].adresse)));

        QTableWidgetItem *id=new QTableWidgetItem;
        id->setData(Qt::EditRole, pat[i].id);
        ui->patTable->setItem(i, 4, id);
    }
    setPatCompleter();
    selectPatRow(ui->patTable->rowCount()-1);
    showPatDetails(getPatientById(getLastPatientId()));
}
void MainWindow::setPatCompleter()
{
    QList<QString> patListSuggest;
    int nb;
    Patient *pat = getAllPatient(&nb);
    for (int i=0;i<nb;i++) {
        patListSuggest.append("PAT-"+QString::number(pat[i].id)+" "+QString(pat[i].nom)+" "+QString(pat[i].prenom)+" ("+QString(pat[i].adresse)+")");
    }
    QCompleter *patCompleter = new QCompleter(patListSuggest, this);
    patCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    patCompleter->setFilterMode(Qt::MatchContains);
    ui->searchBarPat->setCompleter(patCompleter);
    QObject::connect(patCompleter, SIGNAL(activated(QString)), this, SLOT(searchPat(QString)));
}
void MainWindow::searchPat(QString searchInput)
{
    int idPat=searchInput.remove("PAT-").section(" ", 0, 0).toInt();
    showPatDetails(getPatientById(idPat));
    ui->patTable->setRangeSelected(QTableWidgetSelectionRange(-1, -1, -1, -1), true);
    for (int i=0;i<ui->patTable->rowCount();i++) {
        if(ui->patTable->item(i, 0)->text().remove("PAT-").toInt()==idPat){
            selectPatRow(i);
            break;
        }
    }
}
void MainWindow::selectPatRow(int row)
{
    QTableWidgetSelectionRange rangeMed(row, 0, row, 3);
    ui->patTable->setRangeSelected(rangeMed, true);
}
void MainWindow::showPatDetails(Patient pat)
{
    ui->numPatLbl->setText("PAT-"+QString::number(pat.id));
    ui->nomPatLbl->setText(QString(pat.nom));
    ui->prenomPatLbl->setText(QString(pat.prenom));
    ui->sexePatLbl->setText(QString(pat.sexe));
    ui->adressePatLbl->setText(QString(pat.adresse));
    ui->matriculePatLbl->setText("PAT-"+QString::number(pat.id));
}


void MainWindow::allAboutMed()
{
    ui->searchBar->setClearButtonEnabled(true);
    ui->searchBar->setPlaceholderText("Numero, nom, prenom");
    QTableWidgetSelectionRange rangeMed(1, 0, 1, 4);
    ui->medTable->setRangeSelected(rangeMed, true);
    displayMedTable();
}
void MainWindow::setUpNavBtn()
{
    _welcomeBtn = new NavBtn(0, this);
    _welcomeBtn->setIcon(QIcon(":/img/img/welcomeBtn.png"));
    _welcomeBtn->setActive(true);
    QObject::connect(_welcomeBtn, SIGNAL(clicked()), this, SLOT(dashboardStack()));

    _medecinBtn = new NavBtn(1, this);
    _medecinBtn->setIcon(QIcon(":/img/img/medecinBtn.png"));
    QObject::connect(_medecinBtn, SIGNAL(clicked()), this, SLOT(gestMedecinStack()));

    _patientBtn = new NavBtn(2, this);
    _patientBtn->setIcon(QIcon(":/img/img/patientBtn.png"));
    QObject::connect(_patientBtn, SIGNAL(clicked()), this, SLOT(gestPatStack()));

    _traitementBtn = new NavBtn(3, this);
    _traitementBtn->setIcon(QIcon(":/img/img/traitementBtn.png"));
    QObject::connect(_traitementBtn, SIGNAL(clicked()), this, SLOT(gestTraitStack()));

    _etatPrestBtn = new NavBtn(4, this);
    _etatPrestBtn->setIcon(QIcon(":/img/img/etatPrestation.png"));
    QObject::connect(_etatPrestBtn, SIGNAL(clicked()), this, SLOT(gestPrestStack()));

}
void MainWindow::dashboardStack()
{
    ui->mainStackedWidget->setCurrentIndex(DASHBOARD);
    setWindowTitle("Medically");
}
void MainWindow::gestMedecinStack()
{
    ui->mainStackedWidget->setCurrentIndex(GEST_MEDECIN);
    setWindowTitle("Medically - Gestion des medecins");

    allAboutMed();
}
void MainWindow::gestPatStack()
{
    ui->mainStackedWidget->setCurrentIndex(GEST_PATIENT);
    setWindowTitle("Medically - Gestion des patients");
    allAboutPat();
}
void MainWindow::gestTraitStack()
{
    ui->mainStackedWidget->setCurrentIndex(GEST_TRAITEMENT);
    setWindowTitle("Medically - Gestion des traitements");
    allAboutTrait();
}
void MainWindow::gestPrestStack()
{
    ui->mainStackedWidget->setCurrentIndex(GEST_PREST);
    setWindowTitle("Medically - Etat des prestations");
//    allAboutPrest();
    yearList();
}
void MainWindow::setCurrentStack(int index)
{
    ui->mainStackedWidget->setCurrentIndex(index);
    qDebug()<<index;
    return;
}
void MainWindow::displayMedTable()
{
    ui->medTable->setAlternatingRowColors(true);
    ui->medTable->setColumnWidth(0, 80);
    ui->medTable->setColumnWidth(1, 280);
    ui->medTable->setColumnWidth(2, 80);
    ui->medTable->setColumnWidth(3, 220);
    ui->medNumber->setText(QString::number(getMedNumber()));
    showMedDetails(getMedecinById(getLastIdMedecin()));
    int nb;
    qDebug()<<"med number="<<getMedNumber();
    if(getMedNumber()<=0){
        ui->medTable->setRowCount(0);
        return;
    }
    Medecin *med = getAllMedecin(&nb);
    ui->medTable->setRowCount(0);
    ui->medTable->setColumnHidden(5, true);
    ui->medTable->setColumnHidden(6, true);
    for (int i=0;i<nb;i++) {
        QTableWidgetItem *id=new QTableWidgetItem;
        QTableWidgetItem *taux=new QTableWidgetItem;
        ui->medTable->insertRow(i);
        ui->medTable->setItem(i, 0, new QTableWidgetItem("MED-"+QString::number(med[i].id)));
        ui->medTable->setItem(i, 1, new QTableWidgetItem(QString(med[i].nom)+" "+QString(med[i].prenom)));
        ui->medTable->setItem(i, 2, new QTableWidgetItem(QString(med[i].sexe)));
        ui->medTable->setItem(i, 3, new QTableWidgetItem(QString(med[i].specialite)));
        ui->medTable->setItem(i, 4, new QTableWidgetItem(QString::number(med[i].taux)+" Ar"));

        id->setData(Qt::EditRole, med[i].id);
        ui->medTable->setItem(i, 5, id);
        taux->setData(Qt::EditRole, med[i].taux);
        ui->medTable->setItem(i, 6, taux);
    }
    selectMedRow(ui->medTable->rowCount()-1);
    setMedCompleter();
}

void MainWindow::on_pushButton_6_clicked() //slot ajouter medecin
{
    AddMedDialog *dialog = new AddMedDialog(this);
    dialog->exec();
    displayMedTable();
}

void MainWindow::on_pushButton_clicked()  //slot update medecin
{
    int idMed = ui->matriculeLbl->text().remove("MED-").toInt();
    UpdateMedDialog update(this, idMed);
    update.exec();
    displayMedTable();
    showMedDetails(getMedecinById(idMed));
}

void MainWindow::on_medTable_cellActivated(int row, int column)
{
    QTableWidgetSelectionRange rangeMed(row, 0, row, 4);
    ui->medTable->setRangeSelected(rangeMed, true);
    QString matricule=ui->medTable->item(row, 0)->text().remove("MED-");
    int idMed = matricule.toInt();
    showMedDetails(getMedecinById(idMed));
}

void MainWindow::on_medTable_cellChanged(int row, int column)
{

}

void MainWindow::on_medTable_cellClicked(int row, int column)
{
    QTableWidgetSelectionRange rangeMed(row, 0, row, 4);
    ui->medTable->setRangeSelected(rangeMed, true);
    QString matricule=ui->medTable->item(row, 0)->text().remove("MED-");
    int idMed = matricule.toInt();
    showMedDetails(getMedecinById(idMed));

}
void MainWindow::showMedDetails(Medecin med)
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
    qDebug()<<pdpPath;
    QPixmap pdp(pdpPath);


    pdpLbl->setPixmap(pdp.scaled(200, 200, Qt::KeepAspectRatioByExpanding));
    pdpLbl->setFixedSize(200, 200);
    QVBoxLayout *det=new QVBoxLayout(this);
    det->addWidget(pdpLbl);
    det->addStretch();
//    ui->infoMedLayout->setParent(this);
    det->addLayout(ui->infoMedLayout);
    ui->medDetailsGroup->setLayout(det);

}
void MainWindow::setDisableDetailsBtn(bool a)
{
    ui->removeMedBtn->setDisabled(a);
    ui->voirPatient->setDisabled(a);
    ui->pushButton->setDisabled(a);
}

void MainWindow::on_medTable_itemSelectionChanged()
{
//    setDisableDetailsBtn(true);
}

void MainWindow::on_removeMedBtn_clicked()
{
    int idMed = ui->matriculeLbl->text().remove("MED-").toInt();
    int del = QMessageBox::question(this, "Confirmer", "Voulez vous vraiment supprimer ce medecin");
    if(del==QMessageBox::Yes){
        QFile pdp(QString(getMedecinById(idMed).photo));
        qDebug()<<"pdp:  "+QDir::currentPath()+"/"+QString(getMedecinById(idMed).photo);
        pdp.remove();
        removeMedecinById(idMed);
        displayMedTable();
    }
    else return;
}
void MainWindow::setMedCompleter()
{
    QList<QString> medListSuggest;
    int nb;
    Medecin *all=getAllMedecin(&nb);
//    if(nb==)
    for (int i=0;i<nb;i++) {
        medListSuggest.append("MED-"+QString::number(all[i].id)+" " + QString(all[i].nom)+" "+QString(all[i].prenom));
    }
    QCompleter *medCompleter = new QCompleter(medListSuggest, this);
    medCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    medCompleter->setFilterMode(Qt::MatchContains);
    ui->searchBar->setCompleter(medCompleter);
    QObject::connect(medCompleter, SIGNAL(activated(QString)), this, SLOT(searchMed(QString)));
}
void MainWindow::searchMed(QString searchInput)
{
    int idMed = searchInput.remove("MED-").section(" ", 0,0).toInt();
    showMedDetails(getMedecinById(idMed));
    int row=ui->medTable->rowCount();
    ui->medTable->setRangeSelected(QTableWidgetSelectionRange(-1, -1, -1, -1), true);
    for(int i=0; i<row; i++){
        if(ui->medTable->item(i, 0)->text().remove("MED-").toInt()==idMed){
            selectMedRow(i);

            break;
        }
    }
}

void MainWindow::on_clearMedSearch_clicked()
{
    ui->searchBar->clear();
}

void MainWindow::on_comboBox_2_activated(const QString &arg1) //trier par combobox
{
    //saika nanaovana slot sur le combobox
}
void MainWindow::on_comboBox_2_activated(int index) //trier par
{
    //if(index==0) index=5;
   // else if(index==4) index=6;
    index=index==0?5:index;
    index=index==4?6:index;
    if(ui->reverseSortMed->isChecked()) ui->medTable->sortByColumn(index, Qt::DescendingOrder);
    else ui->medTable->sortByColumn(index, Qt::AscendingOrder);
}

void MainWindow::on_reverseSortMed_stateChanged(int arg1)
{
    int index=ui->comboBox_2->currentIndex();
    index=index==0?5:index;
    index=index==4?6:index;
    if(arg1==0) //not checked
        ui->medTable->sortByColumn(index, Qt::AscendingOrder);
    else  ui->medTable->sortByColumn(index, Qt::DescendingOrder);
}
void MainWindow::selectMedRow(int row)
{
    QTableWidgetSelectionRange rangeMed(row, 0, row, 4);
    ui->medTable->setRangeSelected(rangeMed, true);
}
void MainWindow::generatePdfFromTable(QTableWidget *table)
{
    QString pdfName=QFileDialog::getSaveFileName(this, "Enregistrer le pdf sous...", "/home/sans_titre.pdf", "Pdf (*.pdf)");
     if(pdfName=="") return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::NativeFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setOutputFileName(pdfName);

    QString title, effectif, headerColor;
    switch (ui->mainStackedWidget->currentIndex()) {
    case GEST_MEDECIN:
        title="Liste de tous les medecins";
        effectif="Effectif: "+QString::number(getMedNumber());
        headerColor="#0d6f7a";
        break;
    case GEST_PATIENT:
        title="Liste de tous les patients";
        effectif="Effectif: "+QString::number(getPatientNumber());
        headerColor="#03e185";
        break;
    case GEST_TRAITEMENT:
        title="Liste de tous les traitements";
        effectif="Effectif: "+QString::number(getTraitementNumber());
        headerColor="#0d6f7a";
    default:
        title="";
        effectif="";
        headerColor="#0d6f7a";
        break;
    }

    QTextDocument doc;
    QString trWidthLg="width: 280px;";
    QString text("<html><head/><body><p><span style=\" font-size:11pt; font-weight:600; color:#0d6f7a; border: 1px solid red;\">"+title+"</span></p></body></html>");
    text.append("<html><head/><body><p><span style=\" font-size:9pt; font-weight:600; color:#03e185;\">"+effectif+"</span></p></body></html>");
    text.append("<table style:\"color: lightGrey; font-size: 10pt;\"><thead>");
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
    doc.setDocumentMargin(20);
    ui->statusBar->showMessage("Generation du pdf...");
    doc.print(&printer);
    ui->statusBar->showMessage("Generation du pdf terminee!");
    QMessageBox::information(this, this->windowTitle(), "Le pdf a bien ete enregistre sous: "+pdfName);
}
void MainWindow::on_printMedtable_clicked()
{
    generatePdfFromTable(ui->medTable);
}


void MainWindow::on_mainStackedWidget_currentChanged(int arg1)
{
    switch (arg1) {
    case DASHBOARD:
        _welcomeBtn->setActive(true);
        _medecinBtn->setActive(false);
        _patientBtn->setActive(false);
        _traitementBtn->setActive(false);
        _etatPrestBtn->setActive(false);
        break;
    case GEST_MEDECIN:
        _welcomeBtn->setActive(false);
        _medecinBtn->setActive(true);
        _patientBtn->setActive(false);
        _traitementBtn->setActive(false);
        _etatPrestBtn->setActive(false);
        break;
    case GEST_PATIENT:
        _welcomeBtn->setActive(false);
        _medecinBtn->setActive(false);
        _patientBtn->setActive(true);
        _traitementBtn->setActive(false);
        _etatPrestBtn->setActive(false);
        break;
    case GEST_TRAITEMENT:
        _welcomeBtn->setActive(false);
        _medecinBtn->setActive(false);
        _patientBtn->setActive(false);
        _traitementBtn->setActive(true);
        _etatPrestBtn->setActive(false);
        break;
    case GEST_PREST:
        _welcomeBtn->setActive(false);
        _medecinBtn->setActive(false);
        _patientBtn->setActive(false);
        _traitementBtn->setActive(false);
        _etatPrestBtn->setActive(true);

    }
}
void MainWindow::on_medTable_cellPressed(int row, int column)
{
    ui->pushButton->setDisabled(false);
    ui->removeMedBtn->setDisabled(false);
    QTableWidgetSelectionRange rangeMed(row, 0, row, 4);
    ui->medTable->setRangeSelected(rangeMed, true);
    QString matricule=ui->medTable->item(row, 0)->text().remove("MED-");
    int idMed = matricule.toInt();
    showMedDetails(getMedecinById(idMed));
}
void MainWindow::on_medTable_cellEntered(int row, int column){}


void MainWindow::on_patTable_cellPressed(int row, int column)
{
    ui->editPatBtn->setDisabled(false);
    ui->removePatBtn->setDisabled(false);
    QTableWidgetSelectionRange rangeMed(row, 0, row, 3);
    ui->patTable->setRangeSelected(rangeMed, true);
    int idPat = ui->patTable->item(row, 0)->text().remove("PAT-").toInt();
    showPatDetails(getPatientById(idPat));
}

void MainWindow::on_patTable_cellClicked(int row, int column)
{
    ui->editPatBtn->setDisabled(false);
    ui->removePatBtn->setDisabled(false);
    QTableWidgetSelectionRange rangeMed(row, 0, row, 3);
    ui->patTable->setRangeSelected(rangeMed, true);
    int idPat = ui->patTable->item(row, 0)->text().remove("PAT-").toInt();
    showPatDetails(getPatientById(idPat));
}

void MainWindow::on_addPatMed_clicked()
{
    AddPatDialog *dialog = new AddPatDialog(this);
    dialog->exec();
    displayPatTable();
}

void MainWindow::on_removePatBtn_clicked()
{
    int idPat = ui->numPatLbl->text().remove("PAT-").toInt();
    int del = QMessageBox::question(this, "Confirmer", "Voulez vous vraiment supprimer le patient: <strong>PAT-"+ui->patNumber->text()+" "+ui->nomPatLbl->text()+" "+ui->prenomPatLbl->text()+"</strong>?");
    if(del==QMessageBox::Yes){
        removePatientById(idPat);
        displayPatTable();
    }
    else return;
}

void MainWindow::on_editPatBtn_clicked()
{
    int idPat=ui->numPatLbl->text().remove("PAT-").toInt();
    UpdatePatDialog dialog(idPat, this);
    dialog.exec();
    displayPatTable();
    showPatDetails(getPatientById(idPat));
}

void MainWindow::on_printPatTable_clicked()
{
    generatePdfFromTable(ui->patTable);
}

void MainWindow::on_voirTraitBtn_clicked()
{
    VoirTrait fen(ui->numPatLbl->text().remove("PAT-").toInt());
    fen.exec();
    displayTraitTable();
}

void MainWindow::on_traitTable_cellPressed(int row, int column)
{
    ui->editTraitBtn->setDisabled(false);
    ui->removeTraitBtn->setDisabled(false);
    selectTraitRow(row);
}

void MainWindow::on_addTraitBtn_2_clicked()
{
    AddTraitDialog dialog(this);
    dialog.exec();
    displayTraitTable();
}

void MainWindow::on_addTraitBtn_clicked()
{
    AddTraitDialog dialog(ui->numPatLbl->text().remove("PAT-").toInt());
    dialog.exec();
    displayTraitTable();
}

void MainWindow::on_voirPatient_clicked()
{
    VoirPatDuMed dialog(ui->matriculeLbl->text().remove("MED-").toInt(), this);
    dialog.exec();
    displayTraitTable();
}

void MainWindow::on_yearPrest_activated(const QString &arg1)
{
    displayAllPrest(arg1.toInt());
    updateHisto(arg1.toInt());
    updatePie(arg1.toInt());
}

void MainWindow::on_graphe_activated(int index)
{
     ui->grapheStack->setCurrentIndex(index);
     qDebug()<<"ind "<<index;
}

void MainWindow::on_graphe_currentIndexChanged(int index)
{
    ui->grapheStack->setCurrentIndex(index);
}

void MainWindow::on_listeMed_activated(const QString &arg1)
{

}

void MainWindow::on_prestMedBtn_clicked()
{
    prestDialog dialog;
    dialog.exec();

}

void MainWindow::on_removeTraitBtn_clicked()
{
    int idTrait=ui->traitTable->item(ui->traitTable->currentRow(), 0)->text().remove("TRAIT-").toInt();
    int del=QMessageBox::question(this, "Veuiller confirmer", "Voulez vous vraiment supprimer ce traitement?");
    if(del==QMessageBox::Yes){
        removeTraitementById(idTrait);
        displayTraitTable();
    }
    else return;
}

void MainWindow::on_editTraitBtn_clicked()
{
    int row=ui->traitTable->currentRow();
    int idTrait=ui->traitTable->item(row, 0)->text().remove("TRAIT-").toInt();
    Traitement trait=getTraitementById(idTrait);
    AddTraitDialog dialog(trait, this);
    dialog.exec();
    if(dialog.isSaved()) displayTraitTable();
    selectTraitRow(row);
}

void MainWindow::on_printTraitTable_clicked()
{
    generatePdfFromTable(ui->traitTable);
}

void MainWindow::on_printPrestBtn_clicked()
{
    QString pdfName=QFileDialog::getSaveFileName(this, "Enregistrer le pdf sous...", "/home/sans_titre.pdf", "Pdf (*.pdf)");
     if(pdfName=="") return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::NativeFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setOutputFileName(pdfName);

    QTextDocument doc;
    QString title, effectif, headerColor;
    title="Etat des prestations des medecins pour l'année "+ui->yearPrest->currentText();
    effectif="Total: "+ui->totalPrest->text();
    headerColor="rgba(248, 160, 8, 255)";
    QString text("<html><head/><body><p><span style=\" font-size:11pt; font-weight:600; color:#0d6f7a; border: 1px solid red;\">"+title+"</span></p></body></html>");
    text.append("<html><head/><body><p><span style=\" font-size:9pt; font-weight:600; color:#03e185;\">"+effectif+"</span></p></body></html>");
    text.append("<table style:\"color: lightGrey; font-size: 10pt;\"><thead>");
    text.append("<tr style=\"border:1px solid black; background-color: "+headerColor+";color: white; height: 40px;\">");
    for (int i=0;i<ui->prestTable->columnCount();i++) {
        text.append("<th  style=\"border: 1px solid black;\">&nbsp;&nbsp;").append(ui->prestTable->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString()).append("&nbsp;&nbsp;</th>");
    }
    text.append("</tr>");
    text.append("</thead>");
    text.append("<tbody style=\"color: rgb(99, 99, 99);\">");
    for (int i=0;i<ui->prestTable->rowCount();i++) {
        if(i%2==1) text.append("<tr style=\"background-color: rgb(196, 230, 223);\">");
        else text.append("<tr>");
        for (int j=0;j<ui->prestTable->columnCount();j++) {
             text.append("<td>").append(ui->prestTable->item(i, j)->text()).append("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>");
        }
        text.append("</tr>");
    }
    text.append("</tbody></table>");
//    text.append("<p style=\"color:#03e185;\">Hitogramme</p>");


    QTextCursor cursor(&doc);
    cursor.insertHtml(text);

    QImage img(500, 300, QImage::Format_RGB32);
    QPainter painter(&img);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();
    chartView->render(&painter);

    cursor.insertImage(img);

    doc.setPageSize(printer.pageRect().size());
    doc.setDocumentMargin(20);
    ui->statusBar->showMessage("Generation du pdf...");
    doc.print(&printer);
    ui->statusBar->showMessage("Generation du pdf terminee!");
    QMessageBox::information(this, this->windowTitle(), "Le pdf a bien ete enregistre sous: "+pdfName);



}

void MainWindow::on_comboBox_5_activated(int index)
{
    if(index==0) index=3;
    else if(index==2) index=4;

    if(ui->reverseSortMed_4->isChecked()) ui->prestTable->sortByColumn(index, Qt::DescendingOrder);
    else ui->prestTable->sortByColumn(index, Qt::AscendingOrder);

}

void MainWindow::on_reverseSortMed_4_stateChanged(int arg1)
{
    int index=ui->comboBox_5->currentIndex();
    index=index==0?3:index;
    index=index==2?4:index;
    if(arg1==0) //not checked
        ui->prestTable->sortByColumn(index, Qt::AscendingOrder);
    else  ui->prestTable->sortByColumn(index, Qt::DescendingOrder);
}

void MainWindow::on_reverseSortMed_2_stateChanged(int arg1)
{
    int index=ui->comboBox_3->currentIndex();
    index=index==0?4:index;
    if(arg1==0) //not checked
        ui->patTable->sortByColumn(index, Qt::AscendingOrder);
    else  ui->patTable->sortByColumn(index, Qt::DescendingOrder);
}

void MainWindow::on_comboBox_3_activated(int index)
{
    index=index==0?4:index;
    if(ui->reverseSortMed_2->isChecked()) ui->patTable->sortByColumn(index, Qt::DescendingOrder);
    else ui->patTable->sortByColumn(index, Qt::AscendingOrder);
}

void MainWindow::on_actionGerer_les_Medecins_triggered()
{
    ui->mainStackedWidget->setCurrentIndex(GEST_MEDECIN);
}

void MainWindow::on_actionGerer_les_patients_triggered()
{
    ui->mainStackedWidget->setCurrentIndex(GEST_PATIENT);
}

void MainWindow::on_actionGerer_les_traitements_triggered()
{
    ui->mainStackedWidget->setCurrentIndex(GEST_TRAITEMENT);
}

void MainWindow::on_actionEtat_des_prestations_triggered()
{
    ui->mainStackedWidget->setCurrentIndex(GEST_PREST);
}

void MainWindow::on_actionSe_deconnecter_et_quitter_triggered()
{

}
