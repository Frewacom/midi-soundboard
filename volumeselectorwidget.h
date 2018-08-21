#ifndef VOLUMESELECTORWIDGET_H
#define VOLUMESELECTORWIDGET_H

#include <QWidget>
#include <QMap>
#include <QSpacerItem>
#include "audiowrapper.h"
#include "helpers.h"
#include "volumeselectoritem.h"
#include "modalcontentwidget.h"

namespace Ui {
class VolumeSelectorWidget;
}

class VolumeSelectorWidget : public ModalContentWidget
{
    Q_OBJECT

public:
    explicit VolumeSelectorWidget(QWidget *parent = 0);
    ~VolumeSelectorWidget();

    void SetDevices(std::vector<AudioEngine> *devices);

signals:
    void volumeChanged(int value, QString name);

private slots:
    void on_VolumeSelectorClose_clicked();
    void on_Volume_changed(int value, QString name);

private:
    Ui::VolumeSelectorWidget *ui;

};

#endif // VOLUMESELECTORWIDGET_H
