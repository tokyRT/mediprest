#include "prestdialog.h"
#include "ui_prestdialog.h"

prestDialog::prestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::prestDialog)
{
    ui->setupUi(this);
    setWindowTitle("Etat des prestations par medecin");
    setWindowIcon(QIcon(":/img/img/logo.png"));
    populate();
}

prestDialog::~prestDialog()
{
    delete ui;
}
void prestDialog::populate()
{
    populateListeMed();
    populateYear();
    displayTable(ui->listeMed->currentText().remove("MED-").section(" ", 0,0).toInt(), ui->yearPrest->currentText().toInt());
    drawMedHisto(ui->listeMed->currentText().remove("MED-").section(" ", 0,0).toInt(), ui->yearPrest->currentText().toInt());
}
void prestDialog::populateListeMed()
{
    int nb;
    Medecin *med=getAllMedecin(&nb);
    for (int i=0;i<nb;i++) {
        ui->listeMed->addItem("MED-"+QString::number(med[i].id)+" "+QString(med[i].nom)+" "+QString(med[i].prenom));
    }
}
void prestDialog::populateYear()
{
    int nb;
    Traitement *all=getAllTraitement(&nb);
    ui->yearPrest->clear();
    for (int i=0;i<nb;i++) {
        if(allYear.contains(all[i].dateConsu.year)) continue;
        else{
            allYear.append(all[i].dateConsu.year);
            ui->yearPrest->addItem(QString::number(all[i].dateConsu.year));
        }
    }
    int max=allYear[0];
    for (int i=0;i<allYear.length();i++) {
        if(allYear[i]>max) max=allYear[i];
    }
    ui->yearPrest->setCurrentText(QString::number(QDate::currentDate().year()));
}
void prestDialog::displayTable(int idMed, int year)
{
    int nb;
    Traitement *trait=getMedTraitement(idMed, &nb);
    QDate mois(year, 1, 1);
    int total=0;
//    for (int i=0;i<nb;i++) total=+trait[i].taux*trait[i].nbJours;
    for (int i=0;i<12;i++) {
        ui->prestTable->setItem(i, 0, new QTableWidgetItem(QString::number(getPatNb(idMed, i, year))));
        ui->prestTable->setItem(i, 1, new QTableWidgetItem(QString::number(getPrestMounth(idMed, i, year))));
        total+=getPrestMounth(idMed, i, year);
    }
    ui->totalPrest->setText(QString::number(total)+" Ar");

}



void prestDialog::on_listeMed_activated(const QString &arg1)
{
    QString id=arg1;
    displayTable(id.remove("MED-").section(" ", 0,0).toInt(), ui->yearPrest->currentText().toInt());
    updateMedHisto(id.remove("MED-").section(" ", 0,0).toInt(), ui->yearPrest->currentText().toInt());
}


void prestDialog::on_yearPrest_activated(const QString &arg1)
{
    int year=arg1.toInt();
    displayTable(ui->listeMed->currentText().remove("MED-").section(" ", 0,0).toInt(), year);
    updateMedHisto(ui->listeMed->currentText().remove("MED-").section(" ", 0,0).toInt(), year);
}
void prestDialog::drawMedHisto(int idMed, int year)
{
    series=new  QBarSeries();
    QBarSet *set;
    QDate m(year, 1, 1);
    QVector<QBarSet*> mounth;
    for (int i=0;i<12;i++) {
        m.setDate(year, i+1, 1);
        set=new QBarSet(m.toString("MMM"));
        mounth.append(set);
        mounth[i]->append(getPrestMounth(idMed, i+1, year));
        series->append(mounth[i]);
    }
    QChart *histo=new QChart();
    histo->addSeries(series);
    Medecin med=getMedecinById(idMed);
    histo->setTitle("Etat des prestations Du <b>dr "+QString(med.prenom)+" "+QString(med.nom)+"</b> durant l'année "+QString::number(year));
    histo->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append("Mois");
    histo->createDefaultAxes();
    histo->setAxisX(axis, series);
    chartView=new QChartView(histo);
    chartView->setParent(ui->allPrestHisto);
    chartView->setMaximumSize(700, 400);
    chartView->chart()->legend()->setAlignment(Qt::AlignLeft);
}
void prestDialog::updateMedHisto(int idMed, int year)
{
    chartView->repaint();
//    series->clear();
    series=new QBarSeries();
    QBarSet *set;
    QDate m(year, 1, 1);
    QVector<QBarSet*> mounth;
    for (int i=0;i<12;i++) {
        m.setDate(year, i+1, 1);
        set=new QBarSet(m.toString("MMM"));
        qDebug()<<set;
        mounth.append(set);
        mounth[i]->append(getPrestMounth(idMed, i+1, year));
        series->append(mounth[i]);
    }
    QChart *histo=new QChart();
    histo->addSeries(series);
    Medecin med=getMedecinById(idMed);
    histo->setTitle("Etat des prestations Du <b>dr "+QString(med.prenom)+" "+QString(med.nom)+"</b> durant l'année "+QString::number(year));
    histo->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append("Mois");
    histo->createDefaultAxes();
    histo->setAxisX(axis, series);
    chartView->setChart(histo);
    chartView->setParent(ui->allPrestHisto);
    chartView->setMaximumSize(700, 400);
    chartView->chart()->legend()->setAlignment(Qt::AlignLeft);
}


void prestDialog::on_cancelBtn_clicked()
{
    this->close();
}

void prestDialog::on_printPrest_clicked()
{
    QString pdfName=QFileDialog::getSaveFileName(this, "Enregistrer le pdf sous...", "/home/sans_titre.pdf", "Pdf (*.pdf)");
     if(pdfName=="") return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::NativeFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setOutputFileName(pdfName);

    QTextDocument doc;
    QString title, effectif, headerColor;
    title="Etat des prestations pour l'année "+ui->yearPrest->currentText()+". <br>Medecin: "+ui->listeMed->currentText();
    effectif="Total: "+ui->totalPrest->text();
    headerColor="rgba(248, 160, 8, 255)";
    QString text("<html><head/><body><p><span style=\" font-size:11pt; font-weight:600; color:#0d6f7a; border: 1px solid red;\">"+title+"</span></p></body></html>");
    text.append("<html><head/><body><p><span style=\" font-size:9pt; font-weight:600; color:#03e185;\">"+effectif+"</span></p></body></html>");
    text.append("<table style:\"color: lightGrey; font-size: 10pt;\"><thead>");
    text.append("<tr style=\"border:1px solid black; background-color: "+headerColor+";color: white; height: 40px;\">");
    text.append("<th>&nbsp;&nbsp;MOIS&nbsp;&nbsp;</th>");
    text.append("<th>&nbsp;&nbsp;PATIENTS&nbsp;&nbsp;</th>");
    text.append("<th>&nbsp;&nbsp;PRESTATION&nbsp;&nbsp;</th>");
    text.append("</tr></thead>");
    text.append("<tbody style=\"color: rgb(99, 99, 99);\">");
    QDate m;
    for (int i=0;i<12;i++) {
        if(i%2==1) text.append("<tr style=\"background-color: rgb(196, 230, 223);\">");
        else text.append("<tr>");
        m.setDate(2000, i+1, 1);
        text.append("<td>"+m.toString("MMMM")+"</td>");
        text.append("<td>"+ui->prestTable->item(i, 0)->text()+"</td>");
        text.append("<td>"+ui->prestTable->item(i, 1)->text()+"</td>");
        text.append("</tr>");
    }
    text.append("</tbody></table>");
    text.append("<p style=\"color:#03e185;\">Hitogramme</p>");
    QTextCursor cursor(&doc);
    cursor.insertHtml(text);

    QImage img(500, 300, QImage::Format_RGB32);
    QPainter painter(&img);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();
    chartView->render(&painter);

    cursor.insertImage(img);

    doc.setPageSize(printer.pageRect().size());
    doc.setDocumentMargin(20);
    doc.print(&printer);
    QMessageBox::information(this, this->windowTitle(), "Le pdf a bien ete enregistre sous: "+pdfName);
}















