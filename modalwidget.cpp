#include "modalwidget.h"

ModalWidget::ModalWidget(
        QSize *size,
        ModalContentWidget *content,
        QWidget *parent
) : QWidget(parent)
{
    this->_root = new QFrame(this);
    this->_layout = new QVBoxLayout();

    this->resize(0,0);
    this->_root->setAccessibleName("modal-root");
    this->_root->setLayout(this->_layout);

    this->_currentSize = size;
    this->_layout->addWidget(content, 1, Qt::AlignHCenter | Qt::AlignVCenter);
    connect(content, SIGNAL(closeModal()), this, SLOT(onClose()));
}

void ModalWidget::open() {
    this->_isOpen = true;
    this->setSize(this->_currentSize);
}

void ModalWidget::setSize(QSize *size) {
    if (this->_isOpen) {
        this->resize(size->width(), size->height());
        this->_root->resize(size->width(), size->height());
        this->_root->show();
    }
}

void ModalWidget::onClose() {
    this->setSize(new QSize(0,0));
    this->_isOpen = false;
}

void ModalWidget::onResize(QSize *newSize) {
    this->_currentSize = newSize;
    this->setSize(newSize);
}
