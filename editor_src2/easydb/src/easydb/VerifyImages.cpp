#include "VerifyImages.h"

#include <wx/wx.h>
#include <drag2d.h>

namespace edb {

VerifyImages::VerifyImages(const std::string& dirpath)
{
	InitFiles(dirpath);
	VerifyLack();
	VerifySurplus();
	Report();
}

void VerifyImages::InitFiles(const std::string& dirpath)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dirpath, files);

	for (size_t i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex)) {
			std::string filename = filepath.ToStdString();
			StringTools::toLower(filename);
			_complex_files.push_back(filename);
		} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			std::string filename = filepath.ToStdString();
			StringTools::toLower(filename);
			_anim_files.push_back(filename);
		} else if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) {
			std::string filename = d2d::FilenameTools::getFilenameWithExtension(filepath).ToStdString();
			StringTools::toLower(filename);
			_map_images.insert(std::make_pair(filename, false));
		}
	}
}

void VerifyImages::VerifyLack()
{
	for (size_t i = 0, n = _complex_files.size(); i < n; ++i)
	{
		Json::Value value;
		Json::Reader reader;
		std::ifstream fin(_complex_files[i].c_str());
		reader.parse(fin, value);
		fin.close();

		int j = 0;
		Json::Value spriteValue = value["sprite"][j++];
		while (!spriteValue.isNull()) {
			HandleSpritePath(spriteValue["filepath"].asString());
			spriteValue = value["sprite"][j++];
		}		
	}
	for (size_t i = 0, n = _anim_files.size(); i < n; ++i)
	{
		Json::Value value;
		Json::Reader reader;
		std::ifstream fin(_anim_files[i].c_str());
		reader.parse(fin, value);
		fin.close();

		int j = 0;
		Json::Value layerValue = value["layer"][j++];
		while (!layerValue.isNull()) {
			int i = 0;
			Json::Value frameValue = layerValue["frame"][i++];
			while (!frameValue.isNull()) {
				int j = 0;
				Json::Value entryValue = frameValue["actor"][j++];
				while (!entryValue.isNull()) {
					HandleSpritePath(entryValue["filepath"].asString());
					entryValue = frameValue["actor"][j++];
				}
				frameValue = layerValue["frame"][i++];
			}
			layerValue = value["layer"][j++];
		}
	}
}

void VerifyImages::VerifySurplus()
{
	std::map<std::string, bool>::iterator itr = _map_images.begin();
	for ( ; itr != _map_images.end(); ++itr)
		if (!itr->second)
			_reports.insert("Surplus Image " + itr->first);
}

void VerifyImages::Report() const
{
	if (_reports.empty()) {
		std::cout << "Images OK!" << std::endl;
	} else {
		std::set<std::string>::const_iterator itr = _reports.begin();
		for ( ; itr != _reports.end(); ++itr)
			std::cout << *itr << std::endl;
	}
}

void VerifyImages::HandleSpritePath(const std::string& path)
{
	if (!d2d::FileNameParser::isType(path, d2d::FileNameParser::e_image))
		return;

	wxFileName wxname(path);
	wxname.Normalize();
	std::string filename = d2d::FilenameTools::getFilenameWithExtension(wxname.GetFullPath());
	StringTools::toLower(filename);

	std::map<std::string, bool>::iterator itr = _map_images.find(filename);
	if (itr == _map_images.end())
		_reports.insert("Lack Image " + filename);
	else
		itr->second = true;
}

}