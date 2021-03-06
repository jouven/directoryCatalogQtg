#message($$QMAKESPEC)
QT += widgets

!android:QMAKE_CXXFLAGS += -std=c++17
android:QMAKE_CXXFLAGS += -std=c++14

CONFIG -= app_bundle
CONFIG += no_keywords
#(only windows) fixes the extra tier of debug and release build directories inside the first build directories
win32:CONFIG -= debug_and_release

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS       = \
    window.hpp \
    catalogData.hpp \
    appConfig.hpp
SOURCES       = main.cpp \
                window.cpp \
    catalogData.cpp \
    appConfig.cpp

!win32:MYPATH = "/"
win32:MYPATH = "H:/veryuseddata/portable/msys64/"

#mine
INCLUDEPATH += $${MYPATH}home/jouven/mylibs/include

if (!android){
#don't new line the "{"
#release
CONFIG(release, debug|release){
    LIBS += -L$${MYPATH}home/jouven/mylibs/release
    DEPENDPATH += $${MYPATH}home/jouven/mylibs/release
    QMAKE_RPATHDIR += $${MYPATH}home/jouven/mylibs/release
}
#debug
CONFIG(debug, debug|release){
    LIBS += -L$${MYPATH}home/jouven/mylibs/debug/ -lbackwardSTso -ltimeso
!win32:LIBS += -lboost_date_time
win32:LIBS += -lboost_date_time-mt
    DEPENDPATH += $${MYPATH}home/jouven/mylibs/debug
    QMAKE_RPATHDIR += $${MYPATH}home/jouven/mylibs/debug
    #QMAKE_LFLAGS += -rdynamic
    DEFINES += DEBUGJOUVEN
}
}

if (android){
#release
CONFIG(release, debug|release){
    LIBS += -L$${MYPATH}home/jouven/mylibsAndroid/release
    DEPENDPATH += $${MYPATH}home/jouven/mylibsAndroid/release
    QMAKE_RPATHDIR += $${MYPATH}home/jouven/mylibsAndroid/release
}
#debug
CONFIG(debug, debug|release){
    LIBS += -L$${MYPATH}home/jouven/mylibsAndroid/debug
    DEPENDPATH += $${MYPATH}home/jouven/mylibsAndroid/debug
    QMAKE_RPATHDIR += $${MYPATH}home/jouven/mylibsAndroid/debug
    DEFINES += DEBUGJOUVEN
}

}

LIBS += -lessentialQtso -lsignalso -lfileHashQtso -lthreadedFunctionQtso

QMAKE_CXXFLAGS_DEBUG -= -g
QMAKE_CXXFLAGS_DEBUG += -pedantic -Wall -Wextra -g3

linux:QMAKE_CXXFLAGS_RELEASE += -flto=jobserver
win32:QMAKE_CXXFLAGS_RELEASE += -flto
!android:QMAKE_CXXFLAGS_RELEASE += -mtune=sandybridge

#for -flto=jobserver in the link step to work with -jX
linux:!android:QMAKE_LINK = +g++

linux:QMAKE_LFLAGS += -fuse-ld=gold
QMAKE_LFLAGS_RELEASE += -fvisibility=hidden

linux:QMAKE_LFLAGS_RELEASE += -flto=jobserver
win32:QMAKE_LFLAGS_RELEASE += -flto

#Android stuff, for an executable project to work with shared libraries
#the library files used by the project must be manually
#specified
#this is done in the project tab I just add debug/release files,
#then I copy this code and change release <-> debug to get the other
if (android){
#release
CONFIG(release, debug|release){
contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../../mylibsAndroid/release/libbaseClassQtso.so \
        $$PWD/../../mylibsAndroid/release/libcrc32cso.so \
        $$PWD/../../mylibsAndroid/release/libcriptoQtso.so \
        $$PWD/../../mylibsAndroid/release/libessentialQtso.so \
        $$PWD/../../mylibsAndroid/release/libfileHashQtso.so \
        $$PWD/../../mylibsAndroid/release/libqmutexUMapQtso.so \
        $$PWD/../../mylibsAndroid/release/libsignalso.so \
        $$PWD/../../mylibsAndroid/release/libthreadedFunctionQtso.so \
        $$PWD/../../mylibsAndroid/release/libxxhashso.so
}
}

#debug
CONFIG(debug, debug|release){
contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../../mylibsAndroid/debug/libbaseClassQtso.so \
        $$PWD/../../mylibsAndroid/debug/libcrc32cso.so \
        $$PWD/../../mylibsAndroid/debug/libcriptoQtso.so \
        $$PWD/../../mylibsAndroid/debug/libessentialQtso.so \
        $$PWD/../../mylibsAndroid/debug/libfileHashQtso.so \
        $$PWD/../../mylibsAndroid/debug/libqmutexUMapQtso.so \
        $$PWD/../../mylibsAndroid/debug/libsignalso.so \
        $$PWD/../../mylibsAndroid/debug/libthreadedFunctionQtso.so \
        $$PWD/../../mylibsAndroid/debug/libxxhashso.so
}
}

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

}


