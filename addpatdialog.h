#ifndef ADDPATDIALOG_H
#define ADDPATDIALOG_H

#include <QDialog>

namespace Ui {
class AddPatDialog;
}

class AddPatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPatDialog(QWidget *parent = nullptr);
    ~AddPatDialog();

private slots:
    void on_addMedBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::AddPatDialog *ui;
};

#endif // ADDPATDIALOG_H
