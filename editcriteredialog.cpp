#include "editcriteredialog.h"
#include "ui_editcriteredialog.h"

EditCritereDialog::EditCritereDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditCritereDialog)
{
    ui->setupUi(this);
    setWindowTitle("Parametre de listage");
    ui->firstDate->setCalendarPopup(true);
    ui->secondDate->setCalendarPopup(true);
    ui->year->setMinimum(2000);
//    ui->year->setMaximum(QDate::currentDate().year());
    ui->year->setValue(QDate::currentDate().year());
    ui->mounth->setCurrentIndex(QDate::currentDate().month()-1);
    ui->secondDate->setDate(QDate::currentDate());
//    ui->secondDate->setMaximumDate(QDate::currentDate());

}

EditCritereDialog::~EditCritereDialog()
{
    delete ui;
}

void EditCritereDialog::on_buttonBox_accepted()
{
    setEdited(true);
    if(ui->afficherTous->isChecked()){
        setChoice(ALL);
    }
    else if (ui->onlyMounth->isChecked()) {
        setChoice(MOUNTH);
        setMounth(ui->mounth->currentText());
        setMounthIndex(ui->mounth->currentIndex()+1);
        setYearMounth(ui->yearMounth->value());
    }
    else if (ui->onlyYear->isChecked()) {
        setChoice(YEAR);
        setYear(ui->year->value());
    }
    else {
        setChoice(TWO_DATES);
        setFirstDate(ui->firstDate->date());
        setSecondDate(ui->secondDate->date());
    }
}
int EditCritereDialog::getChoice() {return choice;}
int EditCritereDialog::getMounthIndex() {return  mounthIndex;}
QString EditCritereDialog::getMounth() {return  mounth;}
int EditCritereDialog::getYear() {return year;}
QDate EditCritereDialog::getFirstDate() {return firstDate;}
QDate EditCritereDialog::getSecondDate() {return  secondDate;}
bool EditCritereDialog::isEdited(){return edited;}
int EditCritereDialog::getYearMounth(){return yearMounth;}

void EditCritereDialog::setChoice(int ch) {choice=ch;}
void EditCritereDialog::setMounthIndex(int index){mounthIndex=index;}
void EditCritereDialog::setMounth(QString m){mounth=m;}
void EditCritereDialog::setYear(int y){year=y;}
void EditCritereDialog::setFirstDate(QDate d){firstDate=d;}
void EditCritereDialog::setSecondDate(QDate d){secondDate=d;}
void EditCritereDialog::setEdited(bool a){edited=a;}
void EditCritereDialog::setYearMounth(int year){yearMounth=year;}

void EditCritereDialog::on_buttonBox_rejected()
{
    setEdited(false);
}
