#include "reemaker.h"
#include "AllMode.h"

#include <opencv2\opencv.hpp>
#include <Windows.h>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QPushButton>

namespace
{

	uint cvPixelToQPixel(cv::Vec3b color)
	{
		return qRgb(color[2], color[1], color[0]);
	}

	QImage cvImageToQImage(const cv::Mat& cvImage)
	{
		QImage::Format imgFormat = QImage::Format_RGB32;
		if (cvImage.type() == CV_8UC3)
		{
			imgFormat = QImage::Format_RGB32;
		}
		else
		{
			assert(0);
			return QImage();
		}

		QImage qImage(QSize(cvImage.size().width, cvImage.size().height), imgFormat);

		cv::Mat_<cv::Vec3b> cvImage_ = cvImage;
		for (int y = 0; y < cvImage_.rows; ++y)
		{
			for (int x = 0; x < cvImage_.cols; ++x)
			{
				cv::Vec3b color = cvImage_(cv::Point(x, y));
				qImage.setPixel(QPoint(x, y), cvPixelToQPixel(color));
			}
		}

		return qImage;
	}
}

ReeMaker::ReeMaker(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	isAllMode = 0;

	isOpen = 0;

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui.actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
	connect(ui.actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
	connect(ui.actionZoomReset, SIGNAL(triggered()), this, SLOT(zoomReset()));
	connect(ui.btnApplyFilters, SIGNAL(clicked()), this, SLOT(applyFilters()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui.actionAllMode, SIGNAL(triggered()), this, SLOT(allMode()));

	ui.graphicsView_2->setScene(&m_inputImage);

	ui.verticalLayout_2->setAlignment(Qt::AlignTop);
	ui.verticalLayout_5->setAlignment(Qt::AlignTop);

	loadFiltersAndBuilFiltersUi();
}

ReeMaker::~ReeMaker()
{

}

void ReeMaker::allMode()
{
	AllMode* a = new AllMode();
	a->thiss = this;
	a->show();
}

void ReeMaker::zoomIn()
{
	ui.graphicsView_2->scale(1.1, 1.1);
}
void ReeMaker::zoomOut()
{
	ui.graphicsView_2->scale(0.9, 0.9);
}
void ReeMaker::zoomReset()
{
	ui.graphicsView_2->resetTransform();
}

void ReeMaker::open()
{
	QString imageFileName = QFileDialog::getOpenFileName(this, "Load Image", QString(), "Image Files (*.*)");
	if (imageFileName.isEmpty()) { return; }

	QImage qImage;
	bool res = qImage.load(imageFileName);
	cv::Mat cvImage = cv::imread(imageFileName.toStdString(), cv::IMREAD_COLOR);

	if (!res || cvImage.empty()) {
		QMessageBox::critical(this, "Error", "Can't load image!");
		return;
	}

	m_img = cvImage;
	isOpen = 1;
	QGraphicsPixmapItem* img = new QGraphicsPixmapItem(QPixmap::fromImage(qImage));
	m_inputImage.clear();
	m_inputImage.addItem(img);
	ui.graphicsView_2->update();

}
//
void ReeMaker::addFilter(QString filterId)
{
	auto stdString = filterId.toStdString();

	if (m_filterManager.getFiltersData().find(stdString) != m_filterManager.getFiltersData().end())
	{
		m_filterManager.addFilter(filterId.toStdString(), ui.verticalLayout_2, this);
	}
}

void ReeMaker::filterUp(int id)
{
	m_filterManager.filterUp(id);
}

void ReeMaker::filterDown(int id)
{
	m_filterManager.filterDown(id);
}

void ReeMaker::filterDelete(int id)
{
	m_filterManager.deleteFilter(id);
}

void ReeMaker::filterCheck(int id)
{
	m_filterManager.setFilterActive(id);
}

void ReeMaker::loadFiltersAndBuilFiltersUi()
{
	m_filterManager.loadFilters();

	const std::unordered_map<std::string, std::shared_ptr<FilterData>>& filtersData = m_filterManager.getFiltersData();

	// Add filters buttons
	for (auto it = filtersData.begin(); it != filtersData.end(); ++it)
	{
		std::shared_ptr<FilterData> filterData = it->second;

		QPushButton* btn = new QPushButton(this);
		btn->setText(QString::fromStdString(filterData->getName()));

		connect(btn, &QPushButton::clicked, this,
			[this, it]() {
				addFilter(QString::fromStdString(it->first));
			}
		);
		
		ui.verticalLayout_5->addWidget(btn);
	}
}

void ReeMaker::applyFilters()
{
	if (isOpen == 0){ return; }
	m_resImg = m_img.clone();
	m_filterManager.applyFilters(m_resImg);

	if (isAllMode == 0)
	{
		QImage qImage = cvImageToQImage(m_resImg);
		QGraphicsPixmapItem* img = new QGraphicsPixmapItem(QPixmap::fromImage(qImage));
		m_inputImage.clear();
		m_inputImage.addItem(img);
		ui.graphicsView_2->update(); 
	}
}

void ReeMaker::save()
{
	QString imageFileName = QFileDialog::getSaveFileName(this, "Save Image", QString(), "Image Files (*.*)");
	if (imageFileName.isEmpty()) { return; }

	if (m_resImg.empty()) { return; }
	if (imageFileName.isEmpty()) { return; }

	cv::imwrite(imageFileName.toStdString(), m_resImg);
}