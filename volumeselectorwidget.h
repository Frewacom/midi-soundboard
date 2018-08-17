#ifndef VOLUMESELECTORWIDGET_H
#define VOLUMESELECTORWIDGET_H

#include <QWidget>
#include "helpers.h"
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

private slots:
    void on_VolumeSelectorClose_clicked();

private:
    Ui::VolumeSelectorWidget *ui;
};

#endif // VOLUMESELECTORWIDGET_H
