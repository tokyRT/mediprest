#ifndef ADDMEDDIALOG_H
#define ADDMEDDIALOG_H

#include <QDialog>
#include <QFile>

namespace Ui {
class AddMedDialog;
}

class AddMedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMedDialog(QWidget *parent = nullptr);
    ~AddMedDialog();
    void uploadPdp(QString pathOrigin, int idMed);

private slots:
    void on_cancelBtn_clicked();

    void on_addMedBtn_clicked();

    void on_pdpBtn_clicked();

private:
    Ui::AddMedDialog *ui;
    QString pdpPath="Aucune photo choisie";
};

#endif // ADDMEDDIALOG_H
