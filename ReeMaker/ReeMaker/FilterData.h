#pragma once

#include <opencv2\opencv.hpp>
#include <Windows.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <QObject>
#include <QVBoxLayout>

enum UiElementType
{
	eIntEdit = 0,
	eFloatEdit,
	eTrackBar,
	eWrong
};

struct FitlerUiElement
{
	QObject* element;
	std::string elementId;
	std::string elementName;
	UiElementType elementType;
	float min;
	float max;
};

typedef bool(__cdecl *FILTERFUNC)(unsigned char* imageData, int imageWidth, int imageHeight, int channelsCount, void** parameters);

class FilterData
{

public:

	FilterData();
	~FilterData();

	bool load(const std::string &fileName);

	bool isValid() const;

	std::string getId() const;
	std::string getName() const;
	std::unordered_map<std::string, FitlerUiElement> getUiMap() const;
	FILTERFUNC getFilterApplyFunc() const;

private:

	HINSTANCE m_hinstLib;
	FILTERFUNC m_filterFunc;
	std::unordered_map<std::string, FitlerUiElement> m_uiMap;

	std::string m_name;
	std::string m_id;

	bool loadDll(const std::string &fileName);
	bool loadXml(const std::string &fileName);
	void cleanup();

};