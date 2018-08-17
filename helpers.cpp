#include "helpers.h"

void Helpers::ApplyBoxShadow(QWidget *widget)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(20);
    effect->setXOffset(1);
    effect->setYOffset(1);
    effect->setColor(QColor::fromRgb(20,20,20));

    widget->setGraphicsEffect(effect);
}

void Helpers::SetStyleSheet(QWidget *widget, QString url) {
    QFile file(url);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    widget->setStyleSheet(styleSheet);
}
