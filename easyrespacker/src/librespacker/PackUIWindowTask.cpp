#include "PackUIWindowTask.h"
#include "PackUI.h"
#include "pack_ui_cfg.h"

#include <ee/FileHelper.h>
#include <ee/std_functor.h>
#include <ee/StringHelper.h>
#include <ee/Exception.h>

#include <algorithm>

namespace erespacker
{

PackUIWindowTask::PackUIWindowTask(const std::string& filepath, const Json::Value& value)
	: PackUITask(filepath)
	, m_wrapper_id(-1)
{
	m_name = value["name"].asString();

	m_width = value["view"]["width"].asInt();
	m_height = value["view"]["height"].asInt();

	m_wrapper_filepath = value["wrapper filepath"].asString();
	m_wrapper_filepath = ee::FileHelper::GetAbsolutePathFromFile(filepath, m_wrapper_filepath);
	m_wrapper_filepath = ee::FileHelper::FormatFilepathAbsolute(m_wrapper_filepath);

	PackUI::Instance()->Instance()->AddListener(m_wrapper_filepath, this);

	LoadItems(value, ee::FileHelper::GetFileDir(filepath));
}

PackUIWindowTask::~PackUIWindowTask()
{
	for_each(m_items.begin(), m_items.end(), ee::DeletePointerFunctor<Item>());
}

void PackUIWindowTask::OnKnownPackID(const std::string& filepath, int id)
{
	if (filepath == m_wrapper_filepath) {
		m_wrapper_id = id;
		return;
	}

	for (int i = 0, n = m_items.size(); i < n; ++i) {
		Item* item = m_items[i];
		if (item->filepath == filepath) {
			item->id = id;
		}
	}
}

void PackUIWindowTask::Output(const std::string& dir, Json::Value& value) const
{
	if (m_wrapper_id == -1) {
		throw ee::Exception("PackUIWindowTask::Output out id -1, wrapper_file %s", m_wrapper_filepath.c_str());
	}

	Json::Value val;

	val["type"] = UI_WINDOW;

	val["name"] = ee::StringHelper::ToUtf8(m_name);

	val["width"] = m_width;
	val["height"] = m_height;

	val["id"] = m_wrapper_id;

	for (int i = 0, n = m_items.size(); i < n; ++i) {
		Item* item = m_items[i];
		Json::Value item_val;
		item_val["id"] = item->id;
		item_val["anchor"] = item->anchor;
		val["sprite"][i] = item_val;
	}

	value[value.size()] = val;
}

void PackUIWindowTask::LoadItems(const Json::Value& value, const std::string& dir)
{
	int idx = 0;
	Json::Value spr_val = value["sprite"][idx++];
	while (!spr_val.isNull()) {
		Item* item = new Item;

		item->name = spr_val["name"].asString();
		item->filepath = spr_val["filepath"].asString();
		item->id = -1;
		item->anchor = -1;

		item->filepath = ee::FileHelper::GetAbsolutePath(dir, item->filepath);
		item->filepath = ee::FileHelper::FormatFilepathAbsolute(item->filepath);
		PackUI::Instance()->Instance()->AddListener(item->filepath, this);

		m_items.push_back(item);

		spr_val = value["sprite"][idx++];
	}

	for (int i = 0; i < 9; ++i) 
	{
		Json::Value anchor_val = value["anchor"][i];
		if (anchor_val.isNull()) {
			continue;
		}

		int idx = 0;
		Json::Value spr_val = anchor_val[idx++];
		while (!spr_val.isNull()) {
			std::string name = spr_val["name"].asString();
			Item* item = NULL;
			for (int j = 0, m = m_items.size(); j < m; ++j) {
				if (name == m_items[j]->name) {
					item = m_items[j];
					break;
				}
			}

			assert(item);
			item->anchor = i;

			spr_val = anchor_val[idx++];
		}
	}

	idx = 0;
	Json::Value ref_val = value["ref_spr"][idx++];
	while (!spr_val.isNull()) {
		std::string filepath = ref_val["filepath"].asString();
		filepath = ee::FileHelper::GetAbsolutePath(dir, filepath);

		Json::Value ext_val;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filepath.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, ext_val);
		fin.close();

		LoadItems(ext_val, ee::FileHelper::GetFileDir(filepath));

		ref_val = value["ref_spr"][idx++];
	}
}

}