#pragma once

#include <MyGUI.h>
#include "sigslot.h"


class BackwardCompatibilityManager :
	public MyGUI::Singleton<BackwardCompatibilityManager>,
	public sigslot::has_slots<>
{
public:
	BackwardCompatibilityManager();
	virtual ~BackwardCompatibilityManager();

	void initialise();
	void shutdown();

	const MyGUI::VectorString& getVersions() const;
	const std::string& getDefaultVersion() const;

	const std::string& getCurrentVersion() const;
	void setCurrentVersion(const std::string& _value);

	void serialiseProperty(MyGUI::xml::Element* _node, const std::string& _widgetType, const MyGUI::PairString& _property, bool _compatibility);

private:
	void notifySettingsChanged(const std::string& _path);

private:
	MyGUI::VectorString mVersions;
	std::string mCurrentVersion;
	MyGUI::MapString mPropertyNames;
};

