#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#include "FilterData.h"
#include "Filter.h"

#include <QVBoxLayout>

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

class ReeMaker;

class FilterManager
{

public:

	FilterManager();
	~FilterManager();
	void loadFilters();

	bool addFilter(const std::string id, QVBoxLayout* parent, ReeMaker* cParent);

	void applyFilters(cv::Mat& image);

	const std::unordered_map<std::string, std::shared_ptr<FilterData>>& getFiltersData() const;

	void filterUp(int id);
	void filterDown(int id);
	void deleteFilter(int id);
	void setFilterActive(int id);

private:

	std::unordered_map<std::string, std::shared_ptr<FilterData>> m_filtersData;
	std::vector<std::shared_ptr<Filter>> m_filters;

	QVBoxLayout* m_uiParent;
};

#endif