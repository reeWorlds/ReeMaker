#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "ui_AllMode.h"
#include "reemaker.h"
#include <QGraphicsScene>
#include <QDialog>


class AllMode : public QDialog, private Ui_AllMode
{
    Q_OBJECT

public:
	AllMode(QWidget *parent = 0);
	~AllMode();
	ReeMaker *thiss;
public slots:
	void apply();
	void chooseIn();
	void chooseOut();

private:
	Ui::AllMode ui;
	void listViewBuild(QString);
	QString getIn();
	QString getOut();
	int getTypeInf();
};

#endif // ABOUTDIALOG_H