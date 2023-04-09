#include "FilterManager.h"
#include "reemaker.h"

FilterManager::FilterManager()
: m_uiParent(nullptr)
{

}

FilterManager::~FilterManager()
{

}

void FilterManager::loadFilters()
{
	std::vector<char> path(MAX_PATH);
	GetCurrentDirectoryA(MAX_PATH, path.data());
	std::string fullPath(path.begin(), path.end());
	std::cout << "Full path: " << fullPath << std::endl;

	// TEST
	WIN32_FIND_DATAA FindFileData;
	HANDLE hf;
	hf = FindFirstFileA("Filters\\*.dll", &FindFileData);

	if (hf != INVALID_HANDLE_VALUE)
	{
		std::string filterName = FindFileData.cFileName;
		filterName = filterName.substr(0, filterName.length() - 4);

		auto filterData = std::make_shared<FilterData>();
		bool res = filterData->load("Filters/" + filterName);
		
		if (res) { m_filtersData[filterData->getId()] = filterData; }
		
		while (FindNextFileA(hf, &FindFileData))
		{
			std::string filterName = FindFileData.cFileName;
			filterName = filterName.substr(0, filterName.length() - 4);

			auto filterData = std::make_shared<FilterData>();
			bool res = filterData->load("Filters/" + filterName);

			if (res) { m_filtersData[filterData->getId()] = filterData; }
		}
	}

	FindClose(hf);
}

bool FilterManager::addFilter(const std::string id, QVBoxLayout* parent, ReeMaker* _parent)
{
	m_uiParent = parent;

	auto it = m_filtersData.find(id);
	if (it == m_filtersData.end())
		return false;

	int idi = rand();
	bool isUnique = false;
	while (!isUnique)
	{
		isUnique = true;
		for (size_t i = 0; i < m_filters.size(); ++i)
		if (idi == m_filters[i]->getId())
		{
			isUnique = false;
			idi = rand();
			break;
		}
	}

	auto filter = std::make_shared<Filter>(*it->second, idi);

	filter->buildUi(parent, _parent);

	m_filters.push_back(filter);

	return true;
}

const std::unordered_map<std::string, std::shared_ptr<FilterData>>& FilterManager::getFiltersData() const
{
	return m_filtersData;
}

void FilterManager::applyFilters(cv::Mat& image)
{
	for (auto it = m_filters.begin(); it != m_filters.end(); ++it)
	{
		if ((*it)->getActive())
			(*it)->apply(image);
	}
}

void FilterManager::filterUp(int id)
{
	int ind = -1;
	for (size_t i = 0; i < m_filters.size(); ++i)
	if (m_filters[i]->getId() == id)
	{
		ind = i;
		break;
	}

	if (ind < 0)
		return;

	if (ind == 0)
		return;

	QGroupBox* box = m_filters[ind]->getFilterUiBox();
	int ii = m_uiParent->indexOf(box);
	m_uiParent->removeWidget(box);
	m_uiParent->insertWidget(ii - 1, box);

	m_filters[ind].swap(m_filters[ind - 1]);

}

void FilterManager::filterDown(int id)
{
	int ind = -1;
	for (size_t i = 0; i < m_filters.size(); ++i)
	if (m_filters[i]->getId() == id)
	{
		ind = i;
		break;
	}

	if (ind < 0)
		return;

	if (ind == m_filters.size() - 1)
		return;

	QGroupBox* box = m_filters[ind]->getFilterUiBox();
	int ii = m_uiParent->indexOf(box);
	m_uiParent->removeWidget(box);
	m_uiParent->insertWidget(ii + 1, box);

	m_filters[ind].swap(m_filters[ind + 1]);
}

void FilterManager::deleteFilter(int id)
{
	auto it = m_filters.begin();
	for (; it != m_filters.end(); ++it)
	if ((*it)->getId() == id)
	{
		break;
	}

	if (it == m_filters.end())
		return;

	if (m_uiParent)
		m_uiParent->removeWidget((*it)->getFilterUiBox());

	//delete (*it)->getFilterUiBox();
	(*it)->getFilterUiBox()->setVisible(false);

	m_filters.erase(it);
}

void FilterManager::setFilterActive(int id)
{
	int ind = -1;
	for (size_t i = 0; i < m_filters.size(); ++i)
	if (m_filters[i]->getId() == id)
	{
		ind = i;
		break;
	}

	if (ind < 0)
		return;

	m_filters[ind]->setActive();
}