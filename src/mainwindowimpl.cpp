#include "mainwindowimpl.h"
#include "settingsimpl.h"
#include "mainlistsimpl.h"
#include "pupilsform.h"
#include "search.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QMessageBox>

MainWindowImpl::MainWindowImpl( QWidget * parent, Qt::WFlags f) 
	: QMainWindow(parent, f)
{
    setupUi(this);
menuMenu->setTitle(tr("Menu"));
actionExit->setText(tr("Exit"));
connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
connect(actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindowImpl::on_pbSettings_clicked()
{
    SettingsImpl *settings = new(SettingsImpl);
    settings->exec();
}

void MainWindowImpl::on_pbMainLists_clicked()
{
    MainListsImpl *mainlists = new(MainListsImpl);
    mainlists->exec();
}

void MainWindowImpl::on_pbPupils_clicked()
{
    PupilsForm *pupilsform = new(PupilsForm);
    pupilsform->exec();
}

void MainWindowImpl::on_pbSearch_clicked()
{
    Search * wizard = new Search(this);
    wizard->exec();

}
