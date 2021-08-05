#include "navbtn.h"
#include <QPushButton>
NavBtn::NavBtn(int position, QWidget *parent) : QPushButton(parent)
{
    this->setParent(parent);
    _baseCss="NavBtn{"
             "background-color: none;"
             "width: 80px;"
             "height: 80px;"
             "border: none;"
             "}";
    _activeCss="NavBtn{"
               "background-color: rgb(3, 225, 133);"
               "border-left: 2px solid white;"
               "}";
    _hoverCss="NavBtn:hover{"
              "background-color: rgba(3, 225, 132, 0.534);"
              "}";
    _height=80;
    this->setStyleSheet(_baseCss+_hoverCss);
    this->setFixedSize(80, _height);
    this->setPosition(position);
    this->setIconSize(QSize(70, 70));
}
void NavBtn::setActive(bool a)
{
    _active=a;
    if(_active){
        this->setStyleSheet(_baseCss+_activeCss);
    }
    else {
        this->setStyleSheet(_baseCss+_hoverCss);
    }
}
void NavBtn::setPosition(int y)
{
    this->move(0, y*_height + 24);
}
