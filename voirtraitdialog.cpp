#include "voirtraitdialog.h"
#include "ui_voirtraitdialog.h"

VoirTraitDialog::VoirTraitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoirTraitDialog)
{
    ui->setupUi(this);
}

VoirTraitDialog::~VoirTraitDialog()
{
    delete ui;
}
