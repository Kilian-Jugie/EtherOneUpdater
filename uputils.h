#ifndef UPUTILS_H
#define UPUTILS_H

/*! \file uputils.h
 *  \brief Fichier de library pour EtherOne Updater
 */

#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>
#include <sstream>
#include <vector>
/*
namespace util {

void delayms( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

}*/

/*!
 * \brief Class contenant les outils
 *
 * Class servant de namespace pour des raisons de link. Toutes les foncions de cette class sont static afin d'être utilisés sans créer une instance de la classe
 */
class utils {
public:
    /*!
     * \brief recherche value entre first et last (iterators) et retourne le nombre de fois où l'expression apparaît
     * \return Le nombre de fois où appraît l'expression
     */
    template< class InputIt, class T >static int findNum( InputIt first, InputIt last, const T& value ) {
        int _ret(0);
        for(; first!=last; ++first) {
            if((*first)==value)
                ++_ret;
        }
        return _ret;
    }

    /*!
     *  \brief recherche value entre first et last, si trouvé retourne true
     *  \return True si value est entre first et last sinon false
     */
    template< class InputIt, class T >static bool find( InputIt first, InputIt last, const T& value ) {
        for(; first!=last; ++first) {
            if((*first)==value)
                return true;
        }
        return false;
    }

    /*
    template< class InputIt, class T>static std::vector<InputIt> findEach(InputIt first, InputIt last, const T& value) {
        std::vector<IntputIt> _ret({});
        for(; first!=last; ++first) {
            if((*first)==value)
                _ret.push_back(first);
        }
        return _ret;
    }*/

    /*!
     * \brief Créé une boite de dialogue de type "information"
     * \param msg   Message à afficher
     * \param title Titre de la fenêtre
     */
    static void boxInfo(QString msg, QString title="info") {
        QMessageBox box(title, msg, QMessageBox::Information, 0, 0, 0);
        box.exec();
    }

    /*!
     * \brief Créé une boite de dialogue de type "warning"
     * \param msg   Message à afficher
     * \param title Titre de la fenêtre
     */
    static void boxWarning(QString msg, QString title="warning") {
        QMessageBox box(title, msg, QMessageBox::Warning, 0, 0, 0);
        box.exec();
    }

    /*!
     * \brief Créé une boite de dialogue de type "error"
     * \param msg   Message à afficher
     * \param title Titre de la fenêtre
     */
    static void boxCrit(QString msg, QString title="critical") {
        QMessageBox box(title, msg, QMessageBox::Critical, 0, 0, 0);
        box.exec();
    }

    /*!
     *  \brief Cast T vers QString
     */
    template<typename T>static QString toQString(T& any) {
        std::stringstream ss;
        ss<<any;
        return QString::fromStdString(ss.str());
    }

    template<>static QString toQString(int& l) {
        return QString::number(l);
    }

    template<>static QString toQString(float& l) {
        return QString::number(l);
    }

    template<>static QString toQString(double& l) {
        return QString::number(l);
    }

    template<>static QString toQString(QChar& l) {
        return QString(l);
    }

    template<>static QString toQString(std::string& l) {
        return QString::fromStdString(l);
    }

    /*!
     * \brief [MARCHE PAS]Remplace un string dans un string par un autre string
     * \param str Conteneur
     * \param old   Ancien string
     * \param newStr    Nouveau string
     */
    static void replace(std::string str, std::string old, std::string newStr) {
        for(int pos(0); (pos=str.find(old, pos))!=std::string::npos; pos+=newStr.length())
            str.replace(pos, old.length(), newStr);
    }
};



#define GetLastOf(cont) cont.at(cont.size()-1)
#define GetLastOfPtr(cont) cont->at(cont->size()-1)

#endif // UPUTILS_H
