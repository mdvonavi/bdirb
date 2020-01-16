#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H
//
#include <QMainWindow>
#include "ui_mainwindow.h"
//
class MainWindowImpl : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindowImpl( QWidget * parent = 0, Qt::WFlags f = 0 );

private slots:
    void on_pbMainLists_clicked();
    void on_pbSettings_clicked();
    void on_pbPupils_clicked();
    void on_pbSearch_clicked();

private:

};
#endif







