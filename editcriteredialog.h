#ifndef EDITCRITEREDIALOG_H
#define EDITCRITEREDIALOG_H

#include <QDialog>
#include <QDate>

#define ALL 0
#define MOUNTH 1
#define YEAR 2
#define TWO_DATES 3

namespace Ui {
class EditCritereDialog;
}

class EditCritereDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditCritereDialog(QWidget *parent = nullptr);
    ~EditCritereDialog();
    int getChoice();
    int getMounthIndex();
    QString getMounth();
    int getYear();
    QDate getFirstDate();
    QDate getSecondDate();
    bool isEdited();
    int getYearMounth();

    void setChoice(int ch);
    void setMounthIndex(int index);
    void setMounth(QString m);
    void setYear(int y);
    void setFirstDate(QDate d);
    void setSecondDate(QDate d);
    void setEdited(bool a);
    void setYearMounth(int year);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::EditCritereDialog *ui;
    int choice=ALL;
    int mounthIndex;
    QString mounth;
    int year, yearMounth;
    QDate firstDate, secondDate;
    bool edited=false;
};

#endif // EDITCRITEREDIALOG_H
