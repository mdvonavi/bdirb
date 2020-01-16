#include <QtGui>
#include "measform.h"
#include <QTableWidget>
#include "mainwindowimpl.h"
#include <QtCore>
#include <QtSql>
#include "mainlistsimpl.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include "pupilsform.h"
#include "addmeas.h"
int idPup1;


AddMeas::AddMeas( QWidget * parent, Qt::WFlags f)
    : QDialog(parent, f)
{
    setupUi(this);

    QSqlRelationalTableModel *measur = new QSqlRelationalTableModel;

    measur->setTable("typeMeasurement");
    measur->select();

    int measid = measur->fieldIndex("name");

    cbMeas->setModel(measur);
    cbMeas->setModelColumn(measid);
}

void AddMeas::on_pbOK_clicked()
{
    QString str;
    QSqlQuery qwer;
    int curid;

    str = cbMeas->currentText();
    qwer.exec("select * from typeMeasurement where name='"+ str +"'");
    qwer.first();

    curid = qwer.value(0).toInt();
    qwer.prepare("insert into Measurement(idTM, idPupils, DateMeas) values(:idtm, :idpup, :date)");
    qwer.bindValue(":idtm",curid);
    qwer.bindValue(":idpup",idPup1);
    qwer.bindValue(":date",dateEdit->date());
    qwer.exec();
    AddMeas::close();
}
