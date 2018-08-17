#include "volumeselectoritem.h"
#include "ui_volumeselectoritem.h"

VolumeSelectorItem::VolumeSelectorItem(
        QString name,
        int volume,
        QWidget *parent
) :
    QWidget(parent),
    ui(new Ui::VolumeSelectorItem)
{
    ui->setupUi(this);

    ui->AudioDevice->setText(name);
    ui->VolumeLabel->setText(QString::number(volume));
    ui->VolumeSlider->setValue(volume);
}

void VolumeSelectorItem::on_VolumeSlider_valueChanged(int value) {
    ui->VolumeLabel->setText(QString::number(value));
}

VolumeSelectorItem::~VolumeSelectorItem()
{
    delete ui;
}
