#include "OutlineToTriStrip.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/FileType.h>
#include <ee/JsonSerializer.h>
#include <ee/Triangulation.h>

#include <wx/arrstr.h>

#include <easyimage.h>

#include <fstream>

namespace edb
{

std::string OutlineToTriStrip::Command() const
{
	return "outline2strip";
}

std::string OutlineToTriStrip::Description() const
{
	return "create triangle strips from outline";
}

std::string OutlineToTriStrip::Usage() const
{
	// outline2strip E:\test2\image
	return Command() + " [dir]";
}

int OutlineToTriStrip::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_folder(argv[2])) return -1;

	Trigger(argv[2]);

	return 0;
}


void OutlineToTriStrip::Trigger(const std::string& dir) const
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (!ee::FileType::IsType(filepath, ee::FileType::e_image)) {
			continue;
		}

		std::string outline_path = ee::FileHelper::GetFilenameAddTag(
			filepath, eimage::OUTLINE_FILE_TAG, "json");
		if (!ee::FileHelper::IsFileExist(filepath)) {
			continue;
		}

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(outline_path.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		std::vector<sm::vec2> vertices;
		ee::JsonSerializer::Load(value["normal"], vertices);
		if (vertices.empty()) {
			continue;
		}

		std::vector<sm::vec2> tris;
		ee::Triangulation::Normal(vertices, tris);
		std::vector<std::vector<sm::vec2> > strips;
//		ee::Triangulation::strips(tris, strips);
		strips.push_back(tris);

		Json::Value value_out;
		for (int i = 0, n = strips.size(); i < n; ++i) {
			ee::JsonSerializer::Store(strips[i], value_out["strips"][i]);
		}

		std::string out_file = ee::FileHelper::GetFilenameAddTag(filepath, 
			eimage::TRI_STRIP_FILE_TAG, "json");
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(out_file.c_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value_out);
		fout.close();	
	}
}

}