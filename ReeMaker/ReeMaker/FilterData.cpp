#include "FilterData.h"

#include <QSpinbox>
#include <QDoubleSpinbox>
#include <QTextEdit>
#include <QSlider>
#include <QGroupBox>
#include <QLabel>

#include "pugixml.hpp"

#include <cstring>
#include <direct.h>

namespace
{
	UiElementType typeStringToUiType(const std::string& type)
	{
		if (type == "int")
			return eIntEdit;
		else if (type == "float")
			return eFloatEdit;
		else if (type == "trackbar")
			return eTrackBar;

		return eWrong;
	}
}

FilterData::FilterData()
	: m_hinstLib(NULL)
	, m_filterFunc(NULL)
	, m_id("")
	, m_name("")
{

}

FilterData::~FilterData()
{
	cleanup();
}

bool FilterData::load(const std::string &fileName)
{
	bool res = loadDll(fileName + ".dll");
	if (!res)
	{
		return false;
	}

	return loadXml(fileName + ".xml");
}

bool FilterData::loadDll(const std::string &fileName)
{
	if (m_hinstLib != NULL)
	{
		// Free the DLL module.
		FreeLibrary(m_hinstLib);
	}

	m_hinstLib = NULL;
	m_filterFunc = NULL;

	// Get a handle to the DLL module.
	m_hinstLib = LoadLibraryA(fileName.c_str());

	// If the handle is valid, try to get the function address.
	if (m_hinstLib != NULL)
	{
		m_filterFunc = (FILTERFUNC)GetProcAddress(m_hinstLib, "apply");

		// If the function address is valid, call the function.
		if (NULL != m_filterFunc)
		{
			return true;
		}
		else
		{
			// Free the DLL module.
			FreeLibrary(m_hinstLib);
			m_hinstLib = NULL;
			m_filterFunc = NULL;

			return false;
		}
	}

	return false;
}

bool FilterData::loadXml(const std::string &fileName)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(fileName.c_str());

	if (!result)
	{
		return false;
	}

	pugi::xml_node filterNode = doc.child("Filter");
	if (!filterNode) { return false; }

	m_name = filterNode.attribute("name").as_string("");
	if (m_name == "") { return false; }

	m_id = filterNode.attribute("id").as_string("");
	if (m_id == "") { return false; }
	
	pugi::xml_node parametersNode = filterNode.child("Parameters");
	if (!filterNode) { return false; }

	m_uiMap.clear();

	auto parametersNodes = parametersNode.children("Parameter");
	for (auto paramNode = parametersNodes.begin(); paramNode != parametersNodes.end(); ++paramNode)
	{
		FitlerUiElement uiElement;
		uiElement.elementName = paramNode->attribute("name").as_string("");
		if (uiElement.elementName == "") { continue; }

		uiElement.elementId = paramNode->attribute("id").as_string("");
		if (uiElement.elementId == "") { continue; }

		std::string type = paramNode->attribute("type").as_string("");
		if (type == "") { continue; }

		uiElement.elementType = typeStringToUiType(type);
		if (uiElement.elementType == eWrong) { continue ;}

		uiElement.min = paramNode->attribute("min").as_float(0.f);
		uiElement.max = paramNode->attribute("max").as_float(100.f);

		// Save parameter
		m_uiMap[uiElement.elementId] = uiElement;
	}

	return true;
}

void FilterData::cleanup()
{
	if (m_hinstLib != NULL)
	{
		// Free the DLL module.
		FreeLibrary(m_hinstLib);
	}
}

bool FilterData::isValid() const
{
	return m_hinstLib != NULL && m_filterFunc != NULL && m_id != "" && m_name != "";
}

std::string FilterData::getId() const
{
	return m_id;
}

std::string FilterData::getName() const
{
	return m_name;
}

std::unordered_map<std::string, FitlerUiElement> FilterData::getUiMap() const
{
	return m_uiMap;
}

FILTERFUNC FilterData::getFilterApplyFunc() const
{
	return m_filterFunc;
}