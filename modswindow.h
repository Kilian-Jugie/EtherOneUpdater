#ifndef MODSWINDOW_H
#define MODSWINDOW_H

#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <vector>
#include "datamanager.h"
#include "defs.h"
#include "kprocess.h"

class modsCheckBox : public QCheckBox {
    Q_OBJECT
public:
    QString m_Mod;

    modsCheckBox(QString mod) : m_Mod(mod), QCheckBox(mod) {

    }


};

class modswindow : public QWidget  {
    Q_OBJECT

    DataManager* dataManager = new DataManager(FILE_DATA);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* exitLayout = new QHBoxLayout(this);

    std::vector<modsCheckBox*>* onMods = new std::vector<modsCheckBox*>();

    QPushButton* b_Exit = new QPushButton("Save & Exit");


    QString etPath;

    bool exitPlaced;

public:

    modswindow() {
        _MyBase();
        this->setLayout(layout);

    }

    modswindow(std::vector<std::pair<QString, bool>> modList) {
        _MyBase();
        for(std::vector<std::pair<QString, bool>>::iterator it(modList.begin()); it!=modList.end(); ++it) {
            addMod((*it).first, (*it).second);
        }
    }

    modswindow(std::vector<QString> modList) {
        _MyBase();
        for(std::vector<QString>::iterator it(modList.begin()); it!=modList.end(); ++it) {
            addMod(*it, false);
        }
    }

    ~modswindow() {
        delete onMods;
    }

    void addMod(QString name, bool isChecked=false) {
        if(exitPlaced)
            layout->removeItem(exitLayout);
        QHBoxLayout* newLayout = new QHBoxLayout();
        modsCheckBox* check = new modsCheckBox(name);
        check->setChecked(isChecked);
        onMods->push_back(check);
        newLayout->addWidget(check);
        layout->addLayout(newLayout);
        layout->addLayout(exitLayout);
    }

    void setEtPath(QString newPath) {
        etPath = newPath;
    }

    void _MyBase() {
        connect(b_Exit, SIGNAL(clicked(bool)), this, SLOT(Save()));
        exitLayout->addWidget(b_Exit);
        layout->addLayout(exitLayout);
        exitPlaced=true;
    }

public slots:
    void display() {
        if(!exitPlaced) {
            layout->addLayout(exitLayout);
            exitPlaced=true;
        }
        this->show();
    }

    void downloadFile(QString file, QString newFile) {
        dlWindow* dlwin = new dlWindow();
        kProcess* proc = new kProcess(dlwin);
        proc->start(EXTERN_DL_PROGRAM" "+file+" --progress=bar:force:noscroll --output-document="+newFile);
        if(!proc->waitForFinished())
            utils::boxCrit("Impossible de télécharger le mod "+file);
    }

    void Save() {
        DataManager data(FILE_DATA);
        for(std::vector<modsCheckBox*>::iterator it(onMods->begin()); it!=onMods->end(); ++it) {
            if(!(*it)->isChecked())
                continue;
            bool success(true);
            downloadFile(DB_FOLDER_BASE DB_FOLDER_DATA"/Mods/"+(*it)->m_Mod, FOLDER_TEMP"\\"+(*it)->m_Mod);
            if(!QFile::copy(FOLDER_TEMP"\\"+(*it)->m_Mod, etPath+"\\mods\\"+(*it)->m_Mod)) {
                utils::boxCrit("Impossible de copy le mod"+(*it)->m_Mod+" vers le dossier 'mods' !");
                success=false;
            }

            if(!QFile(FOLDER_TEMP"\\"+(*it)->m_Mod).remove())
                utils::boxCrit("Impossible de supprimer les fichiers temporaires.");



            if((*it)->isChecked()&&success)
                data.addToArray("AdditionnalMods", (*it)->m_Mod);
        }

        this->hide();
    }

};

#endif // MODSWINDOW_H
