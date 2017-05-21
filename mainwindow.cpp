/*! \file mainwindow.cpp
 *  \brief Implémentation de mainWindow.h
 *  \see mainWindow.h
 *
 */

#include "mainwindow.h"
#include "uputils.h"
#include "webreader.h"
#include <QDir>
#include <QCoreApplication>
#include <QTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QException>
#include <QtCore>
#include <QFileDialog>
#include <QInputDialog>

/*
void mainWindow::readStdOutput() {
    dlwindow->te_Output->append(extrn->readAllStandardOutput());
}

void mainWindow::readError() {
    dlwindow->te_Output->append(extrn->readAllStandardError());
}*/

void mainWindow::changeData(const QString data, const QString value) {
    dataManager->writeData(data, value);
}

void mainWindow::getData() {
    etVersion=dataManager->getStringData("McVersion");
    etPath=dataManager->getStringData("McPath");
    updaterVersion=dataManager->getStringData("UpVersion");
    l_etVersion->setText(etVersion);
}

void mainWindow::getEtPath() {

}
/*
void mainWindow::__READED(QString data) {
    msgInfo(data);
}*/

void mainWindow::startStatus() {
    downloadFile("https://mcapi.us/server/status?ip=mc50.boxtoplay.com&port=27440", FOLDER_UP_DATA"\\SERVER_STATUS.stt", false);

}

mainWindow::mainWindow(QWidget *parent)
    : QWidget(parent) {
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    //pingWindow pw({"google.fr"}); //0
    //pingWindow pw2({"http://godwnjgwsrgiokndrwglk.fr/"}); //1
    //pingWindow pw3({"mc50.boxtoplay.com"}); //0 on

    /*webReader* wr = new webReader();
    wr->read("https://mcapi.us/server/status?ip=mc50.boxtoplay.com&port=27440");
    //connect(wr, SIGNAL(finished(QString)), this, SLOT(__READED(QString))); */

    //getFtp();

    DontPrompUpdateErrors=false;

    //dlwindow = new dlWindow();
    dataManager=new DataManager(FILE_DATA);

    extrn = new kProcess();

    if(!checkUpdaterFiles())
        setUpUpdater();

    getData();

    opWin->addtoModWindow("testmod_1.23.5.jar", false);

    if(etPath=="NONE") {
        msgInfo("Entrez le chemin d'accès vers le dossier où est installé EtherOne");
        etPath=QFileDialog::getExistingDirectory(this, tr("Entrez le chemin d'accés d'EtherOne"), "/home", QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
        changeData("McPath", etPath);
    }

    if(etVersion=="4.0.0") {
        QMessageBox msg("Question", "Votre version du modpack est-elle la 4.0.0 ?", QMessageBox::Question, QMessageBox::Yes, QMessageBox::No, 0);
        switch(msg.exec()) {
        case QMessageBox::Yes :{
            msgInfo("Pensez à faire la dernière mise à jour !");
        }
            break;
        case QMessageBox::No : {
            QString text = QInputDialog::getText(this, tr("Version"), tr("Entrez votre version du modpack:"), QLineEdit::Normal, tr("4.1.5"));
            if(text.isEmpty()) {
                msgCritical("Vous devez entrer une version, redémarrez l'updater");
                text="4.1.5";
            }
            changeData("McVersion", text);
            getData();
            break;
        }
        }
    }

    l_img->setPixmap(*pm_BackGround);

    //Mise en place des différents éléments grahpiques
    layout->addWidget(b_Exit, 20, 15, 2, 1, Qt::AlignRight|Qt::AlignBottom);
    layout->addWidget(b_Install, 20, 1, 2, 1, Qt::AlignLeft|Qt::AlignBottom);
    layout->addWidget(b_Options, 20, 13, 2, 1, Qt::AlignBottom);
    layout->addWidget(l_img, 1, 1, 15, 15);
    layout->addWidget(l_Credits, 20, 3, 2, 1, Qt::AlignBottom);
    layout->addWidget(l_McVersion, 20, 6, 2, 1, Qt::AlignBottom);
    layout->addWidget(l_etVersion, 20, 7, 2, 1, Qt::AlignBottom);

    extrn->setProcessChannelMode(QProcess::MergedChannels);

    setLayout(layout);
    connect(quietMode, SIGNAL(activated()), this, SLOT(onQuietMode()));
    connect(b_Exit, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(b_Install, SIGNAL(clicked(bool)), this, SLOT(startModpackUpdate()));
    connect(b_Options, SIGNAL(clicked(bool)), opWin, SLOT(show()));

    /* USING QT-INTEGRATED (NEARLY) DOWNLOAD SYSTEM (MAYBE FUCKING LATER)

    DownloadManager manager;
    manager.append(QStringList({"http://etherone.web.boxtoplay.com/Updater/ResonantRise.7zip"}));
    QObject::connect(&manager, SIGNAL(finished()), this, SLOT(close()));

    */

    //downloadFile("http://etherone.web.boxtoplay.com/Updater/ResonantRise.7zip", "Test1.rar", false);
    checkForUpdates(true);

    /*END TEST PART*/
    //startDl();
    //downloadFile();
    this->show();
    setFocus();
}

/*
void mainWindow::startDl() {
    for(QList<QPair<QString, QString>>::iterator it(dl_Files.begin()); it!=dl_Files.end(); ++it) {
        downloadFile((*it).first, (*it).second, false);
        while(ftp->hasPendingCommands()) {}
            //delayms(2000);
    }
}*/

void mainWindow::onQuietMode() {
    DontPrompUpdateErrors=true;
    msgInfo("QUIET MODE SUCCESSFUL ACTIVATED !");
}

void mainWindow::startModpackUpdate() {
    checkForUpdates(false);
}

void mainWindow::versionParser(bool isUpdater) {
    if(isUpdater) {
        if(!QFile().exists(DL_UPDATER_VERSIONS)) {
            msgCritical("ERROR: Le fichier de version de l'updater n'a pas pu être téléchargé !");
            return;
        }

        QFile updte(DL_UPDATER_VERSIONS);
        if(updte.open(QFile::ReadOnly)) {
            QTextStream str(&updte);
            QString ret, nVer;
            bool isRecognizedVersion(false);
            while(str.readLineInto(&ret)) {
                if(ret==updaterVersion)
                    isRecognizedVersion=true;
                nVer=ret;
            }
            if(!isRecognizedVersion) {
                msgCritical("ERROR: La version de l'updater n'est pas reconnue ! Réinstallez l'updater pour résoudre ce problème.");
                return; }
            if(nVer==updaterVersion)
                return;
            else {
                //msgInfo(nVer);
                QMessageBox msg;
                msg.setText("Une mise à jour de l'updater est disponible :"+nVer);
                msg.setInformativeText("La faire maintenant ?");
                msg.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
                msg.setDefaultButton(QMessageBox::Yes);
                switch(msg.exec()) {
                    case QMessageBox::No:
                        return;
                    break;
                    case QMessageBox::Yes:
                        downloadFile(DB_FOLDER_BASE DB_FOLDER_VERSIONS +nVer+".7z", "../Update.7z", false);
                        installUpUpdate();
                        msgWarning("Exécutez UPINSTALLER.bat situé dans le dossier ou est situé l'updater");
                        exit(0);
                        QCoreApplication::quit();
                        //QProcess::startDetached("..\\UPINSTALLER.bat > log.txt");
                        //QCoreApplication::quit();
                        //exit(0);
                    break;
                }
            }
        }
    }
    getData();
    QFile ver(DL_VERSIONS);
    if(!ver.open(QFile::ReadOnly)) {
        msgCritical("ERROR: Imposible d'ouvrir le fichier de version du modpack");
        return;
    }
    QTextStream qts(&ver);
    QStringList vers;
    QString ret;
    QString nVer;
    bool isRecognizedVersion(false);
    while(qts.readLineInto(&ret)) {
        if(isRecognizedVersion)
            vers.push_back(ret);
        if(etVersion==ret)
            isRecognizedVersion=true;
        nVer=ret;
    }

    if(!isRecognizedVersion) {
        msgCritical("ERROR: La version actuel du modpack n'existe pas !");
        return;
    }
    if(nVer==etVersion) {
        msgInfo("Le modpack est à jour !");
        return;
    }
    if(vers.size()>1)
        msgInfo("Plusieurs versions successives seront installés...");

    bool sucess(true);
    for(QStringList::Iterator it(vers.begin()); it!=vers.end(); ++it) {
        sucess=installationParser(*it);
        if(sucess==false)
            break;
    }
    if(sucess==false) {
        return;
    }

    changeData("McVersion", nVer);

    if(vers.size()>1) {
        msgInfo("Toutes les mises à jour ont été effectuées avec succés !");
        return;
    }

    msgInfo("La mise à jour a été faite avec succés !");

}

QStringList mainWindow::parseString(QString toParse) {
    QTextStream ss(&toParse, QIODevice::ReadOnly);
    //QString ret;
    QStringList fl;
    fl=toParse.split(" ");
    if(fl.size()>3)
        for(int i(3); i<fl.size(); ++i)
            fl[2]+=" "+fl.at(i);
    if(fl.size()<3) {
        msgFatal("FL [SPLITTER] SIZE < 3");
    }
    return fl;
}

bool mainWindow::installationParser(QString version) {
    downloadFile(DB_FOLDER_BASE DB_FOLDER_DATA+version+".7z", FOLDER_TEMP"/ETUpdate.7z", false);
    QProcess unzip;
    unzip.start("7z x " FOLDER_TEMP "\\ETUpdate.7z -aoa -o" FOLDER_TEMP "\\");
    unzip.waitForFinished();

    QFile instr(FOLDER_TEMP"\\Update\\install.etu");
    if(!instr.open(QIODevice::ReadOnly)) {
        msgCritical("ERROR: Impossible d'ouvrir le fichier d'instruction ("+instr.errorString()+")");
        return false;
    }
    QTextStream ts(&instr);
    QString ret;
    QStringList list;
    QFile errGetter;
    while(ts.readLineInto(&ret)) {
        bool fileError(false);
        bool dirError(false);
        bool scriptError(false);
        list=parseString(ret);
        if(list.at(0)=="mods") {
            if(list.at(1)=="delete") {
                if(!errGetter.remove(etPath+"\\mods\\"+list.at(2)))
                    fileError=true;
            }
            else if(list.at(1)=="copy") {
                if(!errGetter.copy(FOLDER_TEMP"\\Update\\"+list.at(2), etPath+"\\mods\\"+list.at(2)))
                    fileError=true;
            }
            else {
#ifndef DONT_PROMP_UPDATE_ERRORS
                if(!DontPrompUpdateErrors)
                    msgCritical("ERROR: Script d'installation érroné lvl-1m ("+ret+")");
#endif
#ifndef IGNORE_AND_PROMP_UPDATE_ERRORS
                return false;
#endif
            }
        }
        else if(list.at(0)=="config") {
            if(list.at(1)=="delete") {
                if(!errGetter.remove(etPath+"\\config\\"+list.at(2)))
                    fileError=true;
            }
            else if(list.at(1)=="createdir") {
                if(!QDir(etPath+"\\config\\").mkdir(list.at(2)))
                    dirError=true;
            }
            else if(list.at(1)=="copy") {
                if(!errGetter.copy(FOLDER_TEMP"\\Update\\"+list.at(2), etPath+"\\config\\"+list.at(2)))
                    fileError=true;
            }
            else {
#ifndef DONT_PROMP_UPDATE_ERRORS
                if(!DontPrompUpdateErrors)
                    msgCritical("ERROR: Script d'installation érroné lvl-1c ("+ret+")");
#endif
#ifndef IGNORE_AND_PROMP_UPDATE_ERRORS
                return false;
#endif
            }
        }
        else if(list.at(0)=="scripts"||list.at(0)=="script") {
            if(list.at(1)=="delete") {
                if(!errGetter.remove(etPath+"\\scripts\\"+list.at(2)))
                    scriptError=true;
            }
            else if(list.at(1)=="copy") {
                if(!errGetter.copy(FOLDER_TEMP"\\Update\\"+list.at(2), etPath+"\\scripts\\"+list.at(2)))
                    scriptError=true;
            }
            else if(list.at(1)=="replace") {
                if(!errGetter.remove(etPath+"\\scripts\\"+list.at(2))||!errGetter.copy(FOLDER_TEMP"\\Update\\"+list.at(2), etPath+"\\scripts\\"+list.at(2)))
                    scriptError=true;
            }
            else {
#ifndef DONT_PROMP_UPDATE_ERRORS
                msgCritical("ERROR: Script d'installation érroné lvl-1s ("+ret+"), SKIPPING...");
#endif
            }
        }
        else if(list.at(0)=="if") {
            if(list.at(1)=="exists") {

            }
        }
        else {
#ifndef DONT_PROMP_UPDATE_ERRORS
            if(!DontPrompUpdateErrors)
                msgCritical("ERROR: Script d'installation érroné lvl-0u : "+list.at(0)+" into {"+ret+"}");
#endif
#ifndef IGNORE_AND_PROMP_UPDATE_ERRORS
            return false;
#endif
        }
        if(fileError==true) {
#ifndef DONT_PROMP_UPDATE_ERRORS
            if(!DontPrompUpdateErrors)
                msgCritical("ERROR: Erreur de fichier ("+ret+") -> "+errGetter.errorString());
#endif
#ifndef IGNORE_AND_PROMP_UPDATE_ERRORS
            return false;
#endif
        }
        if(dirError==true) {
#ifndef DONT_PROMP_UPDATE_ERRORS
            if(!DontPrompUpdateErrors)
                msgCritical("ERROR: Erreur durant la création d'un dossier ("+ret+")");
#endif
#ifndef IGNORE_AND_PROMP_UPDATE_ERRORS
            return false;
#endif
        }
        if(scriptError==true) {
#ifndef DONT_PROMP_UPDATE_ERRORS
            if(!DontPrompUpdateErrors)
                msgCritical("ERROR: Erreur durant le changement de script... Ignorée");
#endif
        }
    }



    QFile::remove(FOLDER_TEMP"/ETUpdate.7z");

    QDir dir(FOLDER_TEMP"/Update");
    dir.removeRecursively();
    unzip.kill();
    return true;
}

void mainWindow::checkForUpdates(bool isUpdater) {
    if(isUpdater) {
        downloadFile(DB_FILE_UPDATER_VERSIONS, DL_UPDATER_VERSIONS, false);
        versionParser(true);
        return;
    }
    downloadFile(DB_FILE_VERSION, DL_VERSIONS, false);
    versionParser(false);
}

mainWindow::~mainWindow()
{

}

bool mainWindow::checkUpdaterFiles() {
    for(QList<QString>::iterator it(m_installerFolders.begin()); it!=m_installerFolders.end(); ++it) {
        if(!QDir(*it).exists())
            return false;
    }
    for(QList<QString>::iterator it(m_installerFiles.begin()); it!=m_installerFiles.end(); ++it) {
        if(!QFile(*it).exists())
            return false;
    }
    return true;
}

void mainWindow::setUpUpdater() {
    for(QList<QString>::iterator it(m_installerFolders.begin()); it!=m_installerFolders.end(); ++it) {
        if(!QDir(*it).exists())
            QDir().mkdir(*it);
    }
    for(QList<QString>::iterator it(m_installerFiles.begin()); it!=m_installerFiles.end(); ++it) {
        if(!QFile(*it).exists()) {
            QFile temp(*it);
            temp.open(QIODevice::ReadWrite);
        }
    }
}

/*
void mainWindow::finishReply() {
    if(reply->readAll().size()>0) {
        QByteArray data=reply->readAll();
        reply->deleteLater();
        dl_file = new QFile(GetLastOf(dl_Files).second);
        dl_file->open(QFile::WriteOnly);
        dl_file->write(data);
        delete dl_file;
        dl_file=NULL;
    }
    else {
        msgCritical("NON-DETECTED ERROR DURING DOWNLOAD !");
    }
    dl_Files.erase(dl_Files.end(), dl_Files.end()-1);
    if(!dl_Files.empty()) {
        dlNext(); }
}

void mainWindow::networkError(QNetworkReply::NetworkError error) {
    msgCritical("Network error!");
}


void mainWindow::dlNext() {
    QNetworkAccessManager* nam = new QNetworkAccessManager();
    QUrl host("ftp://"FTP_HOST"/"FTP_UPBASEFILE+QString(GetLastOf(dl_Files).first));
    msgInfo(host.toString());
    host.setPassword(FTP_PASS);
    host.setUserName(FTP_USER);
    msgInfo("Before Login");
    QNetworkRequest req(host);
    msgInfo("Login Passed");
    msgInfo("Internal connections passed");
    reply=nam->get(req);
    msgInfo("Get passed");

}*/

void mainWindow::downloadFile(QString _file, QString newFile, bool useFTP=false) {
    /*//      CURL PROTOCOL
    QStringList arg;
    arg<<"curl -o "<<newFile<<" -u "<<FTP_USER<<":"<<FTP_PASS<<" ftp://"<<FTP_HOST<<":"<<QString::number(FTP_PORT)<<"/Updater/"<<_file;
    pr_Curl->setArguments(arg);*/

    /*//       QFTP PROTOCOL

    ftp->clearPendingCommands();

    if(warnings&&QFile::exists(_file)) {
        msgWarning("Le fichier en téléchargement va en remplacer un autre");
    }

    dl_file = new QFile(newFile);
    if(!dl_file->open(QIODevice::WriteOnly)) {
        msgCritical("Impossible d'écrire les nouveaux fichiers !");
        delete dl_file;
        return;
    }
    dlwindow->show();
    //dlwindow->setWindowTitle(_file);
    dlwindow->l_Name->setText(_file);
    ftp->get(_file, dl_file, QFtp::Binary);*/

    //USING WGET
    extrn->showWin();
    //msgInfo("Finished initializing");
    //msgInfo(EXTERN_DL_PROGRAM" "+_file+" --progress=bar:force:noscroll --output-document="+newFile);
    extrn->start(EXTERN_DL_PROGRAM" "+_file+" --progress=bar:force:noscroll --output-document="+newFile);

    if(!extrn->waitForStarted()) {
        msgCritical("Impossible de démarer le téléchargement [EXTERNDL1]");
    }

    if(!extrn->waitForFinished()) {
        msgCritical("Impossible de finir le téléchargement [EXTERNDL1]");
    }

}

void mainWindow::installUpUpdate() {
    QFile installer("../UPINSTALLER.bat");
    installer.open(QIODevice::WriteOnly);
    QTextStream stream_installer(&installer);
    stream_installer<<"echo LAUNCHING BLED UPDATER\n";
    stream_installer<<"echo %0\n";
    stream_installer<<"timeout 3 > nul\n";
    stream_installer<<"@echo off\n";
    stream_installer<<QDir::toNativeSeparators(QDir::currentPath())+"\\7z x "+QDir::toNativeSeparators(QDir::currentPath())+"\\..\\Update.7z -o"+
                      QDir::toNativeSeparators(QDir::currentPath())+"..\\ \n";
    //stream_installer<<"echo Extract: "+QDir::toNativeSeparators(QDir::currentPath())+"\\7z x "+QDir::toNativeSeparators(QDir::currentPath())+"\\..\\Update.7z -o"+
    //                                    QDir::toNativeSeparators(QDir::currentPath())+"..\\ \n";
    stream_installer<<"timeout 5\n";
    stream_installer<<"del /s "+QDir::toNativeSeparators(QDir::currentPath())+"\\* /Q\n";
    stream_installer<<"xcopy "+QDir::toNativeSeparators(QDir::currentPath())+"\\..\\Update "+QDir::toNativeSeparators(QDir::currentPath())+" /E /C\n";
    stream_installer<<"echo xcopy! "+QDir::toNativeSeparators(QDir::currentPath())+"\\..\\Update "+QDir::toNativeSeparators(QDir::currentPath())+" /E /C\n";
    stream_installer<<"rmdir "+QDir::toNativeSeparators(QDir::currentPath())+"\\..\\Update /S /Q\n";
    stream_installer<<"echo MsgBox \"Update Terminee !\">msgbox.vbs\n";
    stream_installer<<"cscript msgbox.vbs\n";
    stream_installer<<"del msgbox.vbs\n";
    stream_installer<<"del "+QDir::toNativeSeparators(QDir::currentPath())+"\\..\\Update.7z\n";
    stream_installer<<"(goto) 2>nul & del \"%~f0\"\n";
}

/*void mainWindow::downloadFile() {
    dlNext();
}


void mainWindow::ftpCommandFinished(int commandId, bool error) {
    if(ftp->currentCommand()==QFtp::ConnectToHost) {
        if(error) {
            msgCritical("Connection au serveur de téléchargement impossible !");
            return;
        }
    }

    if(ftp->currentCommand()==QFtp::Login)
        ftp->list();

    if(ftp->currentCommand()==QFtp::Get&&error) {
        msgInfo("Download Canceled !");
        msgInfo(ftp->errorString());
        dlwindow->hide();
        delete dl_file;
        leaveFtp();
    }
    else if(ftp->currentCommand()==QFtp::Get) {
        dlwindow->hide();
        leaveFtp();
    }

}*/

/*

void mainWindow::cancelDownload() {
    ftp->abort();
    //if(dl_file->exists()) {
    //    dl_file->close();
    //dl_file->remove();
    //}
    extrn->hideWin();
    //leaveFtp();
}
void mainWindow::updateDataTransferProgress(qint64 read, qint64 total) {

}*/

/*
void mainWindow::getFtp() {
    QUrl ftpUrl(FTP_HOST);
    if(!ftpUrl.isValid()) {
        msgCritical("Le serveur n'existe pas !");
    }
    ftp->connectToHost(FTP_HOST, FTP_PORT);
    ftp->login(FTP_USER, FTP_PASS);
    ftp->cd("Updater");

}

void mainWindow::leaveFtp() {
    //ftp->disconnect();
}*/

