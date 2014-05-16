#include "TransToGif.h"

#include <drag2d.h>
#include <easyanim.h>
#include <glfw.h>

namespace edb
{

void TransToGif::trigger(const std::string& srcdir, const std::string& dstdir)
{
	glfwInit();
	if(!glfwOpenWindow(800, 600, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return;
	}
	d2d::Snapshoot ss;

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(srcdir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.fn_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

			std::string name = value["name"].asString();
			if (name.empty()) {
				continue;
			}
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
			anim::Symbol* anim = static_cast<anim::Symbol*>(symbol);

			int max_frame = anim->getMaxFrameIndex();
			int width = symbol->getSize().xLength();
			int height = symbol->getSize().yLength();
			AnimatedGifSaver saver(width, height);
			for (int i = 0; i < max_frame; ++i)
			{
				unsigned char* rgba = ss.outputAnimToMemory(anim, i + 1, true);
				unsigned char* rgb = formatRGBA(rgba, width, height);
				saver.AddFrame(rgb, 1 / 30.0f);
				delete[] rgba;
				delete[] rgb;
			}
			std::string filename = dstdir + "//" + name + ".gif";
			saver.Save(filename.c_str());
			
			symbol->release();
		}
	}
}

unsigned char* TransToGif::formatRGBA(const unsigned char* rgba, int width, int height)
{
	int ptr = 0;
	unsigned char* ret = new unsigned char[width * height * 3];
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int p = ((height - 1 - i) * width + j) * 4;
			ret[ptr++] = rgba[p++];
			ret[ptr++] = rgba[p++];
			ret[ptr++] = rgba[p++];
		}
	}
	return ret;
}

}