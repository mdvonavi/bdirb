#ifndef MEASFORM_H
#define MEASFORM_H
#include "ui_measform.h"
#include <QDialog>

class QSqlTableModel;
class QSqlRelationalTableModel;


class MeasForm : public QDialog, public Ui::MeasForm
{
    Q_OBJECT

public:
    MeasForm( QWidget *parent = 0, Qt::WFlags f = 0);


private:
    QSqlRelationalTableModel *unit;
    QSqlTableModel *tm_params;
    QMap<int, int> map_t;//map для заполнения
    QMap<int, int> map_c;
    QMap<int, int> map_t_r;//map для сохраниения
    QMap<int, int> map_c_r;
    QMap<int, QString> head_v;//для сохранения названия столбцов
    QMap<int, QString> head_h;//для сохранения названия строк

private slots:
   // void on_pbAddM_clicked();
    void on_pbOK_clicked();
    void on_pbApply_clicked();
    void on_pbCancel_clicked();
    void on_pbExport_clicked();
};


#endif
