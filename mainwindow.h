#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "navbtn.h"
//#include "patient.h"
#include "medecin.h"
#include "patient.h"
#include <QString>
#include <QFile>
#include <QDir>
#include <QLabel>
#include <QTableWidgetSelectionRange>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QTextDocument>
#include <QFileDialog>
#include "addtraitdialog.h"
#include <QVector>
#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QPieSeries>
#include <QPieSlice>
#include <QTextCursor>
#include <QPainter>
#include <QFont>
#include <QImage>

#define DASHBOARD 0
#define GEST_MEDECIN 1
#define GEST_PATIENT 2
#define GEST_TRAITEMENT 3
#define GEST_PREST 4

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void allAboutMed();
    void displayMedTable();
    void setDisableDetailsBtn(bool a);
    void showMedDetails(Medecin med);
    void setMedCompleter();
    void selectMedRow(int row);
    void generatePdfFromTable(QTableWidget *table);
    void setUpNavBtn();

    void allAboutPat();
    void displayPatTable();
    void setPatCompleter();
    void showPatDetails(Patient pat);
    void selectPatRow(int row);

    void allAboutTrait();
    void displayTraitTable();
    void selectTraitRow(int row);

    void allAboutPrest();
    void displayAllPrest(int year);
    void permut(int *a, int *b);
    void yearList();
    void updateYearList();
    void listeMedPrest();
    void drawAllPrestHisto(int year);
    void updateHisto(int newYear);
    void drawPiePrest(int year);
    void updatePie(int newYear);
    void displayMedPrest();


private slots:
    void setCurrentStack(int index);
    void dashboardStack();
    void gestMedecinStack();
    void gestPatStack();
    void gestTraitStack();
    void gestPrestStack();
    void on_pushButton_6_clicked();
    void on_pushButton_clicked();

    void on_medTable_cellActivated(int row, int column);

    void on_medTable_cellChanged(int row, int column);

    void on_medTable_cellClicked(int row, int column);

    void on_medTable_itemSelectionChanged();

    void on_removeMedBtn_clicked();
    void searchMed(QString searchInput);
    void searchPat(QString searchInput);

    void on_clearMedSearch_clicked();

    void on_comboBox_2_activated(const QString &arg1);

    void on_comboBox_2_activated(int index);

    void on_reverseSortMed_stateChanged(int arg1);

    void on_printMedtable_clicked();

    void on_mainStackedWidget_currentChanged(int arg1);

    void on_medTable_cellPressed(int row, int column);

    void on_medTable_cellEntered(int row, int column);

    void on_patTable_cellPressed(int row, int column);

    void on_patTable_cellClicked(int row, int column);

    void on_addPatMed_clicked();

    void on_removePatBtn_clicked();

    void on_editPatBtn_clicked();

    void on_printPatTable_clicked();

    void on_voirTraitBtn_clicked();

    void on_traitTable_cellPressed(int row, int column);

    void on_addTraitBtn_2_clicked();

    void on_addTraitBtn_clicked();

    void on_voirPatient_clicked();

    void on_yearPrest_activated(const QString &arg1);

    void on_graphe_activated(int index);

    void on_graphe_currentIndexChanged(int index);

    void on_listeMed_activated(const QString &arg1);

    void on_prestMedBtn_clicked();

    void on_removeTraitBtn_clicked();

    void on_editTraitBtn_clicked();

    void on_printTraitTable_clicked();

    void on_printPrestBtn_clicked();

    void on_comboBox_5_activated(int index);

    void on_reverseSortMed_4_stateChanged(int arg1);

    void on_reverseSortMed_2_stateChanged(int arg1);

    void on_comboBox_3_activated(int index);

    void on_actionGerer_les_Medecins_triggered();

    void on_actionGerer_les_patients_triggered();

    void on_actionGerer_les_traitements_triggered();

    void on_actionEtat_des_prestations_triggered();

    void on_actionSe_deconnecter_et_quitter_triggered();

private:
    Ui::MainWindow *ui;
    NavBtn *_welcomeBtn, *_medecinBtn, *_patientBtn, *_etatPrestBtn, *_traitementBtn;
    QLabel *pdpLbl = new QLabel(this);

    QVector<int> allYear;

    QChartView *chartView, *chartPieView;
    QBarSeries *series;
    QPieSeries *pieSeries;

};

#endif // MAINWINDOW_H
