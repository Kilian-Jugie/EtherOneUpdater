#ifndef KPROCESS_H
#define KPROCESS_H

/*!
 * \file kprocess.h
 * \brief Processus de téléchargement
 *
 */

#include <QtCore>
#include <QtGui>
#include <QIODevice>
#include <QFileSystemWatcher>
#include <QTemporaryFile>
#include "dlwindow.h"
#include "uputils.h"

/*!
 * \brief Class temporaire, sera retirée dans une prochaine version
 */
class kUtils {
public:
    static void wait4ms(const int ms) {
        QTime die = QTime::currentTime().addMSecs(ms);
        while(QTime::currentTime()<die)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

};

/*!
 * \brief Protocol de téléchargement
 *
 * Utilise une ligne de commande pour télécharger des fichiers et récupère l'output généré par wget
 */
class kProcess : public QProcess {
    Q_OBJECT
public:
    /*!
     * \brief Fenêtre pour montrer l'output
     */
    dlWindow* dlwin;
    kProcess(dlWindow* downloadWindow) : QProcess() {
        dlwin = new dlWindow(*downloadWindow);
        _MyBase();
    }

    kProcess() : QProcess() {
        dlwin = new dlWindow();
        _MyBase();
    }

    /*!
     * \brief Fonction appelée par TOUS les constructeurs pour setup les connections
     */
    void _MyBase() {
        connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdOutput()));
        connect(this, SIGNAL(error(QProcess::ProcessError)), this, SLOT(readError()));
        connect(this, SIGNAL(finished(int)), this, SLOT(finishProc(int)));
        connect(dlwin->b_Cancel, SIGNAL(clicked(bool)), this, SLOT(cancelDL()));
    }

public slots:
    /*!
     * \brief Affiche la fenêtre
     */
    void showWin() {
        dlwin->show();
    }

    /*!
     * \brief Cache la fanêtre
     */
    void hideWin() {
        dlwin->hide();
    }

    /*!
     * \brief Annule la commande
     */
    void cancelDL() {
        this->close();
        dlwin->hide();
    }

    /*!
     * \brief La commande a terminée
     * \param code retournée par la ligne de commande
     */
    void finishProc(int code) {
        dlwin->te_Output->append("FINISHED DL ! Code: "+QString::number(code));
        dlwin->hide();
    }

private slots:

    /*!
     * \brief Lire l'output de la ligne de commande
     */
    void readStdOutput() {
        dlwin->te_Output->append(readAllStandardOutput());
        kUtils::wait4ms(200);

    }

    /*!
     * \brief Lire correctement une erreur
     */
    void readError() {
        dlwin->te_Output->append("Erreur pendant le téléchargement: "+QString::number(error()));
    }

};


//#define FILE_TEMP_OUT_PATH "out.txt"
/*
class kProcess : public QProcess {
    dlWindow* dlwin;
    QFileSystemWatcher* fileWatcher;
    QTemporaryFile* fileTemp;
    Q_OBJECT
public:
    kProcess() : QProcess() {
        dlwin = new dlWindow();
        fileTemp = new QTemporaryFile(FILE_TEMP_OUT_PATH);
        fileWatcher = new QFileSystemWatcher({FILE_TEMP_OUT_PATH});
        fileTemp->setAutoRemove(false); //TO TRUE
        _MyBase();
    }

    ~kProcess() {
        fileTemp->remove(); //TO REMOVE
    }

    void _MyBase() {
        connect(fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(updateTE(QString)));
        connect(dlwin->b_Cancel, SIGNAL(clicked(bool)), this, SLOT(terminate()));
        connect(dlwin->b_Cancel, SIGNAL(clicked(bool)), this, SLOT(hideWin()));

    }


public slots:

    void showWin() {
        dlwin->show();
    }

    void hideWin() {
        dlwin->hide();
    }

    void updateTE(QString str) {
        dlwin->te_Output->append(str);
    }

};

*/

#endif // KPROCESS_H
