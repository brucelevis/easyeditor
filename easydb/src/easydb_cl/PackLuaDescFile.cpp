#include "PackLuaDescFile.h"
#include "check_params.h"

#include <ee/FileHelper.h>
#include <ee/SymbolMgr.h>
#include <ee/StringHelper.h>

#include <easytexpacker.h>

namespace edb
{

PackLuaDescFile::~PackLuaDescFile()
{
	for_each(m_symbols.begin(), m_symbols.end(), ee::ReleaseObjectFunctor<ee::Symbol>());
}

std::string PackLuaDescFile::Command() const
{
	return "pack-lua-desc";
}

std::string PackLuaDescFile::Description() const
{
	return "pack lua desc file";
}

std::string PackLuaDescFile::Usage() const
{
	return Command() + " [json dir] [tp json] [tp dir] [output file]";
}

int PackLuaDescFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 6)) return -1;
	if (!check_folder(argv[2])) return -1;
	if (!check_folder(argv[4])) return -1;

	Trigger(argv[2], argv[3], argv[4], argv[5]);

	return 0;
}

void PackLuaDescFile::Trigger(const std::string& json_dir, const std::string& tp_json,
							  const std::string& tp_dir, const std::string& out_file)
{
	LoadJsonFiles(json_dir);
	LoadTexPacker(tp_json, tp_dir);
	
	ecoco::epd::CocoPacker packer(m_symbols, m_tex_mgr);
	packer.Parser();
	packer.Output(out_file.c_str());
}

void PackLuaDescFile::LoadJsonFiles(const std::string& dir)
{
	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir, files);

	for (int i = 0, n = files.size(); i < n; ++i) 
	{
		std::string filepath = ee::FileHelper::GetAbsolutePath(files[i].ToStdString());
		if (ee::FileType::IsType(filepath, ee::FileType::e_complex) || 
			ee::FileType::IsType(filepath, ee::FileType::e_anim))
		{
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			m_symbols.push_back(symbol);
		}
	}
}

void PackLuaDescFile::LoadTexPacker(const std::string& tp_json,
									const std::string& tp_dir)
{
	m_tex_mgr.SetSrcDataDir(tp_dir);

	int i = 1;
	while (true)
	{
		std::string path = tp_json + ee::StringHelper::ToString(i) + ".json";
		if (ee::FileHelper::IsFileExist(path)) {
			m_tex_mgr.Add(path, i-1);
		} else {
			break;
		}
		++i;
	}
}

}