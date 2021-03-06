#ifndef _EASYSHADER_FILE_IO_H_
#define _EASYSHADER_FILE_IO_H_

#include "Uniform.h"
#include "SliderItem.h"

#include <JSON/value.h>

namespace ee { class StageCanvas; }

namespace eshader
{

class ToolbarPanel;
class Shader;

class FileIO
{
public:
	static Shader* LoadShader(const std::string& filepath, ee::StageCanvas* canvas,
		ToolbarPanel* toolbar, bool is_2d);
	static void StoreShader(const std::string& filepath, const ToolbarPanel* toolbar);

private:
	static Shader* LoadShader(const std::string& dir, const Json::Value& value,
		ToolbarPanel* toolbar, bool is_2d);
	static Uniform* LoadUniform(const Json::Value& value, ToolbarPanel* toolbar,
		Shader* shader);

	static void StoreShader(const ToolbarPanel* toolbar, Json::Value& value);
	static void StoreUniform(const double val[16], Json::Value& value);

	static UniformType TransStrToUType(const std::string& str);
	static std::string TransUTypeToStr(UniformType type);

	template <typename T>
	static void LoadValue(const Json::Value& value, int count,
		std::vector<SliderItem<T> >& int_items, Uniform* uniform);
	static void StoreValue(const double val[16], Json::Value& value,
		bool is_float, int count);

}; // FileIO

}

#endif // _EASYSHADER_FILE_IO_H_