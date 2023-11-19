#ifndef DEVELOPMENT_H
#define DEVELOPMENT_H

#include <QString>
#include <QDir>

class Logger {
public:
    Logger(const QString& logDir, int maxFiles);
    void log(const QString& message);

private:
    QString logDirectory;
    int maxLogFiles;

    void manageLogFiles();
};

#endif
