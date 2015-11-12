#include "Sprite.h"
#include "PropertySetting.h"

namespace etext
{

Sprite::Sprite()
	: m_symbol(NULL)
{
	m_width = 100;
	m_height = 20;

	m_font = 0;
	m_font_size = 16;

	m_has_edge = false;

	m_color.set(1, 1, 1);

	m_align_hori = HAT_LEFT;
	m_align_vert = VAT_TOP;

	m_space_hori = 1;
	m_space_vert = 1;
}

Sprite::Sprite(const Sprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
	m_symbol->Retain();

	m_width = sprite.m_width;
	m_height = sprite.m_height;

	m_font = sprite.m_font;
	m_font_size = sprite.m_font_size;

	m_has_edge = sprite.m_has_edge;

	m_color = sprite.m_color;

	m_align_hori = sprite.m_align_hori;
	m_align_vert = sprite.m_align_vert;

	m_space_hori = sprite.m_space_hori;
	m_space_vert = sprite.m_space_vert;
}

Sprite::Sprite(Symbol* symbol)
	: m_symbol(symbol)
{
	m_symbol->Retain();

	m_width = symbol->m_width;
	m_height = symbol->m_height;

	m_font = symbol->m_font;
	m_font_size = symbol->m_font_size;

	m_has_edge = symbol->m_has_edge;

	m_color = d2d::transColor(symbol->m_color, d2d::PT_ARGB);

	m_align_hori = symbol->m_align_hori;
	m_align_vert = symbol->m_align_vert;

	m_space_hori = symbol->m_space_hori;
	m_space_vert = symbol->m_space_vert;

	BuildBounding();	
}

Sprite::~Sprite()
{
	if (m_symbol) {
		m_symbol->Release();
	}
}

Sprite* Sprite::Clone() const
{
	Sprite* sprite = new Sprite(*this);
	d2d::SpriteFactory::Instance()->insert(sprite);
	return sprite;
}

const Symbol& Sprite::GetSymbol() const
{
	return *m_symbol;
}

void Sprite::SetSymbol(d2d::ISymbol* symbol)
{
	d2d::ISprite::SetSymbol(&m_symbol, symbol);
}

void Sprite::Load(const Json::Value& val)
{
	ISprite::Load(val);

	const Json::Value& text_val = val["text"];

	m_width = text_val["width"].asInt();
	m_height = text_val["height"].asInt();

	m_font = text_val["font"].asInt();
	m_font_size = text_val["size"].asInt();

	m_has_edge = text_val["edge"].asBool();

	m_color = transColor(text_val["color"].asString(), d2d::PT_ARGB);

	m_align_hori = HoriAlignType(text_val["align_hori"].asInt());
	m_align_vert = VertAlignType(text_val["align_vert"].asInt());

	m_space_hori = text_val["space_hori"].asDouble();
	m_space_vert = text_val["space_vert"].asDouble();

	m_text = text_val["text"].asString();
	m_tid = text_val["tid"].asString();

	BuildBounding();
}

void Sprite::Store(Json::Value& val) const
{
	ISprite::Store(val);

	Json::Value text_val;

	text_val["width"] = m_width;
	text_val["height"] = m_height;

	text_val["font"] = m_font;
	text_val["size"] = m_font_size;

	text_val["edge"] = m_has_edge;

	text_val["color"] = transColor(m_color, d2d::PT_ARGB);

	text_val["align_hori"] = m_align_hori;
	text_val["align_vert"] = m_align_vert;

	text_val["space_hori"] = m_space_hori;
	text_val["space_vert"] = m_space_vert;	

	text_val["text"] = m_text;
	text_val["tid"] = m_tid;

	val["text"] = text_val;
}

d2d::IPropertySetting* Sprite::CreatePropertySetting(d2d::EditPanelImpl* stage)
{
	return new PropertySetting(stage, this);
}

void Sprite::GetSize(int& width, int& height) const
{
	width = m_width;
	height = m_height;
}

void Sprite::SetSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void Sprite::GetAlign(int& halign, int& valign) const
{
	halign = m_align_hori;
	valign = m_align_vert;
}

void Sprite::SetAlign(int halign, int valign)
{
	m_align_hori = HoriAlignType(halign);
	m_align_vert = VertAlignType(valign);
}

void Sprite::GetSpace(float& hori, float& vert) const
{
	hori = m_space_hori;
	vert = m_space_vert;
}

void Sprite::SetSpace(float hori, float vert)
{
	m_space_hori = hori;
	m_space_vert = vert;
}

}