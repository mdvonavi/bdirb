#include <QtGui>
#include "search.h"
#include <QDebug>
#include <QSqlTableModel>
#include <QtSql>
#include <QSqlRelationalTableModel>
#include "measform.h"

extern int idPup;
extern int idM;
extern int idMeas;

Search::Search(QWidget *parent)
    : QWizard(parent)
{
    setupUi(this);
    qDebug() << this->pageIds();
    this->setStartId(1);


#ifndef Q_WS_MAC
    setWizardStyle(ModernStyle);
#endif

    setWindowTitle(tr("License Wizard"));

    m_search = new QSqlTableModel;
    m_search->setTable("typeMeasurement");
    m_search->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_search->setHeaderData(1, Qt::Horizontal, tr("Measurement"));
    m_search->setHeaderData(2, Qt::Horizontal, tr("Comment"));
    m_search->select();

    tvSearch->setModel(m_search);
    tvSearch->hideColumn(0);
    tvSearch->resizeColumnsToContents();

    r_search = new QSqlRelationalTableModel;
}

int Search::nextId() const
{
    if (this->currentId()==1) return 2;

    if (this->currentId()==2)

    {

        QSqlQuery qwer;

        QString query;
        QString string;
        QString lst_sex;
        QString lst_name;
        QString lst_age;
        QString tmp;

        QList <int> list_sex;
        QList <int> list_age;
        QList <int> list_name;

        int idM = m_search->data(m_search->index(tvSearch->currentIndex().row(), 0)).toInt();

        query = "select * from measurement where idTM=:idM";

        r_search->setTable("Measurement");
        r_search->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
        r_search->setRelation(2,QSqlRelation("pupils", "id", "Name"));



        if (gbSex->isChecked()==true)

        {

            QSqlQuery qwe;

            qwe.prepare("select * from measurement where idTM=:idM"
                        " and idpupils in (select id from pupils where sex=:sex)");
            qwe.bindValue(":idM",idM);
            if (rbFem->isChecked()) {qwe.bindValue(":sex",tr("F"));}
            if (rbMel->isChecked()) {qwe.bindValue(":sex",tr("M"));}
            qwe.exec();
            qwe.first();

            if (qwe.value(2).toInt()!=0) list_sex<<qwe.value(2).toInt();
            while (qwe.next())
            {
                if (qwe.value(2).toInt()!=0) list_sex<<qwe.value(2).toInt();
            }

            lst_sex="(";
            for (int i=0 ;i<list_sex.length()-1 ;i++ )
            {
                if (list_sex.length()==1) {lst_sex+=tmp.setNum(list_sex[i]);}
                else {lst_sex+=tmp.setNum(list_sex[i])+",";}
            }
            lst_sex+=tmp.setNum(list_sex[list_sex.length()-1]);
            lst_sex+=")";
        }

        if (gbName->isChecked()==true)

        {
            QSqlQuery qwe;
            qwe.prepare("select * from measurement where idTM=:idM"
                        " and idpupils in (select id from pupils where Name LIKE '%"+lineEdit->text()+"%')");
            qwe.bindValue(":idM",idM);
            qwe.exec();

            qwe.first();

            if (qwe.value(2).toInt()) list_name<<qwe.value(2).toInt();
            while (qwe.next())
            {
                if (qwe.value(2).toInt()) list_name<<qwe.value(2).toInt();
            }

            lst_name="(";
            if (list_name.length()!=0)
            {
            for (int i=0 ;i<list_name.length()-1 ;i++ )
            {
                if (list_name.length()==1) {lst_name+=tmp.setNum(list_name[i]);}
                else {lst_name+=tmp.setNum(list_name[i])+",";}
            }
            lst_name+=tmp.setNum(list_name[list_name.length()-1]);
        }
            lst_name+=")";
        }

        if (gbAge->isChecked()==true)

        {

            int from = leFrom->text().toInt();
            int to = leTo->text().toInt();

            QSqlQuery qwe;
            QSqlQuery qwert;
            QString date;

            qwe.prepare("select count(*) from measurement where idTM=:idM");
            qwe.bindValue(":idM",idM);
            qwe.exec();

            qwe.first();

            int count = qwe.value(0).toInt();

            qwe.prepare("select * from measurement where idTM=:idM");
            qwe.bindValue(":idM",idM);
            qwe.exec();

            qwe.first();

            for (int i=0 ;i<count ;i++ )
            {
                QString strin;
                int idPupuls=qwe.value(2).toInt();

                date = qwe.value(3).toString();

                qwert.exec("select * from pupils where id="+strin.setNum(idPupuls) +" and BirthDay between "
                           "(select date('"+date+"','-6 month','-"+string.setNum(to-1)+" years')) and "
                           "(select date('"+date+"','-6 month','-"+string.setNum(from-1)+" years'))");

                qwert.first();

                if (qwert.value(0).toInt()!=0)

                {
                    list_age<<qwert.value(0).toInt();
                }
                qwe.next();
            }


            lst_age="(";
            for (int i=0 ;i<list_age.length()-1 ;i++ )
            {
                if (list_age.length()==1) {lst_age+=tmp.setNum(list_age[i]);}
                else {lst_age+=tmp.setNum(list_age[i])+",";}
            }
            lst_age+=tmp.setNum(list_age[list_age.length()-1]);
            lst_age+=")";
        }

        QString filter;

        filter = QString("idTM=%1").arg(idM);

        if (gbAge->isChecked())
        {
            filter += QString(" and idPupils in %1").arg(lst_age);
        }

        if (gbSex->isChecked())
        {
            filter += QString(" and idPupils in %1").arg(lst_sex);
        }

        if (gbName->isChecked())
        {
            filter += QString(" and idPupils in %1").arg(lst_name);
        }

        if (gbDate->isChecked())
        {

            if (radioButton->isChecked())
            {
                filter += QString(" and DateMeas='%1'").arg(deFromDate->date().toString("yyyy-MM-dd"));
            }

            if (radioButton_2->isChecked())
            {
                filter += QString(" and DateMeas between '%1' and '%2'")
                          .arg(de_1->date().toString("yyyy-MM-dd"))
                          .arg(de_2->date().toString("yyyy-MM-dd"));
            }
        }

qDebug()<<filter;

r_search->setHeaderData(2, Qt::Horizontal, tr("Name"));
r_search->setHeaderData(3, Qt::Horizontal, tr("Date"));
r_search->select();
r_search->setFilter(filter);

tvResults->setModel(r_search);
tvResults->resizeColumnsToContents();


tvResults->hideColumn(0);
tvResults->hideColumn(1);
tvResults->hideColumn(4);


return 4;
}

    return -1;
}

void Search::on_pbOpen_clicked()
{
    idM = r_search->data(r_search->index(tvResults->currentIndex().row(), 0)).toInt();
    QSqlQuery qwe;

    qwe.prepare(QString("select * from Measurement where id=%1").arg(idM));
    qwe.exec();
    qwe.first();

    idPup=qwe.value(2).toInt();
    idMeas = m_search->data(m_search->index(tvSearch->currentIndex().row(), 0)).toInt();

    if (idPup==0 or idM==0 or idMeas==0)
    {
        QMessageBox::information(this,tr("Info"),tr("Can't open.\n"
                                                "No results"));
    }
    else
    {
        MeasForm *measform = new(MeasForm);
        measform->exec();
    }
}
