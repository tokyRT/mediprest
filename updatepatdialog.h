#ifndef UPDATEPATDIALOG_H
#define UPDATEPATDIALOG_H

#include <QDialog>
#include "patient.h"

namespace Ui {
class UpdatePatDialog;
}

class UpdatePatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdatePatDialog(QWidget *parent = nullptr);
    UpdatePatDialog(int idPat, QWidget *parent=nullptr);
    ~UpdatePatDialog();

private slots:
    void on_savePatBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::UpdatePatDialog *ui;
};

#endif // UPDATEPATDIALOG_H
