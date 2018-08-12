#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include <QErrorMessage>

namespace Logger {
    void DisplayError(QString error);
    void Debug(QString error);
}

#endif // LOGGER_H
