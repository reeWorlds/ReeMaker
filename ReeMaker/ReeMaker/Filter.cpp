#include "Filter.h"

#include "reemaker.h"

#include <QCheckBox>
#include <QSpinbox>
#include <QDoubleSpinbox>
#include <QTextEdit>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>
#include <QToolButton>
#include <QHBoxLayout>
#include <QObject>

#include "pugixml.hpp"

Filter::Filter(const FilterData& filterData, int id) : m_filterData(filterData), m_id(id), 
	m_check(nullptr), m_box(nullptr)
{
	// Make local copy of ui data
	m_uiMap = m_filterData.getUiMap();
}

Filter::~Filter()
{
	cleanup();
}

void Filter::cleanup()
{
	
}

void Filter::buildUi(QVBoxLayout* parent, ReeMaker* _parent)
{
	if (!m_filterData.isValid()) { return; }

	QGroupBox* box = new QGroupBox();
	m_box = box;
	box->setTitle(QString::fromStdString(m_filterData.getName()));
	box->setMaximumHeight(m_uiMap.size() * 40 + 40 * 2);
	box->setMinimumHeight(m_uiMap.size() * 35 + 35 * 2);

	QGridLayout* layout = new QGridLayout(box);

	box->setLayout(layout);

	QCheckBox* check = new QCheckBox(box);
	m_check = check;
	check->setText("Enabled");
	check->setChecked(true);
	layout->addWidget(check, 0, 0);

	QObject::connect(check, &QCheckBox::clicked, _parent,
		[this, _parent]() {
			_parent->filterCheck(m_id);
		});

	int ind = 0;
	for(auto& it : m_uiMap)
	{
		auto& uiElement = it.second;

		if (uiElement.elementType == eWrong)
		{
			ind++;
			continue;
		}

		QLabel* label = new QLabel(box);
		label->setText(QString::fromStdString(uiElement.elementName));
		layout->addWidget(label, ind + 1, 0);

		switch (uiElement.elementType)
		{
		case eIntEdit:
		{
			QSpinBox *spin = new QSpinBox(box);
			// TODO: read this from UI config
			spin->setMinimum((int)uiElement.min);
			spin->setMaximum((int)uiElement.max);
			layout->addWidget(spin, ind + 1, 1);
			uiElement.element = (QObject*)spin;
		}
			break;
		case eFloatEdit:
		{
			QDoubleSpinBox *spin = new QDoubleSpinBox(box);
			// TODO: read this from UI config
			spin->setMinimum(uiElement.min);
			spin->setMaximum(uiElement.max);
			layout->addWidget(spin, ind + 1, 1);
			uiElement.element = (QObject*)spin;
		}
			break;
		case eTrackBar:
		{
			QSlider *slider = new QSlider(box);
			// TODO: read this from UI config
			slider->setMinimum((int)uiElement.min);
			slider->setMaximum((int)uiElement.max);
			slider->setOrientation(Qt::Orientation::Horizontal);
			layout->addWidget(slider, ind + 1, 1);
			uiElement.element = (QObject*)slider;
		}
			break;
		}

		ind++;
	}

	QHBoxLayout* hLayout = new QHBoxLayout(box);
	layout->addLayout(hLayout, m_uiMap.size() + 1, 1);

	QToolButton* tb = new QToolButton(box);
	tb->setText("^");
	hLayout->addWidget(tb, 0);

	QObject::connect(tb, &QCheckBox::clicked, _parent,
		[this, _parent]() {
			_parent->filterUp(m_id);
		});

	tb = new QToolButton(box);
	hLayout->addWidget(tb, 1);
	tb->setText("v");

	QObject::connect(tb, &QCheckBox::clicked, _parent,
		[this, _parent]() {
			_parent->filterDown(m_id);
		});

	tb = new QToolButton(box);
	hLayout->addWidget(tb, 2);
	tb->setText("x");

	QObject::connect(tb, &QCheckBox::clicked, _parent,
		[this, _parent]() {
			_parent->filterDelete(m_id);
		});

	parent->addWidget(box);
}

bool Filter::apply(cv::Mat& image)
{
	if (!m_filterData.isValid())
		return false;

	void** params = (m_uiMap.empty()) ? 0 : (new void*[m_uiMap.size()]);
	int ind = 0;
	for (const auto& it : m_uiMap)
	{
		params[ind] = 0;
		ind++;
	}

	// Set params
	ind = 0;
	for (const auto& it : m_uiMap)
	{
		auto& uiElement = it.second;

		if (uiElement.element == NULL)
		{
			ind++;
			continue;
		}

		switch (uiElement.elementType)
		{
		case eIntEdit:
		{
			int* value = new int;
			*value = ((QSpinBox*)uiElement.element)->value();

			params[ind] = (void*)value;
		}
			break;
		case eFloatEdit:
		{
			float* value = new float;
			*value = (float)((QDoubleSpinBox*)uiElement.element)->value();

			params[ind] = (void*)value;
		}
			break;
		case eTrackBar:
		{
			int* value = new int;
			*value = ((QSlider*)uiElement.element)->value();

			params[ind] = (void*)value;
		}
			break;
		}

		ind++;
	}

	bool res = false;
	if (image.isContinuous())
	{
		res = m_filterData.getFilterApplyFunc()(image.data, image.cols, image.rows, image.channels(), params);
	}
	else
	{
		// TODO
	}

	// Cleanup params
	ind = 0;
	for (const auto& it : m_uiMap)
	{
		if (params[ind] == 0)
		{
			ind++;
			continue;
		}

		auto& uiElement = it.second;

		switch (uiElement.elementType)
		{
		case eIntEdit:
		{
			delete (int*)params[ind];
		}
			break;
		case eFloatEdit:
		{
			delete (float*)params[ind];
		}
			break;
		case eTrackBar:
		{
			delete (int*)params[ind];
		}
			break;
		}

		ind++;
	}

	if (params != 0)
		delete[] params;

	return res;

}

const FilterData& Filter::getFilerData() const
{
	return m_filterData;
}

void Filter::setActive(bool active)
{
	m_active = active;
}

void Filter::setActive()
{
	if (m_check)
		m_active = m_check->isChecked();
}

bool Filter::getActive() const
{
	return m_active;
}

void Filter::setId(int id)
{
	m_id = id;
}

int Filter::getId() const
{
	return m_id;
}

QGroupBox* Filter::getFilterUiBox() const
{
	return m_box;
}