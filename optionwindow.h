#ifndef OPTIONWINDOW_H
#define OPTIONWINDOW_H

/*! \file optionwindow.h
 *  \brief Option Window
 */

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "modswindow.h"

class mainWindow;

class OptionWindow : public QWidget {
    Q_OBJECT

    /*!
     * \brief Layout principale
     */
    QVBoxLayout* layout = new QVBoxLayout(this);

    QPushButton* b_mods = new QPushButton("Mods additionnels", this);

    QPushButton* b_quietMode = new QPushButton("Mode silencieu", this);

    QPushButton* b_SaveAndQuit = new QPushButton("Save And Quit", this);

    modswindow* modsWin = new modswindow();

public:

    OptionWindow(mainWindow* parent);

public slots:

    void saveAndQuit();

    bool parseClientModList();

    void addtoModWindow(QString mod, bool isChecked=false) {
        modsWin->addMod(mod, isChecked);
    }

};


#endif // OPTIONWINDOW_H
