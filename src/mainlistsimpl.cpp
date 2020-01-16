#include "mainlistsimpl.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QTableWidget>
#include "mainwindowimpl.h"
#include <QtGui>
#include <QtCore>
#include <QtSql>
 
MainListsImpl::MainListsImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
    setupUi(this);

    tp1_model = new QSqlTableModel;
    tp1_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    tp1_model->setTable("typeMeasurement");

    tp1_model->setHeaderData(1, Qt::Horizontal, tr("Name"));
    tp1_model->setHeaderData(2, Qt::Horizontal, tr("Comment"));

    tp1_model->select();

    tvTypesMeasurement->setModel(tp1_model);
    tvTypesMeasurement->hideColumn(0);

    tp2_model = new QSqlRelationalTableModel;
    tp2_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    tp2_model->setTable("tm_params");
    tp2_model->setHeaderData(2, Qt::Horizontal, tr("Parametr"));

    tp2_model->setFilter(QString("idTM=%1")
                         .arg(tp1_model->data(tp1_model->index(0, 0)).toInt()));

    //tp2_model->select();

    tvTMParams->setModel(tp2_model);
    tvTMParams->setItemDelegate(new QSqlRelationalDelegate(tvTMParams));

    tvTMParams->hideColumn(0);
    tvTMParams->hideColumn(1);

///////////////////////////////

    tp3_model = new QSqlRelationalTableModel;
    tp3_model->setEditStrategy(QSqlTableModel::OnFieldChange);

    tp3_model->setTable("tm_categories");
    tp3_model->setHeaderData(2, Qt::Horizontal, tr("Category"));
    //tp3_model->select();

    tvCateg->setModel(tp3_model);

    tvCateg->hideColumn(0);
    tvCateg->hideColumn(1);

///////////////////////////
}



void MainListsImpl::on_pbClose_clicked()
{
    MainListsImpl::close();
}


void MainListsImpl::on_pbAddTMParam_clicked()
{
    int id=tp1_model->data(tp1_model->index(tvTypesMeasurement->currentIndex().row(), 0)).toInt();
    if (id>0)
{
        int row = tp2_model->rowCount();
        QSqlRecord rec;

        QSqlField field1("idTM", QVariant::Int);
        field1.setValue(id);
        rec.append(field1);

        tp2_model->insertRecord(-1, rec);
        tvTMParams->selectRow(row);

    }
}

void MainListsImpl::on_pbDelTMParam_clicked()
{
    if (QMessageBox::question(0, tr("Delete parametre warning!"),
                              tr("The selected parametre now will\n"
                                 "be removed. You are assured?"),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
        == QMessageBox::Yes)
        {
    QSqlQuery qwe;
    QString str;
    int idM=tp2_model->data(tp2_model->index(tvTMParams->currentIndex().row(), 1)).toInt();
    int idTMP=tp2_model->data(tp2_model->index(tvTMParams->currentIndex().row(), 0)).toInt();
    int id=tp1_model->data(tp1_model->index(tvTypesMeasurement->currentIndex().row(), 0)).toInt();

    if (id>0)
    {
        int row = tvTMParams->currentIndex().row();
        tp2_model->removeRows(row, 1);
        tvTMParams->selectRow(row);
    }
    qwe.exec("delete from m_unit where idM="+str.setNum(idM)+" and idTMP="+str.setNum(idTMP));
}
}

void MainListsImpl::on_pbAddTypeMeasurement_clicked()
{
    int row = tp1_model->rowCount();
    tp1_model->insertRecord(-1, QSqlRecord());
    tvTypesMeasurement->selectRow(row);
}

void MainListsImpl::on_pbDelTypeMeasurement_clicked()
{
    if (QMessageBox::question(0, tr("Delete type warning!"),
                              tr("The selected type now will\n"
                                 "be removed. You are assured?"),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
        == QMessageBox::Yes)

        {
    int row = tvTypesMeasurement->currentIndex().row();
    tp1_model->removeRows(row, 1);
    tvTypesMeasurement->selectRow(row);
}
}

void MainListsImpl::on_tvTypesMeasurement_clicked(QModelIndex index)
{
    tp2_model->setFilter(QString("idTM=%1")
    	.arg(tp1_model->data(
    		tp1_model->index(
    			tvTypesMeasurement->currentIndex().row(), 0
    			)
    		).toInt()
    	)
    );
    
    tp2_model->select();


    tp3_model->setFilter(QString("idTM=%1")
        .arg(tp1_model->data(
                tp1_model->index(
                        tvTypesMeasurement->currentIndex().row(), 0
                        )
                ).toInt()
        )
    );

    tp3_model->select();
}

void MainListsImpl::on_pbDel_c_clicked()
{
    if (QMessageBox::question(0, tr("Delete category warning!"),
                              tr("The selected category now will\n"
                                 "be removed. You are assured?"),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
        == QMessageBox::Yes)

        {
    QSqlQuery qwe;
    QString str;
    int idM = tp3_model->data(
            tp3_model->index(
                    tvCateg->currentIndex()
                    .row(), 1)).toInt();

    int idTMC = tp3_model->data(
            tp3_model->index(
                    tvCateg->currentIndex()
                    .row(), 0)).toInt();

    int id = tp1_model->data(
            tp1_model->index(
                    tvTypesMeasurement->currentIndex()
                    .row(), 0)).toInt();
    if (id>0)
    {
        int row = tvCateg->currentIndex().row();
        tp3_model->removeRows(row, 1);
        tvCateg->selectRow(row);
    }
    qwe.exec("delete from m_unit where idM="+str.setNum(idM)+" and idTMC="+str.setNum(idTMC));
}
}

void MainListsImpl::on_pbAdd_c_clicked()
{
    int id = tp1_model->data(
               tp1_model->index(
                 tvTypesMeasurement->currentIndex()
                   .row(), 0)).toInt();

    int row = tp3_model->rowCount();
    QSqlRecord rec;

    QSqlField field1("idTM", QVariant::Int);
    field1.setValue(id);
    rec.append(field1);

    tp3_model->insertRecord(-1, rec);
    tvCateg->selectRow(row);
}

