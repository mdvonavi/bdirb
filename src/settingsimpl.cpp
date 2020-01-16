#include "settingsimpl.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtGui>
#include <QtSql>

SettingsImpl::SettingsImpl( QWidget * parent, Qt::WFlags f)
    : QDialog(parent, f)
{
    setupUi(this);
    cbSex->addItem(tr("M"));
    cbSex->addItem(tr("F"));
}

void SettingsImpl::on_pbOK_clicked()
{
    QSqlQuery qwe;
    qwe.prepare("insert into pupils(Name, CardNo, Birthday, Sex, Info) values("
                ":Name, :No, :BD, :Sex, :Info)");
    qwe.bindValue(":Name",leName->text());
    qwe.bindValue(":No",leNo->text());
    qwe.bindValue(":BD",deBD->date());
    qwe.bindValue(":Sex",cbSex->currentText());
    qwe.bindValue(":Info",teInfo->toHtml());
    qwe.exec();
    SettingsImpl::close();
}

