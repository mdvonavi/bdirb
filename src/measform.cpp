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
#include <QDebug>
#include <QTextDocumentWriter>

int idPup;
int idM;
int idMeas;
bool comen;
bool tven;
bool daten;
bool line_pb;
int tm_categories_count;
int tm_params_count;
int m_unit_count;

MeasForm::MeasForm( QWidget * parent, Qt::WFlags f)
        : QDialog(parent, f)
{
    setupUi(this);

    unit = new QSqlRelationalTableModel;
    unit->setTable("m_unit");
    unit->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
    unit->select();


    tw_unit->setColumnCount(1);
    tw_unit->setRowCount(1);

    tm_params = new QSqlTableModel;
    tm_params->setTable("tm_params");
    tm_params->select();

    QString str;
    QSqlQuery qwe;

    //установим нужную дату и комментарий
    qwe.exec("select * from measurement where id="+str.setNum(idM)+" and idPupils="+str.setNum(idPup));
    qwe.first();
    Date_m->setDate(QDate::fromString(qwe.value(3).toString(), "yyyy-MM-dd"));
    teComment->setText(qwe.value(4).toString());

    //заполним названия параметров
    qwe.exec("SELECT COUNT(*) FROM tm_params WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();
    tm_params_count=qwe.value(0).toInt();
    tw_unit->setRowCount(tm_params_count);

    qwe.exec("SELECT * FROM tm_params WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();

for (int i=0 ;i<tm_params_count ;i++ )
{
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(qwe.value(2).toString());
    tw_unit->setVerticalHeaderItem(i,item);

    map_t.insert(qwe.value(0).toInt(),i);
    map_t_r.insert(i, qwe.value(0).toInt());
    head_h.insert(i, qwe.value(2).toString());

    qwe.next();
}

    //заполним названия категорий
    qwe.exec("SELECT COUNT(*) FROM tm_categories WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();

    tm_categories_count=qwe.value(0).toInt();
    tw_unit->setColumnCount(tm_categories_count);

    qwe.exec("SELECT * FROM tm_categories WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();

for (int i=0 ;i<tm_categories_count ;i++ )
{
    QTableWidgetItem *item = new QTableWidgetItem;

    item->setText(qwe.value(2).toString());
    tw_unit->setHorizontalHeaderItem(i,item);

    map_c.insert(qwe.value(0).toInt(), i);
    map_c_r.insert(i, qwe.value(0).toInt());
    head_v.insert(i, qwe.value(2).toString());

    qwe.next();
}

    //узнаем, сколько есть интересующих нас записей
    qwe.exec("SELECT COUNT(*) FROM m_unit WHERE idM="+ str.setNum(idM) +"");
    qwe.first();
    m_unit_count=qwe.value(0).toInt();

    //заполним данные
    qwe.exec("SELECT * FROM m_unit WHERE idM="+ str.setNum(idM) +"");
    qwe.first();

for (int i=0 ;i< m_unit_count ;i++ )
{
    QTableWidgetItem *item = new QTableWidgetItem;

    int row = map_t.value(qwe.value(1).toInt());
    int col = map_c.value(qwe.value(2).toInt());

    item->setText(qwe.value(3).toString());
    if( (qwe.value(1).toInt() != map_t_r.value(row)) or (qwe.value(2).toInt() != map_c_r.value(col)) )
{}

else
{
    tw_unit->setItem(row,col,item);
}
    qwe.next();
}

    tw_unit->resizeColumnsToContents();
    tw_unit->resizeRowsToContents();

    qwe.exec("select * from typeMeasurement where id="+ str.setNum(idMeas));
    qwe.first();
    leType->setText(qwe.value(1).toString());
}


void MeasForm::on_pbOK_clicked()     
{
    int tmp;
    int tmc;
    QSqlQuery qwe;
    QString str;
    QString val;

    qwe.exec("SELECT COUNT(*) FROM tm_params WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();
    int row_count = qwe.value(0).toInt();

    qwe.exec("SELECT COUNT(*) FROM tm_categories WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();
    int col_count = qwe.value(0).toInt();

for (int i=0 ;i<row_count ;i++ )
    {
for (int j=0 ;j<col_count ;j++ )
    {
    tmp = map_t_r.value(i);
    tmc = map_c_r.value(j);

    QTableWidgetItem *item = new QTableWidgetItem;
    item = tw_unit->takeItem(i,j);

if (item)
{
    val = item->text();
    qwe.exec("insert or replace into m_unit values("
             + str.setNum(idM)+","+ str.setNum(tmp)+
             ","+str.setNum(tmc)+",'"+val+"')");
}
    }
    }

    QString string;
    qwe.prepare("replace into Measurement(id, idTM, idPupils, DateMeas, Comment)"
                "values(:id, :idTM, :idPupils, :DateMeas, :Comment)");
    qwe.bindValue(":id", idM);
    qwe.bindValue(":idTM", idMeas);
    qwe.bindValue(":idPupils", idPup);
    qwe.bindValue(":DateMeas", Date_m->date());
    qwe.bindValue(":Comment", teComment->toHtml());
    qwe.exec();



    MeasForm::close();
}

void MeasForm::on_pbCancel_clicked()
{
    MeasForm::close();
}

void MeasForm::on_pbExport_clicked()
{
    QTextDocument *doc = new QTextDocument;
    int tmp;
    int tmc;
    QSqlQuery qwe;
    QString str;
    QString html;
    QString val;

    qwe.exec("SELECT COUNT(*) FROM tm_params WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();
    int row_count = qwe.value(0).toInt();

    qwe.exec("SELECT COUNT(*) FROM tm_categories WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();
    int col_count = qwe.value(0).toInt();


    html =  "<H1>"+leType->text()+"</H1>"
        "<TABLE BORDER=1>";

    html += "<TR><th></th>";
for (int i=0 ;i<col_count ;i++ )
{
    html += "<th>"+head_v.value(i)+"</th>";
}
    html += "</TR>";

for (int i=0 ;i<row_count ;i++ )
{
    html += "<TR><th>"+head_h.value(i)+"</th>";
for (int j=0 ;j<col_count ;j++ )
{
    html += "<TD>";
    tmp = map_t_r.value(i);
    tmc = map_c_r.value(j);

    QTableWidgetItem *item = new QTableWidgetItem;
    item = tw_unit->takeItem(i,j);

if (item)
    {
    html += item->text();
    }
    html += "</TD>";
}
    html += "</TR>";
}
    html += "</TABLE>";

    html +="<p> </p>";
    html +="<p>"+teComment->toHtml()+"</p>";


    doc->setHtml(html);

    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                              QString(), tr("HTML-Files (*.htm *.html)"));
    QTextDocumentWriter writer(fn);
    writer.write(doc);//сохраним файл

    QTextStream out(&fn);//установим в сохраненном файле кодировку UTF-8
    out.setCodec("UTF-8");

///////////////////////////////////////////////////////////////////////////
    unit = new QSqlRelationalTableModel;
    unit->setTable("m_unit");
    unit->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
    unit->select();

    tm_params = new QSqlTableModel;
    tm_params->setTable("tm_params");
    tm_params->select();

    //заполним названия параметров
    qwe.exec("SELECT COUNT(*) FROM tm_params WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();
    tm_params_count=qwe.value(0).toInt();
    tw_unit->setRowCount(tm_params_count);

    qwe.exec("SELECT * FROM tm_params WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();

for (int i=0 ;i<tm_params_count ;i++ )
{
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(qwe.value(2).toString());
    tw_unit->setVerticalHeaderItem(i,item);

    map_t.insert(qwe.value(0).toInt(),i);
    map_t_r.insert(i, qwe.value(0).toInt());
    head_h.insert(i, qwe.value(2).toString());

    qwe.next();
}

    //заполним названия категорий
    qwe.exec("SELECT COUNT(*) FROM tm_categories WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();

    tm_categories_count=qwe.value(0).toInt();
    tw_unit->setColumnCount(tm_categories_count);

    qwe.exec("SELECT * FROM tm_categories WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();

for (int i=0 ;i<tm_categories_count ;i++ )
{
    QTableWidgetItem *item = new QTableWidgetItem;

    item->setText(qwe.value(2).toString());
    tw_unit->setHorizontalHeaderItem(i,item);

    map_c.insert(qwe.value(0).toInt(), i);
    map_c_r.insert(i, qwe.value(0).toInt());
    head_v.insert(i, qwe.value(2).toString());

    qwe.next();
}

    //узнаем, сколько есть интересующих нас записей
    qwe.exec("SELECT COUNT(*) FROM m_unit WHERE idM="+ str.setNum(idM) +"");
    qwe.first();
    m_unit_count=qwe.value(0).toInt();

    //заполним данные
    qwe.exec("SELECT * FROM m_unit WHERE idM="+ str.setNum(idM) +"");
    qwe.first();

for (int i=0 ;i< m_unit_count ;i++ )
{
    QTableWidgetItem *item = new QTableWidgetItem;

    int row = map_t.value(qwe.value(1).toInt());
    int col = map_c.value(qwe.value(2).toInt());

    item->setText(qwe.value(3).toString());
    if( (qwe.value(1).toInt() != map_t_r.value(row)) or (qwe.value(2).toInt() != map_c_r.value(col)) )
{}

else
{
    tw_unit->setItem(row,col,item);
}
    qwe.next();
}

    tw_unit->resizeColumnsToContents();
    tw_unit->resizeRowsToContents();
/////////////////////////////////////////////////////////////////////////////////////////////////////

}

void MeasForm::on_pbApply_clicked()
{
    int tmp;
    int tmc;
    QSqlQuery qwe;
    QString str;
    QString val;

    qwe.exec("SELECT COUNT(*) FROM tm_params WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();
    int row_count = qwe.value(0).toInt();

    qwe.exec("SELECT COUNT(*) FROM tm_categories WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();
    int col_count = qwe.value(0).toInt();




for (int i=0 ;i<row_count ;i++ )
    {

for (int j=0 ;j<col_count ;j++ )
    {
    tmp = map_t_r.value(i);
    tmc = map_c_r.value(j);

    QTableWidgetItem *item = new QTableWidgetItem;
    item = tw_unit->takeItem(i,j);

if (item)
{
    val = item->text();
    qwe.exec("insert or replace into m_unit values("
             + str.setNum(idM)+","+ str.setNum(tmp)+
             ","+str.setNum(tmc)+",'"+val+"')");
}

    }

    }

    QString string;
    qwe.prepare("replace into Measurement(id, idTM, idPupils, DateMeas, Comment)"
                "values(:id, :idTM, :idPupils, :DateMeas, :Comment)");
    qwe.bindValue(":id", idM);
    qwe.bindValue(":idTM", idMeas);
    qwe.bindValue(":idPupils", idPup);
    qwe.bindValue(":DateMeas", Date_m->date());
    qwe.bindValue(":Comment", teComment->toHtml());
    qwe.exec();



 ///////////////////////////////////////////////////////////////////////////
    unit = new QSqlRelationalTableModel;
    unit->setTable("m_unit");
    unit->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
    unit->select();

    tm_params = new QSqlTableModel;
    tm_params->setTable("tm_params");
    tm_params->select();

    //заполним названия параметров
    qwe.exec("SELECT COUNT(*) FROM tm_params WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();
    tm_params_count=qwe.value(0).toInt();
    tw_unit->setRowCount(tm_params_count);

    qwe.exec("SELECT * FROM tm_params WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();

for (int i=0 ;i<tm_params_count ;i++ )
{
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(qwe.value(2).toString());
    tw_unit->setVerticalHeaderItem(i,item);

    map_t.insert(qwe.value(0).toInt(),i);
    map_t_r.insert(i, qwe.value(0).toInt());
    head_h.insert(i, qwe.value(2).toString());

    qwe.next();
}

    //заполним названия категорий
    qwe.exec("SELECT COUNT(*) FROM tm_categories WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();

    tm_categories_count=qwe.value(0).toInt();
    tw_unit->setColumnCount(tm_categories_count);

    qwe.exec("SELECT * FROM tm_categories WHERE idTM="+str.setNum(idMeas)+"");
    qwe.first();

for (int i=0 ;i<tm_categories_count ;i++ )
{
    QTableWidgetItem *item = new QTableWidgetItem;

    item->setText(qwe.value(2).toString());
    tw_unit->setHorizontalHeaderItem(i,item);

    map_c.insert(qwe.value(0).toInt(), i);
    map_c_r.insert(i, qwe.value(0).toInt());
    head_v.insert(i, qwe.value(2).toString());

    qwe.next();
}

    //узнаем, сколько есть интересующих нас записей
    qwe.exec("SELECT COUNT(*) FROM m_unit WHERE idM="+ str.setNum(idM) +"");
    qwe.first();
    m_unit_count=qwe.value(0).toInt();

    //заполним данные
    qwe.exec("SELECT * FROM m_unit WHERE idM="+ str.setNum(idM) +"");
    qwe.first();

for (int i=0 ;i< m_unit_count ;i++ )
{
    QTableWidgetItem *item = new QTableWidgetItem;

    int row = map_t.value(qwe.value(1).toInt());
    int col = map_c.value(qwe.value(2).toInt());

    item->setText(qwe.value(3).toString());
    if( (qwe.value(1).toInt() != map_t_r.value(row)) or (qwe.value(2).toInt() != map_c_r.value(col)) )
{}

else
{
    tw_unit->setItem(row,col,item);
}
    qwe.next();
}

    tw_unit->resizeColumnsToContents();
    tw_unit->resizeRowsToContents();
/////////////////////////////////////////////////////////////////////////////////////////////////////
}
