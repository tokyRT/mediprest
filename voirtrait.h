#ifndef VOIRTRAIT_H
#define VOIRTRAIT_H

#include <QDialog>
#include "medecin.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QDate>
#include "traitement.h"
#include "addtraitdialog.h"
#include "editcriteredialog.h"

namespace Ui {
class VoirTrait;
}

class VoirTrait : public QDialog
{
    Q_OBJECT

public:
    explicit VoirTrait(QWidget *parent = nullptr);
    VoirTrait(int patId, QWidget *parent = nullptr);
    ~VoirTrait();
    void displayTraitTable();
    void display(Traitement *trait, int nb);
    void showMedDetails(Medecin med);
    void allMed();
    void displayTraitTable(QString mounth, int mounthIndex, int yearMounth);
    void displayTraitTable(int year);
    void displayTraitTable(QDate firstDate, QDate secondDate);
    void selectMedRow(int row);

private slots:
    void on_cancelBtn_clicked();

    void on_medTable_cellClicked(int row, int column);

    void on_critereBtn_clicked();

    void on_consuMedBtn_clicked();

    void on_printMedtable_clicked();

private:
    Ui::VoirTrait *ui;
    int idPat;
    QLabel *pdpLbl = new QLabel(this);
    EditCritereDialog *dialog;
};

#endif // VOIRTRAIT_H
