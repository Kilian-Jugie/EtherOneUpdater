#ifndef DATAMANAGER_H
#define DATAMANAGER_H

/*! \file datamanager.h
 * \brief Manager pour donnés
 *
 *
 */

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QString>
#include "uputils.h"

/*! \def RAPIDJSON_HAS_STDSTRING
 *   \brief RapidJson utilise les string de STL
 */
#define RAPIDJSON_HAS_STDSTRING 1

#include <cstdio>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

using namespace rapidjson;

//#define DM_DEBUG

/*!
 * \brief Manager pour donnés JSON V1.0
 *
 * Permet de créer, modifier des fichiers data en utilisant du json. Cette class n'est utilisble qu'avec Qt, une version STL sera probablement faite
 * toujours avec le parser actuel, une version templated peut aussi être envisagée avec un allocator custom. Cette class reste en développement avec
 * par exemple remToArray qui n'est actuellement pas disponible.
 */
class DataManager : public QObject {
    Q_OBJECT
    QString m_File;
public:
    DataManager(const QString file) : m_File(file) {
        if(!QFile::exists(file)) {

            QFile WRITE(file);
            WRITE.open(QIODevice::WriteOnly);
            if(!WRITE.isWritable()) {
                //utils::boxCrit("Cannot create data file :"+WRITE.errorString());
                return;
            }
            WRITE.close();
            WriteDefault();
        }
        else {
            Document data;
            GetDataDocument(&data);
            if(!data.HasMember("Data")||!data["Data"].IsTrue()) {
                WriteDefault();
            }

        }
    }

    /*!
     * \brief Créé un fichier avec la data par défaut (syntax to normalize)
     */
    void WriteDefault() const {
#ifdef DM_DEBUG
        utils::boxInfo("Writing default");
#endif
        const char json[] = "{ \"McVersion\" : \"4.0.0\","
                            "\"McPath\" : \"NONE\","
                            "\"AdditionnalMods\" : [],"
                            "\"Data\": true,"
                            "\"UpVersion\": \"2.0.0\" }";
        Document document;
        if(document.Parse(json).HasParseError()) {
            utils::boxCrit("Cannot output default data!");
            return;
        }
        SaveDocument(&document);
    }

    /*!
     * \brief Sauvegarde des donnés dans un fichier (non-RAM)
     * \param doc Document
     */
    void SaveDocument(const Document* doc) const {
#ifdef DM_DEBUG
        utils::boxInfo("Saving Document");
#endif
        QFile toSave(m_File);
        toSave.open(QIODevice::WriteOnly);
        QTextStream ss(&toSave);
        StringBuffer sb;
        PrettyWriter<StringBuffer> writer(sb);
        doc->Accept(writer);
        ss<<sb.GetString();
        toSave.close();
    }

    /*!
     * \brief Remplit un pointeur depuis un fichier (non-RAM)
     *
     * Créé une instance Document à partir d'un fichier locale (non-RAM)
     * puis remplace le pointeur fournis en argument par cette valeur.
     * \param doc Container pour stocker le résultat
     */
    void GetDataDocument(Document* doc) const {
#ifdef DM_DEBUG
        utils::boxInfo("Getting Data by reading file into another document");
#endif
        QFile save(m_File);
        save.open(QIODevice::ReadOnly);
        QTextStream ss(&save);
        std::string json(ss.readAll().toStdString());
        //utils::boxInfo(QString::fromStdString(json), "mabite");
        if(doc->Parse(json.c_str(), json.size()).HasParseError())
            utils::boxCrit("Cannot get data from document (Parse error)");
    }

    /*!
     * \brief Change la valeur de arg pour value (String)
     * \param arg Argument à changer
     * \param value Valeur à appliquer (String)
     * \return Succés = true sinon false
     */
    bool writeData(const QString arg, const QString value) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(arg.toStdString().c_str()))
            return false;
        if(!data[arg.toStdString().c_str()].IsString())
            return false;
        Document::AllocatorType& allocator = data.GetAllocator();
        Value strVal;
        strVal.SetString(value.toStdString().c_str(), value.toStdString().length(), allocator);
        data[arg.toStdString().c_str()]=strVal;
        SaveDocument(&data);
        return true;
    }

    /*!
     * \brief Change la valeur de arg pour value (int)
     * \param arg Argument à changer
     * \param value Valeur à appliquer (int)
     * \return Succés = true sinon false
     */
    bool writeData(const QString arg, const int value) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(arg.toStdString().c_str()))
            return false;
        if(!data[arg.toStdString().c_str()].IsInt())
            return false;
        data[arg.toStdString().c_str()]=value;
        SaveDocument(&data);
        return true;
    }

    /*!
     * \brief Change la valeur de arg pour value (bool)
     * \param arg Argument à changer
     * \param value Valeur à appliquer (bool)
     * \return Succés = true sinon false
     */
    bool writeData(const QString arg, const bool value) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(arg.toStdString().c_str()))
            return false;
        if(!data[arg.toStdString().c_str()].IsBool())
            return false;
        data[arg.toStdString().c_str()]=value;
        SaveDocument(&data);
        return true;
    }

    /*!
     * \brief Vérifi si value (QString) existe dans array
     * \param array Array où il faut vérifier
     * \param value Valeur qu'il faut chercher
     * \return Trouvée = true sinon false
     */
    bool isInArray(const QString array, QString value) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(array.toStdString().c_str()))
            return false;
        if(!data[array.toStdString().c_str()].IsArray())
            return false;
        Value strVal;
        Document::AllocatorType& allocator = data.GetAllocator();
        strVal.SetString(value.toStdString().c_str(), value.toStdString().length(), allocator);
        for(size_t i(0); i<data[array.toStdString().c_str()].Size(); ++i) {
            if(data[array.toStdString().c_str()][i].IsString()&&data[array.toStdString().c_str()][i]==strVal)
                return true;
        }
        return false;
    }

    /*!
     * \brief Vérifi si value (int) existe dans array
     * \param array Array où il faut vérifier
     * \param value Valeur qu'il faut chercher
     * \return Trouvée = true sinon false
     *
    bool isInArray(const QString array, int value) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(array.toStdString().c_str()))
            return false;
        if(!data[array.toStdString().c_str()].IsArray())
            return false;
        for(size_t i(0); i<data[array.toStdString().c_str()].Array.Size(); ++i) {
            if(data[array.toStdString().c_str()].Array[i].IsInt()&&data[array.toStdString().c_str()].Array[i]==value)
                return true;
        }
    }*/

    /*!
     * \brief Ajoute la valeur value (String) a l'array array
     * \param array Array où il faut ajouter
     * \param value Valeur à ajouter (String)
     * \return Succés = true sinon false
     */
    bool addToArray(const QString array, const QString value) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(array.toStdString().c_str()))
            return false;
        if(!data[array.toStdString().c_str()].IsArray())
            return false;
        Value strVal;
        Document::AllocatorType& allocator = data.GetAllocator();
        strVal.SetString(value.toStdString().c_str(), value.toStdString().length(), allocator);
        data[array.toStdString().c_str()].PushBack(strVal, allocator);
        SaveDocument(&data);
        return true;
    }

    /*!
     * \brief Ajoute la valeur value (int) a l'array array
     * \param array Array où il faut ajouter
     * \param value Valeur à ajouter (int)
     * \return Succés = true sinon false
     */
    bool addToArray(const QString array, const int value) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(array.toStdString().c_str()))
            return false;
        if(!data[array.toStdString().c_str()].IsArray())
            return false;
        Document::AllocatorType& allocator = data.GetAllocator();
        data[array.toStdString().c_str()].PushBack(value, allocator);
        SaveDocument(&data);
        return true;
    }

    /*!
     * \brie Ajoute la valeur value (bool) a l'array array
     * \para array Array où il faut ajouter
     * \para value Valeur à ajouter (bool)
     * \retur Succés = true sinon false
     *
    bool addToArray(const QString array, const bool value) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(array.toStdString().c_str()))
            return false;
        if(!data[array.toStdString().c_str()].IsArray())
            return false;
        Document::AllocatorType& allocator = data.GetAllocator();
        data[array.toStdString().c_str()].PushBack(value, allocator);
        SaveDocument(&data);
        return true;
    }*/


    /*!
     * \brief Enleve un string d'une array
     * \param array Array d'où il faut enlever un string
     * \param elem String qu'il faut enlever
     * \return true si la suppression a marché sinon false
     */
    bool remToArray(const QString array, const QString elem) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(array.toStdString().c_str()))
            return false;
        Value& arr(data[array.toStdString().c_str()]);
        if(!arr.IsArray())
            return false;
        for(Value::ConstValueIterator it=arr.Begin(); it!=arr.End(); ++it) {
            if(!(*it).IsString())
                break;
            if((*it).GetString()==elem)
                arr.Erase(it, it+1);
        }
        SaveDocument(&data);
        return true;
    }

    /*!
     * \brief Enleve un int d'une array
     * \param array Array d'où il faut enlever un string
     * \param elem int qu'il faut enlever
     * \return true si la suppression a marché sinon false
     */
    bool remToArray(const QString array, const int elem) const {
        Document data;
        GetDataDocument(&data);
        Value& arr(data[array.toStdString().c_str()]);
        if(!data.HasMember(array.toStdString().c_str()))
            return false;
        if(!arr.IsArray())
            return false;
        for(Value::ConstValueIterator it=arr.Begin(); it!=arr.End(); ++it) {
            if(!(*it).IsInt())
                break;
            if((*it).GetInt()==elem)
                arr.Erase(it, it+1);
        }
        SaveDocument(&data);
        return true;
    }

    /*!
     * \brie Enleve un bool d'une array
     * \para array Array d'où il faut enlever un string
     * \para elem bool qu'il faut enlever
     * \retur true si la suppression a marché sinon false
     *
    bool remToArray(const QString array, const bool elem) const {
        Document data;
        GetDataDocument(&data);
        Value& arr(data[array.toStdString().c_str()]);
        if(!data.HasMember(array.toStdString().c_str()))
            return false;
        if(!arr.IsArray())
            return false;
        for(Value::ConstValueIterator it=arr.Begin(); it!=arr.End(); ++it) {
            if((*it).GetBool()==elem)
                arr.Erase(it, it+1);
        }
        SaveDocument(&data);
        return true;
    }*/


    /*!
     * \brief Return la valeur d'une variable (String) (STL)
     * \param name Nom de la variable à check
     * \return Sa valeur (String (STL))
     */
    std::string getStdStringData(const std::string name) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(name.c_str()))
            return "";
        return data[name.c_str()].GetString();
    }

    /*!
     * \brief Return la valeur d'une variable (String) (Qt)
     * \param name Nom de la variable à check
     * \return Sa valeur (String (Qt))
     */
    QString getStringData(const QString name) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(name.toStdString().c_str()))
            return "";
        return QString::fromStdString(data[name.toStdString().c_str()].GetString());
    }

    /*!
     * \brief Return la valeur d'une variable (int)
     * \param name Nom de la variable à check
     * \return Sa valeur (int)
     */
    int getIntData(const QString name) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(name.toStdString().c_str()))
            return NULL;
        return data[name.toStdString().c_str()].GetInt();
    }

    /*!
     * \brief Return la valeur d'une variable (bool)
     * \param name Nom de la variable à check
     * \return Sa valeur (bool)
     */
    bool getBoolData(const QString name) const {
        Document data;
        GetDataDocument(&data);
        if(!data.HasMember(name.toStdString().c_str()))
            return false;
        return data[name.toStdString().c_str()].GetBool();
    }

};

#endif // DATAMANAGER_H
