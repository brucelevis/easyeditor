#include "LRJsonPacker.h"
#include "check_params.h"
#include "lr_typedef.h"
#include "lr_tools.h"

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

void LRJsonPacker::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return;
	if (!check_file(argv[2])) return;

	Run(argv[2]);
}

void LRJsonPacker::Run(const std::string& filepath)
{
	std::string str = filepath.substr(0, filepath.find("_lr.json"));
	if (str.find(".") != std::string::npos) {
		PackLogic(filepath);
	} else {
		PackGraphics(filepath);
	}
}

void LRJsonPacker::PackAll(const std::string& filepath)
{
	Json::Value lr_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	m_dir = d2d::FilenameTools::getFileDir(filepath) + "\\";

	Json::Value out_val;

	out_val["width"] = lr_val["size"]["width"];
	out_val["height"] = lr_val["size"]["height"];
	out_val["view width"] = lr_val["size"]["view width"];
	out_val["view height"] = lr_val["size"]["view height"];

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
	ParserCharacter(lr_val, grids, 2, "character", out_val);
	ParserPoint(lr_val, 3, "point", out_val);
	ParserShapeLayer(lr_val, grids, false, 4, "path", out_val);
	ParserShapeLayer(lr_val, grids, true, 5, "region", out_val);
	ParserShapeLayer(lr_val, grids, true, 6, "collision region", out_val);
	ParserCamera(lr_val, 7, "camera", out_val);

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

void LRJsonPacker::PackGraphics(const std::string& filepath)
{
	Json::Value lr_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, lr_val);
	fin.close();

	m_dir = d2d::FilenameTools::getFileDir(filepath) + "\\";

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

	m_dir = d2d::FilenameTools::getFileDir(filepath) + "\\";

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
	
	std::string outfile = filepath.substr(0, filepath.find_last_of('_')) + ".json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(outfile.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, out_val);
	fout.close();
}

void LRJsonPacker::ParserShape(d2d::IShape* shape, const d2d::Vector& offset, float angle,
							   const lr::Grids& grids, bool force_grids, Json::Value& out_val)
{
	if (libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shape))
	{
		std::vector<int> grid_idx;

		std::vector<d2d::Vector> bound = poly->GetVertices();
		for (int i = 0, n = bound.size(); i < n; ++i) {
			bound[i] = d2d::Math::rotateVector(bound[i], angle) + offset;
		}
		grid_idx = grids.IntersectPolygon(bound);

		for (int i = 0, n = grid_idx.size(); i < n; ++i) {
			int sz = out_val["grid"].size();
			out_val["grid"][sz] = grid_idx[i];
		}
	}
	else if (libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(shape))
	{
		std::vector<d2d::Vector> bound = chain->GetVertices();
		for (int i = 0, n = bound.size(); i < n; ++i) {
			bound[i] = d2d::Math::rotateVector(bound[i], angle) + offset;
		}

		if (force_grids) {
			std::vector<int> grid_idx;
			grid_idx = grids.IntersectPolyline(bound);

			for (int i = 0, n = grid_idx.size(); i < n; ++i) {
				int sz = out_val["grid"].size();
				out_val["grid"][sz] = grid_idx[i];
			}
		} else {
			d2d::JsonIO::Store(bound, out_val["pos"]);
		}
	}
	else
	{
		throw d2d::Exception("LRJsonPacker::ParserPolyLayer error shape type");
	}
}

void LRJsonPacker::ParserShapeLayer(const Json::Value& src_val, const lr::Grids& grids, bool force_grids,
								 int layer_idx, const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value src_spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	while (!src_spr_val.isNull()) 
	{
		std::string spr_path = d2d::SymbolSearcher::GetSymbolPath(m_dir, src_spr_val);
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(spr_path);
		assert(symbol);

		Json::Value dst_val;
		dst_val["name"] = src_spr_val["name"];

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->Load(src_spr_val);

		libshape::Sprite* shape_spr = dynamic_cast<libshape::Sprite*>(sprite);
		assert(shape_spr);
		const std::vector<d2d::IShape*>& shapes = shape_spr->GetSymbol().GetShapes();
		for (int i = 0, n = shapes.size(); i < n; ++i) {
			ParserShape(shapes[i], sprite->GetPosition(), sprite->GetAngle(), grids, force_grids, dst_val);
		}

		int sz = out_val[name].size();
		out_val[name][sz] = dst_val;

		sprite->Release();
		symbol->Release();

		src_spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	}

	idx = 0;
	Json::Value src_shape_val = src_val["layer"][layer_idx]["shape"][idx++];
	while (!src_shape_val.isNull()) 
	{
		d2d::IShape* shape = libshape::ShapeFactory::CreateShapeFromFile(src_shape_val, m_dir);
		
		Json::Value dst_val;
		dst_val["name"] = src_shape_val["name"];
		ParserShape(shape, d2d::Vector(0, 0), 0, grids, force_grids, dst_val);

		int sz = out_val[name].size();
		out_val[name][sz] = dst_val;

		shape->Release();

		src_shape_val = src_val["layer"][layer_idx]["shape"][idx++];
	}
}

void LRJsonPacker::ParserPoint(const Json::Value& src_val, int layer_idx, const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	while (!spr_val.isNull()) 
	{
		std::string spr_path = d2d::SymbolSearcher::GetSymbolPath(m_dir, spr_val);
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(spr_path);
		if (!symbol) {
			std::string filepath = spr_val["filepath"].asString();
			throw d2d::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", 
				m_dir.c_str(), filepath.c_str());
		}

		Json::Value shape_val;
		shape_val["name"] = spr_val["name"];

		d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
		sprite->Load(spr_val);

		shape_val["x"] = sprite->GetPosition().x;
		shape_val["y"] = sprite->GetPosition().y;

		int sz = out_val[name].size();
		out_val[name][sz] = shape_val;

		sprite->Release();
		symbol->Release();

		spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	}
}

void LRJsonPacker::ParserCamera(const Json::Value& src_val, int layer_idx, 
									 const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	while (!spr_val.isNull()) 
	{
		Json::Value cam_val;
		cam_val["name"] = spr_val["name"];
		cam_val["x"] = spr_val["position"]["x"];
		cam_val["y"] = spr_val["position"]["y"];
		cam_val["scale"] = spr_val["x scale"];

		int sz = out_val[name].size();
		out_val[name][sz] = cam_val;

		spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	}
}

void LRJsonPacker::ParserCharacter(const Json::Value& src_val, const lr::Grids& grids,
								   int layer_idx, const char* name, Json::Value& out_val)
{
	int idx = 0;
	Json::Value spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	while (!spr_val.isNull()) 
	{
		std::string filepath = spr_val["filepath"].asString();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_particle3d)) {
			spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
			continue;
		}				

		Json::Value char_val;
		char_val["name"] = spr_val["name"];
		char_val["x"] = spr_val["position"]["x"];
		char_val["y"] = spr_val["position"]["y"];

		// region
		std::string shape_tag = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_shape);
		std::string shape_filepath = d2d::FilenameTools::getFilenameAddTag(filepath, shape_tag, "json");
		std::string tag_ext;
		if (d2d::FilenameTools::IsFileExist(shape_filepath)) {
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(shape_filepath);
			const std::vector<d2d::IShape*>& shapes = static_cast<libshape::Symbol*>(symbol)->GetShapes();
			if (!shapes.empty()) {
				tag_ext = shapes[0]->name;

				if (libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shapes[0])) {
					float x = spr_val["position"]["x"].asDouble(),
						y = spr_val["position"]["y"].asDouble();
					float ang = spr_val["angle"].asDouble();					
					ParserShape(poly, d2d::Vector(x, y), ang, grids, true, char_val["grids"]);
				}
			}
		}

		// tags
		std::string tag = spr_val["tag"].asString();
		if (!tag.empty() && tag[tag.size()-1] != ';') {
			tag += ";";
		}
		tag += tag_ext;
		std::vector<std::string> tags;
		int pos = tag.find_first_of(';');
		tags.push_back(tag.substr(0, pos));
		do 
		{
			int next_pos = tag.find_first_of(';', pos + 1);
			tags.push_back(tag.substr(pos + 1, next_pos - pos - 1));
			pos = next_pos;
		} while (pos != std::string::npos);
		for (int i = 0, n = tags.size(); i < n; ++i) {
			const std::string& str = tags[i];
			int pos = str.find_first_of('=');
			std::string key = str.substr(0, pos);
			std::string val = str.substr(pos+1);
			char_val["tag"][key] = val;
		}

		// filename
		std::string filename = d2d::FilenameTools::getFilename(filepath);
		lr::CharacterFileName out_name(filename);
		char_val["filename"] = out_name.GetOutputName();

		// angle
		int dir = 1 + (out_name.GetField(lr::CharacterFileName::FT_DIRECTION)[0] - '1');
		if (spr_val["x mirror"].asBool()) {
			dir = 10 - dir;
		}
		dir = (dir + 7) % 8;
		char_val["angle"] = dir + 1;

		int sz = out_val[name].size();
		out_val[name][sz] = char_val;

		spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
	}
}

void LRJsonPacker::ParserSpecial(const Json::Value& src_val, const std::string& name,
								 Json::Value& out_val)
{
	for (int layer_idx = 0; layer_idx < 3; ++layer_idx)
	{
		int idx = 0;
		Json::Value spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
		while (!spr_val.isNull()) 
		{
			std::string filepath = spr_val["filepath"].asString();
			std::string tag = spr_val["tag"].asString();
			if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_particle3d)) {
				bool top_layer = false;
				if (layer_idx == 2 || tag.find(TOP_LAYER_STR) != std::string::npos) {
					top_layer = true;
				}
				ParserParticleLayer(spr_val, out_val, top_layer);
			} else if (tag.find(COVER_LAYER_STR) != std::string::npos) {
				ParserSpecialLayer(spr_val, "cover", out_val);
			} else if (tag.find(TOP_LAYER_STR) != std::string::npos) {
//				ParserSpecialLayer(spr_val, "top", out_val);
				out_val["top"] = name + "_top";
			}
			spr_val = src_val["layer"][layer_idx]["sprite"][idx++];
		}
	}
}

void LRJsonPacker::ParserSpecialLayer(const Json::Value& spr_val, const std::string& name, Json::Value& out_val)
{
	Json::Value dec_val;

	float px = spr_val["position"]["x"].asDouble(),
		py = spr_val["position"]["y"].asDouble();

	std::string export_name = "";
	wxString spr_path = d2d::SymbolSearcher::GetSymbolPath(m_dir, spr_val);
	if (spr_path.Contains(".json")) 
	{
		Json::Value val;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(spr_path.fn_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, val);
		fin.close();

		export_name = val["name"].asString();

		int idx = 0;
		const Json::Value& spr_val = val["sprite"][idx];
		float cx = spr_val["position"]["x"].asDouble(),
			cy = spr_val["position"]["y"].asDouble(); 
		px += cx;
		py += cy;
	}
	dec_val["export"] = export_name;

	dec_val["x"] = px;
	dec_val["y"] = py;

	int sz = out_val[name].size();
	out_val[name][sz] = dec_val;
}

void LRJsonPacker::ParserParticleLayer(const Json::Value& spr_val, Json::Value& out_val,
									   bool top_layer)
{
	Json::Value dec_val;

	dec_val["x"] = spr_val["position"]["x"].asDouble();
	dec_val["y"] = spr_val["position"]["y"].asDouble();

	float sx = spr_val["x scale"].asDouble(),
		sy = spr_val["y scale"].asDouble();
	dec_val["scale"] = std::max(sx, sy);

	std::string sym_path = spr_val["filepath"].asString();
	std::string name = d2d::FilenameTools::getFilename(sym_path);
	name = name.substr(0, name.find("_particle"));
	d2d::StringTools::ToLower(name);

	dec_val["export"] = name;

	if (top_layer) {
		dec_val["layer"] = "top";
	}

	Json::Value dir_val;
	dir_val["x"] = 0;
	dir_val["y"] = 0;
	dir_val["z"] = 1;
	dec_val["dir"] = dir_val;

	int sz = out_val["particle"].size();
	out_val["particle"][sz] = dec_val;
}

}