#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap rect(":/img/img/splash2.jpg");
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(rect.scaled(1000, 500, Qt::KeepAspectRatio));
    splash->show();



    MainWindow w;
    QTimer::singleShot(5000, splash, SLOT(close));
    QTimer::singleShot(4000, &w, SLOT(show()));
//    w.show();

    return a.exec();
}
