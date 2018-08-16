#include "volumeselectorwidget.h"
#include "ui_volumeselectorwidget.h"

VolumeSelectorWidget::VolumeSelectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VolumeSelectorWidget)
{
    ui->setupUi(this);

    ui->VolumeSelectorList->addItem("test");
    ui->VolumeSelectorList->addItem("test2");
}

VolumeSelectorWidget::~VolumeSelectorWidget()
{
    delete ui;
}
