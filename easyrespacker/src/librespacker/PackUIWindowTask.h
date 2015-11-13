#ifndef _LIBRESPACKER_PACK_UI_WINDOW_TASK_H_
#define _LIBRESPACKER_PACK_UI_WINDOW_TASK_H_

#include "PackUITask.h"

namespace librespacker
{

class PackUIWindowTask : public PackUITask
{
public:
	PackUIWindowTask(const std::string& filepath, const Json::Value& value);
	virtual ~PackUIWindowTask();

	virtual void OnKnownPackID(const std::string& filepath, int id);
	virtual void Output(const std::string& dir, Json::Value& value) const;

private:
	struct Item {
		std::string name;
		std::string filepath;
		int id;
		int anchor;
	};

private:
	std::string m_wrapper_filepath;
	int m_wrapper_id;

	std::vector<Item*> m_items;

	std::string m_name;

	int m_width, m_height;

}; // PackUIWindowTask

}

#endif // _LIBRESPACKER_PACK_UI_WINDOW_TASK_H_