#include "LRJsonPacker.h"
#include "check_params.h"
#include "lr_typedef.h"
#include "lr_tools.h"

#include <ee/FileHelper.h>
#include <ee/SymbolSearcher.h>
#include <ee/SpriteFactory.h>
#include <ee/JsonSerializer.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/StringHelper.h>
#include <ee/Math2D.h>
#include <ee/SpriteIO.h>

#include <lr/dataset/Grids.h>
#include <lr/dataset/CharacterFileName.h>

#include <easyshape.h>

namespace edb
{

std::string LRJsonPacker::Command() const
{
	return "lr-json-pack";
}

std::string LRJsonPacker::Description() const
{
	return "create shape table from lr file";
}

std::string LRJsonPacker::Usage() const
{
	// lr-json-pack e:/test2/test_lr.json
	std::string usage = Command() + " [filepath]";
	return usage;
}

int LRJsonPacker::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_file(argv[2])) return -1;

	Run(argv[2]);

	return 0;
}

void LRJsonPacker::Run(const std::string& filepath)
{
	std::string filename = ee::FileHelper::GetFilename(filepath);
	if (filename.find(".") != std::string::npos) {
		PackLogic(filepath);
	} else {
		PackGraphics(filepath);
	}
}

void LRJsonPacker::PackGraphics(const std::string& filepath)
{
	Json::Value lr_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	m_dir = ee::FileHelper::GetFileDir(filepath) + "\\";

	Json::Value out_val;

	lr::Grids grids;
	int w = lr_val["size"]["width"].asUInt(),
		h = lr_val["size"]["height"].asUInt();
	grids.Build(w, h);

	int col, row;
	grids.GetGridSize(col, row);
	out_val["col"] = col;
	out_val["row"] = row;

	std::string lr_name = get_lr_name_from_file(filepath);

	ParserSpecial(lr_val, lr_name, out_val);
	ParserShapeLayer(lr_val, grids, true, 6, "collision region", out_val);

	out_val["package"] = lr_name + "_scene";

	out_val["base"] = lr_name + "_base";

	std::string outfile = filepath.substr(0, filepath.find_last_of('_')) + ".json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(outfile.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, out_val);
	fout.close();
}

void LRJsonPacker::PackLogic(const std::string& filepath)
{
	Json::Value lr_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	m_dir = ee::FileHelper::GetFileDir(filepath) + "\\";

	Json::Value out_val;

	out_val["width"] = lr_val["size"]["width"];
	out_val["height"] = lr_val["size"]["height"];
	out_val["view width"] = lr_val["size"]["view width"];
	out_val["view height"] = lr_val["size"]["view height"];
	out_val["view offset x"] = lr_val["size"]["view offset x"];
	out_val["view offset y"] = lr_val["size"]["view offset y"];

	lr::Grids grids;
	int w = lr_val["size"]["width"].asUInt(),
		h = lr_val["size"]["height"].asUInt();
	grids.Build(w, h);

	int col, row;
	grids.GetGridSize(col, row);
	out_val["col"] = col;
	out_val["row"] = row;

	ParserCharacter(lr_val, grids, 2, "character", out_val);
	ParserPoint(lr_val, 3, "point", out_val);
	ParserShapeLayer(lr_val, grids, false, 4, "path", out_val);
	ParserShapeLayer(lr_val, grids, true, 5, "region", out_val);
	ParserCamera(lr_val, 7, "camera", out_val);
	ParserLevel(lr_val, 8, "level", out_val);

	std::string outfile = filepath.substr(0, filepath.find_last_of('_')) + ".json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(outfile.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, out_val);
	fout.close();
}

void LRJsonPacker::ParserShapeLayer(const Json::Value& src_val, const lr::Grids& grids, bool force_grids,
									int layer_idx, const char* name, Json::Value& out_val)
{
	const Json::Value& layer_val = src_val["layer"][layer_idx];

	ParserShapeFromSprite(layer_val, grids, force_grids, name, out_val);
	ParserShapeFromShape(layer_val, grids, force_grids, name, out_val);

	int idx = 0;
	Json::Value cl_val = layer_val["layers"][idx++];
	while (!cl_val.isNull()) {
		ParserShapeFromSprite(cl_val, grids, force_grids, name, out_val);
		ParserShapeFromShape(cl_val, grids, force_grids, name, out_val);
		cl_val = layer_val["layers"][idx++];
	}
}

void LRJsonPacker::ParserShapeFromSprite(const Json::Value& src_val, const lr::Grids& grids, 
										 bool force_grids, const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value spr_val = src_val["sprite"][idx++];
	while (!spr_val.isNull()) {
		std::string spr_path = ee::SymbolSearcher::GetSymbolPath(m_dir, spr_val);
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(spr_path);
		if (!symbol) {
			throw ee::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", 
				m_dir.c_str(), spr_path.c_str());
		}

		Json::Value dst_val;
		dst_val["name"] = spr_val["name"];

		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		sprite->Load(spr_val);

		eshape::Sprite* shape_spr = dynamic_cast<eshape::Sprite*>(sprite);
		assert(shape_spr);
		const std::vector<ee::Shape*>& shapes = shape_spr->GetSymbol().GetShapes();
		for (int i = 0, n = shapes.size(); i < n; ++i) {
			ParserShape(shapes[i], sprite->GetPosition(), sprite->GetAngle(), grids, force_grids, dst_val);
		}

		int sz = out_val[name].size();
		out_val[name][sz] = dst_val;

		sprite->Release();
		symbol->Release();

		spr_val = src_val["sprite"][idx++];
	}
}

void LRJsonPacker::ParserShapeFromShape(const Json::Value& src_val, const lr::Grids& grids, 
										bool force_grids, const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value shape_val = src_val["shape"][idx++];
	while (!shape_val.isNull()) {
		ee::Shape* shape = eshape::ShapeFactory::CreateShapeFromFile(shape_val, m_dir);

		Json::Value dst_val;
		dst_val["name"] = shape_val["name"];
		ParserShape(shape, sm::vec2(0, 0), 0, grids, force_grids, dst_val);

		int sz = out_val[name].size();
		out_val[name][sz] = dst_val;

		shape->Release();

		shape_val = src_val["shape"][idx++];
	}
}

void LRJsonPacker::ParserShape(ee::Shape* shape, const sm::vec2& offset, float angle,
							   const lr::Grids& grids, bool force_grids, Json::Value& out_val)
{
	if (eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(shape))
	{
		std::vector<int> grid_idx;

		std::vector<sm::vec2> bound = poly->GetVertices();
		for (int i = 0, n = bound.size(); i < n; ++i) {
			bound[i] = ee::Math2D::RotateVector(bound[i], angle) + offset;
		}
		grid_idx = grids.IntersectPolygon(bound);

		for (int i = 0, n = grid_idx.size(); i < n; ++i) {
			int sz = out_val["grid"].size();
			out_val["grid"][sz] = grid_idx[i];
		}
	}
	else if (eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(shape))
	{
		std::vector<sm::vec2> bound = chain->GetVertices();
		for (int i = 0, n = bound.size(); i < n; ++i) {
			bound[i] = ee::Math2D::RotateVector(bound[i], angle) + offset;
		}

		if (force_grids) {
			std::vector<int> grid_idx;
			grid_idx = grids.IntersectPolyline(bound);

			for (int i = 0, n = grid_idx.size(); i < n; ++i) {
				int sz = out_val["grid"].size();
				out_val["grid"][sz] = grid_idx[i];
			}
		} else {
			ee::JsonSerializer::Store(bound, out_val["pos"]);
		}
	}
	else
	{
		throw ee::Exception("LRJsonPacker::ParserPolyLayer error shape type");
	}
}

void LRJsonPacker::ParserPoint(const Json::Value& src_val, int layer_idx, const char* name, Json::Value& out_val)
{
	const Json::Value& layer_val = src_val["layer"][layer_idx];

	ParserPointFromSprite(layer_val, name, out_val);
	
	int idx = 0;
	Json::Value cl_val = layer_val["layers"][idx++];
	while (!cl_val.isNull()) {
		ParserPointFromSprite(cl_val, name, out_val);
		cl_val = layer_val["layers"][idx++];
	}
}

void LRJsonPacker::ParserPointFromSprite(const Json::Value& src_val, const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value spr_val = src_val["sprite"][idx++];
	while (!spr_val.isNull()) 
	{
		std::string spr_path = ee::SymbolSearcher::GetSymbolPath(m_dir, spr_val);
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(spr_path);
		if (!symbol) {
			std::string filepath = spr_val["filepath"].asString();
			throw ee::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", 
				m_dir.c_str(), filepath.c_str());
		}

		Json::Value shape_val;
		shape_val["name"] = spr_val["name"];

		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		sprite->Load(spr_val);

		shape_val["x"] = sprite->GetPosition().x;
		shape_val["y"] = sprite->GetPosition().y;

		int sz = out_val[name].size();
		out_val[name][sz] = shape_val;

		sprite->Release();
		symbol->Release();

		spr_val = src_val["sprite"][idx++];
	}
}

void LRJsonPacker::ParserCamera(const Json::Value& src_val, int layer_idx, const char* name, Json::Value& out_val)
{
	const Json::Value& layer_val = src_val["layer"][layer_idx];

	ParserCameraFromSprite(layer_val, name, out_val);

	int idx = 0;
	Json::Value cl_val = layer_val["layers"][idx++];
	while (!cl_val.isNull()) {
		ParserCameraFromSprite(cl_val, name, out_val);
		cl_val = layer_val["layers"][idx++];
	}
}

void LRJsonPacker::ParserCameraFromSprite(const Json::Value& src_val, const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value spr_val = src_val["sprite"][idx++];
	while (!spr_val.isNull()) 
	{
		Json::Value cam_val;

		ee::SpriteIO::Data data;
		ee::SpriteIO::Load(spr_val, data);

		cam_val["name"] = data.name;
		cam_val["x"] = data.position.x;
		cam_val["y"] = data.position.y;
		cam_val["scale"] = data.scale.x;

		int sz = out_val[name].size();
		out_val[name][sz] = cam_val;

		spr_val = src_val["sprite"][idx++];
	}
}

void LRJsonPacker::ParserCharacter(const Json::Value& src_val, 
								   const lr::Grids& grids, int layer_idx, 
								   const char* name, Json::Value& out_val)
{
	const Json::Value& layer_val = src_val["layer"][layer_idx];

	ParserCharacterFromSprite(layer_val, grids, name, out_val);

	int idx = 0;
	Json::Value cl_val = layer_val["layers"][idx++];
	while (!cl_val.isNull()) {
		ParserCharacterFromSprite(cl_val, grids, name, out_val);
		cl_val = layer_val["layers"][idx++];
	}
}

void LRJsonPacker::ParserCharacterFromSprite(const Json::Value& src_val, const lr::Grids& grids, const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value spr_val = src_val["sprite"][idx++];
	while (!spr_val.isNull()) 
	{
		std::string filepath = spr_val["filepath"].asString();
		filepath = ee::FileHelper::GetAbsolutePath(m_dir, filepath);
		if (ee::FileType::IsType(filepath, ee::FileType::e_particle3d)) {
			spr_val = src_val["sprite"][idx++];
			continue;
		}				

		ee::SpriteIO::Data data;
		ee::SpriteIO::Load(spr_val, data);

		Json::Value char_val;
		char_val["name"] = data.name;
		char_val["x"] = data.position.x;
		char_val["y"] = data.position.y;

		// region
		std::string shape_tag = ee::FileType::GetTag(ee::FileType::e_shape);
		std::string shape_filepath = ee::FileHelper::GetFilenameAddTag(filepath, shape_tag, "json");
		std::string tag_ext;
		if (ee::FileHelper::IsFileExist(shape_filepath)) {
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(shape_filepath);
			const std::vector<ee::Shape*>& shapes = static_cast<eshape::Symbol*>(symbol)->GetShapes();
			if (!shapes.empty()) {
				tag_ext = shapes[0]->GetName();

				if (eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(shapes[0])) {
					ParserShape(poly, data.position, data.angle, grids, true, char_val["grids"]);
				}
			}
		}

		// tags
		std::string tag = data.tag;
		if (!tag.empty() && tag[tag.size()-1] != ';') {
			tag += ";";
		}
		tag += tag_ext;
		ParserSprTag(tag, filepath, char_val);

		// filename
		std::string filename = ee::FileHelper::GetFilename(filepath);
		lr::CharacterFileName out_name(filename);
		//		char_val["filename"] = out_name.GetOutputName();

		// angle
		int dir = 1 + (out_name.GetField(lr::CharacterFileName::FT_DIRECTION)[0] - '1');
		if (data.mirror.x) {
			dir = 10 - dir;
		}
		dir = (dir + 7) % 8;
		char_val["angle"] = dir + 1;

		int sz = out_val[name].size();
		out_val[name][sz] = char_val;

		spr_val = src_val["sprite"][idx++];
	}
}

void LRJsonPacker::ParserLevel(const Json::Value& src_val, int layer_idx, const char* name, Json::Value& out_val)
{
	const Json::Value& layer_val = src_val["layer"][layer_idx];

	ParserLevelFromSprite(layer_val, name, out_val);

	int idx = 0;
	Json::Value cl_val = layer_val["layers"][idx++];
	while (!cl_val.isNull()) {
		ParserLevelFromSprite(cl_val, name, out_val);
		cl_val = layer_val["layers"][idx++];
	}
}

void LRJsonPacker::ParserLevelFromSprite(const Json::Value& src_val, const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value spr_val = src_val["sprite"][idx++];
	while (!spr_val.isNull()) 
	{
		std::string spr_path = ee::SymbolSearcher::GetSymbolPath(m_dir, spr_val);
		ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(spr_path);
		if (!symbol) {
			std::string filepath = spr_val["filepath"].asString();
			throw ee::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", 
				m_dir.c_str(), filepath.c_str());
		}

		Json::Value level_val;

		level_val["name"] = spr_val["name"];

		ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);
		sprite->Load(spr_val);
		level_val["x"] = sprite->GetPosition().x;
		level_val["y"] = sprite->GetPosition().y;

		std::string tag = spr_val["tag"].asString();
		ParserSprTag(tag, spr_path, level_val);

		int sz = out_val[name].size();
		out_val[name][sz] = level_val;

		sprite->Release();
		symbol->Release();

		spr_val = src_val["sprite"][idx++];
	}
}

void LRJsonPacker::ParserSpecial(const Json::Value& src_val, const std::string& name,
								 Json::Value& out_val)
{
	for (int layer_idx = 0; layer_idx < 3; ++layer_idx)
	{
		const Json::Value& layer_val = src_val["layer"][layer_idx];

		ParserSpecialFromSprite(layer_val, name, layer_idx == 2, out_val);

		int idx = 0;
		Json::Value cl_val = layer_val["layers"][idx++];
		while (!cl_val.isNull()) {
			ParserSpecialFromSprite(cl_val, name, layer_idx == 2, out_val);
			cl_val = layer_val["layers"][idx++];
		}
	}
}

void LRJsonPacker::ParserSpecialFromSprite(const Json::Value& src_val, const std::string& name, 
										   bool is_layer2, Json::Value& out_val)
{
	int idx = 0;
	Json::Value spr_val = src_val["sprite"][idx++];
	while (!spr_val.isNull()) 
	{
		std::string filepath = spr_val["filepath"].asString();

		std::string tag = spr_val["tag"].asString();
		if (ee::FileType::IsType(filepath, ee::FileType::e_particle3d)) {
			bool top_layer = false;
			if (is_layer2 || tag.find(TOP_LAYER_STR) != std::string::npos) {
				top_layer = true;
			}
			// 				ParserParticleLayer(spr_val, out_val, top_layer);
			if (top_layer) {
				out_val["top"] = name + "_top";
			}
		} else if (tag.find(COVER_LAYER_STR) != std::string::npos) {
			ParserSpecialLayer(spr_val, "cover", out_val);
		} else if (tag.find(TOP_LAYER_STR) != std::string::npos) {
			//				ParserSpecialLayer(spr_val, "top", out_val);
			out_val["top"] = name + "_top";
		}
		spr_val = src_val["sprite"][idx++];
	}
}

void LRJsonPacker::ParserSpecialLayer(const Json::Value& spr_val, const std::string& name, Json::Value& out_val)
{
	ee::SpriteIO::Data src_data;
	ee::SpriteIO::Load(spr_val, src_data);

	Json::Value dec_val;

	sm::vec2 pos = src_data.position;

	std::string s_name;
	std::string export_name;
	std::string spr_path = ee::SymbolSearcher::GetSymbolPath(m_dir, spr_val);
	if (spr_path.find(".json") != std::string::npos) 
	{
		Json::Value val;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(spr_path.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, val);
		fin.close();

		export_name = val["name"].asString();

		assert(val["sprite"].size() == 1);
		s_name = val["sprite"][(Json::Value::UInt)0]["name"].asString();

		int idx = 0;

		ee::SpriteIO::Data data;
		ee::SpriteIO::Load(val["sprite"][idx], data);
		pos += data.position;
	}

	if (!s_name.empty() && s_name[0] != '_') {
		dec_val["name"] = s_name;
	}

	dec_val["export"] = export_name;

	dec_val["x"] = pos.x;
	dec_val["y"] = pos.y;

	int sz = out_val[name].size();
	out_val[name][sz] = dec_val;
}

void LRJsonPacker::ParserParticleLayer(const Json::Value& spr_val, Json::Value& out_val,
									   bool top_layer)
{
	Json::Value dec_val;

	ee::SpriteIO::Data data;
	ee::SpriteIO::Load(spr_val, data);

	dec_val["x"] = data.position.x;
	dec_val["y"] = data.position.y;

	dec_val["scale"] = std::max(data.scale.x, data.scale.y);

	std::string sym_path = spr_val["filepath"].asString();
	std::string name = ee::FileHelper::GetFilename(sym_path);
	name = name.substr(0, name.find("_particle"));
	ee::StringHelper::ToLower(name);

	dec_val["export"] = name;
//	dec_val["export"] = spr_val["name"];

	if (top_layer) {
		dec_val["layer"] = "top";
	}

// 	float qx = spr_val["particle3d"]["dir"]["x"].asDouble(),
// 		qy = spr_val["particle3d"]["dir"]["y"].asDouble(),
// 		qz = spr_val["particle3d"]["dir"]["z"].asDouble(),
// 		qw = spr_val["particle3d"]["dir"]["w"].asDouble();
// 	Quaternion quat(qx, qy, qz, qw);
// 	mat4 mat = quat.ToMatrix();
// 	vec3 dir = mat * vec3(0, 0, 1);

	Json::Value dir_val;

// 	dir_val["x"] = dir.x;
// 	dir_val["y"] = dir.y;
// 	dir_val["z"] = dir.z;

	dir_val["x"] = 0;
	dir_val["y"] = 0;
	dir_val["z"] = 1;

	dec_val["dir"] = dir_val;

	int sz = out_val["particle"].size();
	out_val["particle"][sz] = dec_val;
}

void LRJsonPacker::ParserSprTag(const std::string& tag, const std::string& symbol_path, Json::Value& out_val)
{
	if (tag.find("[symbol]") != std::string::npos) {
		Json::Value symbol_val;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(symbol_path.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, symbol_val);
		fin.close();

		ParserSprTag(symbol_val["tag"].asString(), out_val);
	}

	ParserSprTag(tag, out_val);
}

void LRJsonPacker::ParserSprTag(const std::string& tag, Json::Value& out_val)
{
	std::vector<std::string> tags;
	ee::StringHelper::Split(tag, ";", tags);
	for (int i = 0, n = tags.size(); i < n; ++i) {
		const std::string& str = tags[i];
		int pos = str.find_first_of('=');
		if (pos == std::string::npos) {
			continue;
		}
		std::string key = str.substr(0, pos);
		std::string val = str.substr(pos+1);
		out_val["tag"][key] = val;
	}
}

}