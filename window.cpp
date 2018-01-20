#include "window.hpp"

#include "catalogData.hpp"
#include "appConfig.hpp"

#include "signalso/signal.hpp"

#include "threadedFunctionQtso/threadedFunctionQt.hpp"

#include <QtWidgets>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>

void Window_c::closeEvent(QCloseEvent* event)
{
    if (eines::signal::isRunning_f())
    {
        eines::signal::stopRunning_f();
    }
    event->ignore();
}

Window_c::Window_c()
{
    statusBarLabel_pri = new QLabel;
    //statusBarLabel_pri->setWordWrap(true);

//    qInfo() << "1 statusBar()->size().width() " << statusBarLabel_pri->size().width() << endl;
//    qInfo() << "1 statusBar()->sizeHint().width() " << statusBarLabel_pri->sizeHint().width() << endl;
//    qInfo() << "1 statusBar()->geometry().width() " << statusBarLabel_pri->geometry().width() << endl;

    statusBarLabel_pri->setText(tr("Pick a folder to catalog, browse or drop a folder inside the window"));

//    qInfo() << "2 statusBar()->size().width() " << statusBarLabel_pri->size().width() << endl;
//    qInfo() << "2 statusBar()->sizeHint().width() " << statusBarLabel_pri->sizeHint().width() << endl;
//    qInfo() << "2 statusBar()->geometry().width() " << statusBarLabel_pri->geometry().width() << endl;

    QGridLayout* centralLayout = new QGridLayout;

    directoryComboBox_pri = new QComboBox();
    //directoryComboBox_pri->setEditable(true);
    directoryComboBox_pri->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    directoryComboBox_pri->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    centralLayout->addWidget(new QLabel(tr("Last/Recent")), 0, 0);
    centralLayout->addWidget(directoryComboBox_pri, 0, 1);

    connect(directoryComboBox_pri, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
          [=](int index_par)
    {
        if (index_par > 0)
        {
            QString textTmp(directoryComboBox_pri->itemText(index_par));
            directoryComboBox_pri->removeItem(index_par);
            directoryComboBox_pri->insertItem(0, textTmp);
            directoryComboBox_pri->setCurrentIndex(0);
        }
    });

    //add directory
    QIcon browseIcon = QIcon::fromTheme("document-open", QIcon(":/images/browse.png"));
    browseDirectoryButton_pri = new QPushButton(browseIcon, tr("Browse"));
    //browseDirectoryButton_pri->setFont(defaultButtonFont);
    centralLayout->addWidget(browseDirectoryButton_pri, 1, 0);
    QObject::connect(browseDirectoryButton_pri, &QPushButton::clicked, this, &Window_c::browseDirectory_f);

    //catalog Button
    //QIcon browseIcon = QIcon::fromTheme("document-open", QIcon(":/images/browse.png"));
    catalogButton_pri = new QPushButton(tr("Catalog"));
    //browseDirectoryButton_pri->setFont(defaultButtonFont);
    centralLayout->addWidget(catalogButton_pri, 1, 1);
    QObject::connect(catalogButton_pri, &QPushButton::clicked, this, &Window_c::generateCatalog_f);

    generateHashCheckBox_pri = new QCheckBox(tr("Generate hash (slower)"));
    centralLayout->addWidget(generateHashCheckBox_pri, 2, 0);
#ifdef Q_OS_WIN32
    useSlashSeparatorCheckbox_pri = new QCheckBox(tr("Use slash as folder separator"));
    centralLayout->addWidget(useSlashSeparatorCheckbox_pri, 2, 1);
#endif

    this->setAcceptDrops(true);

    mainLayout_pri = new QVBoxLayout;
    mainLayout_pri->addLayout(centralLayout);
    mainLayout_pri->addWidget(statusBarLabel_pri);
    this->setLayout(mainLayout_pri);

    setWindowTitle(tr("directoryCatalogQtg"));
    //const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    //resize(screenGeometry.width() / 3, 0);
    QTimer* mainLoopTimer = new QTimer(qApp);
    QObject::connect(mainLoopTimer, &QTimer::timeout, this, &Window_c::mainLoop_f);
    mainLoopTimer->start(1000);

    if (appConfig_f().configLoaded_f())
    {
        //qInfo() << "configFile_f().second" << endl;
        if (appConfig_f().windowGeometrySet_f())
        {
            //qInfo() << "appConfig_f().windowGeometry_f() " << appConfig_f().windowGeometry_f() << endl;
            restoreGeometry(appConfig_f().windowGeometry_f());
        }

        if (appConfig_f().recentlyCataloguedListSet_f())
        {
            //qInfo() << "appConfig_f().recentlyCataloguedListSet_f() " << appConfig_f().recentlyCataloguedListSet_f() << endl;
            directoryComboBox_pri->addItems(appConfig_f().recentlyCataloguedList_f());
        }
    }

//    QTimer::singleShot(1,[=]
//    {
//        statusBar()->resize(statusBar()->sizeHint().width(),statusBar()->sizeHint().height());
//        //statusBar()->updateGeometry();
//        //resize(minimumSizeHint());
//        adjustSize();
//        //updateGeometry();
    //    });
}

Window_c::~Window_c()
{
    appConfig_f().setWindowGeometry_f(saveGeometry());
    QStringList comboStringsTmp;
    comboStringsTmp.reserve(directoryComboBox_pri->count());
    for (auto i = 0, l = directoryComboBox_pri->count(); i < l; ++i)
    {
        comboStringsTmp.append(directoryComboBox_pri->itemText(i));
    }
    appConfig_f().setRecentlyCataloguedList_f(comboStringsTmp);
    appConfig_f().saveConfigFile_f();
    if (eines::signal::isRunning_f())
    {
        eines::signal::stopRunning_f();
    }
}

void Window_c::mainLoop_f()
{
    //qInfo() << "qThreadCount_f() " << qThreadCount_f() << endl;
//    qInfo() << "3 statusBar()->sizeHint().width() " << statusBarLabel_pri->sizeHint().width() << endl;
//    qInfo() << "3 statusBar()->geometry().width() " << statusBarLabel_pri->geometry().width() << endl;

    //statusBarLabel_pri->updateGeometry();
    //statusBar()->resize(statusBar()->sizeHint().width(),statusBar()->sizeHint().height());
    //adjustSize();
    //updateGeometry();
    if (finalCounterSeconds_pri == 0 and qThreadCount_f() == 0)
    {
        QApplication::exit();
    }
    if (not eines::signal::isRunning_f())
    {
        statusBarLabel_pri->setText(tr("Exiting..."));
        finalCounterSeconds_pri = finalCounterSeconds_pri - 1;
    }
}

void Window_c::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls() and not(saving_pri or generating_pri))
    {
        event->acceptProposedAction();
    }
}

void Window_c::dropEvent(QDropEvent* event)
{
    bool validDirectoryFound(false);
    QString possibleDirectory;

    for (const QUrl& url : event->mimeData()->urls())
    {
        if (url.isLocalFile())
        {
            possibleDirectory = url.toLocalFile();
            QFileInfo directoryInfoTmp(possibleDirectory);
            if (directoryInfoTmp.exists() and directoryInfoTmp.isDir())
            {

                validDirectoryFound = true;
                break;
            }
        }
    }
    if (validDirectoryFound)
    {
        addDirectoryToCombobox_f(possibleDirectory);
        statusBarLabel_pri->setText(tr("Directory added"));
    }
    else
    {
        statusBarLabel_pri->setText(tr("Dropped item is not a valid directory"));
    }
}

//qt doesn't like threading dialogs, so keep it on the same thread
//void Window_c::browseDirectoryThreaded_f()
//{
//    browseFileDialogThread_c* browseDirectoryThreadTmp = new browseFileDialogThread_c(
//                std::bind(&Window_c::browseDirectory_f, this)
//    , qApp);
//    QObject::connect(browseDirectoryThreadTmp, &QThread::destroyed, [this]
//    {
//        statusBar()->showMessage(tr("Directory added, thread destroyed"));
//    });
//    QObject::connect(browseDirectoryThreadTmp, &QThread::finished, browseDirectoryThreadTmp, &QThread::deleteLater);
//    browseDirectoryThreadTmp->start();
//}

void Window_c::browseDirectory_f()
{
    if (generating_pri or saving_pri)
    {
        statusBarLabel_pri->setText(tr("No browsing while generating a catalog"));
         return;
    }

    QFileDialog selectFolderDialogTmp;
    selectFolderDialogTmp.setFileMode(QFileDialog::Directory);
    if (directoryComboBox_pri->count() > 0)
    {
        selectFolderDialogTmp.setDirectory(QDir::fromNativeSeparators(directoryComboBox_pri->currentText()));
    }
    else
    {
        selectFolderDialogTmp.setDirectory(QDir::currentPath());
    }

    selectFolderDialogTmp.setWindowTitle(tr("Select folder"));
    selectFolderDialogTmp.setViewMode(QFileDialog::Detail);
    selectFolderDialogTmp.setFilter(QDir::Hidden | QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs | QDir::Drives);
    selectFolderDialogTmp.setOption(QFileDialog::DontUseNativeDialog, true);
    //selectFolderDialogTmp.setOption(QFileDialog::ShowDirsOnly, true);
    selectFolderDialogTmp.setOption(QFileDialog::ReadOnly, true);
#ifdef __ANDROID__
    selectFolderDialogTmp.setGeometry(QApplication::desktop()->availableGeometry(this));
#endif
    selectFolderDialogTmp.exec();

    if (selectFolderDialogTmp.result() == QDialog::Accepted)
    {
        QString directoryTmp;// = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, , QDir::currentPath()));
        if (not selectFolderDialogTmp.selectedFiles().isEmpty())
        {
            directoryTmp = selectFolderDialogTmp.selectedFiles().first();
        }

        if (not directoryTmp.isEmpty())
        {
            addDirectoryToCombobox_f(directoryTmp);
            statusBarLabel_pri->setText(tr("Directory added"));
        }
        else
        {
            statusBarLabel_pri->setText(tr("No directory to catalog selected"));
        }
    }
}

void Window_c::addDirectoryToCombobox_f(const QString& directory_par_con)
{
//    if (directoryComboBox_pri->findText(directory_par_con) == -1)
//    {

//    }
    QString nativeDirectoryTmp(QDir::toNativeSeparators(directory_par_con));
    auto findResultIndex(directoryComboBox_pri->findText(nativeDirectoryTmp));
    if (findResultIndex > 0)
    {
        directoryComboBox_pri->removeItem(findResultIndex);
    }
    directoryComboBox_pri->insertItem(0, nativeDirectoryTmp);
    directoryComboBox_pri->setCurrentIndex(0);
    //directoryComboBox_pri->setCurrentIndex(directoryComboBox_pri->findText(directory_par_con));

#ifndef __ANDROID__
    //it's necessary to wait for the execution to return to the event loop before
    //it can adjustSize properly
    QTimer::singleShot(0,[=]
    {
        //directoryComboBox_pri->adjustSize();
        adjustSize();
    });
#endif
}

void Window_c::generateCatalog_f()
{
    if (generating_pri or saving_pri)
    {
        statusBarLabel_pri->setText(tr("Already generating a catalog"));
        return;
    }
    QString directoryTmp;
    if (directoryComboBox_pri->currentIndex() == -1)
    {
        statusBarLabel_pri->setText(tr("Pick a folder to catalog, browse or drop a folder inside the window"));
        return;
    }
    else
    {
        directoryTmp = QDir::fromNativeSeparators(directoryComboBox_pri->currentText());
    }

    QFileDialog saveDialogTmp;
    saveDialogTmp.setAcceptMode(QFileDialog::AcceptSave);
    saveDialogTmp.setFileMode(QFileDialog::AnyFile);
    saveDialogTmp.setDirectory(QDir::currentPath());
    saveDialogTmp.selectFile("catalogFile.json");
    saveDialogTmp.setWindowTitle(tr("Save catalog file..."));
    saveDialogTmp.setViewMode(QFileDialog::Detail);
    saveDialogTmp.setFilter(QDir::Hidden | QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs | QDir::Drives);
    saveDialogTmp.setDefaultSuffix("json");
    saveDialogTmp.setOption(QFileDialog::DontUseNativeDialog, true);
#ifdef __ANDROID__
    saveDialogTmp.setGeometry(QApplication::desktop()->availableGeometry(this));
#endif
    saveDialogTmp.exec();

    while (saveDialogTmp.result() == QDialog::Accepted)
    {
        QString saveFileName;
        if (not saveDialogTmp.selectedFiles().isEmpty())
        {
            saveFileName = saveDialogTmp.selectedFiles().first();
        }

        //    QString saveFileName = QFileDialog::getSaveFileName(
        //                Q_NULLPTR
        //                , tr("Save directory catalog file")
        //                , QDir::currentPath()
        //                , tr("JSON (*.json)")
        //    );
        if (saveFileName.isEmpty())
        {
            statusBarLabel_pri->setText(tr("No Save file selected"));
            break;
        }

        QFileInfo directoryFI(directoryTmp);
        statusBarLabel_pri->setText(tr("Cataloging..."));
        cataloguer_c* cataloguerP = new cataloguer_c;

        threadedFunction_c* funcGenerateCatalog = new threadedFunction_c([=]
        {
#ifdef Q_OS_WIN32
            auto catalogTemp(cataloguerP->catalogDirectory_f(directoryFI, generateHashCheckBox_pri->isChecked(), useSlashSeparatorCheckbox_pri->isChecked()));
#else
            auto catalogTemp(cataloguerP->catalogDirectory_f(directoryFI, generateHashCheckBox_pri->isChecked()));
#endif
            saving_pri = true;
            generating_pri = false;
            if (not catalogTemp.second)
            {
                statusBarLabel_pri->setText(tr("Generation unsuccessful or aborted"));
            }
            else
            {
#ifdef Q_OS_WIN32
                catalog_c catalogTmp(useSlashSeparatorCheckbox_pri->isChecked() ? directoryTmp : QDir::toNativeSeparators(directoryTmp), catalogTemp.first, "xxhash");
#else
                catalog_c catalogTmp(QDir::toNativeSeparators(directoryTmp), catalogTemp.first, "xxhash");
#endif
                QFile saveFile(saveFileName);
                if (saveFile.open(QIODevice::WriteOnly))
                {
                    statusBarLabel_pri->setText(tr("Creating JSON save file..."));
                    QJsonObject somethingJson;
                    catalogTmp.write_f(somethingJson);
                    QJsonDocument somethingJsonD(somethingJson);
                    saveFile.write(somethingJsonD.toJson(QJsonDocument::Indented));
                    statusBarLabel_pri->setText(tr("File saved successfully"));
                }
                else
                {
                    statusBarLabel_pri->setText(tr("Couldn't create the save file"));
                }
            }
            saving_pri = false;
        });

        //this "works", but it might fail in future versions since it modifies GUI elements in a thread != mainthread
        //it should be changed to do emits so the changes are done in the main thread (like in hasherQtg)
        threadedFunction_c* funcDisplayProgres = new threadedFunction_c([=]
        {
            while (generating_pri and eines::signal::isRunning_f())
            {
                while (eines::signal::isRunning_f() and generating_pri)
                {

                    if (statusBarLabel_pri->text().startsWith("Cataloging..."))
                    {
                        statusBarLabel_pri->setText(tr("Cataloging.") + " "" "" " + cataloguerP->currentDirectory_f());
                        break;
                    }
                    if (statusBarLabel_pri->text().startsWith("Cataloging. "))
                    {
                        statusBarLabel_pri->setText(tr("Cataloging..") + " "" " + cataloguerP->currentDirectory_f());
                        break;
                    }
                    if (statusBarLabel_pri->text().startsWith("Cataloging.. "))
                    {
                        statusBarLabel_pri->setText(tr("Cataloging...") + " " + cataloguerP->currentDirectory_f());
                        break;
                    }
                    QThread::msleep(1000);
                    statusBarLabel_pri->setText(tr("Cataloging...") + " " + cataloguerP->currentDirectory_f());
                    break;
                }
                QThread::msleep(15);
            }
        });

        generating_pri = true;

        QObject::connect(funcDisplayProgres, &QThread::finished, funcDisplayProgres, &QThread::deleteLater);
        funcDisplayProgres->start();

        QObject::connect(funcGenerateCatalog, &QThread::finished, funcGenerateCatalog, &QThread::deleteLater);
        QObject::connect(funcGenerateCatalog, &QThread::finished, cataloguerP, &QThread::deleteLater);
        funcGenerateCatalog->start();
        break;
    }
}

//void browseFileDialogThread_c::run()
//{
//     func_pri();
//     exec();
//}

//browseFileDialogThread_c::browseFileDialogThread_c(std::function<void ()> func_par_con, QObject* parent_par)
// : threadedFunction_c(func_par_con, parent_par)
//{
//}

//void resizingStatusBar_c::resizeEvent(QResizeEvent* e)
//{
//    qInfo() << "5 statusBar()->size().width() " << this->size().width() << endl;
//    QSize sizeTmp(fontMetrics().boundingRect(currentMessage()).size());
//    this->setMinimumWidth(sizeTmp.width());
//}
