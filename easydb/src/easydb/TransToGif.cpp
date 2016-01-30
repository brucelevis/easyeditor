#include <gl/glew.h>

#include "TransToGif.h"
#include "check_params.h"

#include <glfw.h>
#include <drag2d.h>
#include <easyanim.h>
#include <easyimage.h>

namespace edb
{

std::string TransToGif::Command() const
{
	return "togif";
}

std::string TransToGif::Description() const
{
	return "translate anims to gif files";
}

std::string TransToGif::Usage() const
{
	// togif D:\projects\ejoy\coco-tools\sg_characters_new\data\json\2003daoke\attack1\1 E:\gif

	return Command() + " [src path] [dst path]";
}

void TransToGif::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 4)) return;
	if (!check_folder(argv[2])) return;
	if (!check_folder(argv[3])) return;

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

	Run(ss, argv[2], argv[3]);
}

void TransToGif::Run(d2d::Snapshoot& ss, const std::string& srcdir, const std::string& dstdir) const
{
	wxArrayString files;
	d2d::FileHelper::FetchAllFiles(srcdir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath().ToStdString();
		if (d2d::FileType::IsType(filepath, d2d::FileType::e_anim))
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.c_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

			std::string name = value["name"].asString();
			if (name.empty()) {
				continue;
			}
			d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
			libanim::Symbol* anim = static_cast<libanim::Symbol*>(symbol);

			int max_frame = anim->getMaxFrameIndex();
			int width = symbol->GetSize().Width();
			int height = symbol->GetSize().Height();
			AnimatedGifSaver saver(width, height);
			for (int i = 0; i < max_frame; ++i)
			{
				anim->setFrameIndex(i + 1);
				uint8_t* rgba = ss.OutputToMemory(symbol, true);

				uint8_t* rgb = eimage::RGBA2RGB(rgba, width, height, true);
				saver.AddFrame(rgb, 1.0f / anim->getFPS());
				delete[] rgba;
				delete[] rgb;
			}
			anim->setFrameIndex(0);
			std::string filename = dstdir + "//" + name + ".gif";
			saver.Save(filename.c_str());

			symbol->Release();
		}
	}
}

}