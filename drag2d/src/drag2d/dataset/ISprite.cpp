#include "ISprite.h"
#include "ISymbol.h"

#include "common/Math.h"
#include "common/Matrix.h"
#include "common/color_trans.h"
#include "common/Exception.h"
#include "common/config.h"
#include "dataset/BVFactory.h"
#include "dataset/AbstractBV.h"
#include "dataset/SpriteFactory.h"
#include "view/SpritePropertySetting.h"

namespace d2d
{

ISprite::ISprite()
	: m_observer(NULL)
{
	multiCol.set(1, 1, 1, 1);
	addCol.set(0, 0, 0, 0);
	r_trans.set(1, 0, 0, 1);
	g_trans.set(0, 1, 0, 1);
	b_trans.set(0, 0, 1, 1);

	clip = false;

	visiable = editable = true;

	m_angle = 0.0f;
	m_scale.set(1, 1);
	m_shear.set(0, 0);
	m_xMirror = m_yMirror = false;
	m_perspective.set(0, 0);
	m_bounding = NULL;

	m_blend_mode = BM_NORMAL;
	m_filter_mode = FM_NORMAL;
}

ISprite::ISprite(const ISprite& sprite)
	: m_observer(NULL)
{
	name = sprite.name;
	multiCol = sprite.multiCol;
	addCol = sprite.addCol;
	clip = sprite.clip;
	r_trans = sprite.r_trans;
	g_trans = sprite.g_trans;
	b_trans = sprite.b_trans;

	visiable = sprite.visiable;
	editable = sprite.editable;
	tag = sprite.tag;

	m_pos = sprite.m_pos;
	m_angle = sprite.m_angle;
	m_offset = sprite.m_offset;
	m_scale = sprite.m_scale;
	m_shear = sprite.m_shear;
	m_xMirror = sprite.m_xMirror;
	m_yMirror = sprite.m_yMirror;
	m_perspective = sprite.m_perspective;
	m_bounding = sprite.m_bounding->clone();

	m_blend_mode = sprite.m_blend_mode;
	m_filter_mode = sprite.m_filter_mode;
}

ISprite::~ISprite()
{
	delete m_bounding;
	SpriteFactory::Instance()->remove(this);
}

ISprite* ISprite::clone() const
{
	return NULL;
}

void ISprite::clearUserData(bool deletePtr)
{
	delete m_userData, m_userData = NULL;
}

void ISprite::load(const Json::Value& val)
{
	// info
	name = val["name"].asString();
	tag = val["tag"].asString();
	clip = val["clip"].asBool();

	// color
	std::string str = val["multi color"].asString();
	if (str.empty())
		multiCol = Colorf(1, 1, 1, 1);
	else
		multiCol = transColor(str, PT_BGRA);
	str = val["add color"].asString();
	if (str.empty())
		addCol = Colorf(0, 0, 0, 0);
	else
		addCol = transColor(str, PT_ABGR);

	str = val["r trans"].asString();
	if (str.empty())
		r_trans = Colorf(1, 0, 0, 1);
	else
		r_trans = transColor(str, PT_RGBA);
	str = val["g trans"].asString();
	if (str.empty())
		g_trans = Colorf(0, 1, 0, 1);
	else
		g_trans = transColor(str, PT_RGBA);
	str = val["b trans"].asString();
	if (str.empty())
		b_trans = Colorf(0, 0, 1, 1);
	else
		b_trans = transColor(str, PT_RGBA);

	// scale
	float sx, sy;
	if (val["scale"].isNull())
	{
		sx = val["x scale"].asDouble();
		sy = val["y scale"].asDouble();
	}
	else
	{
		sx = sy = val["scale"].asDouble();
	}
	setScale(sx, sy);

	// shear
	float kx, ky;
	if (!val["x shear"].isNull())
	{
		kx = val["x shear"].asDouble();
		ky = val["y shear"].asDouble();
	}
	else
	{
		kx = ky = 0;
	}
	setShear(kx, ky);

	// mirror
	bool mx = val["x mirror"].asBool();
	bool my = val["y mirror"].asBool();
	setMirror(mx, my);

	// perspective
	if (!val["x perspective"].isNull())
	{
		Vector persp;
		persp.x = val["x perspective"].asDouble();
		persp.y = val["y perspective"].asDouble();
		SetPerspective(persp);
	}

	// offset
	float ox, oy;
	if (!val["x offset"].isNull())
	{
		ox = val["x offset"].asDouble();
		oy = val["y offset"].asDouble();
	}
	else
	{
		ox = oy = 0;
	}
	setOffset(Vector(ox, oy));

	// translate
	float x = val["position"]["x"].asDouble();
	float y = val["position"]["y"].asDouble();
	if (fabs(x) >= FLT_MAX || fabs(y) >= FLT_MAX) {
		throw Exception("sprite pos err, filepath: %s, name: %s", val["filepath"].asString().c_str(), name.c_str());
	}

	// rotate
	float angle = val["angle"].asDouble();
	setTransform(Vector(x, y), angle);

	// filter
	if (!val["filter"].isNull()) {
		std::string disc = val["filter"].asString();
		if (Config::Instance()->IsRenderOpen()) {
			m_filter_mode = FilterModes::Instance()->GetIDFromNameEN(disc);
		}
	}
}

void ISprite::store(Json::Value& val) const
{
	val["name"] = name;
	val["tag"] = tag;
	val["clip"] = clip;

	val["multi color"] = transColor(multiCol, PT_BGRA);
	val["add color"] = transColor(addCol, PT_ABGR);
	val["r trans"] = transColor(r_trans, PT_RGBA);
	val["g trans"] = transColor(g_trans, PT_RGBA);
	val["b trans"] = transColor(b_trans, PT_RGBA);

	val["position"]["x"] = m_pos.x;
	val["position"]["y"] = m_pos.y;

	val["angle"] = m_angle;

	val["x scale"] = m_scale.x;
	val["y scale"] = m_scale.y;

	val["x shear"] = m_shear.x;
	val["y shear"] = m_shear.y;

	val["x mirror"] = m_xMirror;
	val["y mirror"] = m_yMirror;

	val["x offset"] = m_offset.x;
	val["y offset"] = m_offset.y;

	val["x perspective"] = m_perspective.x;
	val["y perspective"] = m_perspective.y;

	val["filter"] = FilterModes::Instance()->GetNameENFromID(m_filter_mode);
}

void ISprite::buildBounding()
{
	if (!m_bounding) {
		m_bounding = BVFactory::createBV(e_obb);
	}
	const ISymbol& symbol = getSymbol();
	Rect rect(symbol.getSize(this));
	if (m_offset.x == 0 && m_offset.y == 0)
		m_offset.set(rect.xCenter(), rect.yCenter());
	rect.scale(m_scale.x, m_scale.y);
	rect.shear(m_shear.x, m_shear.y);
	m_bounding->initFromRect(rect);
	m_bounding->setTransform(m_pos, m_offset, m_angle);
}

IPropertySetting* ISprite::CreatePropertySetting(EditPanel* stage)
{
	return new SpritePropertySetting(stage, this);
}

void ISprite::setTransform(const Vector& position, float angle)
{
	if (m_pos != position) translate(position - m_pos);
	if (m_angle != angle) rotate(angle - m_angle);
}

void ISprite::setScale(float xScale, float yScale)
{
	Vector dscale;
	dscale.x = xScale / m_scale.x;
	dscale.y = yScale / m_scale.y;

	Vector old_offset = m_offset;
	Vector new_offset(m_offset.x * dscale.x, m_offset.y * dscale.y);
	m_offset = new_offset;

	translate(old_offset - new_offset);

	//////////////////////////////////////////////////////////////////////////

// 	Matrix mat_old, mat_new;
// 	mat_old.scale(m_scale.x, m_scale.y);
// 	mat_new.scale(xScale, yScale);
// 
// 	Vector offset = Math::transVector(m_offset, mat_new) - Math::transVector(m_offset, mat_old);
// 
// 	m_offset += offset;
// 	translate(-offset);

	//////////////////////////////////////////////////////////////////////////

	m_scale.set(xScale, yScale);
	buildBounding();
 }

void ISprite::setShear(float xShear, float yShear)
{
	Matrix mat_old, mat_new;
	mat_old.shear(m_shear.x, m_shear.y);
	mat_new.shear(xShear, yShear);

	Vector offset = Math::transVector(m_offset, mat_new) - Math::transVector(m_offset, mat_old);

	m_offset += offset;
	translate(-offset);

	m_shear.set(xShear, yShear);
	buildBounding();
}

bool ISprite::isContain(const Vector& pos) const
{
	return m_bounding ? m_bounding->isContain(pos) : false;
}

bool ISprite::isIntersect(const Rect& rect) const
{
	return m_bounding ? m_bounding->isIntersect(rect) : false;
}

void ISprite::translate(const Vector& offset)
{
	if (m_observer)
		m_observer->translate(this, offset);

	m_pos += offset;
	if (m_bounding) {
		m_bounding->setTransform(m_pos, m_offset, m_angle);
	}
}

void ISprite::rotate(float delta)
{
	if (m_observer)
		m_observer->rotate(this, delta);

	m_angle += delta;

	if (m_bounding) {
		m_bounding->setTransform(m_pos, m_offset, m_angle);
	}
}

void ISprite::setOffset(const Vector& offset) 
{
	// rotate + offset -> offset + rotate	
	Vector old_center = getCenter();
	m_offset = offset;
	Vector new_center = getCenter();
	m_pos += (old_center - new_center);

	if (m_bounding) {
		m_bounding->setTransform(m_pos, m_offset, m_angle);
	}
}

Vector ISprite::getCenter() const
{
	d2d::Vector center_offset = Math::rotateVector(-m_offset, m_angle) + m_offset;
	d2d::Vector center = m_pos + center_offset;
	return center;
}

d2d::Rect ISprite::GetRect() const
{
	std::vector<Vector> bound;
	getBounding()->getBoundPos(bound);
	d2d::Rect rect;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		rect.combine(bound[i]);
	}
	return rect;
}

void ISprite::GetTransMatrix(Matrix& mt) const
{
	const float xScale = m_xMirror ? -m_scale.x : m_scale.x,
		yScale = m_yMirror ? -m_scale.y : m_scale.y;

	d2d::Vector center = getCenter();
	mt.setTransformation(center.x, center.y, m_angle, 
		xScale, yScale, 0, 0, m_shear.x, m_shear.y);
}

Matrix ISprite::GetTransInvMatrix() const
{
	d2d::Matrix mat;
	mat.rotate(-m_angle);
	mat.shear(-m_shear.x, -m_shear.y);
	mat.translate(-m_pos.x/m_scale.x, -m_pos.y/m_scale.y);
	mat.scale(1/m_scale.x, 1/m_scale.y);
	return mat;
}

//////////////////////////////////////////////////////////////////////////
// class SpriteCmp
//////////////////////////////////////////////////////////////////////////

SpriteCmp::SpriteCmp(Type type /*= e_file*/)
	: m_type(type)
{
}

bool SpriteCmp::operator() (const ISprite* s0, const ISprite* s1) const
{
	switch (m_type)
	{
	case e_file:
		return s0->getSymbol().getFilepath() < s1->getSymbol().getFilepath();
	case e_x:
		return s0->getPosition().x < s1->getPosition().x;
	case e_y:
		return s0->getPosition().y < s1->getPosition().y;
	case e_x_invert:
		return s0->getPosition().x > s1->getPosition().x;
	case e_y_invert:
		return s0->getPosition().y > s1->getPosition().y;
	default:
		return s0->getSymbol().getFilepath() < s1->getSymbol().getFilepath();
	}
}

} // d2d