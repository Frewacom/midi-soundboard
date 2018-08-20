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

void Helpers::EmptyLayout(QLayout *layout) {
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != 0) {
        delete child;
    }
}

QDir Helpers::GetApplicationPath() {
    return QCoreApplication::applicationDirPath();
}

bool Helpers::CheckIfFileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

QJsonDocument Helpers::GetFileJSONContents(QString path) {
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    QJsonDocument json = QJsonDocument::fromJson(val.toUtf8());
    file.close();

    return json;
}
