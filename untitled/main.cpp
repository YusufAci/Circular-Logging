#include <QCoreApplication>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "development.h"

QJsonObject readConfig(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open config file:" << fileName << "Error:" << file.errorString();
        return QJsonObject();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc(QJsonDocument::fromJson(data));
    if (doc.isNull()) {
        qWarning() << "Error parsing JSON from config file:" << fileName;
        return QJsonObject();
    }

    return doc.object();
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    qDebug() << "Current working directory:" << QDir::currentPath();

    QString configFilePath = QDir::currentPath() + "/config.json";
    QJsonObject config = readConfig(configFilePath);

    if (config.isEmpty()) {
        qWarning() << "Configuration is empty. Exiting...";
        return 1;
    }

    // Logger nesnesini oluştur
    QString logDirPath = QDir::currentPath() + "/logs";
    int maxLogFiles = config["maxLogFiles"].toInt(); // Config'den max log dosyası sayısını alma
    Logger logger(logDirPath, maxLogFiles);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&logger]() {
        logger.log("Log mesajı");
    });

    int logFrequencySeconds = config.contains("logFrequencySeconds") ? config["logFrequencySeconds"].toInt() : 3; //Varsayılan olarak 3 saniye ayarı
    timer.start(logFrequencySeconds * 1000);
    return app.exec();
}

