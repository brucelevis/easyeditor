#include "CocoPacker.h"
#include "PackAnimation.h"
#include "PackNodeFactory.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easybuilder.h>

namespace libcoco
{

CocoPacker::CocoPacker(const std::string& json_dir, const std::string& tp_name, 
					   const std::string& tp_dir)
	: m_tp(tp_dir)
{
	d2d::TextureFactory::Instance()->InitTexturePacker(tp_dir);

	d2d::SettingData& data = d2d::Config::Instance()->GetSettings();
	bool old_cfg = data.load_image;
	data.load_image = false;

	PackNodeFactory::Instance()->SetFilesDir(json_dir);

	LoadTPData(tp_name);
	LoadJsonData(json_dir);

	Pack();

	data.load_image = old_cfg;
}

void CocoPacker::OutputLua(const std::string& outfile) const
{
	ebuilder::CodeGenerator gen;
	gen.line("return {");
	PackNodeFactory::Instance()->PackToLuaString(gen, m_tp);
	gen.line("}");

	std::locale::global(std::locale(""));
	std::ofstream fout(outfile.c_str());
	std::locale::global(std::locale("C"));
	fout << gen.toText() << std::endl;
	fout.close();
}

void CocoPacker::OutputBin(const std::string& outfile) const
{
	
}

void CocoPacker::LoadJsonData(const std::string& dir)
{
	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(dir, files);

	std::vector<std::string> filepaths;
	for (int i = 0, n = files.size(); i < n; ++i) 
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		std::string filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_complex) || 
			d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim)) {
			filepaths.push_back(filepath);
		}
	}

	std::sort(filepaths.begin(), filepaths.end());
	for (int i = 0, n = filepaths.size(); i < n; ++i) {
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepaths[i]);
		m_symbols.push_back(symbol);
	}
}

void CocoPacker::LoadTPData(const std::string& tp_name)
{
	int i = 1;
	while (true) {
		std::string tp_path = tp_name + d2d::StringTools::ToString(i) + ".json";
		if (d2d::FilenameTools::IsFileExist(tp_path)) {
			m_tp.Add(tp_path);
			d2d::TextureFactory::Instance()->AddTextureFromConfig(tp_path);
		} else {
			break;
		}
		++i;
	}
}

void CocoPacker::Pack() const
{
	PackNodeFactory* factory = PackNodeFactory::Instance();
	for (int i = 0, n = m_symbols.size(); i < n; ++i) 
	{
		const d2d::ISymbol* symbol = m_symbols[i];
		if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(symbol)) {
			factory->CreateComplex(complex);
		} else if (const libanim::Symbol* anim = dynamic_cast<const libanim::Symbol*>(symbol)) {
			factory->CreateAnim(anim);
		} else {
			throw d2d::Exception("CocoPacker::Pack unhandled type.");
		}
	}
}

}
