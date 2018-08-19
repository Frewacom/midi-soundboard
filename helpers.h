#ifndef HELPERS_H
#define HELPERS_H

#include <QWidget>
#include <QDir>
#include <QColor>
#include <QFile>
#include <QLayout>
#include <QLayoutItem>
#include <QCoreApplication>
#include <QGraphicsDropShadowEffect>

namespace Helpers {
    void ApplyBoxShadow(QWidget *widget);
    void SetStyleSheet(QWidget *widget, QString name);
    void EmptyLayout(QLayout *layout);
    QDir GetApplicationPath();
}

#endif // HELPERS_H
