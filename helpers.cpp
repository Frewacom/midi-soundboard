#include "helpers.h"

void Helpers::ApplyBoxShadow(QWidget *widget)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(20);
    effect->setXOffset(1);
    effect->setYOffset(1);
    effect->setColor(QColor::fromRgb(100,100,100));

    widget->setGraphicsEffect(effect);
}
