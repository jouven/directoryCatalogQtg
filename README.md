# directoryCatalogQtg
GUI program to generate a JSON file with the list of files, and some of its details, of a folder

Compilation
-----------
Requires:

Qt library

https://github.com/jouven/criptoQtso

https://github.com/jouven/essentialQtso

https://github.com/jouven/signalso

https://github.com/jouven/fileHashQtso

https://github.com/jouven/threadedFunctionQtso

Run (in directoryCatalogQtg source directory or pointing to it):

    qmake

and then:

    make

Images
------
*Linux Xfce4*

![](../master/images/Screenshot_linux_xfce4.png)

*Windows 10*

![](../master/images/Screenshot_windows10.PNG)

Catalog file
------------
```javascript
{
    "files": [
        {
            "filename": ".qmake.stash",
            "hash": "14035345046794946951",
            "lastModificationDatetime": "2017-11-10 21:21:32",
            "size": "739"
        },
        {
            "filename": "Makefile",
            "hash": "12576195940119551167",
            "lastModificationDatetime": "2017-11-14 23:05:18",
            "size": "23937"
        },
        {
            "filename": "appConfig.o",
            "hash": "1235621528237230182",
            "lastModificationDatetime": "2017-11-14 23:05:07",
            "size": "1271656"
        },
        {
            "filename": "catalogData.o",
            "hash": "5297590266855046189",
            "lastModificationDatetime": "2017-11-14 23:05:07",
            "size": "1457656"
        },
        {
            "filename": "catalogFile.json",
            "hash": "3258556045129348439",
            "lastModificationDatetime": "2017-11-14 15:24:49",
            "size": "992772"
        },
        {
            "filename": "catalogFile2.json",
            "hash": "11638744431102265027",
            "lastModificationDatetime": "2017-11-14 15:13:55",
            "size": "8367959"
        },
        {
            "filename": "catalogFile3.json",
            "hash": "18390950545472483372",
            "lastModificationDatetime": "2017-11-14 15:18:45",
            "size": "6908073"
        },
        {
            "filename": "catalogFile4.json",
            "hash": "6699401177950713264",
            "lastModificationDatetime": "2017-11-14 15:17:37",
            "size": "5576635"
        },
        {
            "filename": "catalogFile5.json",
            "hash": "12387807938540885351",
            "lastModificationDatetime": "2017-11-14 15:12:22",
            "size": "6908071"
        },
        {
            "filename": "catalogFile6.json",
            "hash": "6699401177950713264",
            "lastModificationDatetime": "2017-11-14 15:09:49",
            "size": "5576635"
        },
        {
            "filename": "catalogFileNoHash.json",
            "hash": "15581035423845398133",
            "lastModificationDatetime": "2017-11-14 23:07:11",
            "size": "28356168"
        },
        {
            "filename": "directoryCatalogQtg",
            "hash": "10566853107534298904",
            "lastModificationDatetime": "2017-11-15 14:59:22",
            "size": "3120352"
        },
        {
            "filename": "directoryCatalogQtg_config.json",
            "hash": "322645246627266089",
            "lastModificationDatetime": "2017-11-14 23:07:48",
            "size": "498"
        },
        {
            "filename": "main.o",
            "hash": "8905659979821436189",
            "lastModificationDatetime": "2017-11-14 23:05:07",
            "size": "1376680"
        },
        {
            "filename": "moc_catalogData.cpp",
            "hash": "12844752980126139892",
            "lastModificationDatetime": "2017-11-14 23:05:19",
            "size": "2673"
        },
        {
            "filename": "moc_catalogData.o",
            "hash": "15243409668584778740",
            "lastModificationDatetime": "2017-11-14 23:05:20",
            "size": "1021728"
        },
        {
            "filename": "moc_predefs.h",
            "hash": "18081407709792551049",
            "lastModificationDatetime": "2017-11-12 04:52:34",
            "size": "14536"
        },
        {
            "filename": "moc_window.cpp",
            "hash": "6369177533455107402",
            "lastModificationDatetime": "2017-11-14 23:05:07",
            "size": "3699"
        },
        {
            "filename": "moc_window.o",
            "hash": "4068235118274574788",
            "lastModificationDatetime": "2017-11-14 23:05:08",
            "size": "1154344"
        },
        {
            "filename": "qtLauncher.o",
            "hash": "16952112400084240355",
            "lastModificationDatetime": "2017-11-11 14:05:44",
            "size": "1354264"
        },
        {
            "filename": "window.o",
            "hash": "3745929485380207429",
            "lastModificationDatetime": "2017-11-15 14:59:22",
            "size": "2815200"
        }
    ],
    "hashType": "xxhash",
    "root": "/home/jouven/qtcreatorprojects/directoryCatalogQtg/build-Desktop-Debug"
}
```
