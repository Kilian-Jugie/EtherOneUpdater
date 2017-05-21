#ifndef PINGWINDOW_H
#define PINGWINDOW_H
#pragma once

#include <QProcess>
#include <QVBoxLayout>
#include <QHBoxLayout>
//#include <QList>
#include <QLabel>
#include <QWidget>
#include <QMessageBox>
#include <QTextStream>
#include <vector>
#include <algorithm>
#include <QFile>
#include <QTextStream>
#include "uputils.h"






//TO DO








/* TOO COMPLEX METHOD
namespace statusData {
    class SecToken {
    public:
        QString secToken;
        QString strToDeduce;
        SecToken(QString tok, QString toDeduce) : secToken(tok), strToDeduce(toDeduce) {}

        bool operator!=(SecToken& token) {
            return secToken!=token.secToken&&strToDeduce!=token.strToDeduce;
        }

        static SecToken null(){
            return SecToken("", "");
        }
    };

    class MainToken {
    public:
        QString token;
        std::vector<SecToken> secTokens;

        MainToken(QString to, std::vector<SecToken> sec) : token(to), secTokens(sec) {}

        bool operator!=(MainToken& mtoken) {
            return token!=mtoken.token;
        }

        static MainToken null() {
            return MainToken("", {SecToken("", "")});
        }
    };
}

class statusWindow : public QWidget {
    Q_OBJECT
    QVBoxLayout* layout = new QVBoxLayout();
    std::vector<statusData::MainToken> tokenList;
    QString f_data;

public:
    statusWindow(QString dataFile) : QWidget(), f_data(dataFile), tokenList() {
        tokenList.push_back(statusData::MainToken("status", {statusData::SecToken("sucess", "SUCCESSFULL"), statusData::SecToken("error", "ERROR")}));
        tokenList.push_back(statusData::MainToken("online", {statusData::SecToken("true", "Online"), statusData::SecToken("false", "Down")}));
        tokenList.push_back(statusData::MainToken("motd", {statusData::SecToken("%ANY%", "%SAME%")}));
        tokenList.push_back(statusData::MainToken("error", {statusData::SecToken("%ANY%", "%SAME%")}));
        tokenList.push_back(statusData::MainToken("players", {statusData::SecToken("max:%INT%,now:%INT%", "%SAME%")}));
        tokenList.push_back(statusData::MainToken("server", {statusData::SecToken("name:%ANY%,protocol:%INT%", "%SAME%")}));
    }

    bool isAToken(QString str) {
        for(std::vector<statusData::MainToken>::iterator it(tokenList.begin()); it!=tokenList.end(); ++it) {
            if((*it).token==str)
                return true;
        }
        return false;
    }

    statusData::MainToken getToken(QString str) {
        for(std::vector<statusData::MainToken>::iterator it(tokenList.begin()); it!=tokenList.end(); ++it) {
            if((*it).token==str)
                return *it;
        }
        return statusData::MainToken::null();
    }

    bool isASecondToken(statusData::MainToken* token, QString sec) {
        for(std::vector<statusData::SecToken>::iterator it(token->secTokens.begin()); it!=token->secTokens.end(); ++it) {
            if((*it).secToken==sec)
                return true;
        }
        return false;
    }

    statusData::SecToken getSecToken(statusData::MainToken* token, QString sec) {
        for(std::vector<statusData::SecToken>::iterator it(token->secTokens.begin()); it!=token->secTokens.end(); ++it) {
            if((*it).secToken==sec)
                return *it;
        }
        return statusData::SecToken::null();
    }

    void msgError(QString err) {
        QMessageBox box("error", err, QMessageBox::Critical, 0, 0, 0);
        box.exec();
    }

    void parseData() {
        bool isParsing;
        std::vector<QString> words;
        for(QString::Iterator it(f_data.begin()+1); it!=f_data.end(); ++it) {
            if(*it=='"'&&!isParsing)
                isParsing=true;
            else if(isParsing&&*it!='"')
                words.size()>0?words.at(words.size()-1).push_back(*it):words.push_back(QString(&*it));
            else if(*it=='"'&&isParsing)
                isParsing=false;
            else
                words.push_back(QString(&*it));
        }
        for(std::vector<QString>::iterator it(words.begin()); it!=words.end(); ++it) {
            statusData::MainToken token(getToken(*it));
            if(token!=statusData::MainToken::null()) {
                ++it;
                statusData::SecToken sectoken(getSecToken(&token, *it));
                if()

            }else {
                msgError("LVL1");
            }
        }
    }

}; */

class statusWindow : public QWidget {
    Q_OBJECT
    QVBoxLayout* layout = new QVBoxLayout();
    std::vector<QHBoxLayout*>* hlayouts = new std::vector<QHBoxLayout*>();
    QLabel l;
    QString file;

    bool GlobalStatus;
    bool Online;
    QString Motd;
    QString Errors;
    int MaxPlayers;
    int NowPlayers;
    QString Version;

public:

    void addLine(QWidget* wid1, QWidget* wid2) {
        QHBoxLayout ret(this);
        ret.addWidget(wid1);
        ret.addWidget(wid2);
        hlayouts->push_back(&ret);
    }


    statusWindow(QString sttfile) : QWidget(),  file(sttfile) {

    }

    void getAllStatus() {
        QFile stt(file);
        stt.open(QIODevice::ReadOnly);
        QTextStream ss(&stt);
        QString line=ss.readAll();
        std::replace(line.begin(), line.end() ,':', ' ');
        std::replace(line.begin(), line.end(), ',', ' ');
        ss.setString(&line, QIODevice::ReadOnly);
        QString buf, buf2;
        while(!ss.atEnd()) {
            ss>>buf;
            while(utils::findNum(buf.begin(), buf.end(), '"')%2!=0) {
                ss>>buf2;
                buf+=buf2;
            }
          parseAnExpression(buf);
        }
    }

    void parseAnExpression(QString toParse) {
        std::remove(toParse.begin(), toParse.end(), '{');
        std::remove(toParse.begin(), toParse.end(), '}');
    }
};

class pingWindow : public QWidget {
    Q_OBJECT
    QVBoxLayout* layout = new QVBoxLayout();
    QProcess* pingProcess = new QProcess();
    //QList<pingProcess*> pingListProcess;
    int lastPingResult;
    void msgbox(QString str) {
        QMessageBox msg("test", str, QMessageBox::Information, 0, 0, 0);
        msg.exec();
    }

public:
    pingWindow(QStringList pingList) : QWidget(), lastPingResult(0) {
        connect(pingProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(pingResult(int,QProcess::ExitStatus)));
        for(QStringList::Iterator it(pingList.begin()); it!=pingList.end(); ++it) {
            pingProcess->start("ping -n 1 "+(*it));
            pingProcess->waitForFinished();
            //msgbox(QString("Result:"+QString::number(lastPingResult)));
            //msgbox("ping -n 1 "+(*it));
        }

    }
public slots:
    void pingResult(int exitCode, QProcess::ExitStatus exitStatus) {
        lastPingResult=exitCode;
    }

};

/*
class pingProcess : public QProcess {
    Q_OBJECT
    QString pingHost;
    pingWindow* win_Ping;
public:
    void reconfigure(QString ping, pingWindow* pwin) {
        pingHost=ping;
        win_Ping=pwin;
    }

    pingProcess(QString ping, pingWindow* pwin) : QProcess(), pingHost(ping), win_Ping(pwin) {
        connect(this, SIGNAL(finished(int,QProcess::ExitStatus)), pwin, SLOT(pingResult(int,QProcess::ExitStatus)));
    }

    bool startPing() {
        this->start("ping -n 1 "+pingHost);
    }
};
*/

#endif // PINGWINDOW_H
