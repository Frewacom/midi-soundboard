#include "volumeselectorwidget.h"
#include "ui_volumeselectorwidget.h"

VolumeSelectorWidget::VolumeSelectorWidget(QWidget *parent) :
    ModalContentWidget(parent),
    ui(new Ui::VolumeSelectorWidget)
{
    ui->setupUi(this);

    Helpers::ApplyBoxShadow(ui->VolumeSelectorFrame);
}

void VolumeSelectorWidget::SetDevices(std::vector<AudioEngine> *devices) {
    QLayoutItem *child;
    while ((child = ui->AudioDeviceListLayout->takeAt(0)) != 0) {
        ui->AudioDeviceListLayout->layout()->removeItem(child);
        delete child->widget();
    }

    ui->AudioDeviceListLayout->addItem(
        new QSpacerItem(
            0, 50, QSizePolicy::Expanding, QSizePolicy::Expanding
        )
    );

    if (devices->size() == 0) {
        QLabel *label = new QLabel("No output devices selected");
        label->setAccessibleName("no-audio-devices");
        ui->AudioDeviceListLayout->insertWidget(0, label);
    } else {
        for (int i = 0; i < devices->size(); i++) {
            VolumeSelectorItem *item = new VolumeSelectorItem(
                devices->at(i).Data->Name,
                (int)devices->at(i).Engine->getSoundVolume() * 100
            );

            connect(
                item, SIGNAL(volumeChanged(int, QString)),
                this, SLOT(on_Volume_changed(int, QString))
            );

            ui->AudioDeviceListLayout->insertWidget(0, item);
        }
    }
}

void VolumeSelectorWidget::on_Volume_changed(int value, QString name) {
    emit this->volumeChanged(value, name);
}

void VolumeSelectorWidget::on_VolumeSelectorClose_clicked() {
    emit this->closeModal();
}

VolumeSelectorWidget::~VolumeSelectorWidget()
{
    delete ui;
}
