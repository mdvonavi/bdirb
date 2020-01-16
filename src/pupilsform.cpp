#include <QtGui>
#include "pupilsform.h"
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
#include "measform.h"
#include "addmeas.h"
#include "settingsimpl.h"

extern int idPup;
extern int idPup1;
extern int idM;
extern int idMeas;
extern bool comen;
extern bool tven;
extern bool daten;
extern bool line_pb;
int curid;


PupilsForm::PupilsForm( QWidget * parent, Qt::WFlags f)
        : QDialog(parent, f)
{
    setupUi(this);

    t_model = new QSqlTableModel;
    t_model->setTable("typeMeasurement");

    tm_pupils = new QSqlTableModel;
    tm_pupils->setTable("pupils");

    tm_pupils->setEditStrategy(QSqlTableModel::OnFieldChange);
    tm_pupils->select();

    tm_pupils->setHeaderData(1, Qt::Horizontal, tr("Name"));
    tm_pupils->setHeaderData(2, Qt::Horizontal, tr("Card No"));
    tm_pupils->setHeaderData(3, Qt::Horizontal, tr("Birthday"));
    tm_pupils->setHeaderData(4, Qt::Horizontal, tr("Sex"));

    tvPupils->setModel(tm_pupils);
    tvPupils->resizeColumnsToContents();

    tvPupils->hideColumn(0);
    tvPupils->hideColumn(5);




    tm_mpupils = new QSqlRelationalTableModel;
    tm_mpupils->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
    tm_mpupils->setTable("Measurement");
    tm_mpupils->setHeaderData(1, Qt::Horizontal, tr("Investigation"));
    tm_mpupils->setHeaderData(3, Qt::Horizontal, tr("Date"));

    tm_mpupils->setRelation(1, QSqlRelation("typeMeasurement", "id", "name"));
    tm_mpupils->setFilter(QString("idPupils=%2")
                          .arg(tm_pupils->data(
                                  tm_pupils->index(0, 0)).toInt()));

    tvMPupils->setModel(tm_mpupils);
    tvMPupils->setItemDelegate(new QSqlRelationalDelegate(tvMPupils));//штука для создания выпадающего списка

    tvMPupils->hideColumn(0);
    tvMPupils->hideColumn(2);
    tvMPupils->hideColumn(4);

    tvMPupils->resizeColumnsToContents();
}

void PupilsForm::on_pbAdd_clicked()
{
    SettingsImpl *settings = new(SettingsImpl);
    settings->exec();
    tm_pupils->select();
}

void PupilsForm::on_pbDel_clicked()
{
    if (QMessageBox::question(0, tr("Delete under investigation warning!"),
                              tr("The selected under investigation now will\n"
                                 "be removed. You are assured?"),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
        == QMessageBox::Yes)

        {
        int id=tm_pupils->data(tm_pupils->index(tvPupils->currentIndex().row(), 0)).toInt();
        QSqlQuery qwe;

        qwe.prepare("delete from measurement where idPupils=:id");
        qwe.bindValue(":id",id);

        qwe.exec();

        int row = tvPupils->currentIndex().row();
        tm_pupils->removeRows(row, 1);
        tvPupils->selectRow(row);

    }
}

void PupilsForm::on_pbAdd_m_clicked()
{
    idPup = tm_pupils->data(
            tm_pupils->index(
                    tvPupils->currentIndex()
                    .row(), 0)).toInt();

    idPup1 = tm_pupils->data(
            tm_pupils->index(
                    tvPupils->currentIndex()
                    .row(), 0)).toInt();

    idM = tm_mpupils->data(
            tm_mpupils->index(
                    tvMPupils->currentIndex()
                    .row(), 0)).toInt();

    AddMeas *addmeas = new(AddMeas);
    addmeas->exec();

    tm_mpupils->select();
}

void PupilsForm::on_pbDel_m_clicked()
{
    if (QMessageBox::question(0, tr("Delete parametre warning!"),
                              tr("The selected parametre now will\n"
                                 "be removed. You are assured?"),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
        == QMessageBox::Yes)

        {int id=tm_pupils->data(tm_pupils->index(tvPupils->currentIndex().row(), 0)).toInt();
        if (id>0)
        {
            int row = tvMPupils->currentIndex().row();
            tm_mpupils->removeRows(row, 1);
            tvMPupils->selectRow(row);
        }

    }

}

void PupilsForm::on_tvPupils_clicked(QModelIndex index)
{
    int id = tm_pupils->data(
                tm_pupils->index(
                        tvPupils->currentIndex().row(), 0
                        )
                ).toInt();

    tm_mpupils->setFilter(QString("idPupils=%2").arg(id));
    tm_mpupils->select();

    QSqlQuery qwe;

    qwe.prepare("select * from pupils where id=:id");
    qwe.bindValue(":id",id);

    qwe.exec();
    qwe.first();

    teInfo->setText(qwe.value(5).toString());

    tvMPupils->resizeColumnsToContents();
}

void PupilsForm::on_pbEdit_clicked()
{
    idPup = tm_pupils->data(tm_pupils->index(tvPupils->currentIndex().row(), 0)).toInt();
    idM = tm_mpupils->data(tm_mpupils->index(tvMPupils->currentIndex().row(), 0)).toInt();

    QString str;
    QSqlQuery qwe;

    qwe.exec("select * from Measurement where id="+ str.setNum(idM) +"");
    qwe.first();
    idMeas=qwe.value(1).toInt();

if (idPup==0 or idM==0 or idMeas==0)
{
    QMessageBox::information(this,
                             tr("Info"),
                             tr("Please, choose investigation\n"
                                "and under investigation"));
}
else
{
    MeasForm *measform = new(MeasForm);
    measform->exec();
    tm_mpupils->select();
}

}

void PupilsForm::on_pbOK_clicked()
{
    PupilsForm::close();
}

void PupilsForm::on_pbSave_p_clicked()
{
    QSqlQuery qwe;
    qwe.prepare("update pupils set Info=:Info where id=:id");
    qwe.bindValue(":id",tm_pupils->data(tm_pupils->index(tvPupils->currentIndex().row(), 0)).toInt());
    qwe.bindValue(":Info",teInfo->toHtml());
    qwe.exec();
}
