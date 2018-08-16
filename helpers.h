#ifndef HELPERS_H
#define HELPERS_H

#include <QWidget>
#include <QColor>
#include <QFile>
#include <QGraphicsDropShadowEffect>

namespace Helpers {
    void ApplyBoxShadow(QWidget *widget);
    void SetStyleSheet(QWidget *widget, QString name);
}

#endif // HELPERS_H
