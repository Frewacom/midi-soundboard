#ifndef MODALCONTENTWIDGET_H
#define MODALCONTENTWIDGET_H

#include <QWidget>
#include <QDebug>

class ModalContentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModalContentWidget(QWidget *parent = nullptr);

signals:
    void closeModal();

public slots:
};

#endif // MODALCONTENTWIDGET_H
