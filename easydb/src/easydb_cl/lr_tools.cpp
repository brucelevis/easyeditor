#include "lr_tools.h"

#include <drag2d.h>

namespace edb
{

std::string get_lr_name_from_file(const std::string& filepath)
{
	std::string name = d2d::FilenameTools::getFilename(filepath);
	size_t pos = name.find_last_of(".");
	if (pos == std::string::npos) {
		pos = name.find_last_of("_lr") - 2;
	}
	name = name.substr(0, pos);
	return name;
}

}