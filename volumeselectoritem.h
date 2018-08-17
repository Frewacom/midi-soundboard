#ifndef VOLUMESELECTORITEM_H
#define VOLUMESELECTORITEM_H

#include <QWidget>

namespace Ui {
class VolumeSelectorItem;
}

class VolumeSelectorItem : public QWidget
{
    Q_OBJECT

public:
    explicit VolumeSelectorItem(
        QString name,
        int volume,
        QWidget *parent = 0
    );
    ~VolumeSelectorItem();

private slots:
    void on_VolumeSlider_valueChanged(int value);

private:
    Ui::VolumeSelectorItem *ui;
};

#endif // VOLUMESELECTORITEM_H
