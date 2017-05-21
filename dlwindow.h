#ifndef DLWINDOW_H
#define DLWINDOW_H

/*! \file dlwindow.h
 *  \brief Fentêtre de téléchargement
 */

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QDesktopWidget>

/*!
 * \brief Fenêtre de téléchargement
 *
 * Fenêtre de téléchargement SANS le protol de téléchargement
 *
 * \see kProcess
 */
class dlWindow : public QWidget {
    Q_OBJECT

    /*!
     * \brief Layout globale
     */
    QGridLayout* layout;

public:
    /*!
     * \brief Output redirection du process
     * \see kProcess
     */
    QTextEdit* te_Output;
    /*!
     * \brief Boutton Cancel
     */
    QPushButton* b_Cancel;

    dlWindow(const dlWindow& other) :   layout(other.layout), te_Output(other.te_Output), b_Cancel(other.b_Cancel) {
        _MyBase();
    }

    dlWindow() {
        layout = new QGridLayout(this);
        te_Output = new QTextEdit(this);
        b_Cancel = new QPushButton("Cancel");
        _MyBase();
    }

    /*!
     * \brief Appelée par le constructeur, met en place le layout et la fenêtre
     */
    void _MyBase() {
        te_Output->setReadOnly(true);
        layout->addWidget(b_Cancel, 5, 9, 1, 2);
        layout->addWidget(te_Output, 2, 1, 3, 10);

        this->setLayout(layout);
        this->setWindowTitle("Downloading...");
        this->resize(QDesktopWidget().availableGeometry(this).size()*0.26);
    }

public slots:
    void updateProgress(/*qint64 dlled, qint64 total*/) {

    }


};



#endif // DLWINDOW_H
