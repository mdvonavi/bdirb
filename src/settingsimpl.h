#ifndef SETTINGSIMPL_H
#define SETTINGSIMPL_H
#include <QDialog>
#include "ui_settings.h"

class SettingsImpl : public QDialog, public Ui::Settings
{
Q_OBJECT
public:
	SettingsImpl( QWidget * parent = 0, Qt::WFlags f = 0 );
private slots:
        void on_pbOK_clicked();
};
#endif






