#ifndef ADDTRAITDIALOG_H
#define ADDTRAITDIALOG_H

#include <QDialog>
#include "traitement.h"
#include "medecin.h"
#include "patient.h"
#include <QDate>
#include <QMessageBox>
#include <QDebug>
#include "editcriteredialog.h"

#define MEDECIN 1


namespace Ui {
class AddTraitDialog;
}

class AddTraitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTraitDialog(QWidget *parent = nullptr);
    AddTraitDialog(int idPat, EditCritereDialog *critere=nullptr, QWidget *parent = nullptr);
    AddTraitDialog(int idMed, int type, EditCritereDialog *critere, QWidget *parent = nullptr);
    AddTraitDialog(Traitement trait, QWidget *parent = nullptr); //for trait update
    ~AddTraitDialog();
    void basicSetup();
    bool isSaved();
    void setInUpCritere(EditCritereDialog *critere);

private slots:
    void on_addMedBtn_clicked();

    void on_listeMed_activated(const QString &arg1);

    void on_nbJours_valueChanged(int arg1);

    void on_cancelBtn_clicked();

private:
    Ui::AddTraitDialog *ui;
    bool editing=false;
    bool saved=false;
};

#endif // ADDTRAITDIALOG_H
