#include "FileIO.h"
#include "OceanMesh.h"

namespace eterrain2d
{

void FileIO::StoreOceanMesh(const OceanMesh* ocean, const std::string& dir, Json::Value& value)
{
	if (const libshape::PolygonShape* shape = ocean->GetBounding()) {
		d2d::JsonTools::store(shape->GetVertices(), value["bound"]);
	}
	if (const d2d::ImageSymbol* img = ocean->GetImage0()) {
		value["tex0"] = d2d::FilenameTools::getRelativePath(dir,
			img->getFilepath()).ToStdString();
	}
	if (const d2d::ImageSymbol* img = ocean->GetImage1()) {
		value["tex1"] = d2d::FilenameTools::getRelativePath(dir,
			img->getFilepath()).ToStdString();
	}

	value["wave"]["open"] = ocean->IsWaveOpen();
	value["wave"]["row"] = ocean->GetWaveGridRow();
	value["wave"]["col"] = ocean->GetWaveGridCol();
	value["wave"]["speed"] = ocean->GetWaveSpeed() / 0.1f;
	value["wave"]["height"] = ocean->GetWaveHeight();

	value["uv_move"]["open"] = ocean->IsUVMoveOpen();
	value["uv_move"]["speed"]["x"] = ocean->GetUVMoveSpeed().x / 0.01f;
	value["uv_move"]["speed"]["y"] = ocean->GetUVMoveSpeed().y / 0.01f;

	value["tex_blend"]["open"] = ocean->IsTexBlendOpen();
	value["tex_blend"]["speed"] = ocean->GetTexBlendSpeed() / 0.01f;
}

OceanMesh* FileIO::LoadOceanMesh(const std::string& dir, const Json::Value& value)
{
	if (value["bound"].isNull() || value["tex0"].isNull()) {
		return NULL;
	}

	std::vector<d2d::Vector> bounding;
	d2d::JsonTools::load(value["bound"], bounding);
	libshape::PolygonShape* shape = new libshape::PolygonShape(bounding);

	std::string tex0_path = dir + "\\" + value["tex0"].asString();
	d2d::ISymbol* tex0 = d2d::SymbolMgr::Instance()->fetchSymbol(tex0_path);
	shape->SetMaterialTexture(static_cast<d2d::ImageSymbol*>(tex0));

	OceanMesh* ocean = new OceanMesh(shape, static_cast<d2d::ImageSymbol*>(tex0));

	if (!value["tex1"].isNull()) {
		std::string tex1_path = dir + "\\" + value["tex1"].asString();
		d2d::ISymbol* tex1 = d2d::SymbolMgr::Instance()->fetchSymbol(tex1_path);
		ocean->SetImage1(static_cast<d2d::ImageSymbol*>(tex1));
	}

	ocean->OpenWave(value["wave"]["open"].asBool());
	ocean->SetSmallGridSize(value["wave"]["row"].asInt(), value["wave"]["col"].asInt());
	ocean->SetWaveInfo(value["wave"]["speed"].asDouble() * 0.1f, value["wave"]["height"].asDouble());

	ocean->OpenUVMove(value["uv_move"]["open"].asBool());
	float x = value["uv_move"]["speed"]["x"].asDouble() * 0.01f,
		y = value["uv_move"]["speed"]["y"].asDouble() * 0.01f;
	ocean->SetTexcoordsSpeed(d2d::Vector(x, y));

	ocean->OpenBlend(value["tex_blend"]["open"].asBool());
	ocean->SetBlendSpeed(value["tex_blend"]["speed"].asDouble() * 0.01f);

	return ocean;
}

}