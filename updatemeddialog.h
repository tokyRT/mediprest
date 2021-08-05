#ifndef UPDATEMEDDIALOG_H
#define UPDATEMEDDIALOG_H

#include <QDialog>
#include "medecin.h"

namespace Ui {
class UpdateMedDialog;
}

class UpdateMedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateMedDialog(QWidget *parent = nullptr);
    UpdateMedDialog(QWidget *parent=nullptr, int idMed=1);
    ~UpdateMedDialog();
    void uploadPdp(QString pathOrigin, int idMed);

private slots:
    void on_updateMedBtn_clicked();

    void on_pdpBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::UpdateMedDialog *ui;
    QString pdpPath="";
    Medecin newMed;
};

#endif // UPDATEMEDDIALOG_H
