#include "modalwidget.h"

ModalWidget::ModalWidget(QSize *size, QWidget *parent) : QWidget(parent)
{
    this->_root = new QFrame(this);
    this->_layout = new QVBoxLayout();

    this->setSize(size);
    this->_root->setAccessibleName("modal-root");

    this->_root->setLayout(this->_layout);
    this->_root->show();
}

void ModalWidget::setSize(QSize *size) {
    this->resize(size->width(), size->height());
    this->_root->resize(size->width(), size->height());
    this->_root->show();
}

void ModalWidget::setContent(QWidget *widget) {
    this->_layout->addWidget(widget, 1, Qt::AlignHCenter | Qt::AlignVCenter);
}

void ModalWidget::onResize(QSize *newSize) {
    this->setSize(newSize);
}
