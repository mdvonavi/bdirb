#ifndef MAINLISTSIMPL_H
#define MAINLISTSIMPL_H
//
#include <QDialog>
#include "ui_mainlists.h"
//
class QSqlTableModel;
class QSqlRelationalTableModel;
//
class MainListsImpl : public QDialog, public Ui::MainLists
{
Q_OBJECT
public:
	MainListsImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
private:
    QSqlTableModel *tp_model;
    QSqlTableModel *tp1_model;
    QSqlRelationalTableModel *tp2_model;
    QSqlRelationalTableModel *tp3_model;

signals:
   void changed();

private slots:
        void on_tvTypesMeasurement_clicked(QModelIndex index);
        void on_pbAddTMParam_clicked();
	void on_pbDelTMParam_clicked();
	void on_pbAddTypeMeasurement_clicked();
	void on_pbDelTypeMeasurement_clicked();
        void on_pbClose_clicked();
        void on_pbAdd_c_clicked();
        void on_pbDel_c_clicked();

};

#endif
