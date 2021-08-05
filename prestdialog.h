#ifndef PRESTDIALOG_H
#define PRESTDIALOG_H

#include <QDialog>
#include <QVector>
#include "medecin.h"
#include "traitement.h"
#include <QDate>
#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QTextDocument>

namespace Ui {
class prestDialog;
}

class prestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit prestDialog(QWidget *parent = nullptr);
    void populateListeMed();
    void populateYear();
    void populate();
    void displayTable(int idMed, int year);
    void drawMedHisto(int idMed, int year);
    void updateMedHisto(int idMed, int year);
    ~prestDialog();

private slots:
    void on_listeMed_activated(const QString &arg1);

    void on_yearPrest_activated(const QString &arg1);

    void on_cancelBtn_clicked();

    void on_printPrest_clicked();

private:
    Ui::prestDialog *ui;
    QVector<int> allYear;
    QChartView *chartView;
    QBarSeries *series;

};

#endif // PRESTDIALOG_H
