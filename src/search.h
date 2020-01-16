#ifndef SEARCH_H
#define SEARCH_H
#include "ui_search.h"
#include <QWizard>
#include <QSqlQueryModel>

class QSqlTableModel;
class QSqlRelationalTableModel;

class Search : public QWizard, public Ui::Search
{
    Q_OBJECT

public:
    Search(QWidget *parent = 0);
    int nextId() const;

private:
    QSqlTableModel *m_search;
    QSqlRelationalTableModel *r_search;



private slots:
    void on_pbOpen_clicked();
};



#endif
