#include "TPAdapter.h"

#include <fstream>

#include <drag2d.h>

namespace libcoco
{

void TPAdapter::Load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	m_width = value["meta"]["size"]["w"].asInt();
	m_height = value["meta"]["size"]["h"].asInt();

	std::string str = value["meta"]["scale"].asString();
	m_invscale = atof(str.c_str());

	int i = 0;
	Json::Value frameValue = value["frames"][i++];
	while (!frameValue.isNull()) {
		Entry entry;
		Load(frameValue, entry);
		m_frames.push_back(entry);
		frameValue = value["frames"][i++];
	}
}

void TPAdapter::Load(const Json::Value& value, Entry& entry)
{
	entry.filename = value["filename"].asString();
	StringTools::toLower(entry.filename);
	d2d::FilenameTools::formatSeparators(entry.filename);

	Load(value["frame"], entry.frame);
	entry.rotated = value["rotated"].asBool();
	entry.trimmed = value["trimmed"].asBool();
	Load(value["spriteSourceSize"], entry.sprite_source_size);
	entry.src_width = value["sourceSize"]["w"].asInt();
	entry.src_height = value["sourceSize"]["h"].asInt();
}

void TPAdapter::Load(const Json::Value& value, Region& region)
{
	region.x = value["x"].asInt();
	region.y = value["y"].asInt();
	region.w = value["w"].asInt();
	region.h = value["h"].asInt();
}

} // d2d