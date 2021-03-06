#include "AverageRectCut.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/ImageData.h>
#include <ee/ImageTrim.h>
#include <ee/ImageClip.h>
#include <ee/StringHelper.h>
#include <ee/ImageSaver.h>
#include <ee/DummySprite.h>
#include <ee/DummySymbol.h>
#include <ee/SettingData.h>
#include <ee/Config.h>

#include <easycomplex.h>

namespace edb
{

static const char* IMAGE_DIR = "arc_image";
static const char* JSON_DIR = "arc_json";

std::string AverageRectCut::Command() const
{
	return "average-rect-cut";
}

std::string AverageRectCut::Description() const
{
	return "average cut image by rect";
}

std::string AverageRectCut::Usage() const
{
	return Command() + " [src dir] [dst dir]";
}

int AverageRectCut::Run(int argc, char *argv[])
{
	// average-rect-cut e:/test2/1001 e:/test2/1001 256

	if (!check_number(this, argc, 4)) return -1;
	if (!check_folder(argv[2])) return -1;

	ee::SettingData& setting = ee::Config::Instance()->GetSettings();
	bool old = setting.pre_multi_alpha;
	setting.pre_multi_alpha = false;
	Trigger(argv[2], argv[3], atoi(argv[4]));
	setting.pre_multi_alpha = old;

	return 0;
}

void AverageRectCut::Trigger(const std::string& src_dir, const std::string& dst_dir, int min_edge)
{
	std::string out_img_dir = dst_dir + "\\" + IMAGE_DIR;
	ee::FileHelper::MkDir(out_img_dir);
	std::string out_json_dir = dst_dir + "\\" + JSON_DIR;
	ee::FileHelper::MkDir(out_json_dir);

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(src_dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();

		std::cout << i << " / " << n << " : " << filepath << "\n";
		if (ee::FileType::IsType(filepath, ee::FileType::e_image)) {
			RectCutImage(src_dir, dst_dir, filepath, min_edge);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_complex)) {
			FixComplex(src_dir, dst_dir, filepath);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_anim)) {
			FixAnim(src_dir, dst_dir, filepath);
		} else if (ee::FileType::IsType(filepath, ee::FileType::e_scale9)) {
			FixScale9(src_dir, dst_dir, filepath);
		}
	}
}

void AverageRectCut::RectCutImage(const std::string& src_dir, const std::string& dst_dir, 
								  const std::string& filepath, int min_edge) const
{
	std::string out_img_dir = dst_dir + "\\" + IMAGE_DIR;
	std::string out_json_dir = dst_dir + "\\" + JSON_DIR;

	ee::ImageData* img = ee::ImageDataMgr::Instance()->GetItem(filepath);		

	ee::ImageTrim trim(*img);
	sm::rect img_r = trim.Trim();
	if (!img_r.IsValid()) {
		img_r.xmin = img_r.ymin = 0;
		img_r.xmax = img->GetWidth();
		img_r.ymax = img->GetHeight();
	}

	const sm::vec2& sz = img_r.Size();

	ee::ImageClip clip(*img);
	const uint8_t* pixels = clip.Clip(img_r.xmin, img_r.xmax, img_r.ymin, img_r.ymax);
	ee::ImageData* img_trimed = new ee::ImageData(pixels, sz.x, sz.y, 4);

	std::string filename = ee::FileHelper::GetRelativePath(src_dir, filepath);
	filename = filename.substr(0, filename.find_last_of('.'));
	ee::StringHelper::ReplaceAll(filename, "\\", "%");

	ecomplex::Symbol complex;
	ee::ImageClip img_cut(*img_trimed, false);

	int row = std::ceil(sz.y / min_edge),
		col = std::ceil(sz.x / min_edge);
	float xedge = sz.x / col,
		  yedge = sz.y / row;
	for (int y = 0; y < row; ++y) {
		for (int x = 0; x < col; ++x) {
			int xmin = std::floor(xedge * x),
				ymin = std::floor(yedge * y);
			int xmax = std::floor(xedge * (x + 1)),
				ymax = std::floor(yedge * (y + 1));
			if (x == col - 1) {
				xmax = sz.x;
			}
			if (y == row - 1) {
				ymax = sz.y;
			}

			int w = xmax - xmin,
				h = ymax - ymin;
			const uint8_t* pixels = img_cut.Clip(xmin, xmax, ymin, ymax);

			std::string img_name = ee::StringHelper::Format("%s#%d#%d#%d#%d#.png", filename.c_str(), xmin, ymin, w, h);
			std::string img_out_path = out_img_dir + "\\" + img_name;
			ee::ImageSaver::StoreToFile(pixels, w, h, 4, img_out_path, ee::ImageSaver::e_png);
			delete[] pixels;

			std::string spr_path = std::string(out_img_dir + "\\" + img_name);
			ee::Sprite* spr = new ee::DummySprite(new ee::DummySymbol(spr_path, w, h));
			sm::vec2 offset;
			offset.x = img_r.xmin + xmin + w * 0.5f - img->GetWidth() * 0.5f;
			offset.y = img_r.ymin + ymin + h * 0.5f - img->GetHeight() * 0.5f;
			spr->Translate(offset);
			complex.Add(spr);
		}
	}

	std::string json_out_path = out_json_dir + "\\" + filename + "_complex.json";
	ecomplex::FileStorer::Store(json_out_path.c_str(), &complex);

	img->Release();
	img_trimed->Release();
}

void AverageRectCut::FixComplex(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);

	int i = 0;
	Json::Value spr_val = value["sprite"][i++];
	while (!spr_val.isNull()) {
		Json::Value& val = value["sprite"][i-1];
		FixSpriteValue(src_dir, dst_dir, dir, val);
		spr_val = value["sprite"][i++];
	}		

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void AverageRectCut::FixAnim(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);

	int i = 0;
	Json::Value layerVal = value["layer"][i++];
	while (!layerVal.isNull()) {
		int j = 0;
		Json::Value frameVal = layerVal["frame"][j++];
		while (!frameVal.isNull()) {
			int k = 0;
			Json::Value entryVal = frameVal["actor"][k++];
			while (!entryVal.isNull()) {
				Json::Value& val = value["layer"][i-1]["frame"][j-1]["actor"][k-1];
				FixSpriteValue(src_dir, dst_dir, dir, val);
				entryVal = frameVal["actor"][k++];
			}

			frameVal = layerVal["frame"][j++];
		}

		layerVal = value["layer"][i++];
	}		

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void AverageRectCut::FixScale9(const std::string& src_dir, const std::string& dst_dir, const std::string& filepath) const
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);

	int i = 0;
	Json::Value spr_val = value["sprite"][i++];
	while (!spr_val.isNull()) {
		Json::Value& val = value["sprite"][i-1];
		FixSpriteValue(src_dir, dst_dir, dir, val);
		spr_val = value["sprite"][i++];
	}		

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void AverageRectCut::FixSpriteValue(const std::string& src_dir, const std::string& dst_dir,
									 const std::string& file_dir, Json::Value& sprite_val) const
{
	std::string filepath = sprite_val["filepath"].asString();
	if (!ee::FileType::IsType(filepath, ee::FileType::e_image)) {
		return;
	}

	filepath = ee::FileHelper::GetAbsolutePath(file_dir, filepath);

	std::string filename = ee::FileHelper::GetRelativePath(src_dir, filepath);
	filename = filename.substr(0, filename.find_last_of('.')) + "_complex.json";

	// todo
	const std::string oldVal = "\\", newVal = "%";
	for(std::string::size_type pos(0); pos != std::string::npos; pos += oldVal.length())   
	{   
		if((pos = filename.find(oldVal, pos)) != std::string::npos)
			filename.replace(pos, oldVal.length(), newVal);   
		else   
			break;   
	}  

	std::string out_json_dir = dst_dir + "\\" + JSON_DIR;
	std::string fixed_filepath = out_json_dir + "\\" + filename;
	sprite_val["filepath"] = ee::FileHelper::GetRelativePath(file_dir, fixed_filepath);
}

}