#pragma once

#include "FilterData.h"

#include <opencv2\opencv.hpp>

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <QObject>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSignalMapper>
#include <QCheckBox>

class ReeMaker;

class Filter
{

public:

	Filter(const FilterData& filterData, int id);
	~Filter();

	void buildUi(QVBoxLayout* parent, ReeMaker* _parent);

	bool apply(cv::Mat& image);

	const FilterData& getFilerData() const;

	void setActive(bool active);
	void setActive();
	bool getActive() const;

	void setId(int id);
	int getId() const;

	QGroupBox* getFilterUiBox() const;

private:

	const FilterData& m_filterData;
	bool m_active;
	QGroupBox* m_box;
	QCheckBox* m_check;
	int m_id;

	std::unordered_map<std::string, FitlerUiElement> m_uiMap;

	void cleanup();

};