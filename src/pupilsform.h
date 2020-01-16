#ifndef PUPILSFORM_H
#define PUPILSFORM_H
#include "ui_pupilsform.h"
#include <QDialog>


class QSqlTableModel;
class QSqlRelationalTableModel;



class PupilsForm : public QDialog, public Ui::PupilsForm
{
    Q_OBJECT

public:
    PupilsForm( QWidget *parent = 0, Qt::WFlags f = 0);


private:
    QSqlTableModel *tm_pupils;
    QSqlTableModel *t_model;
    QSqlRelationalTableModel *tm_mpupils;

private slots:
        void on_pbAdd_clicked();
        void on_pbDel_clicked();
        void on_pbAdd_m_clicked();
        void on_pbDel_m_clicked();
        //void on_tvTypesMeasurement_clicked(QModelIndex index);
        void on_tvPupils_clicked(QModelIndex index);
        void on_pbEdit_clicked();
        void on_pbSave_p_clicked();
        void on_pbOK_clicked();
};


#endif
