#ifndef VOLUMESELECTORWIDGET_H
#define VOLUMESELECTORWIDGET_H

#include <QWidget>
#include "helpers.h"

namespace Ui {
class VolumeSelectorWidget;
}

class VolumeSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VolumeSelectorWidget(QWidget *parent = 0);
    ~VolumeSelectorWidget();

private:
    Ui::VolumeSelectorWidget *ui;
};

#endif // VOLUMESELECTORWIDGET_H
