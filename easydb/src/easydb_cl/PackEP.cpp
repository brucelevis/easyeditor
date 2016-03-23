#include <gl/glew.h>

#include "PackEP.h"
#include "check_params.h"

#include <ee/ShaderMgr.h>

#include <glfw.h>

#include <easyrespacker.h>

namespace edb
{

std::string PackEP::Command() const
{
	return "pack-ep";
}

std::string PackEP::Description() const
{
	return "pack ep file";
}

std::string PackEP::Usage() const
{
	return Command() + " [json dir] [tp json] [tp dir] [output file] [output type] [LOD] [SCALE]";
}

int PackEP::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 7)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[4])) return -1;

	int LOD = 0;
	if (argc >= 8) {
		LOD = atoi(argv[7]);
	}

	float scale = 1;
	if (argc >= 9) {
		scale = atof(argv[8]);
	}

	if (LOD != 0 || scale != 1) {
		glfwInit();
		if(!glfwOpenWindow(100, 100, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
		{
			glfwTerminate();
			return -2;
		}

		if (glewInit() != GLEW_OK) {
			return -2;
		}

		ee::ShaderMgr::Instance()->reload();
	}

// 	Trigger(argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], LOD, scale);
	Trigger(argv[2], argv[3], argv[4], argv[5], argv[6], LOD, scale);

	return 0;
}

// void PackEP::Trigger(const std::string& pkgs_cfg_file,
// 					 const std::string& json_dir, const std::string& tp_json,
// 					 const std::string& tp_dir, const std::string& out_file,
// 					 const std::string& type, int LOD, float scale)
void PackEP::Trigger(const std::string& json_dir, const std::string& tp_json,
					 const std::string& tp_dir, const std::string& out_file,
					 const std::string& type, int LOD, float scale)
{
	if (type == "ept_desc") {
		erespacker::ResPacker::OutputEptDesc(out_file, tp_json);
		return;
	}

	erespacker::ResPacker packer(json_dir, tp_json, tp_dir);
	if (type == "lua") {
		packer.OutputLua(out_file + ".lua", scale);
	} else if (type == "ep") {
		packer.OutputEpe(out_file, true, scale);
		packer.OutputEpt(out_file, erespacker::TT_PNG8, LOD, scale);
		erespacker::ResPacker::OutputEptDesc(out_file, tp_json);
		packer.OutputUIExtra(out_file);
		packer.OutputSprID(out_file);
	} else if (type == "epe") {
		packer.OutputEpe(out_file, true, scale);
		packer.OutputUIExtra(out_file);
		packer.OutputSprID(out_file);
	} else if (type == "ept") {
		packer.OutputEpt(out_file, erespacker::TT_PNG8, LOD, scale);
		erespacker::ResPacker::OutputEptDesc(out_file, tp_json);
	} else if (type == "debug") {
		packer.OutputLua(out_file + ".lua", scale);
		packer.OutputEpe(out_file, true, scale);
		packer.OutputUIExtra(out_file);
		packer.OutputSprID(out_file);
	} else if (type == "all") {
		packer.OutputLua(out_file + ".lua", scale);
		packer.OutputEpe(out_file, true, scale);
		packer.OutputEpt(out_file, erespacker::TT_PNG8, LOD, scale);
		erespacker::ResPacker::OutputEptDesc(out_file, tp_json);
		packer.OutputUIExtra(out_file);
		packer.OutputSprID(out_file);
	}
}

}