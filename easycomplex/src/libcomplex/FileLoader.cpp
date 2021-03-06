#include "FileLoader.h"
#include "Symbol.h"
#include "config.h"

#include "LoadFromJson.h"
#include "LoadFromLua.h"
#include "LoadFromBin.h"
#include "LoadFromPSD.h"

#include <ee/Config.h>
#include <ee/EE_DTex.h>
#include <ee/FileHelper.h>

#include <fstream>

namespace ecomplex
{

//void Symbol::loadResources()
//{
//	bool use_dtex = ee::Config::Instance()->AIsUseDTex();
//	ee::DynamicTexAndFont* dtex = NULL;
//	if (use_dtex) {
//		dtex = ee::DynamicTexAndFont::Instance();
//		dtex->BeginImage();
//	}
//
//	clear();
//
//	Json::Value value;
//	Json::Reader reader;
//	std::locale::global(std::locale(""));
//	std::ifstream fin(m_filepath.c_str());
//	std::locale::global(std::locale("C"));
//	reader.parse(fin, value);
//	fin.close();
//
//	name = value["name"].asString();
//
//	m_clipbox.xmin = value["xmin"].asInt();
//	m_clipbox.xmax = value["xmax"].asInt();
//	m_clipbox.ymin = value["ymin"].asInt();
//	m_clipbox.ymax = value["ymax"].asInt();
//
//	m_use_render_cache = value["use_render_cache"].asBool();
//
// 	std::string dir = ee::FileHelper::getFileDir(m_filepath);
//	int i = 0;
//	Json::Value spriteValue = value["sprite"][i++];
//	while (!spriteValue.isNull()) {
//		std::string path = ee::FileHelper::GetAbsolutePath(dir, spriteValue["filepath"].asString());
//		Symbol* symbol = NULL;
//		std::string real_filepath = path;
//		try {
//			symbol = ee::SymbolMgr::Instance()->fetchSymbol(path);
//		} catch (ee::Exception& e) {
// 			symbol = NULL;
// 			Json::Value filepaths_val = spriteValue["filepaths"];
// 			if (!filepaths_val.isNull()) 
// 			{
// 				int j = 0;
// 				Json::Value filepath_val = filepaths_val[j++];
// 				while (!filepath_val.isNull() && !symbol) {
//					real_filepath = filepath_val.asString();
// 					std::string filepath = ee::FileHelper::GetAbsolutePath(dir, real_filepath);
//					filepath_val = filepaths_val[j++];
// 					try {
// 						symbol = ee::SymbolMgr::Instance()->fetchSymbol(filepath);
// 					} catch (ee::Exception& e) {
// 						symbol = NULL;
// 					}
// 				}
// 			}
//
//			if (!symbol) {
//				throw e;
//			}
//		}
//
// 		// load symbol filepaths
// 		Json::Value filepaths_val = spriteValue["filepaths"];
// 		if (!filepaths_val.isNull()) {
// 			std::vector<std::string> filepaths;
// 			int i = 0;
// 			Json::Value filepath_val = filepaths_val[i++];
// 			while (!filepath_val.isNull()) {
//				std::string p = filepath_val.asString();
//				if (p != real_filepath) {
//					filepaths.push_back(p);
//				}
// 				filepath_val = filepaths_val[i++];
// 			}
//			filepaths.push_back(spriteValue["filepath"].asString());
// 			symbol->SetFilepaths(filepaths);
// 		}
//
////		symbol->refresh();
//		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
//		sprite->load(spriteValue);
//
//		symbol->Release();
//
//		m_sprites.push_back(sprite);
//		spriteValue = value["sprite"][i++];
//	}	
//
//	initBounding();
//
//	if (use_dtex) {
//		dtex->EndImage();
//		if (m_use_render_cache) {
//			dtex->InsertSymbol(*this);
//		}
//	}
//}

void FileLoader::Load(const std::string& filepath, Symbol* complex)
{
	if (ee::Config::Instance()->IsUseDTex()) {
		ee::DTex::Instance()->LoadBegin();
	}

	complex->Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = ee::FileHelper::GetFileDir(filepath);
	if (!value["lua desc"].isNull()) {
		LoadFromLua::Load(value, dir, complex);
	} else if (!value["bin file"].isNull()) {
		LoadFromBin::Load(value, dir, complex);
	} else if (ee::FileType::IsType(filepath, ee::FileType::e_complex)) {
		LoadFromJson::Load(value, dir, complex);
	} else if (ee::FileType::IsType(filepath, ee::FileType::e_psd)) {
		LoadFromPSD::Load(value, dir, complex);
	}

	complex->InitBounding();

	if (ee::Config::Instance()->IsUseDTex()) {
		ee::DTex::Instance()->LoadEnd();

// 		if (complex->m_use_render_cache) {
// 			dtex->InsertSymbol(*complex);
// 		}
	}
}

}