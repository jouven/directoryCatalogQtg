#ifndef DIRECTORYCATALOGQTG_CATALOGDATA_HPP
#define DIRECTORYCATALOGQTG_CATALOGDATA_HPP

#include <QString>
#include <QJsonObject>
#include <QFileInfo>
#include <QMutexLocker>

#include <string>
#include <vector>

class catalogFile_c
{
    QString filename_pri;
    int_fast64_t fileSize_pri = 0;
    uint_fast64_t numericHash_pri = 0;
    bool hashGenerated_pri = false;
    QString lastModifiedDateTime_pri;
public:
    catalogFile_c(const QString& filename_par_con
                  , const int_fast64_t fileSize_par_con
                  , const uint_fast64_t numericHash_par_con
                  , const QString& lastModifiedDateTime_par_con);
    catalogFile_c(const QString& filename_par_con
                  , const int_fast64_t fileSize_par_con
                  , const QString& lastModifiedDateTime_par_con);
    void write_f(QJsonObject &json) const;
};

//root for the data, because Qt doesn't support just a json array "write" ([value1,value2])
//it needs to be on a key-value ({jsonArray: [value1,value2]})
class catalog_c
{
    QString rootDirectory_pri;
    std::vector<catalogFile_c> catalogFiles_pri;
    QString hashType_pri;
public:
    catalog_c(const QString& rootDirectory_par_con
              , const std::vector<catalogFile_c>& catalogFiles_par_con
              , const QString& hashType_par_con);
    void write_f(QJsonObject &json) const;
};

class cataloguer_c : public QObject
{
    Q_OBJECT

    QString currentDirectory_pri;
    QMutex currentDirectoryQMutex_pri;
public:

    //source_par_con must exist and must be a directory otherwise the return value will be empty
    std::pair<std::vector<catalogFile_c>, bool> catalogDirectory_f(
            const QFileInfo& source_par_con
            , const bool generateHash_par_con = true
            //, const bool useRelativePaths_par_con = true
            , const bool includeSubdirectories_par_con = true
            , const QStringList& filenameFilters_par_con = QStringList()
            , const QString& includeDirectoriesWithFileX_par_con = QString());

    QString currentDirectory_f();
};

#endif // DIRECTORYCATALOGQTG_CATALOGDATA_HPP
