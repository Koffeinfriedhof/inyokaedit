/****************************************************************************
*
*   Copyright (C) 2011 by the respective authors (see AUTHORS)
*
*   This file is part of InyokaEdit.
*
*   InyokaEdit is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   InyokaEdit is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with InyokaEdit.  If not, see <http://www.gnu.org/licenses/>.
*
****************************************************************************/

/***************************************************************************
* File Name:  main.cpp
* Purpose:    Main function, start application
***************************************************************************/

#include <QApplication>

// For global translation (Qt MessageBox etc.)
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

#include "CInyokaEdit.h"

int main(int argc, char *argv[])
{
    // Resource file (images, icons)
    Q_INIT_RESOURCE(inyokaeditresources);

    // New application
    QApplication app(argc, argv);
    app.setApplicationName("InyokaEdit");

    QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") );
    QTextCodec::setCodecForTr( QTextCodec::codecForName("UTF-8") );

    // Load global translation
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    // Load app translation if it exists
    QTranslator myAppTranslator;
    myAppTranslator.load(app.applicationName().toLower() + "_" + QLocale::system().name(), "/usr/share/" + app.applicationName().toLower() + "/lang");
    app.installTranslator(&myAppTranslator);

    // New object of InyokaEdit
    CInyokaEdit myInyokaEdit(&app);
    myInyokaEdit.show();

    return app.exec();
}