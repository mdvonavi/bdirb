#ifndef ADDMEAS_H
#define ADDMEAS_H
#include "ui_addmeas.h"
#include <QDialog>



class AddMeas: public QDialog, public Ui::AddMeas
{
    Q_OBJECT

public:
    AddMeas( QWidget *parent = 0, Qt::WFlags f = 0);


private:



private slots:
        void on_pbOK_clicked();


};


#endif
