#ifndef _EASYEDITOR_CONFIG_H_
#define _EASYEDITOR_CONFIG_H_

#include <json/json.h>

#include <set>

namespace ee
{

class SettingData;

class Config
{
public:
	static Config* Instance();

	bool IsUseDTex() const;
	void EnableUseDTex(bool enable) { m_use_dtex = enable; }

	bool IsRenderOpen() const { return m_use_render; }
	void EnableRender(bool enable) { m_use_render = enable; }

	const std::set<std::string>& GetResPathes() const { return m_resource_paths; }
	std::set<std::string>& GetResPathes() { return m_resource_paths; }

	void GetStrings(const std::string& key, std::vector<std::string>& val) const;

	const SettingData& GetSettings() const { return *m_settings; }
	SettingData& GetSettings() { return *m_settings; }

	const std::vector<std::pair<std::string, std::string> >& GetFonts() const { return m_fonts; }
	const std::vector<std::pair<std::string, std::string> >& GetUserFonts() const { return m_user_fonts; }

private:
	Config();

	void LoadFromFile(const char* filename);

	void LoadFontCfg(const Json::Value& value);
	void LoadUserFontCfg(const Json::Value& value);

private:
	Json::Value m_value;

	bool m_use_dtex;
	bool m_use_render;

	std::vector<std::pair<std::string, std::string> > m_fonts;
	std::vector<std::pair<std::string, std::string> > m_user_fonts;

	std::set<std::string> m_resource_paths;

	SettingData* m_settings;

	static bool m_loaded;

private:
	static Config* m_instance;

}; // Config

}

#endif // _EASYEDITOR_CONFIG_H_