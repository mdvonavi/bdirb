#include <QApplication>
#include <QTranslator>
#include "mainwindowimpl.h"
#include <QString>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtGui>
#include <QtSql>
#include "pupilsform.h"
#include "measform.h"
#include <QDebug>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    QTranslator translator;

    translator.load(QString("../ts/")+QLocale::system().name());
    app.installTranslator(&translator);
	
    MainWindowImpl win;
    win.showNormal();

    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    app.addLibraryPath( app.applicationDirPath() + "/plugins");//чтоб этот олень видел драйвер sql

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");
    db.open();
    if (!db.open())
    {
        QMessageBox::critical(0, "Cannot open database",
                                 "Cannot open database", QMessageBox::Cancel);
        return 0;

    }
    return app.exec();

}

