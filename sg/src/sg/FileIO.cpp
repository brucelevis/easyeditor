#include "FileIO.h"
#include "StagePanel.h"
#include "SymbolExt.h"
#include "SpriteExt.h"

namespace sg
{

void FileIO::load(const char* filename, StagePanel* stage)
{
// 	d2d::SymbolMgr::Instance()->Clear();
// 	d2d::BitmapMgr::Instance()->Clear();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string dir = d2d::FileHelper::GetFileDir(filename) + "\\";

	int i = 0;
	Json::Value imgValue = value["image"][i++];
	while (!imgValue.isNull()) {
		d2d::Sprite* sprite = load(imgValue, stage, dir);
		d2d::InsertSpriteSJ::Instance()->Insert(sprite);
		imgValue = value["image"][i++];
	}

//	library->LoadFromSymbolMgr(*d2d::SymbolMgr::Instance());
}

void FileIO::store(const char* filename, StagePanel* stage)
{
	Json::Value value;

	std::string dir = d2d::FileHelper::GetFileDir(filename) + "\\";

	std::vector<d2d::Sprite*> sprites;
	stage->TraverseSprites(d2d::FetchAllVisitor<d2d::Sprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i) {
		value["image"][i] = store(sprites[i], stage, dir);
	}

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

d2d::Sprite* FileIO::load(const Json::Value& value, StagePanel* stage, const std::string& dir)
{
	int row = value["row"].asInt(),
		col = value["col"].asInt();

	std::string filepath = d2d::SymbolSearcher::GetSymbolPath(dir, value);
	d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
	SetSymbolUserData(symbol);

	d2d::Vector pos;
	d2d::Sprite* sprite = d2d::SpriteFactory::Instance()->Create(symbol);
	sprite->tag = value["tag"].asString();
	symbol->Release();

	//// old
	//stage->TransGridPosToCoords(row, col, pos);
 	// new
 	SymbolExt* symbol_info = static_cast<SymbolExt*>(sprite->GetSymbol().GetUserData());
 	int size = symbol_info->building->size;
 	if (size % 2) {
 		int offset = (size - 1) / 2;
 		row += offset;
 		col -= offset;
 	} else {
 		int offset = size / 2;
 		row += (offset - 1);
 		col -= offset;
 	}
 	stage->TransGridPosToCoordsNew(row, col, pos);

	if (!value["level"].isNull()) {
		SpriteExt* spr_info = new SpriteExt;
		spr_info->level = value["level"].asInt();
		sprite->SetUserData(spr_info);
	}
	int level = value["row"].asInt();

 	//////////////////////////////////////////////////////////////////////////

	sprite->Translate(pos);

	return sprite;
}

Json::Value FileIO::store(const d2d::Sprite* sprite, StagePanel* stage, 
						  const std::string& dir)
{
	Json::Value value;

	value["filepath"] = d2d::FileHelper::GetRelativePath(dir,
		sprite->GetSymbol().GetFilepath()).ToStdString();

	int row, col;
	stage->TransCoordsToGridPosNew(sprite->GetPosition(), row, col);

	SymbolExt* symbol_info = static_cast<SymbolExt*>(sprite->GetSymbol().GetUserData());
	int size = symbol_info->building->size;
	if (size % 2) {
		int offset = (size - 1) / 2;
		row -= offset;
		col += offset;
	} else {
		int offset = size / 2;
		row -= (offset - 1);
		col += offset;
	}

	value["row"] = row;
	value["col"] = col;
	value["tag"] = sprite->tag;

	value["name"] = symbol_info->building->name;
	
	SpriteExt* spr_info = static_cast<SpriteExt*>(sprite->GetUserData());	
	value["level"] = spr_info->level;

	return value;
}

void FileIO::SetSymbolUserData(d2d::Symbol* symbol)
{
	if (symbol->GetUserData()) {
		return;
	}

	wxString filepath = symbol->GetFilepath();
	if (!filepath.Contains("wall")) {
		return;
	}

	int pos = filepath.find_last_of('_');
	std::string wall_type = filepath.substr(pos + 1, 1);
	std::string wall_path = filepath.substr(0, pos) + ".png";

	try {
		d2d::Symbol* wall_symbol = d2d::SymbolMgr::Instance()->FetchSymbol(wall_path);
		if (!wall_symbol || !wall_symbol->GetUserData()) {
			return;
		}

		SymbolExt* info = static_cast<SymbolExt*>(wall_symbol->GetUserData());

		SymbolExt* new_info = new SymbolExt;
		new_info->size = info->size;
		new_info->remain = info->remain;
		new_info->wall_type = (wall_type[0] - '0');
		new_info->level = info->level;
		new_info->building = info->building;

		symbol->SetUserData(new_info);
	} catch (d2d::Exception& e) {
		;
	}
}

}