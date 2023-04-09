#include "AllMode.h"
#include <stdio.h>
#include <cstdio>
#include <direct.h>
#include <Windows.h>
#include <QGraphicsScene>
#include <QListView>
#include <QLineEdit>
#include <QlistWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QFileDialog>

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

AllMode::AllMode(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(chooseIn()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(chooseOut()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(apply()));
}

AllMode::~AllMode()
{
	ui.listWidget->clear();
}

void AllMode::apply()
{
	std::string a = getIn().toStdString();
	std::string b = getOut().toStdString();
	if (ui.checkBox_5->isChecked())
	{
		WIN32_FIND_DATAA FindFileData;
		HANDLE hf;
		hf = FindFirstFileA((b + "/*").c_str(), &FindFileData);
		if (hf != INVALID_HANDLE_VALUE)
		{
			std::string filterName = FindFileData.cFileName;
			DeleteFileA((b + "/" + filterName).c_str());

			while (FindNextFileA(hf, &FindFileData))
			{
				filterName = FindFileData.cFileName;
				DeleteFileA((b + "/" + filterName).c_str());

			}

		}
	}
	
	thiss->isAllMode = 1;
	thiss->isOpen++;
	int inf = getTypeInf();
	int png, jpeg, tiff;
	
	tiff = inf % 10;
	inf /= 10;
	jpeg = inf % 10;
	inf /= 10;
	png = inf % 10;

	
	if (png == 1)
	{
		WIN32_FIND_DATAA FindFileData;
		HANDLE hf;
		std::string k = a + "/*.png";
		hf = FindFirstFileA((a + "/*.png").c_str(), &FindFileData);
		int d = 0, l = 0;
		if (hf != INVALID_HANDLE_VALUE)
		{
			l++;
			while (FindNextFileA(hf, &FindFileData))
			{
				l++;
			}
		}


		hf = FindFirstFileA((a + "/*.png").c_str(), &FindFileData);

		if (hf != INVALID_HANDLE_VALUE)
		{
			std::string filterName = FindFileData.cFileName;
			cv::Mat cvImage = cv::imread(a+"/"+filterName, cv::IMREAD_COLOR);
			thiss->m_img = cvImage;
			thiss->applyFilters();
 			cv::imwrite(b + "/" + filterName, thiss->m_resImg);
			d++;
			ui.label_5->setText(QString::number(d) + " of " + QString::number(l));
			ui.label_5->update();
			ui.progressBar->setValue(100.0 / double(l)* double(d));
			ui.progressBar->update();

			
			while (FindNextFileA(hf, &FindFileData))
			{
				filterName = FindFileData.cFileName;
				cv::Mat cvImage = cv::imread(a+"/"+filterName, cv::IMREAD_COLOR);
				thiss->m_img = cvImage;
				thiss->applyFilters();
				cv::imwrite(b + "/" + filterName, thiss->m_resImg);
				d++;
				ui.label_5->setText(QString::number(d) + " of " + QString::number(l));
				ui.label_5->update();
				ui.progressBar->setValue(100.0 / double(l)* double(d));
				ui.progressBar->update();

			}

		}
		//hf = FindFirstFileA(  "D:\\VS2013\\ReeMaker\\x64\\Debug\\Filters\\*.dll", &FindFileData);
	}

	if (jpeg == 1)
	{
		WIN32_FIND_DATAA FindFileData;
		HANDLE hf;
		std::string k = a + "/*.JPG";
		hf = FindFirstFileA((a + "/*.JPG").c_str(), &FindFileData);
		int d = 0, l = 0;
		if (hf != INVALID_HANDLE_VALUE)
		{
			l++;
			while (FindNextFileA(hf, &FindFileData))
			{
				l++;
			}
		}

		hf = FindFirstFileA((a + "/*.JPG").c_str(), &FindFileData);
		if (hf != INVALID_HANDLE_VALUE)
		{
			std::string filterName = FindFileData.cFileName;
			cv::Mat cvImage = cv::imread(a + "/" + filterName, cv::IMREAD_COLOR);
			thiss->m_img = cvImage;
			thiss->applyFilters();
			cv::imwrite(b + "/" + filterName, thiss->m_resImg);
			d++;
			ui.progressBar->update();
			ui.label_5->setText(QString::number(d) + " of " + QString::number(l));
			ui.label_5->update();
			ui.progressBar->setValue(100.0 / double(l)* double(d));

			while (FindNextFileA(hf, &FindFileData))
			{
				filterName = FindFileData.cFileName;
				cv::Mat cvImage = cv::imread(a + "/" + filterName, cv::IMREAD_COLOR);
				thiss->m_img = cvImage;
				thiss->applyFilters();
				cv::imwrite(b + "/" + filterName, thiss->m_resImg);
				d++;
				ui.label_5->setText(QString::number(d) + " of " + QString::number(l));
				ui.label_5->update();
				ui.progressBar->setValue(100.0 / double(l)* double(d));
				ui.progressBar->update();

			}

		}
		//hf = FindFirstFileA(  "D:\\VS2013\\ReeMaker\\x64\\Debug\\Filters\\*.dll", &FindFileData);
	}

	if (tiff == 1)
	{
		WIN32_FIND_DATAA FindFileData;
		HANDLE hf;
		std::string k = a + "/*.TIFF";
		hf = FindFirstFileA((a + "/*.TIFF").c_str(), &FindFileData);
		int d = 0, l = 0;
		if (hf != INVALID_HANDLE_VALUE)
		{
			l++;
			while (FindNextFileA(hf, &FindFileData))
			{
				l++;
			}
		}

		hf = FindFirstFileA((a + "/*.TIFF").c_str(), &FindFileData);

		if (hf != INVALID_HANDLE_VALUE)
		{
			std::string filterName = FindFileData.cFileName;
			cv::Mat cvImage = cv::imread(a + "/" + filterName, cv::IMREAD_COLOR);
			thiss->m_img = cvImage;
			thiss->applyFilters();
			cv::imwrite(b + "/" + filterName, thiss->m_resImg);
			d++;
			ui.label_5->setText(QString::number(d) + " of " + QString::number(l));
			ui.label_5->update();
			ui.progressBar->setValue(100.0 / double(l)* double(d));
			ui.progressBar->update();

			while (FindNextFileA(hf, &FindFileData))
			{
				filterName = FindFileData.cFileName;
				cv::Mat cvImage = cv::imread(a + "/" + filterName, cv::IMREAD_COLOR);
				thiss->m_img = cvImage;
				thiss->applyFilters();
				cv::imwrite(b + "/" + filterName, thiss->m_resImg);
				d++;
				ui.label_5->setText(QString::number(d) + " of " + QString::number(l));
				ui.label_5->update();
				ui.progressBar->setValue(100.0 / double(l)* double(d));
				ui.progressBar->update();

			}

		}
		//hf = FindFirstFileA(  "D:\\VS2013\\ReeMaker\\x64\\Debug\\Filters\\*.dll", &FindFileData);
	}
	
	thiss->isOpen--;
	thiss->isAllMode = 1;
}

void AllMode::chooseIn()
{
	QString imageFolderName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (imageFolderName.isEmpty()) { return; }
	ui.lineEdit->setText(imageFolderName);
	listViewBuild(imageFolderName);
}

void AllMode::chooseOut()
{
	QString imageFolderName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (imageFolderName.isEmpty()) { return; }
	ui.lineEdit_2->setText(imageFolderName);
}

QString AllMode::getIn()
{
	QString a=ui.lineEdit->text();
	return a;
}

QString AllMode::getOut()
{
	QString a = ui.lineEdit_2->text();
	return a;
}

int AllMode::getTypeInf()
{
	int inf = 0;

	if (ui.checkBox->isChecked()){ inf += 100; }
	if (ui.checkBox_2->isChecked()){ inf += 10; }
	if (ui.checkBox_4->isChecked()){ inf += 1; }

	return inf;
}

void AllMode::listViewBuild(QString imageFolderName)
{
	ui.listWidget->clear();

	int inf = getTypeInf();
	int png, jpeg, tiff;

	tiff = inf % 10;
	inf /= 10;
	jpeg = inf % 10;
	inf /= 10;
	png = inf % 10;

	std::string a = imageFolderName.toStdString();

	if (png == 1)
	{
		WIN32_FIND_DATAA FindFileData;
		HANDLE hf;
		hf = FindFirstFileA((a + "/*.png").c_str(), &FindFileData);
		int d = 0, l = 0;
		if (hf != INVALID_HANDLE_VALUE)
		{
			l++;
			while (FindNextFileA(hf, &FindFileData))
			{
				l++;
			}
		}


		hf = FindFirstFileA((a + "/*.png").c_str(), &FindFileData);
		
		if (hf != INVALID_HANDLE_VALUE)
		{
			std::string filterName = FindFileData.cFileName;
			cv::Mat cvImage = cv::imread(a + "/" + filterName, cv::IMREAD_COLOR);
			cv::Size szw; szw.height = 70; szw.width = 90;
			cv::resize(cvImage,cvImage, szw);
			QImage qmImage = cvImageToQImage(cvImage);
			QString qstr = QString::fromStdString(filterName.substr(0, filterName.length() - 4));
			QListWidgetItem *item1 = new QListWidgetItem(QIcon(QPixmap::fromImage(qmImage)), qstr, ui.listWidget);
			ui.listWidget->addItem(item1);
			ui.listWidget->update();
			d++;
			ui.label_5->setText(QString::number(d) + " of " + QString::number(l));
			ui.label_5->update();
			ui.progressBar->setValue(100.0 / double(l)* double(d));
			ui.progressBar->update();

			while (FindNextFileA(hf, &FindFileData))
			{
				std::string filterName = FindFileData.cFileName;
				cv::Mat cvImage = cv::imread(a + "/" + filterName, cv::IMREAD_COLOR);
				cv::Size szw; szw.height = 70; szw.width = 90;
				cv::resize(cvImage, cvImage, szw);
				QImage qmImage = cvImageToQImage(cvImage);
				QString qstr = QString::fromStdString(filterName.substr(0, filterName.length() - 4));
				QListWidgetItem *item2 = new QListWidgetItem(QIcon(QPixmap::fromImage(qmImage)), qstr, ui.listWidget);
				ui.listWidget->addItem(item2);
				ui.listWidget->update();
				d++;
				ui.label_5->setText(QString::number(d) + " of " + QString::number(l));
				ui.label_5->update();
				ui.progressBar->setValue(100.0 / double(l)* double(d));
				ui.progressBar->update();

			}

		}
		//hf = FindFirstFileA(  "D:\\VS2013\\ReeMaker\\x64\\Debug\\Filters\\*.dll", &FindFileData);
	}
	//hf = FindFirstFileA(| | |, &FindFileData);//////

	if (jpeg == 1)
	{
		WIN32_FIND_DATAA FindFileData;
		HANDLE hf;
		hf = FindFirstFileA((a + "/*.JPG").c_str(), &FindFileData);
		int d = 0, l = 0;
		if (hf != INVALID_HANDLE_VALUE)
		{
			l++;
			while (FindNextFileA(hf, &FindFileData))
			{
				l++;
			}
		}

		hf = FindFirstFileA((a + "/*.JPG").c_str(), &FindFileData);

		if (hf != INVALID_HANDLE_VALUE)
		{
			std::string filterName = FindFileData.cFileName;
			cv::Mat cvImage = cv::imread(a + "/" + filterName, cv::IMREAD_COLOR);
			cv::Size szw; szw.height = 70; szw.width = 90;
			cv::resize(cvImage, cvImage, szw);
			QImage qmImage = cvImageToQImage(cvImage);
			QString qstr = QString::fromStdString(filterName.substr(0, filterName.length() - 4));
			QListWidgetItem *item1 = new QListWidgetItem(QIcon(QPixmap::fromImage(qmImage)), qstr, ui.listWidget);
			ui.listWidget->addItem(item1);
			ui.listWidget->update();
			d++;
			ui.label_5->setText(QString::number(d) + " of " + QString::number(l));
			ui.label_5->update();
			ui.progressBar->setValue(100.0 / double(l)* double(d));
			ui.progressBar->update();

			while (FindNextFileA(hf, &FindFileData))
			{
				std::string filterName = FindFileData.cFileName;
				cv::Mat cvImage = cv::imread(a + "/" + filterName, cv::IMREAD_COLOR);
				cv::Size szw; szw.height = 70; szw.width = 90;
				cv::resize(cvImage, cvImage, szw);
				QImage qmImage = cvImageToQImage(cvImage);
				QString qstr = QString::fromStdString(filterName.substr(0, filterName.length() - 4));
				QListWidgetItem *item2 = new QListWidgetItem(QIcon(QPixmap::fromImage(qmImage)), qstr, ui.listWidget);
				ui.listWidget->addItem(item2);
				ui.listWidget->update();
				d++;
				ui.label_5->setText(QString::number(d) + " of " + QString::number(l));
				ui.label_5->update();
				ui.progressBar->setValue(100.0 / double(l)* double(d));
				ui.progressBar->update();

			}

		}
		//hf = FindFirstFileA(  "D:\\VS2013\\ReeMaker\\x64\\Debug\\Filters\\*.dll", &FindFileData);
	}

	if (tiff == 1)
	{
		WIN32_FIND_DATAA FindFileData;
		HANDLE hf;
		hf = FindFirstFileA((a + "/*.TIFF").c_str(), &FindFileData);
		int d = 0, l = 0;
		if (hf != INVALID_HANDLE_VALUE)
		{
			l++;
			while (FindNextFileA(hf, &FindFileData))
			{
				l++;
			}
		}

		hf = FindFirstFileA((a + "/*.TIFF").c_str(), &FindFileData);

		if (hf != INVALID_HANDLE_VALUE)
		{
			std::string filterName = FindFileData.cFileName;
			cv::Mat cvImage = cv::imread(a + "/" + filterName, cv::IMREAD_COLOR);
			cv::Size szw; szw.height = 70; szw.width = 90;
			cv::resize(cvImage, cvImage, szw);
			QImage qmImage = cvImageToQImage(cvImage);
			QString qstr = QString::fromStdString(filterName.substr(0, filterName.length() - 4));
			QListWidgetItem *item1 = new QListWidgetItem(QIcon(QPixmap::fromImage(qmImage)), qstr, ui.listWidget);
			ui.listWidget->addItem(item1);
			ui.listWidget->update();
			d++;
			ui.label_5->setText(QString::number(d) + " of " + QString::number(l));
			ui.label_5->update();
			ui.progressBar->setValue(100.0 / double(l)* double(d));
			ui.progressBar->update();

			while (FindNextFileA(hf, &FindFileData))
			{
				std::string filterName = FindFileData.cFileName;
				cv::Mat cvImage = cv::imread(a + "/" + filterName, cv::IMREAD_COLOR);
				cv::Size szw; szw.height = 70; szw.width = 90;
				cv::resize(cvImage, cvImage, szw);
				QImage qmImage = cvImageToQImage(cvImage);
				QString qstr = QString::fromStdString(filterName.substr(0, filterName.length() - 4));
				QListWidgetItem *item2 = new QListWidgetItem(QIcon(QPixmap::fromImage(qmImage)), qstr, ui.listWidget);
				ui.listWidget->addItem(item2);
				ui.listWidget->update();
				d++;
				ui.label_5->setText(QString::number(d) + " of " + QString::number(l));
				ui.label_5->update();
				ui.progressBar->setValue(100.0 / double(l)* double(d));
				ui.progressBar->update();

			}

		}
		//hf = FindFirstFileA(  "D:\\VS2013\\ReeMaker\\x64\\Debug\\Filters\\*.dll", &FindFileData);
	}
}