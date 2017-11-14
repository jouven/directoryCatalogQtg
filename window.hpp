#ifndef DIRECTORYCATALOGQTG_WINDOW_HPP
#define DIRECTORYCATALOGQTG_WINDOW_HPP

//#include "threadedFunctionQtso/threadedFunctionQt.hpp"

#include <QWidget>

class QComboBox;
class QPushButton;
class QLabel;
class QVBoxLayout;
class QCheckBox;

//class browseFileDialogThread_c : public threadedFunction_c
//{
//    Q_OBJECT

//    void run() override;
//public:
//    browseFileDialogThread_c(std::function<void()> func_par_con, QObject *parent_par = nullptr);
//};

//class resizingStatusBar_c : public QStatusBar
//{
//    Q_OBJECT
//    void resizeEvent(QResizeEvent *e) override;
//};

class Window_c : public QWidget
{
    Q_OBJECT

    QVBoxLayout* mainLayout_pri;

    QPushButton* browseDirectoryButton_pri;
    QPushButton* catalogButton_pri;
    QComboBox* directoryComboBox_pri;
    QLabel* statusBarLabel_pri;
    QCheckBox* generateHash_pri;

    bool generating_pri = false;
    bool saving_pri = false;

    int finalCounterSeconds_pri = 1;

    void closeEvent(QCloseEvent *event) override;
public:
    Window_c();
    ~Window_c();
private slots:
    void browseDirectory_f();
    void generateCatalog_f();
    void mainLoop_f();
    //void browseDirectoryThreaded_f();
private:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void addDirectoryToCombobox_f(const QString &directory_par_con);
};

#endif //DIRECTORYCATALOGQTG_WINDOW_HPP
