#ifndef MODALWIDGET_H
#define MODALWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include "modalcontentwidget.h"

class ModalWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModalWidget(
        QSize *size,
        ModalContentWidget *content,
        QWidget *parent = nullptr
    );

    void setSize(QSize *size);
    void open();

signals:

public slots:
    void onResize(QSize *newSize);
    void onClose();

private:
    QFrame *_root = nullptr;
    QVBoxLayout *_layout = nullptr;
    QSize *_currentSize = nullptr;

    bool _isOpen = false;

};

#endif // MODALWIDGET_H
