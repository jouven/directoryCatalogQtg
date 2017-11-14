#include "window.hpp"

#include "signalso/signal.hpp"
#include "essentialQtso/essentialQt.hpp"

#include <QApplication>
//#include <QString>

namespace
{

}

int main(int argc, char *argv[])
{
    MACRO_signalHandler
    //qSetMessagePattern(QString("[%{type}] %{appname} (%{file}:%{line}) - %{message} %{backtrace}"));

    QApplication qtapp(argc, argv);
    QApplication::setApplicationName("directoryCatalogQtg");
    QApplication::setApplicationVersion("1.0");

    {
        QCommandLineParser commandLineParser;
        commandLineParser.setApplicationDescription("directoryCatalogQtg, GUI program to generate a JSON file with the list of files, and some of its details, of a folder");
        commandLineParser.addHelpOption();
        commandLineParser.addVersionOption();
        commandLineParser.addPositionalArgument("target", "Optional, config.json full path, by default it tries to read this file on the same path where the executable is");

        commandLineParser.process(*qApp);

        locateConfigFilePath_f(commandLineParser, true);
    }

    Window_c window;
    window.show();
    returnValue_ext = qtapp.exec();

    if (eines::signal::isRunning_f())
    {
        eines::signal::stopRunning_f();
    }
    while (not eines::signal::isTheEnd_f())
    {}

    return returnValue_ext;
}

//when running all the qt stuff (except the #include <QApplication>)
//in another thread there is the issue of a warning
//when exiting, I guess?... it's nothing serious...?,
//because it only happens when a widget is added to the window
//and an event loop cycle happens.
//During this event loop it seems that Qt adds a timer to a "global list",
//instead of adding it to the QApplication object, the parent or the same object... anyway
//when the QApplication/Window_c are destroyed this is not destroyed so
//at the end of the program this "global list" or "whatever it is object" is destroyed
//it detects a timer was added from
//a different threadId (prints the warning)
//and because of that it can't be stopped?/deleted?/free/whatever...
//why it can't? I don't know, it's just because.
