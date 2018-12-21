#include "catalogData.hpp"

//just for the hash function
#include "fileHashQtso/fileHashQt.hpp"
#ifdef DEBUGJOUVEN
#include "essentialQtso/essentialQt.hpp"
#endif

#include "signalso/signal.hpp"

#include <QDir>
#include <QJsonArray>
#include <QDateTime>

catalogFile_c::catalogFile_c(const QString &filename_par_con
                             , const int_fast64_t fileSize_par_con
                             , const uint_fast64_t numericHash_par_con
                             , const QString &lastModifiedDateTime_par_con)
    : filename_pri(filename_par_con)
    , fileSize_pri(fileSize_par_con)
    , numericHash_pri(numericHash_par_con)
    , hashGenerated_pri(true)
    , lastModifiedDateTime_pri(lastModifiedDateTime_par_con)
{}

catalogFile_c::catalogFile_c(
        const QString& filename_par_con
        , const int_fast64_t fileSize_par_con
        , const QString& lastModifiedDateTime_par_con)
    : filename_pri(filename_par_con)
    , fileSize_pri(fileSize_par_con)
    , lastModifiedDateTime_pri(lastModifiedDateTime_par_con)
{}

void catalogFile_c::write_f(QJsonObject& json) const
{
    json["filename"] = filename_pri;
    //json/javascript can't fit 64bit numbers, IEEE 754 shit, which is floating point, not integer, so it's less than a 64bit integer
    //so... use string notation
    json["size"] = QString::number(fileSize_pri);
    if (hashGenerated_pri)
    {
        json["hash"] = QString::number(numericHash_pri);
    }
    json["lastModificationDatetime"] = lastModifiedDateTime_pri;
}

catalog_c::catalog_c(const QString& rootDirectory_par_con
                     , const std::vector<catalogFile_c>& catalogFiles_par_con, const QString& hashType_par_con)
    : rootDirectory_pri(rootDirectory_par_con)
    , catalogFiles_pri(catalogFiles_par_con)
    , hashType_pri(hashType_par_con)
{}

void catalog_c::write_f(QJsonObject& json) const
{
    QJsonArray fileStatusArrayTmp;
    for (const catalogFile_c& catalogFile_ite_con : catalogFiles_pri)
    {
        QJsonObject jsonObjectTmp;
        catalogFile_ite_con.write_f(jsonObjectTmp);
        fileStatusArrayTmp.append(jsonObjectTmp);
    }
    json["files"] = fileStatusArrayTmp;
    json["root"] = rootDirectory_pri;
    json["hashType"] = hashType_pri;
}

std::pair<std::vector<catalogFile_c>,bool> cataloguer_c::catalogDirectory_f(
    const QFileInfo& source_par_con
    , const bool generateHash_par_con
    //, const bool useRelativePaths_par_con
    , const bool useSlashSeparator_par_con
    , const bool includeSubdirectories_par_con
    , const QStringList& filenameFilters_par_con
    , const QString& includeDirectoriesWithFileX_par_con)
{
    std::vector<catalogFile_c> result;
    bool success(false);
    if (not source_par_con.exists() or not source_par_con.isDir())
    {
        return {result, success};
    }

    QDir sourceDir(source_par_con.absoluteFilePath());

    //if the setting is empty
    //account it as it is found
    bool xFileFoundRoot(includeDirectoriesWithFileX_par_con.isEmpty());
    if (not xFileFoundRoot)
    {
        //source dir, get the root files
        QStringList rootFileListUnfiltered(sourceDir.entryList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot));
        for (const QString& filename_ite_con : rootFileListUnfiltered)
        {
            if (filename_ite_con == includeDirectoriesWithFileX_par_con)
            {
                xFileFoundRoot = true;
                break;
            }
        }
    }
    if (xFileFoundRoot)
    {
        //source dir, get the root files
        QStringList rootFileList(sourceDir.entryList(filenameFilters_par_con, QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name));
        for (const QString& filename_ite_con : rootFileList)
        {
#ifdef DEBUGJOUVEN
            //QOUT_TS("sourceDir.path() + '/' + filename_ite_con " << sourceDir.path() + '/' + filename_ite_con << endl);
#endif
            QFileInfo sourceFileTmp(sourceDir.path() + '/' + filename_ite_con);
            if (sourceFileTmp.exists() and sourceFileTmp.isFile())
            {
                if (generateHash_par_con)
                {
                    catalogFile_c catalogFileTmp
                    (
                        useSlashSeparator_par_con ? sourceDir.relativeFilePath(sourceFileTmp.filePath()) : QDir::toNativeSeparators(sourceDir.relativeFilePath(sourceFileTmp.filePath()))
                        , sourceFileTmp.size()
                        , getFileHash_f(sourceFileTmp.canonicalFilePath())
                        , sourceFileTmp.lastModified().toString("yyyy-MM-dd HH:mm:ss")
                    );
                    result.emplace_back(catalogFileTmp);
                }
                else
                {
                    catalogFile_c catalogFileTmp
                    (
                        useSlashSeparator_par_con ? sourceDir.relativeFilePath(sourceFileTmp.filePath()) : QDir::toNativeSeparators(sourceDir.relativeFilePath(sourceFileTmp.filePath()))
                        , sourceFileTmp.size()
                        , sourceFileTmp.lastModified().toString("yyyy-MM-dd HH:mm:ss")
                    );
                    result.emplace_back(catalogFileTmp);
                }
            }
            if (not signalso::isRunning_f())
            {
                return {result, success};
            }
        }
    }

    if (includeSubdirectories_par_con)
    {
        //source dir, get the all the subfolders (names) of the base folder
        QStringList subfolders(sourceDir.entryList(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks));

        //while subfolders keep being found
        while (not subfolders.isEmpty())
        {
            QStringList newSubfoldersTmp;
            //for every subfolder gathered
            for (const auto& subfolder_ite_con : subfolders)
            {
                //do a QDir of the subfolder, using the initial folder + all the subfolder "depth" that has been traveled/iterated
                QDir currentSubfolderDirTmp(source_par_con.absoluteFilePath() + '/' + subfolder_ite_con);

                //get the subfolders of the one it's iterating
                QStringList subFoldersTmp(currentSubfolderDirTmp.entryList(QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot | QDir::NoSymLinks));
                //for the found subfolder, prepend the previous subfolder path string
                for (auto& subfolderTmp_ite : subFoldersTmp)
                {
                    //qDebug() << "subfolder_ite_con + QDir::separator() + subfolderTmp_ite " << subfolder_ite_con + QDir::separator() + subfolderTmp_ite << endl;
                    //prepend the parent subfolder
                    subfolderTmp_ite.prepend(subfolder_ite_con + "/");
                }
                newSubfoldersTmp.append(subFoldersTmp);

                //if the setting is empty
                //account it as it is found
                bool xFileFoundSubdirectory(includeDirectoriesWithFileX_par_con.isEmpty());
                if (not xFileFoundSubdirectory)
                {
                    //get the files of the subfolder
                    QStringList subDirectoryFileListUnfiltered(currentSubfolderDirTmp.entryList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot));
                    for (const QString& filename_ite_con : subDirectoryFileListUnfiltered)
                    {
                        if (filename_ite_con == includeDirectoriesWithFileX_par_con)
                        {
                            xFileFoundSubdirectory = true;
                            break;
                        }
                    }
                }

                if (xFileFoundSubdirectory)
                {
                    {
                        QMutexLocker tmp(&currentDirectoryQMutex_pri);
                        currentDirectory_pri = currentSubfolderDirTmp.path();
                    }
                    //get the files of the subfolder
                    QStringList subDirectoryFileList(currentSubfolderDirTmp.entryList(filenameFilters_par_con, QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name));
                    for (const auto& filename_ite_con : subDirectoryFileList)
                    {
                        QFileInfo sourceFileTmp(sourceDir.path() + '/' + subfolder_ite_con + '/' + filename_ite_con);
                        if (sourceFileTmp.exists() and sourceFileTmp.isFile())
                        {
                            if (generateHash_par_con)
                            {
                                catalogFile_c catalogFileTmp
                                (
                                      useSlashSeparator_par_con ? sourceFileTmp.filePath() : QDir::toNativeSeparators(sourceDir.relativeFilePath(sourceFileTmp.filePath()))
                                      , sourceFileTmp.size()
                                      , getFileHash_f(sourceFileTmp.canonicalFilePath())
                                      , sourceFileTmp.lastModified().toString("yyyy-MM-dd HH:mm:ss")
                                );
                                result.emplace_back(catalogFileTmp);
                            }
                            else
                            {
                                catalogFile_c catalogFileTmp
                                (
                                      useSlashSeparator_par_con ? sourceFileTmp.filePath() : QDir::toNativeSeparators(sourceDir.relativeFilePath(sourceFileTmp.filePath()))
                                      , sourceFileTmp.size()
                                      , sourceFileTmp.lastModified().toString("yyyy-MM-dd HH:mm:ss")
                                );
                                result.emplace_back(catalogFileTmp);
                            }
                        }
                        if (not signalso::isRunning_f())
                        {
                            return {result, success};
                        }
                    }
                }
            }
            subfolders = newSubfoldersTmp;
        }
    }
    {
        QMutexLocker tmp(&currentDirectoryQMutex_pri);
        currentDirectory_pri.clear();
    }
    success = true;
    return {result, success};
}

QString cataloguer_c::currentDirectory_f()
{
    QMutexLocker tmp(&currentDirectoryQMutex_pri);
    return currentDirectory_pri;
}

