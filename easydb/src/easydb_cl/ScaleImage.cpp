#include "ScaleImage.h"
#include "check_params.h"
#include "utility.h"

#include <ee/SettingData.h>
#include <ee/Snapshoot.h>
#include <ee/Config.h>
#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/Sprite.h>
#include <ee/Symbol.h>
#include <ee/SpriteFactory.h>
#include <ee/EE_ShaderLab.h>

#include <easyimage.h>

#include <wx/arrstr.h>

namespace edb
{

std::string ScaleImage::Command() const
{
	return "scale-image";
}

std::string ScaleImage::Description() const
{
	return "scale image";
}

std::string ScaleImage::Usage() const
{
	std::string cmd0 = Command() + " [dir] [scale]";
	std::string cmd1 = Command() + " [src] [dst] [scale]";
	return cmd0 + " or " + cmd1;
}

int ScaleImage::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return -1;
	if (argc == 4 && !check_folder(argv[2])) return -1;

	int ret = init_gl();
	if (ret < 0) {
		return ret;
	}

	ee::Snapshoot ss;

	ee::SettingData& data = ee::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;
	bool ori_alpha_cfg = data.pre_multi_alpha;
	data.pre_multi_alpha = false;

	if (argc == 4) {
		Scale(ss, argv[2], atof(argv[3]));
	} else if (argc == 5) {
		Scale(ss, argv[2], argv[3], atof(argv[4]));
	}

	data.open_image_edge_clip = ori_clip_cfg;
	data.pre_multi_alpha = ori_alpha_cfg;

	return 0;
}

void ScaleImage::Scale(ee::Snapshoot& ss, const std::string& dir, float scale)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i) {
		Scale(ss, files[i].ToStdString(), files[i].ToStdString(), scale);
	}
}

void ScaleImage::Scale(ee::Snapshoot& ss, const std::string& src, const std::string& dst, float scale)
{
	std::string filepath = ee::FileHelper::GetAbsolutePath(src);
	if (ee::FileType::IsType(filepath, ee::FileType::e_image))
	{
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
		sm::rect r = symbol->GetSize();

		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		sprite->SetScale(sm::vec2(scale, scale));

		const sm::vec2& sz = r.Size();
		int width = sz.x * scale,
			height = sz.y * scale;
		ss.DrawSprite(sprite, true, width, height);

		sprite->Release();
		symbol->Release();

		ss.SaveToFile(dst, width, height);
	}
}

}