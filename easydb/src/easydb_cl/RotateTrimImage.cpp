#include <gl/glew.h>

#include "RotateTrimImage.h"
#include "check_params.h"

#include <glfw.h>
#include <easyimage.h>

namespace edb
{

static const char* OUTPUT_FILE = "rotate-trim";

std::string RotateTrimImage::Command() const
{
	return "rotate-trim";
}

std::string RotateTrimImage::Description() const
{
	return "rotate and trim image, clip blank part";
}

std::string RotateTrimImage::Usage() const
{
	return Command() + " [dir path]";
}

void RotateTrimImage::Run(int argc, char *argv[])
{
	// rotate-trim e:/test2/1001

	if (!check_number(this, argc, 3)) return;
	if (!check_folder(argv[2])) return;

	glfwInit();
	if(!glfwOpenWindow(100, 100, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return;
	}

	if (glewInit() != GLEW_OK) {
		return;
	}

	d2d::ShaderMgr::Instance()->reload();

	d2d::Snapshoot ss;

	d2d::SettingData& data = d2d::Config::Instance()->GetSettings();
	bool ori_clip_cfg = data.open_image_edge_clip;
	data.open_image_edge_clip = false;
	bool ori_alpha_cfg = data.pre_multi_alpha;
	data.pre_multi_alpha = false;

	RotateTrim(ss, argv[2]);

	data.open_image_edge_clip = ori_clip_cfg;
	data.pre_multi_alpha = ori_alpha_cfg;
}

const char* RotateTrimImage::GetOutputFileName()
{
	return OUTPUT_FILE;
}

void RotateTrimImage::RotateTrim(d2d::Snapshoot& ss, const std::string& dir)
{
	std::string output_file = dir + "\\" + OUTPUT_FILE;
	std::ofstream fout(output_file.c_str(), std::ios::binary);
	if (fout.fail()) {
		std::cout << "Can't open output file. \n";
		return;
	}

	wxArrayString files;
	d2d::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();

		std::cout << i << " / " << n << " : " << filepath << "\n";

		if (d2d::FileType::IsType(filepath, d2d::FileType::e_image))
		{
			d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);

			d2d::Image* image = static_cast<d2d::ImageSymbol*>(symbol)->GetImage();
			int width, height;
			d2d::Vector center;
			float angle;
			bool success = GetRotateTrimInfo(image, width, height, center, angle);
			if (!success || angle == 0) {
				image->Release();
				continue;
			}

			d2d::Sprite* sprite = d2d::SpriteFactory::Instance()->Create(symbol);
			sprite->SetTransform(center, angle);
			ss.DrawSprite(sprite, true, width, height);

			sprite->Release();
			symbol->Release();

			//wxString dir = d2d::FileHelper::getFileDir(filepath);
			//wxString name = d2d::FileHelper::getFilename(filepath);
 		//	wxString outpath = dir + "\\test_" + name + ".png";
 		//	ss.SaveToFile(outpath.ToStdString(), width, height);

			ss.SaveToFile(filepath, width, height);

			// output info
			wxString path = d2d::FileHelper::GetRelativePath(dir, filepath);
			fout << path << " " << center.x << " " << center.y << " " << angle << "\n";
		}
	}

	fout.close();
}

bool RotateTrimImage::GetRotateTrimInfo(const d2d::Image* image, int& width, int& height,
										d2d::Vector& center, float& angle) const
{
	eimage::ExtractOutlineRaw raw(*image);
	raw.CreateBorderLineAndMerge();
	if (raw.GetBorderLine().empty()) {
		return false;
	}
	raw.CreateBorderConvexHull();

	d2d::Vector bound[4];
	bool is_rotate = d2d::MinBoundingBox::Do(raw.GetConvexHull(), bound);

	center = (bound[0] + bound[2]) * 0.5f;
	center.x -= image->GetOriginWidth() * 0.5f;
	center.y -= image->GetOriginHeight() * 0.5f;

	center = -center;

	if (is_rotate) {
		float left = FLT_MAX;
		int left_idx;
		for (int i = 0; i < 4; ++i) {
			if (bound[i].x < left) {
				left = bound[i].x;
				left_idx = i;
			}
		}

		const d2d::Vector& s = bound[left_idx];
		const d2d::Vector& e = bound[left_idx == 3 ? 0 : left_idx + 1];
		d2d::Vector right = s;
		right.x += 1;
		angle = -d2d::Math2D::GetAngle(s, e, right);
		center = d2d::Math2D::RotateVector(center, angle);

		width = std::ceil(d2d::Math2D::GetDistance(s, e));
		height = std::ceil(d2d::Math2D::GetDistance(e, bound[(left_idx+2)%4]));
	} else {
		angle = 0;
	}

	return true;
}

}