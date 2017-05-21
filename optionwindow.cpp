#include "optionwindow.h"
#include "mainwindow.h"
#include "kprocess.h"

OptionWindow::OptionWindow(mainWindow* parent) {
    layout->addWidget(b_mods);
    layout->addWidget(b_quietMode);

    layout->addWidget(b_SaveAndQuit);

    modsWin->setEtPath(parent->retEtPath());

    connect(b_quietMode, SIGNAL(clicked(bool)), parent, SLOT(onQuietMode()));
    connect(b_SaveAndQuit, SIGNAL(clicked(bool)), this, SLOT(saveAndQuit()));
    connect(b_mods, SIGNAL(clicked(bool)), modsWin, SLOT(display()));
}

void OptionWindow::saveAndQuit() {

    this->hide();
}

bool OptionWindow::parseClientModList() {
    dlWindow* dlwin = new dlWindow();
    kProcess* proc = new kProcess(dlwin);
    proc->start(EXTERN_DL_PROGRAM " " DB_FOLDER_BASE DB_FOLDER_VERSIONS "additionnalmods.mds --progress=bar:force:noscroll --output-document=" FOLDER_TEMP "\\addmods.mds");
    if(!proc->waitForFinished()) {
        return false;
    }

    DataManager* data = new DataManager(FILE_DATA);

    QFile file(FOLDER_TEMP"\\addmods.mds");
    QTextStream ss(&file);
    QString line;
    while(ss.readLineInto(&line)) {
        modsWin->addMod(line, data->isInArray("AdditionnalMods", line));
    }
    return true;
}
