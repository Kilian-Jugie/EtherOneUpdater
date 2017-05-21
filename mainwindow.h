#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*! \file mainwindow.h
 * \brief Fenêtre principale de l'Updater
 *
 *
 */

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QFile>
#include <QIODevice>
#include <QProgressBar>
#include <QList>
#include <QPair>
#include <QMessageBox>
#include <QProcess>
#include <QNetworkReply>
#include <QKeySequence>
#include <QShortcut>
#include "optionwindow.h"
#include "qftp/qftp.h"
#include "kprocess.h"
#include "downloadmanager.h"
#include "datamanager.h"
#include "loger.h"
#include "defs.h"

/*!
 * \brief Fenêtre principale de l'Updater
 *
 * Toutes les actions sont coordonnés depuis cette fenêtre, autant les actions de rendu que les action au niveau logiciel (installation d'une MAJ par exemple).
 */
class mainWindow : public QWidget
{
    Q_OBJECT

    /*!
     * \brief Liste des dossiers requies par l'installeur
     */
    QList<QString> m_installerFolders = {FOLDER_IMGS, FOLDER_UP_DATA, FOLDER_TEMP};
    /*!
     * \brief Liste des fichiers requies par l'installeur
     */
    QList<QString> m_installerFiles   = {FILE_DATA, FILE_VERSION, FILE_UP_VERSION};

    /*!
     * \brief NON UTILISE
     */
    QString* m_Tile = new QString();

    /*!
     * \brief Layout principale
     */
    QGridLayout* layout = new QGridLayout();

    /*!
     * \brief Credits
     */
    QLabel* l_Credits = new QLabel("Updater by kilian3008");
    /*!
     * \brief "EtherOne Version"
     */
    QLabel* l_McVersion = new QLabel("EtherOne Version");
    /*!
     * \brief Version d'EtherOne (rempli plus tard)
     */
    QLabel* l_etVersion = new QLabel("ERROR");
    /*!
     * \brief Label qui permet d'afficher le Pixmap qui contien une image (image de fond)
     */
    QLabel* l_img = new QLabel();
    /*!
     * \brief Pixmap contenant l'image en elle même
     */
    QPixmap* pm_BackGround = new QPixmap(FOLDER_IMGS"/back.jpg");

    /*!
     * \brief Boutton 'Exit'
     */
    QPushButton* b_Exit = new QPushButton("Exit");
    /*!
     * \brief Boutton 'Install'
     */
    QPushButton* b_Install = new QPushButton("Install");
    /*!
     * \brief Boutton 'Status' (non-implémenté)
     */
    QPushButton* b_Status = new QPushButton("Status");
    /*!
     * \brief Boutton 'Options' (en cours d'implémentation)
     */
    QPushButton* b_Options = new QPushButton("Options");

    /*!
     * \brief Manager de donnés pour les donnés de configuration (version de l'updateur, du modpack ...)
     */
    DataManager* dataManager;

    /*!
     * \brief Processus de téléchargement
     */
    kProcess* extrn;

    /*!
     * \brief Version de l'updateur
     */
    QString updaterVersion;
    /*!
     * \brief Version du modpack
     */
    QString etVersion;
    /*!
     * \brief Path vers le dossier général du modpack
     */
    QString etPath;
    /*!
     * \brief Shortcut pour activer le quiet mode (Ctrl+Q)
     */
    QShortcut* quietMode = new QShortcut(QKeySequence(tr("Ctrl+Q")), this);
    /*!
     * \brief Marquer les erreurs ou pas ?
     */
    bool DontPrompUpdateErrors;
    /*!
     * \brief Future système de log
     */
    Loger* log = new Loger("logs.log");

    OptionWindow* opWin = new OptionWindow(this);
public:
    /*!
     * \brief Constructeur principale et unique
     */
    mainWindow(QWidget *parent = 0);
    ~mainWindow();

public slots:
    QString retEtPath() {
        return etPath;
    }
private slots:
    /*!
     * \brief Téléchargement d'un fichier
     * \param file Fichier à télécharger (lien http)
     * \param newFile Nom du fichier téléchargé
     * \param useFTP Utilisatio ou non du protocol FTP (non-implémenté)
     */
    void downloadFile(QString file, QString newFile, bool useFTP);
    /*!
     * \brief Vérifie que tous les fichiers ET DOSSIERS requis par l'updateur sont présent
     * \return true s'ils sont tous la sinon false
     */
    bool checkUpdaterFiles();
    /*!
     * \brief Créé les fichier/dossiers manquant(s)
     */
    void setUpUpdater();
    /*!
     * \brief Vérifi s'il y a une mise à jour et la télécharge puis l'installe si besoin
     * \param isUpdater Si true, on vérifira qu'il n'y a pas de mise à jour de l'updater sinon on vérifi qu'il n'y a pas de mise à jour du modpack
     */
    void checkForUpdates(bool isUpdater);
    /*!
     * \brief Parse le fichier de versions obtenu du téléchargement
     * \param isUpdater Si true, le fichier est celui de mise à jour de l'updater sinon c'est celui de mise à jour du modpack
     */
    void versionParser(bool isUpdater);
    /*!
     * \brief [TO BE DEPRECATED] obtient la data à partir de l'ancien system de stockage des donnés
     */
    void getData();
    /*!
     * \brief Installe une mise à jour de l'updater (appelé par checkForUpdates)
     * \see checkForUpdates(bool)
     */
    void installUpUpdate();
    /*!
     * \brief Coordinateur des mises à jour du modpack
     */
    void startModpackUpdate();
    /*!
     * \brief Affiche la fenêtre pour obtenir le path vers le modpack
     */
    void getEtPath();
    /*!
     * \brief [TO BE IMPLEMENTED] démarre le processus de vérification des status
     */
    void startStatus();
    /*!
     * \brief [TO BE DEPRECATED] change la valeur d'une donnée via l'ancien système
     */
    void changeData(const QString, const QString);
    /*!
     * \brief Parseur du fichier d'instruction d'installation (et interpréteur)
     * \return true si tout c'est bien passé sinon false
     */
    bool installationParser(QString);
    /*!
     * \brief fonction interne requise au fonctionnement du installationParser
     * \see installationParser(QString)
     * \return ///
     */
    QStringList parseString(QString);

    void onQuietMode();

    void msgInfo(QString msg) {
        QMessageBox box("Info", msg, QMessageBox::Information, 0, 0, 0);
        box.setFocus();
        box.exec();
    }

    void msgWarning(QString msg) {
        QMessageBox box("Warning", msg, QMessageBox::Warning, 0, 0, 0);
        box.setFocus();
        box.exec();
    }

    void msgCritical(QString msg) {
        QMessageBox box("Error", msg, QMessageBox::Critical, 0, 0, 0);
        box.setFocus();
        box.exec();
    }

    void msgFatal(QString msg) {
        QMessageBox box("Fatal Error", msg, QMessageBox::Critical, 0, 0, 0);
        box.setFocus();
        box.exec();
        abort();
    }

    void DebugMessage(QString msg) {
        QMessageBox box;
        box.setFocus();
        box.setText(msg);
        box.exec();
    }


};

#endif // MAINWINDOW_H
