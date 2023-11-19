#include "development.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>

Logger::Logger(const QString& logDir, int maxFiles)
    : logDirectory(logDir), maxLogFiles(maxFiles) {
    // Eğer log dizini yoksa, oluştur
    QDir dir(logDirectory);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

void Logger::log(const QString& message) {
    QDateTime currentTime = QDateTime::currentDateTimeUtc();
    QString fileName = logDirectory + "/" + currentTime.toString("yyyyMMddHHmmss") + ".log";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream out(&file);
        out << currentTime.toString("yyyy-MM-dd HH:mm:ss") << " - " << message << "\n";
        file.close();
    }
    manageLogFiles();
}


void Logger::manageLogFiles() {
    QDir dir(logDirectory);
    dir.setNameFilters(QStringList() << "*.log");
    dir.setSorting(QDir::Time);

    QStringList files = dir.entryList();
    while (files.size() > maxLogFiles) {
        dir.remove(files.last());
        files.removeLast();
    }
}
