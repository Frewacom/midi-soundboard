#include "volumeselectorwidget.h"
#include "ui_volumeselectorwidget.h"

VolumeSelectorWidget::VolumeSelectorWidget(QWidget *parent) :
    ModalContentWidget(parent),
    ui(new Ui::VolumeSelectorWidget)
{
    ui->setupUi(this);

    ui->VolumeSelectorList->addItem("test");
    ui->VolumeSelectorList->addItem("test2");
}

void VolumeSelectorWidget::on_VolumeSelectorClose_clicked() {
    emit this->closeModal();
}

VolumeSelectorWidget::~VolumeSelectorWidget()
{
    delete ui;
}
