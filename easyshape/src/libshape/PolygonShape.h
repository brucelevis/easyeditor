#ifndef _EASYSHAPE_POLYGON_SHAPE_H_
#define _EASYSHAPE_POLYGON_SHAPE_H_

#include "ChainShape.h"

namespace ee { class ImageSymbol; }

namespace eshape
{

class Material;

class PolygonShape : public ChainShape
{
public:
	PolygonShape();
	PolygonShape(const PolygonShape& polygon);
	PolygonShape(const std::vector<sm::vec2>& vertices);
	virtual ~PolygonShape();
	
	//
	// IObject interface
	//
	virtual PolygonShape* Clone() const;

	//
	// Shape interface
	//
	virtual const char* GetShapeDesc() const { return "polygon"; }
	virtual bool IsContain(const sm::vec2& pos) const;
	// todo
	//virtual bool isIntersect(const sm::rect& rect) const;
	virtual void Translate(const sm::vec2& offset);
	virtual void Draw(const sm::mat4& mt, const s2::RenderColor& color) const;
	virtual ee::PropertySetting* CreatePropertySetting(ee::EditPanelImpl* stage);
	virtual void LoadFromFile(const Json::Value& value, const std::string& dir);
	virtual void StoreToFile(Json::Value& value, const std::string& dir) const;
	virtual void ReloadTexture();

	//
	// ChainShape interface
	//
	virtual void refresh();

	void SetMaterialColor(const s2::Color& color);
	void SetMaterialTexture(ee::ImageSymbol* image);

	Json::Value StoreMaterial(const std::string& dirpath) const;
	void LoadMaterial(const std::string& dirpath, const Json::Value& val);

	const Material* GetMaterial() const { return m_material; }

protected:
	Material* m_material;

}; // PolygonShape

}

#endif // _EASYSHAPE_POLYGON_SHAPE_H_