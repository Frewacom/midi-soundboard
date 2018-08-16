#ifndef MODALWIDGET_H
#define MODALWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

class ModalWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModalWidget(QSize *size, QWidget *parent = nullptr);

    void setSize(QSize *size);
    void setContent(QWidget *widget);

signals:

public slots:
    void onResize(QSize *newSize);

private:
    QFrame *_root = nullptr;
    QVBoxLayout *_layout = nullptr;

};

#endif // MODALWIDGET_H
