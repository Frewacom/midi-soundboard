#include "volumeselectorwidget.h"
#include "ui_volumeselectorwidget.h"

VolumeSelectorWidget::VolumeSelectorWidget(QWidget *parent) :
    ModalContentWidget(parent),
    ui(new Ui::VolumeSelectorWidget)
{
    ui->setupUi(this);

    ui->AudioDeviceListLayout->insertWidget(
        0,
        new VolumeSelectorItem(
            "Högtalare (Line 6 UX2)",
            50
        )
    );
    ui->AudioDeviceListLayout->insertWidget(
        0,
        new VolumeSelectorItem(
            "Steam Link Input (Steam streaming)",
            100
        )
    );
}

void VolumeSelectorWidget::on_VolumeSelectorClose_clicked() {
    emit this->closeModal();
}

VolumeSelectorWidget::~VolumeSelectorWidget()
{
    delete ui;
}
