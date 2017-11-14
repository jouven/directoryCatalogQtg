#ifndef DIRECTORYCATALOGQTG_APPCONFIG_HPP
#define DIRECTORYCATALOGQTG_APPCONFIG_HPP

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QJsonObject>

class appConfig_c
{
    bool configLoaded_pri = false;

    QByteArray windowGeometry_pri;
    bool windowGeometrySet_pri = false;

    QStringList recentlyCataloguedList_pri;
    bool recentlyCataloguedListSet_pri = false;

    void loadConfig_f();

    void read_f(const QJsonObject &json);
    void write_f(QJsonObject &json) const;
    bool anythingSet_f() const;
public:
    appConfig_c();
    //~appConfig_c();

    bool saveConfigFile_f() const;

    bool configLoaded_f() const;
//    appConfig_c(
//            const QByteArray& windowGeometry_par_con
//            , const QStringList& recentlyCataloguedList_par_con);

    QByteArray windowGeometry_f() const;
    void setWindowGeometry_f(const QByteArray& windowGeometry_par_con);
    bool windowGeometrySet_f() const;

    QStringList recentlyCataloguedList_f() const;
    void setRecentlyCataloguedList_f(const QStringList& recentlyCataloguedList_par_con);
    bool recentlyCataloguedListSet_f() const;

};

extern appConfig_c& appConfig_f();

#endif // DIRECTORYCATALOGQTG_APPCONFIG_HPP
