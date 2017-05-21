#ifndef LOGER_H
#define LOGER_H

/*! \file loger.h
 *  \brief Système avancé de log
 *
 */

#include <QObject>
#include <QFile>
#include <QTextStream>
#include "uputils.h"
#include <algorithm>

#define endl "\n"

/*!
 * \brief Système avancé de log orienté performance
 *
 * Système de log A DEVELOPPER
 */

class Loger : public QObject {
    Q_OBJECT

    /*!
     * \brief Fichier Log
     */
    QFile* file;
    /*!
     * \brief Flux de donnés vers ce fichier
     */
    QTextStream* stream;

    /*!
     * \brief [TO BE DEPRECATED]Fichier log (version string)
     */
    QString logFile;

    bool lastEndl;
public:

    Loger(QString _file) : logFile(_file) {
        lastEndl=false;
        file = new QFile(logFile);
        file->open(QIODevice::WriteOnly);
        stream = new QTextStream(file);
    }

    ~Loger() {
        file->close();
    }

    /*!
     * \brief Change le fichier de log
     * \param newFile Nouveau fichier de log (QString)
     */
    void changeLogFile(const QString newFile) {
        logFile=newFile;
        file->close();
        file->setFileName(logFile);
        file->open(QIODevice::WriteOnly);
    }

    /*!
     *  \brief [MARCHE PAS] Ecris str dans le fichier de log
     */
    template<typename T>void operator<<(T& str) const {
        std::string ret = utils::toQString(str).toStdString();
        //std::replace(ret.begin(), ret.end(), std::string("\n"), std::string("\n[" __TIME__ "]"));
        //ret.replace(ret.begin(), ret.end(), std::string("\n"), std::string("\n[" __TIME__ "]"));
        //ret.replace(
        utils::replace(ret, "\n", "\n[" __TIME__ "]");

        file->write(ret.c_str());
        utils::boxInfo(QString::fromStdString(ret));
    }

};

#endif // LOGER_H
