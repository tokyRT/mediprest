#ifndef VOIRPATDUMED_H
#define VOIRPATDUMED_H

#include <QDialog>
#include "medecin.h"
#include "patient.h"
#include "traitement.h"
#include <QDate>
#include "editcriteredialog.h"

namespace Ui {
class VoirPatDuMed;
}

class VoirPatDuMed : public QDialog
{
    Q_OBJECT

public:
    explicit VoirPatDuMed(QWidget *parent = nullptr);
    VoirPatDuMed(int id, QWidget *parent = nullptr);
    ~VoirPatDuMed();
    void displayTable();
    void showPatDetails(Patient pat);
    void displayTable(QString mounth, int mountIndex, int yearMounth);
    void displayTable(int year);
    void displayTable(QDate firstDate, QDate secondDate);
    void selectRow(int row);

private slots:
    void on_medTable_cellPressed(int row, int column);

    void on_medTable_cellClicked(int row, int column);

    void on_critereBtn_clicked();

    void on_traitPat_clicked();

    void on_printMedtable_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::VoirPatDuMed *ui;
    int idMed;
    EditCritereDialog *dialog;
};

#endif // VOIRPATDUMED_H
