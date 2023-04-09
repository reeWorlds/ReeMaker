#ifndef REEMAKER_H
#define REEMAKER_H

#include "FilterManager.h"

#include <opencv2\opencv.hpp>
#include <Windows.h>
#include <QtWidgets/QMainWindow>
#include <QGraphicsScene>
#include <QSignalMapper>
#include "ui_reemaker.h"

#include <string>
#include <memory>

class ReeMaker : public QMainWindow
{
	Q_OBJECT

public:
	ReeMaker(QWidget *parent = 0);
	~ReeMaker();
	int isOpen;
public slots:
	void allMode();
	void zoomIn();
	void zoomOut();
	void save();
	void zoomReset();
	void open();
	void addFilter(QString filterId);
	void applyFilters();
	void filterUp(int id);
	void filterDown(int id);
	void filterDelete(int id);
	void filterCheck(int id);
public:
	cv::Mat m_img;
	cv::Mat m_resImg;
	int isAllMode;

private:
	Ui::ReeMakerClass ui;
	QGraphicsScene m_inputImage;

	FilterManager m_filterManager;

	void loadFiltersAndBuilFiltersUi();

};

#endif // REEMAKER_H
