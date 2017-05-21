#ifndef WEBREADER_H
#define WEBREADER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>


class webReader : public QObject {
    Q_OBJECT
    QNetworkAccessManager* manager;

public:
    QString strdata;

    webReader() {
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    }

    void read(QString strUrl) {
        manager->get(QNetworkRequest(QUrl(strUrl)));
    }

public slots:
    void replyFinished(QNetworkReply* reply) {
        QByteArray data=reply->readAll();
        strdata = QString(data);
        emit finished(strdata);
    }

signals:
    void finished(QString data);
};

#endif // WEBREADER_H
