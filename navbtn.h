#ifndef NAVBTN_H
#define NAVBTN_H

#include <QWidget>
#include <QPushButton>
#include <QString>

class NavBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit NavBtn(int position=0, QWidget *parent = nullptr);
    void setActive(bool a);
    void setPosition(int y);

signals:

public slots:
private:
    bool _active=false;
    int _height;
    QString _baseCss;
    QString _activeCss;
    QString _hoverCss;
};

#endif // NAVBTN_H
